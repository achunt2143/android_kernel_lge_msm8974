<<<<<<< HEAD
/*******************************************************************************

  Intel 10 Gigabit PCI Express Linux driver
  Copyright(c) 1999 - 2012 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

  Contact Information:
  e1000-devel Mailing List <e1000-devel@lists.sourceforge.net>
  Intel Corporation, 5200 N.E. Elam Young Parkway, Hillsboro, OR 97124-6497

*******************************************************************************/
=======
// SPDX-License-Identifier: GPL-2.0
/* Copyright(c) 1999 - 2018 Intel Corporation. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/types.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/in.h>
#include <linux/interrupt.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/sctp.h>
#include <linux/pkt_sched.h>
#include <linux/ipv6.h>
#include <linux/slab.h>
#include <net/checksum.h>
#include <net/ip6_checksum.h>
<<<<<<< HEAD
#include <linux/ethtool.h>
#include <linux/if.h>
#include <linux/if_vlan.h>
#include <linux/prefetch.h>
#include <scsi/fc/fc_fcoe.h>
=======
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/if.h>
#include <linux/if_vlan.h>
#include <linux/if_macvlan.h>
#include <linux/if_bridge.h>
#include <linux/prefetch.h>
#include <linux/bpf.h>
#include <linux/bpf_trace.h>
#include <linux/atomic.h>
#include <linux/numa.h>
#include <generated/utsrelease.h>
#include <scsi/fc/fc_fcoe.h>
#include <net/udp_tunnel.h>
#include <net/pkt_cls.h>
#include <net/tc_act/tc_gact.h>
#include <net/tc_act/tc_mirred.h>
#include <net/vxlan.h>
#include <net/mpls.h>
#include <net/netdev_queues.h>
#include <net/xdp_sock_drv.h>
#include <net/xfrm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "ixgbe.h"
#include "ixgbe_common.h"
#include "ixgbe_dcb_82599.h"
<<<<<<< HEAD
#include "ixgbe_sriov.h"
=======
#include "ixgbe_phy.h"
#include "ixgbe_sriov.h"
#include "ixgbe_model.h"
#include "ixgbe_txrx_common.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

char ixgbe_driver_name[] = "ixgbe";
static const char ixgbe_driver_string[] =
			      "Intel(R) 10 Gigabit PCI Express Network Driver";
#ifdef IXGBE_FCOE
char ixgbe_default_device_descr[] =
			      "Intel(R) 10 Gigabit Network Connection";
#else
static char ixgbe_default_device_descr[] =
			      "Intel(R) 10 Gigabit Network Connection";
#endif
<<<<<<< HEAD
#define MAJ 3
#define MIN 8
#define BUILD 21
#define DRV_VERSION __stringify(MAJ) "." __stringify(MIN) "." \
	__stringify(BUILD) "-k"
const char ixgbe_driver_version[] = DRV_VERSION;
static const char ixgbe_copyright[] =
				"Copyright (c) 1999-2012 Intel Corporation.";

static const struct ixgbe_info *ixgbe_info_tbl[] = {
	[board_82598] = &ixgbe_82598_info,
	[board_82599] = &ixgbe_82599_info,
	[board_X540] = &ixgbe_X540_info,
=======
static const char ixgbe_copyright[] =
				"Copyright (c) 1999-2016 Intel Corporation.";

static const char ixgbe_overheat_msg[] = "Network adapter has been stopped because it has over heated. Restart the computer. If the problem persists, power off the system and replace the adapter";

static const struct ixgbe_info *ixgbe_info_tbl[] = {
	[board_82598]		= &ixgbe_82598_info,
	[board_82599]		= &ixgbe_82599_info,
	[board_X540]		= &ixgbe_X540_info,
	[board_X550]		= &ixgbe_X550_info,
	[board_X550EM_x]	= &ixgbe_X550EM_x_info,
	[board_x550em_x_fw]	= &ixgbe_x550em_x_fw_info,
	[board_x550em_a]	= &ixgbe_x550em_a_info,
	[board_x550em_a_fw]	= &ixgbe_x550em_a_fw_info,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* ixgbe_pci_tbl - PCI Device ID Table
 *
 * Wildcard entries (PCI_ANY_ID) should come last
 * Last entry must be all 0s
 *
 * { Vendor ID, Device ID, SubVendor ID, SubDevice ID,
 *   Class, Class Mask, private data (not used) }
 */
<<<<<<< HEAD
static DEFINE_PCI_DEVICE_TABLE(ixgbe_pci_tbl) = {
=======
static const struct pci_device_id ixgbe_pci_tbl[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82598), board_82598 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82598AF_DUAL_PORT), board_82598 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82598AF_SINGLE_PORT), board_82598 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82598AT), board_82598 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82598AT2), board_82598 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82598EB_CX4), board_82598 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82598_CX4_DUAL_PORT), board_82598 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82598_DA_DUAL_PORT), board_82598 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82598_SR_DUAL_PORT_EM), board_82598 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82598EB_XF_LR), board_82598 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82598EB_SFP_LOM), board_82598 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82598_BX), board_82598 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_KX4), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_XAUI_LOM), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_KR), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_SFP), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_SFP_EM), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_KX4_MEZZ), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_CX4), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_BACKPLANE_FCOE), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_SFP_FCOE), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_T3_LOM), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_COMBO_BACKPLANE), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X540T), board_X540 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_SFP_SF2), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_LS), board_82599 },
<<<<<<< HEAD
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599EN_SFP), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_SFP_SF_QP), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X540T1), board_X540 },
=======
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_QSFP_SF_QP), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599EN_SFP), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_82599_SFP_SF_QP), board_82599 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X540T1), board_X540 },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550T), board_X550},
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550T1), board_X550},
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_X_KX4), board_X550EM_x},
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_X_XFI), board_X550EM_x},
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_X_KR), board_X550EM_x},
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_X_10G_T), board_X550EM_x},
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_X_SFP), board_X550EM_x},
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_X_1G_T), board_x550em_x_fw},
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_A_KR), board_x550em_a },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_A_KR_L), board_x550em_a },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_A_SFP_N), board_x550em_a },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_A_SGMII), board_x550em_a },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_A_SGMII_L), board_x550em_a },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_A_10G_T), board_x550em_a},
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_A_SFP), board_x550em_a },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_A_1G_T), board_x550em_a_fw },
	{PCI_VDEVICE(INTEL, IXGBE_DEV_ID_X550EM_A_1G_T_L), board_x550em_a_fw },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* required last entry */
	{0, }
};
MODULE_DEVICE_TABLE(pci, ixgbe_pci_tbl);

#ifdef CONFIG_IXGBE_DCA
static int ixgbe_notify_dca(struct notifier_block *, unsigned long event,
			    void *p);
static struct notifier_block dca_notifier = {
	.notifier_call = ixgbe_notify_dca,
	.next          = NULL,
	.priority      = 0
};
#endif

#ifdef CONFIG_PCI_IOV
static unsigned int max_vfs;
module_param(max_vfs, uint, 0);
MODULE_PARM_DESC(max_vfs,
<<<<<<< HEAD
		 "Maximum number of virtual functions to allocate per physical function");
#endif /* CONFIG_PCI_IOV */

static unsigned int allow_unsupported_sfp;
module_param(allow_unsupported_sfp, uint, 0);
=======
		 "Maximum number of virtual functions to allocate per physical function - default is zero and maximum value is 63. (Deprecated)");
#endif /* CONFIG_PCI_IOV */

static bool allow_unsupported_sfp;
module_param(allow_unsupported_sfp, bool, 0444);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_PARM_DESC(allow_unsupported_sfp,
		 "Allow unsupported and untested SFP+ modules on 82599-based adapters");

#define DEFAULT_MSG_ENABLE (NETIF_MSG_DRV|NETIF_MSG_PROBE|NETIF_MSG_LINK)
static int debug = -1;
module_param(debug, int, 0);
MODULE_PARM_DESC(debug, "Debug level (0=none,...,16=all)");

MODULE_AUTHOR("Intel Corporation, <linux.nics@intel.com>");
MODULE_DESCRIPTION("Intel(R) 10 Gigabit PCI Express Network Driver");
<<<<<<< HEAD
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
=======
MODULE_LICENSE("GPL v2");

DEFINE_STATIC_KEY_FALSE(ixgbe_xdp_locking_key);
EXPORT_SYMBOL(ixgbe_xdp_locking_key);

static struct workqueue_struct *ixgbe_wq;

static bool ixgbe_check_cfg_remove(struct ixgbe_hw *hw, struct pci_dev *pdev);
static void ixgbe_watchdog_link_is_down(struct ixgbe_adapter *);

static const struct net_device_ops ixgbe_netdev_ops;

static bool netif_is_ixgbe(struct net_device *dev)
{
	return dev && (dev->netdev_ops == &ixgbe_netdev_ops);
}

static int ixgbe_read_pci_cfg_word_parent(struct ixgbe_adapter *adapter,
					  u32 reg, u16 *value)
{
	struct pci_dev *parent_dev;
	struct pci_bus *parent_bus;

	parent_bus = adapter->pdev->bus->parent;
	if (!parent_bus)
		return -1;

	parent_dev = parent_bus->self;
	if (!parent_dev)
		return -1;

	if (!pci_is_pcie(parent_dev))
		return -1;

	pcie_capability_read_word(parent_dev, reg, value);
	if (*value == IXGBE_FAILED_READ_CFG_WORD &&
	    ixgbe_check_cfg_remove(&adapter->hw, parent_dev))
		return -1;
	return 0;
}

static int ixgbe_get_parent_bus_info(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u16 link_status = 0;
	int err;

	hw->bus.type = ixgbe_bus_type_pci_express;

	/* Get the negotiated link width and speed from PCI config space of the
	 * parent, as this device is behind a switch
	 */
	err = ixgbe_read_pci_cfg_word_parent(adapter, 18, &link_status);

	/* assume caller will handle error case */
	if (err)
		return err;

	hw->bus.width = ixgbe_convert_bus_width(link_status);
	hw->bus.speed = ixgbe_convert_bus_speed(link_status);

	return 0;
}

/**
 * ixgbe_pcie_from_parent - Determine whether PCIe info should come from parent
 * @hw: hw specific details
 *
 * This function is used by probe to determine whether a device's PCI-Express
 * bandwidth details should be gathered from the parent bus instead of from the
 * device. Used to ensure that various locations all have the correct device ID
 * checks.
 */
static inline bool ixgbe_pcie_from_parent(struct ixgbe_hw *hw)
{
	switch (hw->device_id) {
	case IXGBE_DEV_ID_82599_SFP_SF_QP:
	case IXGBE_DEV_ID_82599_QSFP_SF_QP:
		return true;
	default:
		return false;
	}
}

static void ixgbe_check_minimum_link(struct ixgbe_adapter *adapter,
				     int expected_gts)
{
	struct ixgbe_hw *hw = &adapter->hw;
	struct pci_dev *pdev;

	/* Some devices are not connected over PCIe and thus do not negotiate
	 * speed. These devices do not have valid bus info, and thus any report
	 * we generate may not be correct.
	 */
	if (hw->bus.type == ixgbe_bus_type_internal)
		return;

	/* determine whether to use the parent device */
	if (ixgbe_pcie_from_parent(&adapter->hw))
		pdev = adapter->pdev->bus->parent->self;
	else
		pdev = adapter->pdev;

	pcie_print_link_status(pdev);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void ixgbe_service_event_schedule(struct ixgbe_adapter *adapter)
{
	if (!test_bit(__IXGBE_DOWN, &adapter->state) &&
<<<<<<< HEAD
	    !test_and_set_bit(__IXGBE_SERVICE_SCHED, &adapter->state))
		schedule_work(&adapter->service_task);
=======
	    !test_bit(__IXGBE_REMOVING, &adapter->state) &&
	    !test_and_set_bit(__IXGBE_SERVICE_SCHED, &adapter->state))
		queue_work(ixgbe_wq, &adapter->service_task);
}

static void ixgbe_remove_adapter(struct ixgbe_hw *hw)
{
	struct ixgbe_adapter *adapter = hw->back;

	if (!hw->hw_addr)
		return;
	hw->hw_addr = NULL;
	e_dev_err("Adapter removed\n");
	if (test_bit(__IXGBE_SERVICE_INITED, &adapter->state))
		ixgbe_service_event_schedule(adapter);
}

static u32 ixgbe_check_remove(struct ixgbe_hw *hw, u32 reg)
{
	u8 __iomem *reg_addr;
	u32 value;
	int i;

	reg_addr = READ_ONCE(hw->hw_addr);
	if (ixgbe_removed(reg_addr))
		return IXGBE_FAILED_READ_REG;

	/* Register read of 0xFFFFFFF can indicate the adapter has been removed,
	 * so perform several status register reads to determine if the adapter
	 * has been removed.
	 */
	for (i = 0; i < IXGBE_FAILED_READ_RETRIES; i++) {
		value = readl(reg_addr + IXGBE_STATUS);
		if (value != IXGBE_FAILED_READ_REG)
			break;
		mdelay(3);
	}

	if (value == IXGBE_FAILED_READ_REG)
		ixgbe_remove_adapter(hw);
	else
		value = readl(reg_addr + reg);
	return value;
}

/**
 * ixgbe_read_reg - Read from device register
 * @hw: hw specific details
 * @reg: offset of register to read
 *
 * Returns : value read or IXGBE_FAILED_READ_REG if removed
 *
 * This function is used to read device registers. It checks for device
 * removal by confirming any read that returns all ones by checking the
 * status register value for all ones. This function avoids reading from
 * the hardware if a removal was previously detected in which case it
 * returns IXGBE_FAILED_READ_REG (all ones).
 */
u32 ixgbe_read_reg(struct ixgbe_hw *hw, u32 reg)
{
	u8 __iomem *reg_addr = READ_ONCE(hw->hw_addr);
	u32 value;

	if (ixgbe_removed(reg_addr))
		return IXGBE_FAILED_READ_REG;
	if (unlikely(hw->phy.nw_mng_if_sel &
		     IXGBE_NW_MNG_IF_SEL_SGMII_ENABLE)) {
		struct ixgbe_adapter *adapter;
		int i;

		for (i = 0; i < 200; ++i) {
			value = readl(reg_addr + IXGBE_MAC_SGMII_BUSY);
			if (likely(!value))
				goto writes_completed;
			if (value == IXGBE_FAILED_READ_REG) {
				ixgbe_remove_adapter(hw);
				return IXGBE_FAILED_READ_REG;
			}
			udelay(5);
		}

		adapter = hw->back;
		e_warn(hw, "register writes incomplete %08x\n", value);
	}

writes_completed:
	value = readl(reg_addr + reg);
	if (unlikely(value == IXGBE_FAILED_READ_REG))
		value = ixgbe_check_remove(hw, reg);
	return value;
}

static bool ixgbe_check_cfg_remove(struct ixgbe_hw *hw, struct pci_dev *pdev)
{
	u16 value;

	pci_read_config_word(pdev, PCI_VENDOR_ID, &value);
	if (value == IXGBE_FAILED_READ_CFG_WORD) {
		ixgbe_remove_adapter(hw);
		return true;
	}
	return false;
}

u16 ixgbe_read_pci_cfg_word(struct ixgbe_hw *hw, u32 reg)
{
	struct ixgbe_adapter *adapter = hw->back;
	u16 value;

	if (ixgbe_removed(hw->hw_addr))
		return IXGBE_FAILED_READ_CFG_WORD;
	pci_read_config_word(adapter->pdev, reg, &value);
	if (value == IXGBE_FAILED_READ_CFG_WORD &&
	    ixgbe_check_cfg_remove(hw, adapter->pdev))
		return IXGBE_FAILED_READ_CFG_WORD;
	return value;
}

#ifdef CONFIG_PCI_IOV
static u32 ixgbe_read_pci_cfg_dword(struct ixgbe_hw *hw, u32 reg)
{
	struct ixgbe_adapter *adapter = hw->back;
	u32 value;

	if (ixgbe_removed(hw->hw_addr))
		return IXGBE_FAILED_READ_CFG_DWORD;
	pci_read_config_dword(adapter->pdev, reg, &value);
	if (value == IXGBE_FAILED_READ_CFG_DWORD &&
	    ixgbe_check_cfg_remove(hw, adapter->pdev))
		return IXGBE_FAILED_READ_CFG_DWORD;
	return value;
}
#endif /* CONFIG_PCI_IOV */

void ixgbe_write_pci_cfg_word(struct ixgbe_hw *hw, u32 reg, u16 value)
{
	struct ixgbe_adapter *adapter = hw->back;

	if (ixgbe_removed(hw->hw_addr))
		return;
	pci_write_config_word(adapter->pdev, reg, value);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_service_event_complete(struct ixgbe_adapter *adapter)
{
	BUG_ON(!test_bit(__IXGBE_SERVICE_SCHED, &adapter->state));

	/* flush memory to make sure state is correct before next watchdog */
<<<<<<< HEAD
	smp_mb__before_clear_bit();
=======
	smp_mb__before_atomic();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clear_bit(__IXGBE_SERVICE_SCHED, &adapter->state);
}

struct ixgbe_reg_info {
	u32 ofs;
	char *name;
};

static const struct ixgbe_reg_info ixgbe_reg_info_tbl[] = {

	/* General Registers */
	{IXGBE_CTRL, "CTRL"},
	{IXGBE_STATUS, "STATUS"},
	{IXGBE_CTRL_EXT, "CTRL_EXT"},

	/* Interrupt Registers */
	{IXGBE_EICR, "EICR"},

	/* RX Registers */
	{IXGBE_SRRCTL(0), "SRRCTL"},
	{IXGBE_DCA_RXCTRL(0), "DRXCTL"},
	{IXGBE_RDLEN(0), "RDLEN"},
	{IXGBE_RDH(0), "RDH"},
	{IXGBE_RDT(0), "RDT"},
	{IXGBE_RXDCTL(0), "RXDCTL"},
	{IXGBE_RDBAL(0), "RDBAL"},
	{IXGBE_RDBAH(0), "RDBAH"},

	/* TX Registers */
	{IXGBE_TDBAL(0), "TDBAL"},
	{IXGBE_TDBAH(0), "TDBAH"},
	{IXGBE_TDLEN(0), "TDLEN"},
	{IXGBE_TDH(0), "TDH"},
	{IXGBE_TDT(0), "TDT"},
	{IXGBE_TXDCTL(0), "TXDCTL"},

	/* List Terminator */
<<<<<<< HEAD
	{}
=======
	{ .name = NULL }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};


/*
 * ixgbe_regdump - register printout routine
 */
static void ixgbe_regdump(struct ixgbe_hw *hw, struct ixgbe_reg_info *reginfo)
{
<<<<<<< HEAD
	int i = 0, j = 0;
=======
	int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char rname[16];
	u32 regs[64];

	switch (reginfo->ofs) {
	case IXGBE_SRRCTL(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_SRRCTL(i));
		break;
	case IXGBE_DCA_RXCTRL(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_DCA_RXCTRL(i));
		break;
	case IXGBE_RDLEN(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_RDLEN(i));
		break;
	case IXGBE_RDH(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_RDH(i));
		break;
	case IXGBE_RDT(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_RDT(i));
		break;
	case IXGBE_RXDCTL(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_RXDCTL(i));
		break;
	case IXGBE_RDBAL(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_RDBAL(i));
		break;
	case IXGBE_RDBAH(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_RDBAH(i));
		break;
	case IXGBE_TDBAL(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_TDBAL(i));
		break;
	case IXGBE_TDBAH(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_TDBAH(i));
		break;
	case IXGBE_TDLEN(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_TDLEN(i));
		break;
	case IXGBE_TDH(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_TDH(i));
		break;
	case IXGBE_TDT(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_TDT(i));
		break;
	case IXGBE_TXDCTL(0):
		for (i = 0; i < 64; i++)
			regs[i] = IXGBE_READ_REG(hw, IXGBE_TXDCTL(i));
		break;
	default:
<<<<<<< HEAD
		pr_info("%-15s %08x\n", reginfo->name,
			IXGBE_READ_REG(hw, reginfo->ofs));
		return;
	}

	for (i = 0; i < 8; i++) {
		snprintf(rname, 16, "%s[%d-%d]", reginfo->name, i*8, i*8+7);
		pr_err("%-15s", rname);
		for (j = 0; j < 8; j++)
			pr_cont(" %08x", regs[i*8+j]);
		pr_cont("\n");
=======
		pr_info("%-15s %08x\n",
			reginfo->name, IXGBE_READ_REG(hw, reginfo->ofs));
		return;
	}

	i = 0;
	while (i < 64) {
		int j;
		char buf[9 * 8 + 1];
		char *p = buf;

		snprintf(rname, 16, "%s[%d-%d]", reginfo->name, i, i + 7);
		for (j = 0; j < 8; j++)
			p += sprintf(p, " %08x", regs[i++]);
		pr_err("%-15s%s\n", rname, buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

}

<<<<<<< HEAD
=======
static void ixgbe_print_buffer(struct ixgbe_ring *ring, int n)
{
	struct ixgbe_tx_buffer *tx_buffer;

	tx_buffer = &ring->tx_buffer_info[ring->next_to_clean];
	pr_info(" %5d %5X %5X %016llX %08X %p %016llX\n",
		n, ring->next_to_use, ring->next_to_clean,
		(u64)dma_unmap_addr(tx_buffer, dma),
		dma_unmap_len(tx_buffer, len),
		tx_buffer->next_to_watch,
		(u64)tx_buffer->time_stamp);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ixgbe_dump - Print registers, tx-rings and rx-rings
 */
static void ixgbe_dump(struct ixgbe_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct ixgbe_hw *hw = &adapter->hw;
	struct ixgbe_reg_info *reginfo;
	int n = 0;
<<<<<<< HEAD
	struct ixgbe_ring *tx_ring;
=======
	struct ixgbe_ring *ring;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ixgbe_tx_buffer *tx_buffer;
	union ixgbe_adv_tx_desc *tx_desc;
	struct my_u0 { u64 a; u64 b; } *u0;
	struct ixgbe_ring *rx_ring;
	union ixgbe_adv_rx_desc *rx_desc;
	struct ixgbe_rx_buffer *rx_buffer_info;
<<<<<<< HEAD
	u32 staterr;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i = 0;

	if (!netif_msg_hw(adapter))
		return;

	/* Print netdevice Info */
	if (netdev) {
		dev_info(&adapter->pdev->dev, "Net device Info\n");
		pr_info("Device Name     state            "
<<<<<<< HEAD
			"trans_start      last_rx\n");
		pr_info("%-15s %016lX %016lX %016lX\n",
			netdev->name,
			netdev->state,
			netdev->trans_start,
			netdev->last_rx);
=======
			"trans_start\n");
		pr_info("%-15s %016lX %016lX\n",
			netdev->name,
			netdev->state,
			dev_trans_start(netdev));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Print Registers */
	dev_info(&adapter->pdev->dev, "Register Dump\n");
	pr_info(" Register Name   Value\n");
	for (reginfo = (struct ixgbe_reg_info *)ixgbe_reg_info_tbl;
	     reginfo->name; reginfo++) {
		ixgbe_regdump(hw, reginfo);
	}

	/* Print TX Ring Summary */
	if (!netdev || !netif_running(netdev))
<<<<<<< HEAD
		goto exit;

	dev_info(&adapter->pdev->dev, "TX Rings Summary\n");
	pr_info("Queue [NTU] [NTC] [bi(ntc)->dma  ] leng ntw timestamp\n");
	for (n = 0; n < adapter->num_tx_queues; n++) {
		tx_ring = adapter->tx_ring[n];
		tx_buffer = &tx_ring->tx_buffer_info[tx_ring->next_to_clean];
		pr_info(" %5d %5X %5X %016llX %04X %p %016llX\n",
			   n, tx_ring->next_to_use, tx_ring->next_to_clean,
			   (u64)dma_unmap_addr(tx_buffer, dma),
			   dma_unmap_len(tx_buffer, len),
			   tx_buffer->next_to_watch,
			   (u64)tx_buffer->time_stamp);
=======
		return;

	dev_info(&adapter->pdev->dev, "TX Rings Summary\n");
	pr_info(" %s     %s              %s        %s\n",
		"Queue [NTU] [NTC] [bi(ntc)->dma  ]",
		"leng", "ntw", "timestamp");
	for (n = 0; n < adapter->num_tx_queues; n++) {
		ring = adapter->tx_ring[n];
		ixgbe_print_buffer(ring, n);
	}

	for (n = 0; n < adapter->num_xdp_queues; n++) {
		ring = adapter->xdp_ring[n];
		ixgbe_print_buffer(ring, n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Print TX Rings */
	if (!netif_msg_tx_done(adapter))
		goto rx_ring_summary;

	dev_info(&adapter->pdev->dev, "TX Rings Dump\n");

	/* Transmit Descriptor Formats
	 *
<<<<<<< HEAD
	 * Advanced Transmit Descriptor
	 *   +--------------------------------------------------------------+
	 * 0 |         Buffer Address [63:0]                                |
	 *   +--------------------------------------------------------------+
	 * 8 |  PAYLEN  | PORTS  | IDX | STA | DCMD  |DTYP |  RSV |  DTALEN |
	 *   +--------------------------------------------------------------+
	 *   63       46 45    40 39 36 35 32 31   24 23 20 19              0
	 */

	for (n = 0; n < adapter->num_tx_queues; n++) {
		tx_ring = adapter->tx_ring[n];
		pr_info("------------------------------------\n");
		pr_info("TX QUEUE INDEX = %d\n", tx_ring->queue_index);
		pr_info("------------------------------------\n");
		pr_info("T [desc]     [address 63:0  ] "
			"[PlPOIdStDDt Ln] [bi->dma       ] "
			"leng  ntw timestamp        bi->skb\n");

		for (i = 0; tx_ring->desc && (i < tx_ring->count); i++) {
			tx_desc = IXGBE_TX_DESC(tx_ring, i);
			tx_buffer = &tx_ring->tx_buffer_info[i];
			u0 = (struct my_u0 *)tx_desc;
			pr_info("T [0x%03X]    %016llX %016llX %016llX"
				" %04X  %p %016llX %p", i,
				le64_to_cpu(u0->a),
				le64_to_cpu(u0->b),
				(u64)dma_unmap_addr(tx_buffer, dma),
				dma_unmap_len(tx_buffer, len),
				tx_buffer->next_to_watch,
				(u64)tx_buffer->time_stamp,
				tx_buffer->skb);
			if (i == tx_ring->next_to_use &&
				i == tx_ring->next_to_clean)
				pr_cont(" NTC/U\n");
			else if (i == tx_ring->next_to_use)
				pr_cont(" NTU\n");
			else if (i == tx_ring->next_to_clean)
				pr_cont(" NTC\n");
			else
				pr_cont("\n");

			if (netif_msg_pktdata(adapter) &&
			    dma_unmap_len(tx_buffer, len) != 0)
				print_hex_dump(KERN_INFO, "",
					DUMP_PREFIX_ADDRESS, 16, 1,
					phys_to_virt(dma_unmap_addr(tx_buffer,
								    dma)),
					dma_unmap_len(tx_buffer, len),
					true);
=======
	 * 82598 Advanced Transmit Descriptor
	 *   +--------------------------------------------------------------+
	 * 0 |         Buffer Address [63:0]                                |
	 *   +--------------------------------------------------------------+
	 * 8 |  PAYLEN  | POPTS  | IDX | STA | DCMD  |DTYP |  RSV |  DTALEN |
	 *   +--------------------------------------------------------------+
	 *   63       46 45    40 39 36 35 32 31   24 23 20 19              0
	 *
	 * 82598 Advanced Transmit Descriptor (Write-Back Format)
	 *   +--------------------------------------------------------------+
	 * 0 |                          RSV [63:0]                          |
	 *   +--------------------------------------------------------------+
	 * 8 |            RSV           |  STA  |          NXTSEQ           |
	 *   +--------------------------------------------------------------+
	 *   63                       36 35   32 31                         0
	 *
	 * 82599+ Advanced Transmit Descriptor
	 *   +--------------------------------------------------------------+
	 * 0 |         Buffer Address [63:0]                                |
	 *   +--------------------------------------------------------------+
	 * 8 |PAYLEN  |POPTS|CC|IDX  |STA  |DCMD  |DTYP |MAC  |RSV  |DTALEN |
	 *   +--------------------------------------------------------------+
	 *   63     46 45 40 39 38 36 35 32 31  24 23 20 19 18 17 16 15     0
	 *
	 * 82599+ Advanced Transmit Descriptor (Write-Back Format)
	 *   +--------------------------------------------------------------+
	 * 0 |                          RSV [63:0]                          |
	 *   +--------------------------------------------------------------+
	 * 8 |            RSV           |  STA  |           RSV             |
	 *   +--------------------------------------------------------------+
	 *   63                       36 35   32 31                         0
	 */

	for (n = 0; n < adapter->num_tx_queues; n++) {
		ring = adapter->tx_ring[n];
		pr_info("------------------------------------\n");
		pr_info("TX QUEUE INDEX = %d\n", ring->queue_index);
		pr_info("------------------------------------\n");
		pr_info("%s%s    %s              %s        %s          %s\n",
			"T [desc]     [address 63:0  ] ",
			"[PlPOIdStDDt Ln] [bi->dma       ] ",
			"leng", "ntw", "timestamp", "bi->skb");

		for (i = 0; ring->desc && (i < ring->count); i++) {
			tx_desc = IXGBE_TX_DESC(ring, i);
			tx_buffer = &ring->tx_buffer_info[i];
			u0 = (struct my_u0 *)tx_desc;
			if (dma_unmap_len(tx_buffer, len) > 0) {
				const char *ring_desc;

				if (i == ring->next_to_use &&
				    i == ring->next_to_clean)
					ring_desc = " NTC/U";
				else if (i == ring->next_to_use)
					ring_desc = " NTU";
				else if (i == ring->next_to_clean)
					ring_desc = " NTC";
				else
					ring_desc = "";
				pr_info("T [0x%03X]    %016llX %016llX %016llX %08X %p %016llX %p%s",
					i,
					le64_to_cpu((__force __le64)u0->a),
					le64_to_cpu((__force __le64)u0->b),
					(u64)dma_unmap_addr(tx_buffer, dma),
					dma_unmap_len(tx_buffer, len),
					tx_buffer->next_to_watch,
					(u64)tx_buffer->time_stamp,
					tx_buffer->skb,
					ring_desc);

				if (netif_msg_pktdata(adapter) &&
				    tx_buffer->skb)
					print_hex_dump(KERN_INFO, "",
						DUMP_PREFIX_ADDRESS, 16, 1,
						tx_buffer->skb->data,
						dma_unmap_len(tx_buffer, len),
						true);
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/* Print RX Rings Summary */
rx_ring_summary:
	dev_info(&adapter->pdev->dev, "RX Rings Summary\n");
	pr_info("Queue [NTU] [NTC]\n");
	for (n = 0; n < adapter->num_rx_queues; n++) {
		rx_ring = adapter->rx_ring[n];
		pr_info("%5d %5X %5X\n",
			n, rx_ring->next_to_use, rx_ring->next_to_clean);
	}

	/* Print RX Rings */
	if (!netif_msg_rx_status(adapter))
<<<<<<< HEAD
		goto exit;

	dev_info(&adapter->pdev->dev, "RX Rings Dump\n");

	/* Advanced Receive Descriptor (Read) Format
=======
		return;

	dev_info(&adapter->pdev->dev, "RX Rings Dump\n");

	/* Receive Descriptor Formats
	 *
	 * 82598 Advanced Receive Descriptor (Read) Format
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 *    63                                           1        0
	 *    +-----------------------------------------------------+
	 *  0 |       Packet Buffer Address [63:1]           |A0/NSE|
	 *    +----------------------------------------------+------+
	 *  8 |       Header Buffer Address [63:1]           |  DD  |
	 *    +-----------------------------------------------------+
	 *
	 *
<<<<<<< HEAD
	 * Advanced Receive Descriptor (Write-Back) Format
	 *
	 *   63       48 47    32 31  30      21 20 16 15   4 3     0
	 *   +------------------------------------------------------+
	 * 0 | Packet     IP     |SPH| HDR_LEN   | RSV|Packet|  RSS |
	 *   | Checksum   Ident  |   |           |    | Type | Type |
=======
	 * 82598 Advanced Receive Descriptor (Write-Back) Format
	 *
	 *   63       48 47    32 31  30      21 20 16 15   4 3     0
	 *   +------------------------------------------------------+
	 * 0 |       RSS Hash /  |SPH| HDR_LEN  | RSV |Packet|  RSS |
	 *   | Packet   | IP     |   |          |     | Type | Type |
	 *   | Checksum | Ident  |   |          |     |      |      |
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 *   +------------------------------------------------------+
	 * 8 | VLAN Tag | Length | Extended Error | Extended Status |
	 *   +------------------------------------------------------+
	 *   63       48 47    32 31            20 19               0
<<<<<<< HEAD
	 */
=======
	 *
	 * 82599+ Advanced Receive Descriptor (Read) Format
	 *    63                                           1        0
	 *    +-----------------------------------------------------+
	 *  0 |       Packet Buffer Address [63:1]           |A0/NSE|
	 *    +----------------------------------------------+------+
	 *  8 |       Header Buffer Address [63:1]           |  DD  |
	 *    +-----------------------------------------------------+
	 *
	 *
	 * 82599+ Advanced Receive Descriptor (Write-Back) Format
	 *
	 *   63       48 47    32 31  30      21 20 17 16   4 3     0
	 *   +------------------------------------------------------+
	 * 0 |RSS / Frag Checksum|SPH| HDR_LEN  |RSC- |Packet|  RSS |
	 *   |/ RTT / PCoE_PARAM |   |          | CNT | Type | Type |
	 *   |/ Flow Dir Flt ID  |   |          |     |      |      |
	 *   +------------------------------------------------------+
	 * 8 | VLAN Tag | Length |Extended Error| Xtnd Status/NEXTP |
	 *   +------------------------------------------------------+
	 *   63       48 47    32 31          20 19                 0
	 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (n = 0; n < adapter->num_rx_queues; n++) {
		rx_ring = adapter->rx_ring[n];
		pr_info("------------------------------------\n");
		pr_info("RX QUEUE INDEX = %d\n", rx_ring->queue_index);
		pr_info("------------------------------------\n");
<<<<<<< HEAD
		pr_info("R  [desc]      [ PktBuf     A0] "
			"[  HeadBuf   DD] [bi->dma       ] [bi->skb] "
			"<-- Adv Rx Read format\n");
		pr_info("RWB[desc]      [PcsmIpSHl PtRs] "
			"[vl er S cks ln] ---------------- [bi->skb] "
			"<-- Adv Rx Write-Back format\n");

		for (i = 0; i < rx_ring->count; i++) {
			rx_buffer_info = &rx_ring->rx_buffer_info[i];
			rx_desc = IXGBE_RX_DESC(rx_ring, i);
			u0 = (struct my_u0 *)rx_desc;
			staterr = le32_to_cpu(rx_desc->wb.upper.status_error);
			if (staterr & IXGBE_RXD_STAT_DD) {
				/* Descriptor Done */
				pr_info("RWB[0x%03X]     %016llX "
					"%016llX ---------------- %p", i,
					le64_to_cpu(u0->a),
					le64_to_cpu(u0->b),
					rx_buffer_info->skb);
			} else {
				pr_info("R  [0x%03X]     %016llX "
					"%016llX %016llX %p", i,
					le64_to_cpu(u0->a),
					le64_to_cpu(u0->b),
					(u64)rx_buffer_info->dma,
					rx_buffer_info->skb);

				if (netif_msg_pktdata(adapter)) {
					print_hex_dump(KERN_INFO, "",
					   DUMP_PREFIX_ADDRESS, 16, 1,
					   phys_to_virt(rx_buffer_info->dma),
					   ixgbe_rx_bufsz(rx_ring), true);
				}
			}

			if (i == rx_ring->next_to_use)
				pr_cont(" NTU\n");
			else if (i == rx_ring->next_to_clean)
				pr_cont(" NTC\n");
			else
				pr_cont("\n");

		}
	}

exit:
	return;
=======
		pr_info("%s%s%s\n",
			"R  [desc]      [ PktBuf     A0] ",
			"[  HeadBuf   DD] [bi->dma       ] [bi->skb       ] ",
			"<-- Adv Rx Read format");
		pr_info("%s%s%s\n",
			"RWB[desc]      [PcsmIpSHl PtRs] ",
			"[vl er S cks ln] ---------------- [bi->skb       ] ",
			"<-- Adv Rx Write-Back format");

		for (i = 0; i < rx_ring->count; i++) {
			const char *ring_desc;

			if (i == rx_ring->next_to_use)
				ring_desc = " NTU";
			else if (i == rx_ring->next_to_clean)
				ring_desc = " NTC";
			else
				ring_desc = "";

			rx_buffer_info = &rx_ring->rx_buffer_info[i];
			rx_desc = IXGBE_RX_DESC(rx_ring, i);
			u0 = (struct my_u0 *)rx_desc;
			if (rx_desc->wb.upper.length) {
				/* Descriptor Done */
				pr_info("RWB[0x%03X]     %016llX %016llX ---------------- %p%s\n",
					i,
					le64_to_cpu((__force __le64)u0->a),
					le64_to_cpu((__force __le64)u0->b),
					rx_buffer_info->skb,
					ring_desc);
			} else {
				pr_info("R  [0x%03X]     %016llX %016llX %016llX %p%s\n",
					i,
					le64_to_cpu((__force __le64)u0->a),
					le64_to_cpu((__force __le64)u0->b),
					(u64)rx_buffer_info->dma,
					rx_buffer_info->skb,
					ring_desc);

				if (netif_msg_pktdata(adapter) &&
				    rx_buffer_info->dma) {
					print_hex_dump(KERN_INFO, "",
					   DUMP_PREFIX_ADDRESS, 16, 1,
					   page_address(rx_buffer_info->page) +
						    rx_buffer_info->page_offset,
					   ixgbe_rx_bufsz(rx_ring), true);
				}
			}
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_release_hw_control(struct ixgbe_adapter *adapter)
{
	u32 ctrl_ext;

	/* Let firmware take over control of h/w */
	ctrl_ext = IXGBE_READ_REG(&adapter->hw, IXGBE_CTRL_EXT);
	IXGBE_WRITE_REG(&adapter->hw, IXGBE_CTRL_EXT,
			ctrl_ext & ~IXGBE_CTRL_EXT_DRV_LOAD);
}

static void ixgbe_get_hw_control(struct ixgbe_adapter *adapter)
{
	u32 ctrl_ext;

	/* Let firmware know the driver has taken over */
	ctrl_ext = IXGBE_READ_REG(&adapter->hw, IXGBE_CTRL_EXT);
	IXGBE_WRITE_REG(&adapter->hw, IXGBE_CTRL_EXT,
			ctrl_ext | IXGBE_CTRL_EXT_DRV_LOAD);
}

<<<<<<< HEAD
/*
=======
/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ixgbe_set_ivar - set the IVAR registers, mapping interrupt causes to vectors
 * @adapter: pointer to adapter struct
 * @direction: 0 for Rx, 1 for Tx, -1 for other causes
 * @queue: queue to map the corresponding interrupt to
 * @msix_vector: the vector to map to the corresponding queue
 *
 */
static void ixgbe_set_ivar(struct ixgbe_adapter *adapter, s8 direction,
			   u8 queue, u8 msix_vector)
{
	u32 ivar, index;
	struct ixgbe_hw *hw = &adapter->hw;
	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		msix_vector |= IXGBE_IVAR_ALLOC_VAL;
		if (direction == -1)
			direction = 0;
		index = (((direction * 64) + queue) >> 2) & 0x1F;
		ivar = IXGBE_READ_REG(hw, IXGBE_IVAR(index));
		ivar &= ~(0xFF << (8 * (queue & 0x3)));
		ivar |= (msix_vector << (8 * (queue & 0x3)));
		IXGBE_WRITE_REG(hw, IXGBE_IVAR(index), ivar);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
<<<<<<< HEAD
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (direction == -1) {
			/* other causes */
			msix_vector |= IXGBE_IVAR_ALLOC_VAL;
			index = ((queue & 1) * 8);
			ivar = IXGBE_READ_REG(&adapter->hw, IXGBE_IVAR_MISC);
			ivar &= ~(0xFF << index);
			ivar |= (msix_vector << index);
			IXGBE_WRITE_REG(&adapter->hw, IXGBE_IVAR_MISC, ivar);
			break;
		} else {
			/* tx or rx causes */
			msix_vector |= IXGBE_IVAR_ALLOC_VAL;
			index = ((16 * (queue & 1)) + (8 * direction));
			ivar = IXGBE_READ_REG(hw, IXGBE_IVAR(queue >> 1));
			ivar &= ~(0xFF << index);
			ivar |= (msix_vector << index);
			IXGBE_WRITE_REG(hw, IXGBE_IVAR(queue >> 1), ivar);
			break;
		}
	default:
		break;
	}
}

<<<<<<< HEAD
static inline void ixgbe_irq_rearm_queues(struct ixgbe_adapter *adapter,
					  u64 qmask)
=======
void ixgbe_irq_rearm_queues(struct ixgbe_adapter *adapter,
			    u64 qmask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 mask;

	switch (adapter->hw.mac.type) {
	case ixgbe_mac_82598EB:
		mask = (IXGBE_EIMS_RTX_QUEUE & qmask);
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_EICS, mask);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
<<<<<<< HEAD
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mask = (qmask & 0xFFFFFFFF);
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_EICS_EX(0), mask);
		mask = (qmask >> 32);
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_EICS_EX(1), mask);
		break;
	default:
		break;
	}
}

<<<<<<< HEAD
void ixgbe_unmap_and_free_tx_resource(struct ixgbe_ring *ring,
				      struct ixgbe_tx_buffer *tx_buffer)
{
	if (tx_buffer->skb) {
		dev_kfree_skb_any(tx_buffer->skb);
		if (dma_unmap_len(tx_buffer, len))
			dma_unmap_single(ring->dev,
					 dma_unmap_addr(tx_buffer, dma),
					 dma_unmap_len(tx_buffer, len),
					 DMA_TO_DEVICE);
	} else if (dma_unmap_len(tx_buffer, len)) {
		dma_unmap_page(ring->dev,
			       dma_unmap_addr(tx_buffer, dma),
			       dma_unmap_len(tx_buffer, len),
			       DMA_TO_DEVICE);
	}
	tx_buffer->next_to_watch = NULL;
	tx_buffer->skb = NULL;
	dma_unmap_len_set(tx_buffer, len, 0);
	/* tx_buffer must be completely set up in the transmit path */
=======
static void ixgbe_update_xoff_rx_lfc(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	struct ixgbe_hw_stats *hwstats = &adapter->stats;
	int i;
	u32 data;

	if ((hw->fc.current_mode != ixgbe_fc_full) &&
	    (hw->fc.current_mode != ixgbe_fc_rx_pause))
		return;

	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		data = IXGBE_READ_REG(hw, IXGBE_LXOFFRXC);
		break;
	default:
		data = IXGBE_READ_REG(hw, IXGBE_LXOFFRXCNT);
	}
	hwstats->lxoffrxc += data;

	/* refill credits (no tx hang) if we received xoff */
	if (!data)
		return;

	for (i = 0; i < adapter->num_tx_queues; i++)
		clear_bit(__IXGBE_HANG_CHECK_ARMED,
			  &adapter->tx_ring[i]->state);

	for (i = 0; i < adapter->num_xdp_queues; i++)
		clear_bit(__IXGBE_HANG_CHECK_ARMED,
			  &adapter->xdp_ring[i]->state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_update_xoff_received(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	struct ixgbe_hw_stats *hwstats = &adapter->stats;
<<<<<<< HEAD
	u32 data = 0;
	u32 xoff[8] = {0};
	int i;

	if ((hw->fc.current_mode == ixgbe_fc_full) ||
	    (hw->fc.current_mode == ixgbe_fc_rx_pause)) {
		switch (hw->mac.type) {
		case ixgbe_mac_82598EB:
			data = IXGBE_READ_REG(hw, IXGBE_LXOFFRXC);
			break;
		default:
			data = IXGBE_READ_REG(hw, IXGBE_LXOFFRXCNT);
		}
		hwstats->lxoffrxc += data;

		/* refill credits (no tx hang) if we received xoff */
		if (!data)
			return;

		for (i = 0; i < adapter->num_tx_queues; i++)
			clear_bit(__IXGBE_HANG_CHECK_ARMED,
				  &adapter->tx_ring[i]->state);
		return;
	} else if (!(adapter->dcb_cfg.pfc_mode_enable))
		return;

	/* update stats for each tc, only valid with PFC enabled */
	for (i = 0; i < MAX_TX_PACKET_BUFFERS; i++) {
		switch (hw->mac.type) {
		case ixgbe_mac_82598EB:
			xoff[i] = IXGBE_READ_REG(hw, IXGBE_PXOFFRXC(i));
			break;
		default:
			xoff[i] = IXGBE_READ_REG(hw, IXGBE_PXOFFRXCNT(i));
		}
		hwstats->pxoffrxc[i] += xoff[i];
=======
	u32 xoff[8] = {0};
	u8 tc;
	int i;
	bool pfc_en = adapter->dcb_cfg.pfc_mode_enable;

	if (adapter->ixgbe_ieee_pfc)
		pfc_en |= !!(adapter->ixgbe_ieee_pfc->pfc_en);

	if (!(adapter->flags & IXGBE_FLAG_DCB_ENABLED) || !pfc_en) {
		ixgbe_update_xoff_rx_lfc(adapter);
		return;
	}

	/* update stats for each tc, only valid with PFC enabled */
	for (i = 0; i < MAX_TX_PACKET_BUFFERS; i++) {
		u32 pxoffrxc;

		switch (hw->mac.type) {
		case ixgbe_mac_82598EB:
			pxoffrxc = IXGBE_READ_REG(hw, IXGBE_PXOFFRXC(i));
			break;
		default:
			pxoffrxc = IXGBE_READ_REG(hw, IXGBE_PXOFFRXCNT(i));
		}
		hwstats->pxoffrxc[i] += pxoffrxc;
		/* Get the TC for given UP */
		tc = netdev_get_prio_tc_map(adapter->netdev, i);
		xoff[tc] += pxoffrxc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* disarm tx queues that have received xoff frames */
	for (i = 0; i < adapter->num_tx_queues; i++) {
		struct ixgbe_ring *tx_ring = adapter->tx_ring[i];
<<<<<<< HEAD
		u8 tc = tx_ring->dcb_tc;

		if (xoff[tc])
			clear_bit(__IXGBE_HANG_CHECK_ARMED, &tx_ring->state);
	}
=======

		tc = tx_ring->dcb_tc;
		if (xoff[tc])
			clear_bit(__IXGBE_HANG_CHECK_ARMED, &tx_ring->state);
	}

	for (i = 0; i < adapter->num_xdp_queues; i++) {
		struct ixgbe_ring *xdp_ring = adapter->xdp_ring[i];

		tc = xdp_ring->dcb_tc;
		if (xoff[tc])
			clear_bit(__IXGBE_HANG_CHECK_ARMED, &xdp_ring->state);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static u64 ixgbe_get_tx_completed(struct ixgbe_ring *ring)
{
	return ring->stats.packets;
}

static u64 ixgbe_get_tx_pending(struct ixgbe_ring *ring)
{
<<<<<<< HEAD
	struct ixgbe_adapter *adapter = netdev_priv(ring->netdev);
	struct ixgbe_hw *hw = &adapter->hw;

	u32 head = IXGBE_READ_REG(hw, IXGBE_TDH(ring->reg_idx));
	u32 tail = IXGBE_READ_REG(hw, IXGBE_TDT(ring->reg_idx));

	if (head != tail)
		return (head < tail) ?
			tail - head : (tail + ring->count - head);

	return 0;
=======
	unsigned int head, tail;

	head = ring->next_to_clean;
	tail = ring->next_to_use;

	return ((head <= tail) ? tail : tail + ring->count) - head;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline bool ixgbe_check_tx_hang(struct ixgbe_ring *tx_ring)
{
	u32 tx_done = ixgbe_get_tx_completed(tx_ring);
	u32 tx_done_old = tx_ring->tx_stats.tx_done_old;
	u32 tx_pending = ixgbe_get_tx_pending(tx_ring);
<<<<<<< HEAD
	bool ret = false;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	clear_check_for_tx_hang(tx_ring);

	/*
	 * Check for a hung queue, but be thorough. This verifies
	 * that a transmit has been completed since the previous
	 * check AND there is at least one packet pending. The
	 * ARMED bit is set to indicate a potential hang. The
	 * bit is cleared if a pause frame is received to remove
	 * false hang detection due to PFC or 802.3x frames. By
	 * requiring this to fail twice we avoid races with
	 * pfc clearing the ARMED bit and conditions where we
	 * run the check_tx_hang logic with a transmit completion
	 * pending but without time to complete it yet.
	 */
<<<<<<< HEAD
	if ((tx_done_old == tx_done) && tx_pending) {
		/* make sure it is true for two checks in a row */
		ret = test_and_set_bit(__IXGBE_HANG_CHECK_ARMED,
				       &tx_ring->state);
	} else {
		/* update completed stats and continue */
		tx_ring->tx_stats.tx_done_old = tx_done;
		/* reset the countdown */
		clear_bit(__IXGBE_HANG_CHECK_ARMED, &tx_ring->state);
	}

	return ret;
=======
	if (tx_done_old == tx_done && tx_pending)
		/* make sure it is true for two checks in a row */
		return test_and_set_bit(__IXGBE_HANG_CHECK_ARMED,
					&tx_ring->state);
	/* update completed stats and continue */
	tx_ring->tx_stats.tx_done_old = tx_done;
	/* reset the countdown */
	clear_bit(__IXGBE_HANG_CHECK_ARMED, &tx_ring->state);

	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_tx_timeout_reset - initiate reset due to Tx timeout
 * @adapter: driver private struct
 **/
static void ixgbe_tx_timeout_reset(struct ixgbe_adapter *adapter)
{

	/* Do the reset outside of interrupt context */
	if (!test_bit(__IXGBE_DOWN, &adapter->state)) {
<<<<<<< HEAD
		adapter->flags2 |= IXGBE_FLAG2_RESET_REQUESTED;
=======
		set_bit(__IXGBE_RESET_REQUESTED, &adapter->state);
		e_warn(drv, "initiating reset due to tx timeout\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ixgbe_service_event_schedule(adapter);
	}
}

/**
<<<<<<< HEAD
 * ixgbe_clean_tx_irq - Reclaim resources after transmit completes
 * @q_vector: structure containing interrupt and ring information
 * @tx_ring: tx ring to clean
 **/
static bool ixgbe_clean_tx_irq(struct ixgbe_q_vector *q_vector,
			       struct ixgbe_ring *tx_ring)
=======
 * ixgbe_tx_maxrate - callback to set the maximum per-queue bitrate
 * @netdev: network interface device structure
 * @queue_index: Tx queue to set
 * @maxrate: desired maximum transmit bitrate
 **/
static int ixgbe_tx_maxrate(struct net_device *netdev,
			    int queue_index, u32 maxrate)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	struct ixgbe_hw *hw = &adapter->hw;
	u32 bcnrc_val = ixgbe_link_mbps(adapter);

	if (!maxrate)
		return 0;

	/* Calculate the rate factor values to set */
	bcnrc_val <<= IXGBE_RTTBCNRC_RF_INT_SHIFT;
	bcnrc_val /= maxrate;

	/* clear everything but the rate factor */
	bcnrc_val &= IXGBE_RTTBCNRC_RF_INT_MASK |
	IXGBE_RTTBCNRC_RF_DEC_MASK;

	/* enable the rate scheduler */
	bcnrc_val |= IXGBE_RTTBCNRC_RS_ENA;

	IXGBE_WRITE_REG(hw, IXGBE_RTTDQSEL, queue_index);
	IXGBE_WRITE_REG(hw, IXGBE_RTTBCNRC, bcnrc_val);

	return 0;
}

/**
 * ixgbe_update_tx_ring_stats - Update Tx ring specific counters
 * @tx_ring: ring to update
 * @q_vector: queue vector ring belongs to
 * @pkts: number of processed packets
 * @bytes: number of processed bytes
 */
void ixgbe_update_tx_ring_stats(struct ixgbe_ring *tx_ring,
				struct ixgbe_q_vector *q_vector, u64 pkts,
				u64 bytes)
{
	u64_stats_update_begin(&tx_ring->syncp);
	tx_ring->stats.bytes += bytes;
	tx_ring->stats.packets += pkts;
	u64_stats_update_end(&tx_ring->syncp);
	q_vector->tx.total_bytes += bytes;
	q_vector->tx.total_packets += pkts;
}

/**
 * ixgbe_update_rx_ring_stats - Update Rx ring specific counters
 * @rx_ring: ring to update
 * @q_vector: queue vector ring belongs to
 * @pkts: number of processed packets
 * @bytes: number of processed bytes
 */
void ixgbe_update_rx_ring_stats(struct ixgbe_ring *rx_ring,
				struct ixgbe_q_vector *q_vector, u64 pkts,
				u64 bytes)
{
	u64_stats_update_begin(&rx_ring->syncp);
	rx_ring->stats.bytes += bytes;
	rx_ring->stats.packets += pkts;
	u64_stats_update_end(&rx_ring->syncp);
	q_vector->rx.total_bytes += bytes;
	q_vector->rx.total_packets += pkts;
}

/**
 * ixgbe_clean_tx_irq - Reclaim resources after transmit completes
 * @q_vector: structure containing interrupt and ring information
 * @tx_ring: tx ring to clean
 * @napi_budget: Used to determine if we are in netpoll
 **/
static bool ixgbe_clean_tx_irq(struct ixgbe_q_vector *q_vector,
			       struct ixgbe_ring *tx_ring, int napi_budget)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ixgbe_adapter *adapter = q_vector->adapter;
	struct ixgbe_tx_buffer *tx_buffer;
	union ixgbe_adv_tx_desc *tx_desc;
<<<<<<< HEAD
	unsigned int total_bytes = 0, total_packets = 0;
	unsigned int budget = q_vector->tx.work_limit;
	unsigned int i = tx_ring->next_to_clean;
=======
	unsigned int total_bytes = 0, total_packets = 0, total_ipsec = 0;
	unsigned int budget = q_vector->tx.work_limit;
	unsigned int i = tx_ring->next_to_clean;
	struct netdev_queue *txq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (test_bit(__IXGBE_DOWN, &adapter->state))
		return true;

	tx_buffer = &tx_ring->tx_buffer_info[i];
	tx_desc = IXGBE_TX_DESC(tx_ring, i);
	i -= tx_ring->count;

	do {
		union ixgbe_adv_tx_desc *eop_desc = tx_buffer->next_to_watch;

		/* if next_to_watch is not set then there is no work pending */
		if (!eop_desc)
			break;

		/* prevent any other reads prior to eop_desc */
<<<<<<< HEAD
		rmb();
=======
		smp_rmb();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* if DD is not set pending work has not been completed */
		if (!(eop_desc->wb.status & cpu_to_le32(IXGBE_TXD_STAT_DD)))
			break;

		/* clear next_to_watch to prevent false hangs */
		tx_buffer->next_to_watch = NULL;

		/* update the statistics for this packet */
		total_bytes += tx_buffer->bytecount;
		total_packets += tx_buffer->gso_segs;
<<<<<<< HEAD

		/* free the skb */
		dev_kfree_skb_any(tx_buffer->skb);
=======
		if (tx_buffer->tx_flags & IXGBE_TX_FLAGS_IPSEC)
			total_ipsec++;

		/* free the skb */
		if (ring_is_xdp(tx_ring))
			xdp_return_frame(tx_buffer->xdpf);
		else
			napi_consume_skb(tx_buffer->skb, napi_budget);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* unmap skb header data */
		dma_unmap_single(tx_ring->dev,
				 dma_unmap_addr(tx_buffer, dma),
				 dma_unmap_len(tx_buffer, len),
				 DMA_TO_DEVICE);

		/* clear tx_buffer data */
<<<<<<< HEAD
		tx_buffer->skb = NULL;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dma_unmap_len_set(tx_buffer, len, 0);

		/* unmap remaining buffers */
		while (tx_desc != eop_desc) {
			tx_buffer++;
			tx_desc++;
			i++;
			if (unlikely(!i)) {
				i -= tx_ring->count;
				tx_buffer = tx_ring->tx_buffer_info;
				tx_desc = IXGBE_TX_DESC(tx_ring, 0);
			}

			/* unmap any remaining paged data */
			if (dma_unmap_len(tx_buffer, len)) {
				dma_unmap_page(tx_ring->dev,
					       dma_unmap_addr(tx_buffer, dma),
					       dma_unmap_len(tx_buffer, len),
					       DMA_TO_DEVICE);
				dma_unmap_len_set(tx_buffer, len, 0);
			}
		}

		/* move us one more past the eop_desc for start of next pkt */
		tx_buffer++;
		tx_desc++;
		i++;
		if (unlikely(!i)) {
			i -= tx_ring->count;
			tx_buffer = tx_ring->tx_buffer_info;
			tx_desc = IXGBE_TX_DESC(tx_ring, 0);
		}

		/* issue prefetch for next Tx descriptor */
		prefetch(tx_desc);

		/* update budget accounting */
		budget--;
	} while (likely(budget));

	i += tx_ring->count;
	tx_ring->next_to_clean = i;
<<<<<<< HEAD
	u64_stats_update_begin(&tx_ring->syncp);
	tx_ring->stats.bytes += total_bytes;
	tx_ring->stats.packets += total_packets;
	u64_stats_update_end(&tx_ring->syncp);
	q_vector->tx.total_bytes += total_bytes;
	q_vector->tx.total_packets += total_packets;
=======
	ixgbe_update_tx_ring_stats(tx_ring, q_vector, total_packets,
				   total_bytes);
	adapter->tx_ipsec += total_ipsec;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (check_for_tx_hang(tx_ring) && ixgbe_check_tx_hang(tx_ring)) {
		/* schedule immediate reset if we believe we hung */
		struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD
		e_err(drv, "Detected Tx Unit Hang\n"
=======
		e_err(drv, "Detected Tx Unit Hang %s\n"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			"  Tx Queue             <%d>\n"
			"  TDH, TDT             <%x>, <%x>\n"
			"  next_to_use          <%x>\n"
			"  next_to_clean        <%x>\n"
			"tx_buffer_info[next_to_clean]\n"
			"  time_stamp           <%lx>\n"
			"  jiffies              <%lx>\n",
<<<<<<< HEAD
=======
			ring_is_xdp(tx_ring) ? "(XDP)" : "",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			tx_ring->queue_index,
			IXGBE_READ_REG(hw, IXGBE_TDH(tx_ring->reg_idx)),
			IXGBE_READ_REG(hw, IXGBE_TDT(tx_ring->reg_idx)),
			tx_ring->next_to_use, i,
			tx_ring->tx_buffer_info[i].time_stamp, jiffies);

<<<<<<< HEAD
		netif_stop_subqueue(tx_ring->netdev, tx_ring->queue_index);
=======
		if (!ring_is_xdp(tx_ring))
			netif_stop_subqueue(tx_ring->netdev,
					    tx_ring->queue_index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		e_info(probe,
		       "tx hang %d detected on queue %d, resetting adapter\n",
			adapter->tx_timeout_count + 1, tx_ring->queue_index);

		/* schedule immediate reset if we believe we hung */
		ixgbe_tx_timeout_reset(adapter);

		/* the adapter is about to reset, no point in enabling stuff */
		return true;
	}

<<<<<<< HEAD
	netdev_tx_completed_queue(txring_txq(tx_ring),
				  total_packets, total_bytes);

#define TX_WAKE_THRESHOLD (DESC_NEEDED * 2)
	if (unlikely(total_packets && netif_carrier_ok(tx_ring->netdev) &&
		     (ixgbe_desc_unused(tx_ring) >= TX_WAKE_THRESHOLD))) {
		/* Make sure that anybody stopping the queue after this
		 * sees the new next_to_clean.
		 */
		smp_mb();
		if (__netif_subqueue_stopped(tx_ring->netdev,
					     tx_ring->queue_index)
		    && !test_bit(__IXGBE_DOWN, &adapter->state)) {
			netif_wake_subqueue(tx_ring->netdev,
					    tx_ring->queue_index);
			++tx_ring->tx_stats.restart_queue;
		}
	}
=======
	if (ring_is_xdp(tx_ring))
		return !!budget;

#define TX_WAKE_THRESHOLD (DESC_NEEDED * 2)
	txq = netdev_get_tx_queue(tx_ring->netdev, tx_ring->queue_index);
	if (!__netif_txq_completed_wake(txq, total_packets, total_bytes,
					ixgbe_desc_unused(tx_ring),
					TX_WAKE_THRESHOLD,
					!netif_carrier_ok(tx_ring->netdev) ||
					test_bit(__IXGBE_DOWN, &adapter->state)))
		++tx_ring->tx_stats.restart_queue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return !!budget;
}

#ifdef CONFIG_IXGBE_DCA
static void ixgbe_update_tx_dca(struct ixgbe_adapter *adapter,
				struct ixgbe_ring *tx_ring,
				int cpu)
{
	struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD
	u32 txctrl = dca3_get_tag(tx_ring->dev, cpu);
	u16 reg_offset;

=======
	u32 txctrl = 0;
	u16 reg_offset;

	if (adapter->flags & IXGBE_FLAG_DCA_ENABLED)
		txctrl = dca3_get_tag(tx_ring->dev, cpu);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		reg_offset = IXGBE_DCA_TXCTRL(tx_ring->reg_idx);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
		reg_offset = IXGBE_DCA_TXCTRL_82599(tx_ring->reg_idx);
		txctrl <<= IXGBE_DCA_TXCTRL_CPUID_SHIFT_82599;
		break;
	default:
		/* for unknown hardware do not write register */
		return;
	}

	/*
	 * We can enable relaxed ordering for reads, but not writes when
	 * DCA is enabled.  This is due to a known issue in some chipsets
	 * which will cause the DCA tag to be cleared.
	 */
	txctrl |= IXGBE_DCA_TXCTRL_DESC_RRO_EN |
		  IXGBE_DCA_TXCTRL_DATA_RRO_EN |
		  IXGBE_DCA_TXCTRL_DESC_DCA_EN;

	IXGBE_WRITE_REG(hw, reg_offset, txctrl);
}

static void ixgbe_update_rx_dca(struct ixgbe_adapter *adapter,
				struct ixgbe_ring *rx_ring,
				int cpu)
{
	struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD
	u32 rxctrl = dca3_get_tag(rx_ring->dev, cpu);
	u8 reg_idx = rx_ring->reg_idx;

=======
	u32 rxctrl = 0;
	u8 reg_idx = rx_ring->reg_idx;

	if (adapter->flags & IXGBE_FLAG_DCA_ENABLED)
		rxctrl = dca3_get_tag(rx_ring->dev, cpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (hw->mac.type) {
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
		rxctrl <<= IXGBE_DCA_RXCTRL_CPUID_SHIFT_82599;
		break;
	default:
		break;
	}

	/*
	 * We can enable relaxed ordering for reads, but not writes when
	 * DCA is enabled.  This is due to a known issue in some chipsets
	 * which will cause the DCA tag to be cleared.
	 */
	rxctrl |= IXGBE_DCA_RXCTRL_DESC_RRO_EN |
		  IXGBE_DCA_RXCTRL_DATA_DCA_EN |
		  IXGBE_DCA_RXCTRL_DESC_DCA_EN;

	IXGBE_WRITE_REG(hw, IXGBE_DCA_RXCTRL(reg_idx), rxctrl);
}

static void ixgbe_update_dca(struct ixgbe_q_vector *q_vector)
{
	struct ixgbe_adapter *adapter = q_vector->adapter;
	struct ixgbe_ring *ring;
	int cpu = get_cpu();

	if (q_vector->cpu == cpu)
		goto out_no_update;

	ixgbe_for_each_ring(ring, q_vector->tx)
		ixgbe_update_tx_dca(adapter, ring, cpu);

	ixgbe_for_each_ring(ring, q_vector->rx)
		ixgbe_update_rx_dca(adapter, ring, cpu);

	q_vector->cpu = cpu;
out_no_update:
	put_cpu();
}

static void ixgbe_setup_dca(struct ixgbe_adapter *adapter)
{
<<<<<<< HEAD
	int num_q_vectors;
	int i;

	if (!(adapter->flags & IXGBE_FLAG_DCA_ENABLED))
		return;

	/* always use CB2 mode, difference is masked in the CB driver */
	IXGBE_WRITE_REG(&adapter->hw, IXGBE_DCA_CTRL, 2);

	if (adapter->flags & IXGBE_FLAG_MSIX_ENABLED)
		num_q_vectors = adapter->num_msix_vectors - NON_Q_VECTORS;
	else
		num_q_vectors = 1;

	for (i = 0; i < num_q_vectors; i++) {
=======
	int i;

	/* always use CB2 mode, difference is masked in the CB driver */
	if (adapter->flags & IXGBE_FLAG_DCA_ENABLED)
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_DCA_CTRL,
				IXGBE_DCA_CTRL_DCA_MODE_CB2);
	else
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_DCA_CTRL,
				IXGBE_DCA_CTRL_DCA_DISABLE);

	for (i = 0; i < adapter->num_q_vectors; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		adapter->q_vector[i]->cpu = -1;
		ixgbe_update_dca(adapter->q_vector[i]);
	}
}

static int __ixgbe_notify_dca(struct device *dev, void *data)
{
	struct ixgbe_adapter *adapter = dev_get_drvdata(dev);
	unsigned long event = *(unsigned long *)data;

	if (!(adapter->flags & IXGBE_FLAG_DCA_CAPABLE))
		return 0;

	switch (event) {
	case DCA_PROVIDER_ADD:
		/* if we're already enabled, don't do it again */
		if (adapter->flags & IXGBE_FLAG_DCA_ENABLED)
			break;
		if (dca_add_requester(dev) == 0) {
			adapter->flags |= IXGBE_FLAG_DCA_ENABLED;
<<<<<<< HEAD
			ixgbe_setup_dca(adapter);
			break;
		}
		/* Fall Through since DCA is disabled. */
=======
			IXGBE_WRITE_REG(&adapter->hw, IXGBE_DCA_CTRL,
					IXGBE_DCA_CTRL_DCA_MODE_CB2);
			break;
		}
		fallthrough; /* DCA is disabled. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case DCA_PROVIDER_REMOVE:
		if (adapter->flags & IXGBE_FLAG_DCA_ENABLED) {
			dca_remove_requester(dev);
			adapter->flags &= ~IXGBE_FLAG_DCA_ENABLED;
<<<<<<< HEAD
			IXGBE_WRITE_REG(&adapter->hw, IXGBE_DCA_CTRL, 1);
=======
			IXGBE_WRITE_REG(&adapter->hw, IXGBE_DCA_CTRL,
					IXGBE_DCA_CTRL_DCA_DISABLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		break;
	}

	return 0;
}

#endif /* CONFIG_IXGBE_DCA */
<<<<<<< HEAD
=======

#define IXGBE_RSS_L4_TYPES_MASK \
	((1ul << IXGBE_RXDADV_RSSTYPE_IPV4_TCP) | \
	 (1ul << IXGBE_RXDADV_RSSTYPE_IPV4_UDP) | \
	 (1ul << IXGBE_RXDADV_RSSTYPE_IPV6_TCP) | \
	 (1ul << IXGBE_RXDADV_RSSTYPE_IPV6_UDP))

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void ixgbe_rx_hash(struct ixgbe_ring *ring,
				 union ixgbe_adv_rx_desc *rx_desc,
				 struct sk_buff *skb)
{
<<<<<<< HEAD
	if (ring->netdev->features & NETIF_F_RXHASH)
		skb->rxhash = le32_to_cpu(rx_desc->wb.lower.hi_dword.rss);
=======
	u16 rss_type;

	if (!(ring->netdev->features & NETIF_F_RXHASH))
		return;

	rss_type = le16_to_cpu(rx_desc->wb.lower.lo_dword.hs_rss.pkt_info) &
		   IXGBE_RXDADV_RSSTYPE_MASK;

	if (!rss_type)
		return;

	skb_set_hash(skb, le32_to_cpu(rx_desc->wb.lower.hi_dword.rss),
		     (IXGBE_RSS_L4_TYPES_MASK & (1ul << rss_type)) ?
		     PKT_HASH_TYPE_L4 : PKT_HASH_TYPE_L3);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef IXGBE_FCOE
/**
 * ixgbe_rx_is_fcoe - check the rx desc for incoming pkt type
 * @ring: structure containing ring specific data
 * @rx_desc: advanced rx descriptor
 *
 * Returns : true if it is FCoE pkt
 */
static inline bool ixgbe_rx_is_fcoe(struct ixgbe_ring *ring,
				    union ixgbe_adv_rx_desc *rx_desc)
{
	__le16 pkt_info = rx_desc->wb.lower.lo_dword.hs_rss.pkt_info;

	return test_bit(__IXGBE_RX_FCOE, &ring->state) &&
	       ((pkt_info & cpu_to_le16(IXGBE_RXDADV_PKTTYPE_ETQF_MASK)) ==
		(cpu_to_le16(IXGBE_ETQF_FILTER_FCOE <<
			     IXGBE_RXDADV_PKTTYPE_ETQF_SHIFT)));
}

#endif /* IXGBE_FCOE */
/**
 * ixgbe_rx_checksum - indicate in skb if hw indicated a good cksum
 * @ring: structure containing ring specific data
 * @rx_desc: current Rx descriptor being processed
 * @skb: skb currently being received and modified
 **/
static inline void ixgbe_rx_checksum(struct ixgbe_ring *ring,
				     union ixgbe_adv_rx_desc *rx_desc,
				     struct sk_buff *skb)
{
<<<<<<< HEAD
=======
	__le16 pkt_info = rx_desc->wb.lower.lo_dword.hs_rss.pkt_info;
	bool encap_pkt = false;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	skb_checksum_none_assert(skb);

	/* Rx csum disabled */
	if (!(ring->netdev->features & NETIF_F_RXCSUM))
		return;

<<<<<<< HEAD
=======
	/* check for VXLAN and Geneve packets */
	if (pkt_info & cpu_to_le16(IXGBE_RXDADV_PKTTYPE_VXLAN)) {
		encap_pkt = true;
		skb->encapsulation = 1;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* if IP and error */
	if (ixgbe_test_staterr(rx_desc, IXGBE_RXD_STAT_IPCS) &&
	    ixgbe_test_staterr(rx_desc, IXGBE_RXDADV_ERR_IPE)) {
		ring->rx_stats.csum_err++;
		return;
	}

	if (!ixgbe_test_staterr(rx_desc, IXGBE_RXD_STAT_L4CS))
		return;

	if (ixgbe_test_staterr(rx_desc, IXGBE_RXDADV_ERR_TCPE)) {
<<<<<<< HEAD
		__le16 pkt_info = rx_desc->wb.lower.lo_dword.hs_rss.pkt_info;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * 82599 errata, UDP frames with a 0 checksum can be marked as
		 * checksum errors.
		 */
		if ((pkt_info & cpu_to_le16(IXGBE_RXDADV_PKTTYPE_UDP)) &&
		    test_bit(__IXGBE_RX_CSUM_UDP_ZERO_ERR, &ring->state))
			return;

		ring->rx_stats.csum_err++;
		return;
	}

	/* It must be a TCP or UDP packet with a valid checksum */
	skb->ip_summed = CHECKSUM_UNNECESSARY;
<<<<<<< HEAD
}

static inline void ixgbe_release_rx_desc(struct ixgbe_ring *rx_ring, u32 val)
{
	rx_ring->next_to_use = val;

	/* update next to alloc since we have filled the ring */
	rx_ring->next_to_alloc = val;
	/*
	 * Force memory writes to complete before letting h/w
	 * know there are new descriptors to fetch.  (Only
	 * applicable for weak-ordered memory model archs,
	 * such as IA-64).
	 */
	wmb();
	writel(val, rx_ring->tail);
=======
	if (encap_pkt) {
		if (!ixgbe_test_staterr(rx_desc, IXGBE_RXD_STAT_OUTERIPCS))
			return;

		if (ixgbe_test_staterr(rx_desc, IXGBE_RXDADV_ERR_OUTERIPER)) {
			skb->ip_summed = CHECKSUM_NONE;
			return;
		}
		/* If we checked the outer header let the stack know */
		skb->csum_level = 1;
	}
}

static unsigned int ixgbe_rx_offset(struct ixgbe_ring *rx_ring)
{
	return ring_uses_build_skb(rx_ring) ? IXGBE_SKB_PAD : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static bool ixgbe_alloc_mapped_page(struct ixgbe_ring *rx_ring,
				    struct ixgbe_rx_buffer *bi)
{
	struct page *page = bi->page;
<<<<<<< HEAD
	dma_addr_t dma = bi->dma;

	/* since we are recycling buffers we should seldom need to alloc */
	if (likely(dma))
		return true;

	/* alloc new page for storage */
	if (likely(!page)) {
		page = alloc_pages(GFP_ATOMIC | __GFP_COLD,
				   ixgbe_rx_pg_order(rx_ring));
		if (unlikely(!page)) {
			rx_ring->rx_stats.alloc_rx_page_failed++;
			return false;
		}
		bi->page = page;
	}

	/* map page for use */
	dma = dma_map_page(rx_ring->dev, page, 0,
			   ixgbe_rx_pg_size(rx_ring), DMA_FROM_DEVICE);
=======
	dma_addr_t dma;

	/* since we are recycling buffers we should seldom need to alloc */
	if (likely(page))
		return true;

	/* alloc new page for storage */
	page = dev_alloc_pages(ixgbe_rx_pg_order(rx_ring));
	if (unlikely(!page)) {
		rx_ring->rx_stats.alloc_rx_page_failed++;
		return false;
	}

	/* map page for use */
	dma = dma_map_page_attrs(rx_ring->dev, page, 0,
				 ixgbe_rx_pg_size(rx_ring),
				 DMA_FROM_DEVICE,
				 IXGBE_RX_DMA_ATTR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * if mapping failed free memory back to system since
	 * there isn't much point in holding memory we can't use
	 */
	if (dma_mapping_error(rx_ring->dev, dma)) {
<<<<<<< HEAD
		put_page(page);
		bi->page = NULL;
=======
		__free_pages(page, ixgbe_rx_pg_order(rx_ring));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		rx_ring->rx_stats.alloc_rx_page_failed++;
		return false;
	}

	bi->dma = dma;
<<<<<<< HEAD
	bi->page_offset ^= ixgbe_rx_bufsz(rx_ring);
=======
	bi->page = page;
	bi->page_offset = rx_ring->rx_offset;
	page_ref_add(page, USHRT_MAX - 1);
	bi->pagecnt_bias = USHRT_MAX;
	rx_ring->rx_stats.alloc_rx_page++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return true;
}

/**
 * ixgbe_alloc_rx_buffers - Replace used receive buffers
 * @rx_ring: ring to place buffers on
 * @cleaned_count: number of buffers to replace
 **/
void ixgbe_alloc_rx_buffers(struct ixgbe_ring *rx_ring, u16 cleaned_count)
{
	union ixgbe_adv_rx_desc *rx_desc;
	struct ixgbe_rx_buffer *bi;
	u16 i = rx_ring->next_to_use;
<<<<<<< HEAD
=======
	u16 bufsz;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* nothing to do */
	if (!cleaned_count)
		return;

	rx_desc = IXGBE_RX_DESC(rx_ring, i);
	bi = &rx_ring->rx_buffer_info[i];
	i -= rx_ring->count;

<<<<<<< HEAD
=======
	bufsz = ixgbe_rx_bufsz(rx_ring);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	do {
		if (!ixgbe_alloc_mapped_page(rx_ring, bi))
			break;

<<<<<<< HEAD
=======
		/* sync the buffer for use by the device */
		dma_sync_single_range_for_device(rx_ring->dev, bi->dma,
						 bi->page_offset, bufsz,
						 DMA_FROM_DEVICE);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Refresh the desc even if buffer_addrs didn't change
		 * because each write-back erases this info.
		 */
		rx_desc->read.pkt_addr = cpu_to_le64(bi->dma + bi->page_offset);

		rx_desc++;
		bi++;
		i++;
		if (unlikely(!i)) {
			rx_desc = IXGBE_RX_DESC(rx_ring, 0);
			bi = rx_ring->rx_buffer_info;
			i -= rx_ring->count;
		}

<<<<<<< HEAD
		/* clear the hdr_addr for the next_to_use descriptor */
		rx_desc->read.hdr_addr = 0;
=======
		/* clear the length for the next_to_use descriptor */
		rx_desc->wb.upper.length = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		cleaned_count--;
	} while (cleaned_count);

	i += rx_ring->count;

<<<<<<< HEAD
	if (rx_ring->next_to_use != i)
		ixgbe_release_rx_desc(rx_ring, i);
}

/**
 * ixgbe_get_headlen - determine size of header for RSC/LRO/GRO/FCOE
 * @data: pointer to the start of the headers
 * @max_len: total length of section to find headers in
 *
 * This function is meant to determine the length of headers that will
 * be recognized by hardware for LRO, GRO, and RSC offloads.  The main
 * motivation of doing this is to only perform one pull for IPv4 TCP
 * packets so that we can do basic things like calculating the gso_size
 * based on the average data per packet.
 **/
static unsigned int ixgbe_get_headlen(unsigned char *data,
				      unsigned int max_len)
{
	union {
		unsigned char *network;
		/* l2 headers */
		struct ethhdr *eth;
		struct vlan_hdr *vlan;
		/* l3 headers */
		struct iphdr *ipv4;
	} hdr;
	__be16 protocol;
	u8 nexthdr = 0;	/* default to not TCP */
	u8 hlen;

	/* this should never happen, but better safe than sorry */
	if (max_len < ETH_HLEN)
		return max_len;

	/* initialize network frame pointer */
	hdr.network = data;

	/* set first protocol and move network header forward */
	protocol = hdr.eth->h_proto;
	hdr.network += ETH_HLEN;

	/* handle any vlan tag if present */
	if (protocol == __constant_htons(ETH_P_8021Q)) {
		if ((hdr.network - data) > (max_len - VLAN_HLEN))
			return max_len;

		protocol = hdr.vlan->h_vlan_encapsulated_proto;
		hdr.network += VLAN_HLEN;
	}

	/* handle L3 protocols */
	if (protocol == __constant_htons(ETH_P_IP)) {
		if ((hdr.network - data) > (max_len - sizeof(struct iphdr)))
			return max_len;

		/* access ihl as a u8 to avoid unaligned access on ia64 */
		hlen = (hdr.network[0] & 0x0F) << 2;

		/* verify hlen meets minimum size requirements */
		if (hlen < sizeof(struct iphdr))
			return hdr.network - data;

		/* record next protocol */
		nexthdr = hdr.ipv4->protocol;
		hdr.network += hlen;
#ifdef IXGBE_FCOE
	} else if (protocol == __constant_htons(ETH_P_FCOE)) {
		if ((hdr.network - data) > (max_len - FCOE_HEADER_LEN))
			return max_len;
		hdr.network += FCOE_HEADER_LEN;
#endif
	} else {
		return hdr.network - data;
	}

	/* finally sort out TCP */
	if (nexthdr == IPPROTO_TCP) {
		if ((hdr.network - data) > (max_len - sizeof(struct tcphdr)))
			return max_len;

		/* access doff as a u8 to avoid unaligned access on ia64 */
		hlen = (hdr.network[12] & 0xF0) >> 2;

		/* verify hlen meets minimum size requirements */
		if (hlen < sizeof(struct tcphdr))
			return hdr.network - data;

		hdr.network += hlen;
	}

	/*
	 * If everything has gone correctly hdr.network should be the
	 * data section of the packet and will be the end of the header.
	 * If not then it probably represents the end of the last recognized
	 * header.
	 */
	if ((hdr.network - data) < max_len)
		return hdr.network - data;
	else
		return max_len;
}

static void ixgbe_get_rsc_cnt(struct ixgbe_ring *rx_ring,
			      union ixgbe_adv_rx_desc *rx_desc,
			      struct sk_buff *skb)
{
	__le32 rsc_enabled;
	u32 rsc_cnt;

	if (!ring_is_rsc_enabled(rx_ring))
		return;

	rsc_enabled = rx_desc->wb.lower.lo_dword.data &
		      cpu_to_le32(IXGBE_RXDADV_RSCCNT_MASK);

	/* If this is an RSC frame rsc_cnt should be non-zero */
	if (!rsc_enabled)
		return;

	rsc_cnt = le32_to_cpu(rsc_enabled);
	rsc_cnt >>= IXGBE_RXDADV_RSCCNT_SHIFT;

	IXGBE_CB(skb)->append_cnt += rsc_cnt - 1;
=======
	if (rx_ring->next_to_use != i) {
		rx_ring->next_to_use = i;

		/* update next to alloc since we have filled the ring */
		rx_ring->next_to_alloc = i;

		/* Force memory writes to complete before letting h/w
		 * know there are new descriptors to fetch.  (Only
		 * applicable for weak-ordered memory model archs,
		 * such as IA-64).
		 */
		wmb();
		writel(i, rx_ring->tail);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_set_rsc_gso_size(struct ixgbe_ring *ring,
				   struct sk_buff *skb)
{
	u16 hdr_len = skb_headlen(skb);

	/* set gso_size to avoid messing up TCP MSS */
	skb_shinfo(skb)->gso_size = DIV_ROUND_UP((skb->len - hdr_len),
						 IXGBE_CB(skb)->append_cnt);
<<<<<<< HEAD
=======
	skb_shinfo(skb)->gso_type = SKB_GSO_TCPV4;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_update_rsc_stats(struct ixgbe_ring *rx_ring,
				   struct sk_buff *skb)
{
	/* if append_cnt is 0 then frame is not RSC */
	if (!IXGBE_CB(skb)->append_cnt)
		return;

	rx_ring->rx_stats.rsc_count += IXGBE_CB(skb)->append_cnt;
	rx_ring->rx_stats.rsc_flush++;

	ixgbe_set_rsc_gso_size(rx_ring, skb);

	/* gso_size is computed using append_cnt so always clear it last */
	IXGBE_CB(skb)->append_cnt = 0;
}

/**
 * ixgbe_process_skb_fields - Populate skb header fields from Rx descriptor
 * @rx_ring: rx descriptor ring packet is being transacted on
 * @rx_desc: pointer to the EOP Rx descriptor
 * @skb: pointer to current skb being populated
 *
 * This function checks the ring, descriptor, and packet information in
 * order to populate the hash, checksum, VLAN, timestamp, protocol, and
 * other fields within the skb.
 **/
<<<<<<< HEAD
static void ixgbe_process_skb_fields(struct ixgbe_ring *rx_ring,
				     union ixgbe_adv_rx_desc *rx_desc,
				     struct sk_buff *skb)
{
=======
void ixgbe_process_skb_fields(struct ixgbe_ring *rx_ring,
			      union ixgbe_adv_rx_desc *rx_desc,
			      struct sk_buff *skb)
{
	struct net_device *dev = rx_ring->netdev;
	u32 flags = rx_ring->q_vector->adapter->flags;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ixgbe_update_rsc_stats(rx_ring, skb);

	ixgbe_rx_hash(rx_ring, rx_desc, skb);

	ixgbe_rx_checksum(rx_ring, rx_desc, skb);

<<<<<<< HEAD
	if (ixgbe_test_staterr(rx_desc, IXGBE_RXD_STAT_VP)) {
		u16 vid = le16_to_cpu(rx_desc->wb.upper.vlan);
		__vlan_hwaccel_put_tag(skb, vid);
	}

	skb_record_rx_queue(skb, rx_ring->queue_index);

	skb->protocol = eth_type_trans(skb, rx_ring->netdev);
}

static void ixgbe_rx_skb(struct ixgbe_q_vector *q_vector,
			 struct sk_buff *skb)
{
	struct ixgbe_adapter *adapter = q_vector->adapter;

	if (!(adapter->flags & IXGBE_FLAG_IN_NETPOLL))
		napi_gro_receive(&q_vector->napi, skb);
	else
		netif_rx(skb);
=======
	if (unlikely(flags & IXGBE_FLAG_RX_HWTSTAMP_ENABLED))
		ixgbe_ptp_rx_hwtstamp(rx_ring, rx_desc, skb);

	if ((dev->features & NETIF_F_HW_VLAN_CTAG_RX) &&
	    ixgbe_test_staterr(rx_desc, IXGBE_RXD_STAT_VP)) {
		u16 vid = le16_to_cpu(rx_desc->wb.upper.vlan);
		__vlan_hwaccel_put_tag(skb, htons(ETH_P_8021Q), vid);
	}

	if (ixgbe_test_staterr(rx_desc, IXGBE_RXDADV_STAT_SECP))
		ixgbe_ipsec_rx(rx_ring, rx_desc, skb);

	/* record Rx queue, or update MACVLAN statistics */
	if (netif_is_ixgbe(dev))
		skb_record_rx_queue(skb, rx_ring->queue_index);
	else
		macvlan_count_rx(netdev_priv(dev), skb->len + ETH_HLEN, true,
				 false);

	skb->protocol = eth_type_trans(skb, dev);
}

void ixgbe_rx_skb(struct ixgbe_q_vector *q_vector,
		  struct sk_buff *skb)
{
	napi_gro_receive(&q_vector->napi, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_is_non_eop - process handling of non-EOP buffers
 * @rx_ring: Rx ring being processed
 * @rx_desc: Rx descriptor for current buffer
 * @skb: Current socket buffer containing buffer in progress
 *
 * This function updates next to clean.  If the buffer is an EOP buffer
 * this function exits returning false, otherwise it will place the
 * sk_buff in the next buffer to be chained and return true indicating
 * that this is in fact a non-EOP buffer.
 **/
static bool ixgbe_is_non_eop(struct ixgbe_ring *rx_ring,
			     union ixgbe_adv_rx_desc *rx_desc,
			     struct sk_buff *skb)
{
	u32 ntc = rx_ring->next_to_clean + 1;

	/* fetch, update, and store next to clean */
	ntc = (ntc < rx_ring->count) ? ntc : 0;
	rx_ring->next_to_clean = ntc;

	prefetch(IXGBE_RX_DESC(rx_ring, ntc));

<<<<<<< HEAD
	if (likely(ixgbe_test_staterr(rx_desc, IXGBE_RXD_STAT_EOP)))
		return false;

	/* append_cnt indicates packet is RSC, if so fetch nextp */
	if (IXGBE_CB(skb)->append_cnt) {
		ntc = le32_to_cpu(rx_desc->wb.upper.status_error);
		ntc &= IXGBE_RXDADV_NEXTP_MASK;
		ntc >>= IXGBE_RXDADV_NEXTP_SHIFT;
	}

=======
	/* update RSC append count if present */
	if (ring_is_rsc_enabled(rx_ring)) {
		__le32 rsc_enabled = rx_desc->wb.lower.lo_dword.data &
				     cpu_to_le32(IXGBE_RXDADV_RSCCNT_MASK);

		if (unlikely(rsc_enabled)) {
			u32 rsc_cnt = le32_to_cpu(rsc_enabled);

			rsc_cnt >>= IXGBE_RXDADV_RSCCNT_SHIFT;
			IXGBE_CB(skb)->append_cnt += rsc_cnt - 1;

			/* update ntc based on RSC value */
			ntc = le32_to_cpu(rx_desc->wb.upper.status_error);
			ntc &= IXGBE_RXDADV_NEXTP_MASK;
			ntc >>= IXGBE_RXDADV_NEXTP_SHIFT;
		}
	}

	/* if we are the last buffer then there is nothing else to do */
	if (likely(ixgbe_test_staterr(rx_desc, IXGBE_RXD_STAT_EOP)))
		return false;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* place skb in next buffer to be received */
	rx_ring->rx_buffer_info[ntc].skb = skb;
	rx_ring->rx_stats.non_eop_descs++;

	return true;
}

/**
<<<<<<< HEAD
=======
 * ixgbe_pull_tail - ixgbe specific version of skb_pull_tail
 * @rx_ring: rx descriptor ring packet is being transacted on
 * @skb: pointer to current skb being adjusted
 *
 * This function is an ixgbe specific version of __pskb_pull_tail.  The
 * main difference between this version and the original function is that
 * this function can make several assumptions about the state of things
 * that allow for significant optimizations versus the standard function.
 * As a result we can do things like drop a frag and maintain an accurate
 * truesize for the skb.
 */
static void ixgbe_pull_tail(struct ixgbe_ring *rx_ring,
			    struct sk_buff *skb)
{
	skb_frag_t *frag = &skb_shinfo(skb)->frags[0];
	unsigned char *va;
	unsigned int pull_len;

	/*
	 * it is valid to use page_address instead of kmap since we are
	 * working with pages allocated out of the lomem pool per
	 * alloc_page(GFP_ATOMIC)
	 */
	va = skb_frag_address(frag);

	/*
	 * we need the header to contain the greater of either ETH_HLEN or
	 * 60 bytes if the skb->len is less than 60 for skb_pad.
	 */
	pull_len = eth_get_headlen(skb->dev, va, IXGBE_RX_HDR_SIZE);

	/* align pull length to size of long to optimize memcpy performance */
	skb_copy_to_linear_data(skb, va, ALIGN(pull_len, sizeof(long)));

	/* update all of the pointers */
	skb_frag_size_sub(frag, pull_len);
	skb_frag_off_add(frag, pull_len);
	skb->data_len -= pull_len;
	skb->tail += pull_len;
}

/**
 * ixgbe_dma_sync_frag - perform DMA sync for first frag of SKB
 * @rx_ring: rx descriptor ring packet is being transacted on
 * @skb: pointer to current skb being updated
 *
 * This function provides a basic DMA sync up for the first fragment of an
 * skb.  The reason for doing this is that the first fragment cannot be
 * unmapped until we have reached the end of packet descriptor for a buffer
 * chain.
 */
static void ixgbe_dma_sync_frag(struct ixgbe_ring *rx_ring,
				struct sk_buff *skb)
{
	if (ring_uses_build_skb(rx_ring)) {
		unsigned long mask = (unsigned long)ixgbe_rx_pg_size(rx_ring) - 1;
		unsigned long offset = (unsigned long)(skb->data) & mask;

		dma_sync_single_range_for_cpu(rx_ring->dev,
					      IXGBE_CB(skb)->dma,
					      offset,
					      skb_headlen(skb),
					      DMA_FROM_DEVICE);
	} else {
		skb_frag_t *frag = &skb_shinfo(skb)->frags[0];

		dma_sync_single_range_for_cpu(rx_ring->dev,
					      IXGBE_CB(skb)->dma,
					      skb_frag_off(frag),
					      skb_frag_size(frag),
					      DMA_FROM_DEVICE);
	}

	/* If the page was released, just unmap it. */
	if (unlikely(IXGBE_CB(skb)->page_released)) {
		dma_unmap_page_attrs(rx_ring->dev, IXGBE_CB(skb)->dma,
				     ixgbe_rx_pg_size(rx_ring),
				     DMA_FROM_DEVICE,
				     IXGBE_RX_DMA_ATTR);
	}
}

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ixgbe_cleanup_headers - Correct corrupted or empty headers
 * @rx_ring: rx descriptor ring packet is being transacted on
 * @rx_desc: pointer to the EOP Rx descriptor
 * @skb: pointer to current skb being fixed
 *
<<<<<<< HEAD
=======
 * Check if the skb is valid in the XDP case it will be an error pointer.
 * Return true in this case to abort processing and advance to next
 * descriptor.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Check for corrupted packet headers caused by senders on the local L2
 * embedded NIC switch not setting up their Tx Descriptors right.  These
 * should be very rare.
 *
 * Also address the case where we are pulling data in on pages only
 * and as such no data is present in the skb header.
 *
 * In addition if skb is not at least 60 bytes we need to pad it so that
 * it is large enough to qualify as a valid Ethernet frame.
 *
 * Returns true if an error was encountered and skb was freed.
 **/
<<<<<<< HEAD
static bool ixgbe_cleanup_headers(struct ixgbe_ring *rx_ring,
				  union ixgbe_adv_rx_desc *rx_desc,
				  struct sk_buff *skb)
{
	struct skb_frag_struct *frag = &skb_shinfo(skb)->frags[0];
	struct net_device *netdev = rx_ring->netdev;
	unsigned char *va;
	unsigned int pull_len;

	/* if the page was released unmap it, else just sync our portion */
	if (unlikely(IXGBE_CB(skb)->page_released)) {
		dma_unmap_page(rx_ring->dev, IXGBE_CB(skb)->dma,
			       ixgbe_rx_pg_size(rx_ring), DMA_FROM_DEVICE);
		IXGBE_CB(skb)->page_released = false;
	} else {
		dma_sync_single_range_for_cpu(rx_ring->dev,
					      IXGBE_CB(skb)->dma,
					      frag->page_offset,
					      ixgbe_rx_bufsz(rx_ring),
					      DMA_FROM_DEVICE);
	}
	IXGBE_CB(skb)->dma = 0;

	/* verify that the packet does not have any known errors */
	if (unlikely(ixgbe_test_staterr(rx_desc,
					IXGBE_RXDADV_ERR_FRAME_ERR_MASK) &&
	    !(netdev->features & NETIF_F_RXALL))) {
=======
bool ixgbe_cleanup_headers(struct ixgbe_ring *rx_ring,
			   union ixgbe_adv_rx_desc *rx_desc,
			   struct sk_buff *skb)
{
	struct net_device *netdev = rx_ring->netdev;

	/* XDP packets use error pointer so abort at this point */
	if (IS_ERR(skb))
		return true;

	/* Verify netdev is present, and that packet does not have any
	 * errors that would be unacceptable to the netdev.
	 */
	if (!netdev ||
	    (unlikely(ixgbe_test_staterr(rx_desc,
					 IXGBE_RXDADV_ERR_FRAME_ERR_MASK) &&
	     !(netdev->features & NETIF_F_RXALL)))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_kfree_skb_any(skb);
		return true;
	}

<<<<<<< HEAD
	/*
	 * it is valid to use page_address instead of kmap since we are
	 * working with pages allocated out of the lomem pool per
	 * alloc_page(GFP_ATOMIC)
	 */
	va = skb_frag_address(frag);

	/*
	 * we need the header to contain the greater of either ETH_HLEN or
	 * 60 bytes if the skb->len is less than 60 for skb_pad.
	 */
	pull_len = skb_frag_size(frag);
	if (pull_len > 256)
		pull_len = ixgbe_get_headlen(va, pull_len);

	/* align pull length to size of long to optimize memcpy performance */
	skb_copy_to_linear_data(skb, va, ALIGN(pull_len, sizeof(long)));

	/* update all of the pointers */
	skb_frag_size_sub(frag, pull_len);
	frag->page_offset += pull_len;
	skb->data_len -= pull_len;
	skb->tail += pull_len;

	/*
	 * if we sucked the frag empty then we should free it,
	 * if there are other frags here something is screwed up in hardware
	 */
	if (skb_frag_size(frag) == 0) {
		BUG_ON(skb_shinfo(skb)->nr_frags != 1);
		skb_shinfo(skb)->nr_frags = 0;
		__skb_frag_unref(frag);
		skb->truesize -= ixgbe_rx_bufsz(rx_ring);
	}
=======
	/* place header in linear portion of buffer */
	if (!skb_headlen(skb))
		ixgbe_pull_tail(rx_ring, skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef IXGBE_FCOE
	/* do not attempt to pad FCoE Frames as this will disrupt DDP */
	if (ixgbe_rx_is_fcoe(rx_ring, rx_desc))
		return false;

#endif
<<<<<<< HEAD
	/* if skb_pad returns an error the skb was freed */
	if (unlikely(skb->len < 60)) {
		int pad_len = 60 - skb->len;

		if (skb_pad(skb, pad_len))
			return true;
		__skb_put(skb, pad_len);
	}
=======
	/* if eth_skb_pad returns an error the skb was freed */
	if (eth_skb_pad(skb))
		return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return false;
}

/**
<<<<<<< HEAD
 * ixgbe_can_reuse_page - determine if we can reuse a page
 * @rx_buffer: pointer to rx_buffer containing the page we want to reuse
 *
 * Returns true if page can be reused in another Rx buffer
 **/
static inline bool ixgbe_can_reuse_page(struct ixgbe_rx_buffer *rx_buffer)
{
	struct page *page = rx_buffer->page;

	/* if we are only owner of page and it is local we can reuse it */
	return likely(page_count(page) == 1) &&
	       likely(page_to_nid(page) == numa_node_id());
}

/**
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ixgbe_reuse_rx_page - page flip buffer and store it back on the ring
 * @rx_ring: rx descriptor ring to store buffers on
 * @old_buff: donor buffer to have page reused
 *
<<<<<<< HEAD
 * Syncronizes page for reuse by the adapter
=======
 * Synchronizes page for reuse by the adapter
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void ixgbe_reuse_rx_page(struct ixgbe_ring *rx_ring,
				struct ixgbe_rx_buffer *old_buff)
{
	struct ixgbe_rx_buffer *new_buff;
	u16 nta = rx_ring->next_to_alloc;
<<<<<<< HEAD
	u16 bufsz = ixgbe_rx_bufsz(rx_ring);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	new_buff = &rx_ring->rx_buffer_info[nta];

	/* update, and store next to alloc */
	nta++;
	rx_ring->next_to_alloc = (nta < rx_ring->count) ? nta : 0;

<<<<<<< HEAD
	/* transfer page from old buffer to new buffer */
	new_buff->page = old_buff->page;
	new_buff->dma = old_buff->dma;

	/* flip page offset to other buffer and store to new_buff */
	new_buff->page_offset = old_buff->page_offset ^ bufsz;

	/* sync the buffer for use by the device */
	dma_sync_single_range_for_device(rx_ring->dev, new_buff->dma,
					 new_buff->page_offset, bufsz,
					 DMA_FROM_DEVICE);

	/* bump ref count on page before it is given to the stack */
	get_page(new_buff->page);
=======
	/* Transfer page from old buffer to new buffer.
	 * Move each member individually to avoid possible store
	 * forwarding stalls and unnecessary copy of skb.
	 */
	new_buff->dma		= old_buff->dma;
	new_buff->page		= old_buff->page;
	new_buff->page_offset	= old_buff->page_offset;
	new_buff->pagecnt_bias	= old_buff->pagecnt_bias;
}

static bool ixgbe_can_reuse_rx_page(struct ixgbe_rx_buffer *rx_buffer,
				    int rx_buffer_pgcnt)
{
	unsigned int pagecnt_bias = rx_buffer->pagecnt_bias;
	struct page *page = rx_buffer->page;

	/* avoid re-using remote and pfmemalloc pages */
	if (!dev_page_is_reusable(page))
		return false;

#if (PAGE_SIZE < 8192)
	/* if we are only owner of page we can reuse it */
	if (unlikely((rx_buffer_pgcnt - pagecnt_bias) > 1))
		return false;
#else
	/* The last offset is a bit aggressive in that we assume the
	 * worst case of FCoE being enabled and using a 3K buffer.
	 * However this should have minimal impact as the 1K extra is
	 * still less than one buffer in size.
	 */
#define IXGBE_LAST_OFFSET \
	(SKB_WITH_OVERHEAD(PAGE_SIZE) - IXGBE_RXBUFFER_3K)
	if (rx_buffer->page_offset > IXGBE_LAST_OFFSET)
		return false;
#endif

	/* If we have drained the page fragment pool we need to update
	 * the pagecnt_bias and page count so that we fully restock the
	 * number of references the driver holds.
	 */
	if (unlikely(pagecnt_bias == 1)) {
		page_ref_add(page, USHRT_MAX - 1);
		rx_buffer->pagecnt_bias = USHRT_MAX;
	}

	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_add_rx_frag - Add contents of Rx buffer to sk_buff
 * @rx_ring: rx descriptor ring to transact packets on
 * @rx_buffer: buffer containing page to add
<<<<<<< HEAD
 * @rx_desc: descriptor containing length of buffer written by hardware
 * @skb: sk_buff to place the data into
 *
 * This function is based on skb_add_rx_frag.  I would have used that
 * function however it doesn't handle the truesize case correctly since we
 * are allocating more memory than might be used for a single receive.
 **/
static void ixgbe_add_rx_frag(struct ixgbe_ring *rx_ring,
			      struct ixgbe_rx_buffer *rx_buffer,
			      struct sk_buff *skb, int size)
{
	skb_fill_page_desc(skb, skb_shinfo(skb)->nr_frags,
			   rx_buffer->page, rx_buffer->page_offset,
			   size);
	skb->len += size;
	skb->data_len += size;
	skb->truesize += ixgbe_rx_bufsz(rx_ring);
=======
 * @skb: sk_buff to place the data into
 * @size: size of data in rx_buffer
 *
 * This function will add the data contained in rx_buffer->page to the skb.
 * This is done either through a direct copy if the data in the buffer is
 * less than the skb header size, otherwise it will just attach the page as
 * a frag to the skb.
 *
 * The function will then update the page offset if necessary and return
 * true if the buffer can be reused by the adapter.
 **/
static void ixgbe_add_rx_frag(struct ixgbe_ring *rx_ring,
			      struct ixgbe_rx_buffer *rx_buffer,
			      struct sk_buff *skb,
			      unsigned int size)
{
#if (PAGE_SIZE < 8192)
	unsigned int truesize = ixgbe_rx_pg_size(rx_ring) / 2;
#else
	unsigned int truesize = rx_ring->rx_offset ?
				SKB_DATA_ALIGN(rx_ring->rx_offset + size) :
				SKB_DATA_ALIGN(size);
#endif
	skb_add_rx_frag(skb, skb_shinfo(skb)->nr_frags, rx_buffer->page,
			rx_buffer->page_offset, size, truesize);
#if (PAGE_SIZE < 8192)
	rx_buffer->page_offset ^= truesize;
#else
	rx_buffer->page_offset += truesize;
#endif
}

static struct ixgbe_rx_buffer *ixgbe_get_rx_buffer(struct ixgbe_ring *rx_ring,
						   union ixgbe_adv_rx_desc *rx_desc,
						   struct sk_buff **skb,
						   const unsigned int size,
						   int *rx_buffer_pgcnt)
{
	struct ixgbe_rx_buffer *rx_buffer;

	rx_buffer = &rx_ring->rx_buffer_info[rx_ring->next_to_clean];
	*rx_buffer_pgcnt =
#if (PAGE_SIZE < 8192)
		page_count(rx_buffer->page);
#else
		0;
#endif
	prefetchw(rx_buffer->page);
	*skb = rx_buffer->skb;

	/* Delay unmapping of the first packet. It carries the header
	 * information, HW may still access the header after the writeback.
	 * Only unmap it when EOP is reached
	 */
	if (!ixgbe_test_staterr(rx_desc, IXGBE_RXD_STAT_EOP)) {
		if (!*skb)
			goto skip_sync;
	} else {
		if (*skb)
			ixgbe_dma_sync_frag(rx_ring, *skb);
	}

	/* we are reusing so sync this buffer for CPU use */
	dma_sync_single_range_for_cpu(rx_ring->dev,
				      rx_buffer->dma,
				      rx_buffer->page_offset,
				      size,
				      DMA_FROM_DEVICE);
skip_sync:
	rx_buffer->pagecnt_bias--;

	return rx_buffer;
}

static void ixgbe_put_rx_buffer(struct ixgbe_ring *rx_ring,
				struct ixgbe_rx_buffer *rx_buffer,
				struct sk_buff *skb,
				int rx_buffer_pgcnt)
{
	if (ixgbe_can_reuse_rx_page(rx_buffer, rx_buffer_pgcnt)) {
		/* hand second half of page back to the ring */
		ixgbe_reuse_rx_page(rx_ring, rx_buffer);
	} else {
		if (!IS_ERR(skb) && IXGBE_CB(skb)->dma == rx_buffer->dma) {
			/* the page has been released from the ring */
			IXGBE_CB(skb)->page_released = true;
		} else {
			/* we are not reusing the buffer so unmap it */
			dma_unmap_page_attrs(rx_ring->dev, rx_buffer->dma,
					     ixgbe_rx_pg_size(rx_ring),
					     DMA_FROM_DEVICE,
					     IXGBE_RX_DMA_ATTR);
		}
		__page_frag_cache_drain(rx_buffer->page,
					rx_buffer->pagecnt_bias);
	}

	/* clear contents of rx_buffer */
	rx_buffer->page = NULL;
	rx_buffer->skb = NULL;
}

static struct sk_buff *ixgbe_construct_skb(struct ixgbe_ring *rx_ring,
					   struct ixgbe_rx_buffer *rx_buffer,
					   struct xdp_buff *xdp,
					   union ixgbe_adv_rx_desc *rx_desc)
{
	unsigned int size = xdp->data_end - xdp->data;
#if (PAGE_SIZE < 8192)
	unsigned int truesize = ixgbe_rx_pg_size(rx_ring) / 2;
#else
	unsigned int truesize = SKB_DATA_ALIGN(xdp->data_end -
					       xdp->data_hard_start);
#endif
	struct sk_buff *skb;

	/* prefetch first cache line of first page */
	net_prefetch(xdp->data);

	/* Note, we get here by enabling legacy-rx via:
	 *
	 *    ethtool --set-priv-flags <dev> legacy-rx on
	 *
	 * In this mode, we currently get 0 extra XDP headroom as
	 * opposed to having legacy-rx off, where we process XDP
	 * packets going to stack via ixgbe_build_skb(). The latter
	 * provides us currently with 192 bytes of headroom.
	 *
	 * For ixgbe_construct_skb() mode it means that the
	 * xdp->data_meta will always point to xdp->data, since
	 * the helper cannot expand the head. Should this ever
	 * change in future for legacy-rx mode on, then lets also
	 * add xdp->data_meta handling here.
	 */

	/* allocate a skb to store the frags */
	skb = napi_alloc_skb(&rx_ring->q_vector->napi, IXGBE_RX_HDR_SIZE);
	if (unlikely(!skb))
		return NULL;

	if (size > IXGBE_RX_HDR_SIZE) {
		if (!ixgbe_test_staterr(rx_desc, IXGBE_RXD_STAT_EOP))
			IXGBE_CB(skb)->dma = rx_buffer->dma;

		skb_add_rx_frag(skb, 0, rx_buffer->page,
				xdp->data - page_address(rx_buffer->page),
				size, truesize);
#if (PAGE_SIZE < 8192)
		rx_buffer->page_offset ^= truesize;
#else
		rx_buffer->page_offset += truesize;
#endif
	} else {
		memcpy(__skb_put(skb, size),
		       xdp->data, ALIGN(size, sizeof(long)));
		rx_buffer->pagecnt_bias++;
	}

	return skb;
}

static struct sk_buff *ixgbe_build_skb(struct ixgbe_ring *rx_ring,
				       struct ixgbe_rx_buffer *rx_buffer,
				       struct xdp_buff *xdp,
				       union ixgbe_adv_rx_desc *rx_desc)
{
	unsigned int metasize = xdp->data - xdp->data_meta;
#if (PAGE_SIZE < 8192)
	unsigned int truesize = ixgbe_rx_pg_size(rx_ring) / 2;
#else
	unsigned int truesize = SKB_DATA_ALIGN(sizeof(struct skb_shared_info)) +
				SKB_DATA_ALIGN(xdp->data_end -
					       xdp->data_hard_start);
#endif
	struct sk_buff *skb;

	/* Prefetch first cache line of first page. If xdp->data_meta
	 * is unused, this points extactly as xdp->data, otherwise we
	 * likely have a consumer accessing first few bytes of meta
	 * data, and then actual data.
	 */
	net_prefetch(xdp->data_meta);

	/* build an skb to around the page buffer */
	skb = napi_build_skb(xdp->data_hard_start, truesize);
	if (unlikely(!skb))
		return NULL;

	/* update pointers within the skb to store the data */
	skb_reserve(skb, xdp->data - xdp->data_hard_start);
	__skb_put(skb, xdp->data_end - xdp->data);
	if (metasize)
		skb_metadata_set(skb, metasize);

	/* record DMA address if this is the start of a chain of buffers */
	if (!ixgbe_test_staterr(rx_desc, IXGBE_RXD_STAT_EOP))
		IXGBE_CB(skb)->dma = rx_buffer->dma;

	/* update buffer offset */
#if (PAGE_SIZE < 8192)
	rx_buffer->page_offset ^= truesize;
#else
	rx_buffer->page_offset += truesize;
#endif

	return skb;
}

static struct sk_buff *ixgbe_run_xdp(struct ixgbe_adapter *adapter,
				     struct ixgbe_ring *rx_ring,
				     struct xdp_buff *xdp)
{
	int err, result = IXGBE_XDP_PASS;
	struct bpf_prog *xdp_prog;
	struct ixgbe_ring *ring;
	struct xdp_frame *xdpf;
	u32 act;

	xdp_prog = READ_ONCE(rx_ring->xdp_prog);

	if (!xdp_prog)
		goto xdp_out;

	prefetchw(xdp->data_hard_start); /* xdp_frame write */

	act = bpf_prog_run_xdp(xdp_prog, xdp);
	switch (act) {
	case XDP_PASS:
		break;
	case XDP_TX:
		xdpf = xdp_convert_buff_to_frame(xdp);
		if (unlikely(!xdpf))
			goto out_failure;
		ring = ixgbe_determine_xdp_ring(adapter);
		if (static_branch_unlikely(&ixgbe_xdp_locking_key))
			spin_lock(&ring->tx_lock);
		result = ixgbe_xmit_xdp_ring(ring, xdpf);
		if (static_branch_unlikely(&ixgbe_xdp_locking_key))
			spin_unlock(&ring->tx_lock);
		if (result == IXGBE_XDP_CONSUMED)
			goto out_failure;
		break;
	case XDP_REDIRECT:
		err = xdp_do_redirect(adapter->netdev, xdp, xdp_prog);
		if (err)
			goto out_failure;
		result = IXGBE_XDP_REDIR;
		break;
	default:
		bpf_warn_invalid_xdp_action(rx_ring->netdev, xdp_prog, act);
		fallthrough;
	case XDP_ABORTED:
out_failure:
		trace_xdp_exception(rx_ring->netdev, xdp_prog, act);
		fallthrough; /* handle aborts by dropping packet */
	case XDP_DROP:
		result = IXGBE_XDP_CONSUMED;
		break;
	}
xdp_out:
	return ERR_PTR(-result);
}

static unsigned int ixgbe_rx_frame_truesize(struct ixgbe_ring *rx_ring,
					    unsigned int size)
{
	unsigned int truesize;

#if (PAGE_SIZE < 8192)
	truesize = ixgbe_rx_pg_size(rx_ring) / 2; /* Must be power-of-2 */
#else
	truesize = rx_ring->rx_offset ?
		SKB_DATA_ALIGN(rx_ring->rx_offset + size) +
		SKB_DATA_ALIGN(sizeof(struct skb_shared_info)) :
		SKB_DATA_ALIGN(size);
#endif
	return truesize;
}

static void ixgbe_rx_buffer_flip(struct ixgbe_ring *rx_ring,
				 struct ixgbe_rx_buffer *rx_buffer,
				 unsigned int size)
{
	unsigned int truesize = ixgbe_rx_frame_truesize(rx_ring, size);
#if (PAGE_SIZE < 8192)
	rx_buffer->page_offset ^= truesize;
#else
	rx_buffer->page_offset += truesize;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_clean_rx_irq - Clean completed descriptors from Rx ring - bounce buf
 * @q_vector: structure containing interrupt and ring information
 * @rx_ring: rx descriptor ring to transact packets on
 * @budget: Total limit on number of packets to process
 *
 * This function provides a "bounce buffer" approach to Rx interrupt
 * processing.  The advantage to this is that on systems that have
 * expensive overhead for IOMMU access this provides a means of avoiding
 * it by maintaining the mapping of the page to the syste.
 *
<<<<<<< HEAD
 * Returns true if all work is completed without reaching budget
 **/
static bool ixgbe_clean_rx_irq(struct ixgbe_q_vector *q_vector,
			       struct ixgbe_ring *rx_ring,
			       int budget)
{
	unsigned int total_rx_bytes = 0, total_rx_packets = 0;
#ifdef IXGBE_FCOE
	struct ixgbe_adapter *adapter = q_vector->adapter;
	int ddp_bytes = 0;
#endif /* IXGBE_FCOE */
	u16 cleaned_count = ixgbe_desc_unused(rx_ring);

	do {
		struct ixgbe_rx_buffer *rx_buffer;
		union ixgbe_adv_rx_desc *rx_desc;
		struct sk_buff *skb;
		struct page *page;
		u16 ntc;
=======
 * Returns amount of work completed
 **/
static int ixgbe_clean_rx_irq(struct ixgbe_q_vector *q_vector,
			       struct ixgbe_ring *rx_ring,
			       const int budget)
{
	unsigned int total_rx_bytes = 0, total_rx_packets = 0, frame_sz = 0;
	struct ixgbe_adapter *adapter = q_vector->adapter;
#ifdef IXGBE_FCOE
	int ddp_bytes;
	unsigned int mss = 0;
#endif /* IXGBE_FCOE */
	u16 cleaned_count = ixgbe_desc_unused(rx_ring);
	unsigned int offset = rx_ring->rx_offset;
	unsigned int xdp_xmit = 0;
	struct xdp_buff xdp;

	/* Frame size depend on rx_ring setup when PAGE_SIZE=4K */
#if (PAGE_SIZE < 8192)
	frame_sz = ixgbe_rx_frame_truesize(rx_ring, 0);
#endif
	xdp_init_buff(&xdp, frame_sz, &rx_ring->xdp_rxq);

	while (likely(total_rx_packets < budget)) {
		union ixgbe_adv_rx_desc *rx_desc;
		struct ixgbe_rx_buffer *rx_buffer;
		struct sk_buff *skb;
		int rx_buffer_pgcnt;
		unsigned int size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* return some buffers to hardware, one at a time is too slow */
		if (cleaned_count >= IXGBE_RX_BUFFER_WRITE) {
			ixgbe_alloc_rx_buffers(rx_ring, cleaned_count);
			cleaned_count = 0;
		}

<<<<<<< HEAD
		ntc = rx_ring->next_to_clean;
		rx_desc = IXGBE_RX_DESC(rx_ring, ntc);
		rx_buffer = &rx_ring->rx_buffer_info[ntc];

		if (!ixgbe_test_staterr(rx_desc, IXGBE_RXD_STAT_DD))
			break;

		/*
		 * This memory barrier is needed to keep us from reading
		 * any other fields out of the rx_desc until we know the
		 * RXD_STAT_DD bit is set
		 */
		rmb();

		page = rx_buffer->page;
		prefetchw(page);

		skb = rx_buffer->skb;

		if (likely(!skb)) {
			void *page_addr = page_address(page) +
					  rx_buffer->page_offset;

			/* prefetch first cache line of first page */
			prefetch(page_addr);
#if L1_CACHE_BYTES < 128
			prefetch(page_addr + L1_CACHE_BYTES);
#endif

			/* allocate a skb to store the frags */
			skb = netdev_alloc_skb_ip_align(rx_ring->netdev,
							IXGBE_RX_HDR_SIZE);
			if (unlikely(!skb)) {
				rx_ring->rx_stats.alloc_rx_buff_failed++;
				break;
			}

			/*
			 * we will be copying header into skb->data in
			 * pskb_may_pull so it is in our interest to prefetch
			 * it now to avoid a possible cache miss
			 */
			prefetchw(skb->data);

			/*
			 * Delay unmapping of the first packet. It carries the
			 * header information, HW may still access the header
			 * after the writeback.  Only unmap it when EOP is
			 * reached
			 */
			IXGBE_CB(skb)->dma = rx_buffer->dma;
		} else {
			/* we are reusing so sync this buffer for CPU use */
			dma_sync_single_range_for_cpu(rx_ring->dev,
						      rx_buffer->dma,
						      rx_buffer->page_offset,
						      ixgbe_rx_bufsz(rx_ring),
						      DMA_FROM_DEVICE);
		}

		/* pull page into skb */
		ixgbe_add_rx_frag(rx_ring, rx_buffer, skb,
				  le16_to_cpu(rx_desc->wb.upper.length));

		if (ixgbe_can_reuse_page(rx_buffer)) {
			/* hand second half of page back to the ring */
			ixgbe_reuse_rx_page(rx_ring, rx_buffer);
		} else if (IXGBE_CB(skb)->dma == rx_buffer->dma) {
			/* the page has been released from the ring */
			IXGBE_CB(skb)->page_released = true;
		} else {
			/* we are not reusing the buffer so unmap it */
			dma_unmap_page(rx_ring->dev, rx_buffer->dma,
				       ixgbe_rx_pg_size(rx_ring),
				       DMA_FROM_DEVICE);
		}

		/* clear contents of buffer_info */
		rx_buffer->skb = NULL;
		rx_buffer->dma = 0;
		rx_buffer->page = NULL;

		ixgbe_get_rsc_cnt(rx_ring, rx_desc, skb);

=======
		rx_desc = IXGBE_RX_DESC(rx_ring, rx_ring->next_to_clean);
		size = le16_to_cpu(rx_desc->wb.upper.length);
		if (!size)
			break;

		/* This memory barrier is needed to keep us from reading
		 * any other fields out of the rx_desc until we know the
		 * descriptor has been written back
		 */
		dma_rmb();

		rx_buffer = ixgbe_get_rx_buffer(rx_ring, rx_desc, &skb, size, &rx_buffer_pgcnt);

		/* retrieve a buffer from the ring */
		if (!skb) {
			unsigned char *hard_start;

			hard_start = page_address(rx_buffer->page) +
				     rx_buffer->page_offset - offset;
			xdp_prepare_buff(&xdp, hard_start, offset, size, true);
			xdp_buff_clear_frags_flag(&xdp);
#if (PAGE_SIZE > 4096)
			/* At larger PAGE_SIZE, frame_sz depend on len size */
			xdp.frame_sz = ixgbe_rx_frame_truesize(rx_ring, size);
#endif
			skb = ixgbe_run_xdp(adapter, rx_ring, &xdp);
		}

		if (IS_ERR(skb)) {
			unsigned int xdp_res = -PTR_ERR(skb);

			if (xdp_res & (IXGBE_XDP_TX | IXGBE_XDP_REDIR)) {
				xdp_xmit |= xdp_res;
				ixgbe_rx_buffer_flip(rx_ring, rx_buffer, size);
			} else {
				rx_buffer->pagecnt_bias++;
			}
			total_rx_packets++;
			total_rx_bytes += size;
		} else if (skb) {
			ixgbe_add_rx_frag(rx_ring, rx_buffer, skb, size);
		} else if (ring_uses_build_skb(rx_ring)) {
			skb = ixgbe_build_skb(rx_ring, rx_buffer,
					      &xdp, rx_desc);
		} else {
			skb = ixgbe_construct_skb(rx_ring, rx_buffer,
						  &xdp, rx_desc);
		}

		/* exit if we failed to retrieve a buffer */
		if (!skb) {
			rx_ring->rx_stats.alloc_rx_buff_failed++;
			rx_buffer->pagecnt_bias++;
			break;
		}

		ixgbe_put_rx_buffer(rx_ring, rx_buffer, skb, rx_buffer_pgcnt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		cleaned_count++;

		/* place incomplete frames back on ring for completion */
		if (ixgbe_is_non_eop(rx_ring, rx_desc, skb))
			continue;

		/* verify the packet layout is correct */
		if (ixgbe_cleanup_headers(rx_ring, rx_desc, skb))
			continue;

		/* probably a little skewed due to removing CRC */
		total_rx_bytes += skb->len;
<<<<<<< HEAD
		total_rx_packets++;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* populate checksum, timestamp, VLAN, and protocol */
		ixgbe_process_skb_fields(rx_ring, rx_desc, skb);

#ifdef IXGBE_FCOE
		/* if ddp, not passing to ULD unless for FCP_RSP or error */
		if (ixgbe_rx_is_fcoe(rx_ring, rx_desc)) {
			ddp_bytes = ixgbe_fcoe_ddp(adapter, rx_desc, skb);
<<<<<<< HEAD
=======
			/* include DDPed FCoE data */
			if (ddp_bytes > 0) {
				if (!mss) {
					mss = rx_ring->netdev->mtu -
						sizeof(struct fcoe_hdr) -
						sizeof(struct fc_frame_header) -
						sizeof(struct fcoe_crc_eof);
					if (mss > 512)
						mss &= ~511;
				}
				total_rx_bytes += ddp_bytes;
				total_rx_packets += DIV_ROUND_UP(ddp_bytes,
								 mss);
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (!ddp_bytes) {
				dev_kfree_skb_any(skb);
				continue;
			}
		}

#endif /* IXGBE_FCOE */
		ixgbe_rx_skb(q_vector, skb);

		/* update budget accounting */
<<<<<<< HEAD
		budget--;
	} while (likely(budget));

#ifdef IXGBE_FCOE
	/* include DDPed FCoE data */
	if (ddp_bytes > 0) {
		unsigned int mss;

		mss = rx_ring->netdev->mtu - sizeof(struct fcoe_hdr) -
			sizeof(struct fc_frame_header) -
			sizeof(struct fcoe_crc_eof);
		if (mss > 512)
			mss &= ~511;
		total_rx_bytes += ddp_bytes;
		total_rx_packets += DIV_ROUND_UP(ddp_bytes, mss);
	}

#endif /* IXGBE_FCOE */
	u64_stats_update_begin(&rx_ring->syncp);
	rx_ring->stats.packets += total_rx_packets;
	rx_ring->stats.bytes += total_rx_bytes;
	u64_stats_update_end(&rx_ring->syncp);
	q_vector->rx.total_packets += total_rx_packets;
	q_vector->rx.total_bytes += total_rx_bytes;

	if (cleaned_count)
		ixgbe_alloc_rx_buffers(rx_ring, cleaned_count);

	return !!budget;
=======
		total_rx_packets++;
	}

	if (xdp_xmit & IXGBE_XDP_REDIR)
		xdp_do_flush();

	if (xdp_xmit & IXGBE_XDP_TX) {
		struct ixgbe_ring *ring = ixgbe_determine_xdp_ring(adapter);

		ixgbe_xdp_ring_update_tail_locked(ring);
	}

	ixgbe_update_rx_ring_stats(rx_ring, q_vector, total_rx_packets,
				   total_rx_bytes);

	return total_rx_packets;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_configure_msix - Configure MSI-X hardware
 * @adapter: board private structure
 *
 * ixgbe_configure_msix sets up the hardware to properly generate MSI-X
 * interrupts.
 **/
static void ixgbe_configure_msix(struct ixgbe_adapter *adapter)
{
	struct ixgbe_q_vector *q_vector;
<<<<<<< HEAD
	int q_vectors, v_idx;
	u32 mask;

	q_vectors = adapter->num_msix_vectors - NON_Q_VECTORS;

	/* Populate MSIX to EITR Select */
	if (adapter->num_vfs > 32) {
		u32 eitrsel = (1 << (adapter->num_vfs - 32)) - 1;
=======
	int v_idx;
	u32 mask;

	/* Populate MSIX to EITR Select */
	if (adapter->num_vfs > 32) {
		u32 eitrsel = BIT(adapter->num_vfs - 32) - 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_EITRSEL, eitrsel);
	}

	/*
	 * Populate the IVAR table and set the ITR values to the
	 * corresponding register.
	 */
<<<<<<< HEAD
	for (v_idx = 0; v_idx < q_vectors; v_idx++) {
=======
	for (v_idx = 0; v_idx < adapter->num_q_vectors; v_idx++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct ixgbe_ring *ring;
		q_vector = adapter->q_vector[v_idx];

		ixgbe_for_each_ring(ring, q_vector->rx)
			ixgbe_set_ivar(adapter, 0, ring->reg_idx, v_idx);

		ixgbe_for_each_ring(ring, q_vector->tx)
			ixgbe_set_ivar(adapter, 1, ring->reg_idx, v_idx);

<<<<<<< HEAD
		if (q_vector->tx.ring && !q_vector->rx.ring) {
			/* tx only vector */
			if (adapter->tx_itr_setting == 1)
				q_vector->itr = IXGBE_10K_ITR;
			else
				q_vector->itr = adapter->tx_itr_setting;
		} else {
			/* rx or rx/tx vector */
			if (adapter->rx_itr_setting == 1)
				q_vector->itr = IXGBE_20K_ITR;
			else
				q_vector->itr = adapter->rx_itr_setting;
		}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ixgbe_write_eitr(q_vector);
	}

	switch (adapter->hw.mac.type) {
	case ixgbe_mac_82598EB:
		ixgbe_set_ivar(adapter, -1, IXGBE_IVAR_OTHER_CAUSES_INDEX,
			       v_idx);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
<<<<<<< HEAD
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ixgbe_set_ivar(adapter, -1, 1, v_idx);
		break;
	default:
		break;
	}
	IXGBE_WRITE_REG(&adapter->hw, IXGBE_EITR(v_idx), 1950);

	/* set up to autoclear timer, and the vectors */
	mask = IXGBE_EIMS_ENABLE_MASK;
	mask &= ~(IXGBE_EIMS_OTHER |
		  IXGBE_EIMS_MAILBOX |
		  IXGBE_EIMS_LSC);

	IXGBE_WRITE_REG(&adapter->hw, IXGBE_EIAC, mask);
}

<<<<<<< HEAD
enum latency_range {
	lowest_latency = 0,
	low_latency = 1,
	bulk_latency = 2,
	latency_invalid = 255
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * ixgbe_update_itr - update the dynamic ITR value based on statistics
 * @q_vector: structure containing interrupt and ring information
 * @ring_container: structure containing ring performance data
 *
 *      Stores a new ITR value based on packets and byte
 *      counts during the last interrupt.  The advantage of per interrupt
 *      computation is faster updates and more accurate ITR for the current
 *      traffic pattern.  Constants in this function were computed
 *      based on theoretical maximum wire speed and thresholds were set based
 *      on testing data as well as attempting to minimize response time
 *      while increasing bulk throughput.
<<<<<<< HEAD
 *      this functionality is controlled by the InterruptThrottleRate module
 *      parameter (see ixgbe_param.c)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void ixgbe_update_itr(struct ixgbe_q_vector *q_vector,
			     struct ixgbe_ring_container *ring_container)
{
<<<<<<< HEAD
	int bytes = ring_container->total_bytes;
	int packets = ring_container->total_packets;
	u32 timepassed_us;
	u64 bytes_perint;
	u8 itr_setting = ring_container->itr;

	if (packets == 0)
		return;

	/* simple throttlerate management
	 *   0-10MB/s   lowest (100000 ints/s)
	 *  10-20MB/s   low    (20000 ints/s)
	 *  20-1249MB/s bulk   (8000 ints/s)
	 */
	/* what was last interrupt timeslice? */
	timepassed_us = q_vector->itr >> 2;
	bytes_perint = bytes / timepassed_us; /* bytes/usec */

	switch (itr_setting) {
	case lowest_latency:
		if (bytes_perint > 10)
			itr_setting = low_latency;
		break;
	case low_latency:
		if (bytes_perint > 20)
			itr_setting = bulk_latency;
		else if (bytes_perint <= 10)
			itr_setting = lowest_latency;
		break;
	case bulk_latency:
		if (bytes_perint <= 20)
			itr_setting = low_latency;
		break;
	}

	/* clear work counters since we have the values we need */
	ring_container->total_bytes = 0;
	ring_container->total_packets = 0;

	/* write updated itr to ring container */
	ring_container->itr = itr_setting;
=======
	unsigned int itr = IXGBE_ITR_ADAPTIVE_MIN_USECS |
			   IXGBE_ITR_ADAPTIVE_LATENCY;
	unsigned int avg_wire_size, packets, bytes;
	unsigned long next_update = jiffies;

	/* If we don't have any rings just leave ourselves set for maximum
	 * possible latency so we take ourselves out of the equation.
	 */
	if (!ring_container->ring)
		return;

	/* If we didn't update within up to 1 - 2 jiffies we can assume
	 * that either packets are coming in so slow there hasn't been
	 * any work, or that there is so much work that NAPI is dealing
	 * with interrupt moderation and we don't need to do anything.
	 */
	if (time_after(next_update, ring_container->next_update))
		goto clear_counts;

	packets = ring_container->total_packets;

	/* We have no packets to actually measure against. This means
	 * either one of the other queues on this vector is active or
	 * we are a Tx queue doing TSO with too high of an interrupt rate.
	 *
	 * When this occurs just tick up our delay by the minimum value
	 * and hope that this extra delay will prevent us from being called
	 * without any work on our queue.
	 */
	if (!packets) {
		itr = (q_vector->itr >> 2) + IXGBE_ITR_ADAPTIVE_MIN_INC;
		if (itr > IXGBE_ITR_ADAPTIVE_MAX_USECS)
			itr = IXGBE_ITR_ADAPTIVE_MAX_USECS;
		itr += ring_container->itr & IXGBE_ITR_ADAPTIVE_LATENCY;
		goto clear_counts;
	}

	bytes = ring_container->total_bytes;

	/* If packets are less than 4 or bytes are less than 9000 assume
	 * insufficient data to use bulk rate limiting approach. We are
	 * likely latency driven.
	 */
	if (packets < 4 && bytes < 9000) {
		itr = IXGBE_ITR_ADAPTIVE_LATENCY;
		goto adjust_by_size;
	}

	/* Between 4 and 48 we can assume that our current interrupt delay
	 * is only slightly too low. As such we should increase it by a small
	 * fixed amount.
	 */
	if (packets < 48) {
		itr = (q_vector->itr >> 2) + IXGBE_ITR_ADAPTIVE_MIN_INC;
		if (itr > IXGBE_ITR_ADAPTIVE_MAX_USECS)
			itr = IXGBE_ITR_ADAPTIVE_MAX_USECS;
		goto clear_counts;
	}

	/* Between 48 and 96 is our "goldilocks" zone where we are working
	 * out "just right". Just report that our current ITR is good for us.
	 */
	if (packets < 96) {
		itr = q_vector->itr >> 2;
		goto clear_counts;
	}

	/* If packet count is 96 or greater we are likely looking at a slight
	 * overrun of the delay we want. Try halving our delay to see if that
	 * will cut the number of packets in half per interrupt.
	 */
	if (packets < 256) {
		itr = q_vector->itr >> 3;
		if (itr < IXGBE_ITR_ADAPTIVE_MIN_USECS)
			itr = IXGBE_ITR_ADAPTIVE_MIN_USECS;
		goto clear_counts;
	}

	/* The paths below assume we are dealing with a bulk ITR since number
	 * of packets is 256 or greater. We are just going to have to compute
	 * a value and try to bring the count under control, though for smaller
	 * packet sizes there isn't much we can do as NAPI polling will likely
	 * be kicking in sooner rather than later.
	 */
	itr = IXGBE_ITR_ADAPTIVE_BULK;

adjust_by_size:
	/* If packet counts are 256 or greater we can assume we have a gross
	 * overestimation of what the rate should be. Instead of trying to fine
	 * tune it just use the formula below to try and dial in an exact value
	 * give the current packet size of the frame.
	 */
	avg_wire_size = bytes / packets;

	/* The following is a crude approximation of:
	 *  wmem_default / (size + overhead) = desired_pkts_per_int
	 *  rate / bits_per_byte / (size + ethernet overhead) = pkt_rate
	 *  (desired_pkt_rate / pkt_rate) * usecs_per_sec = ITR value
	 *
	 * Assuming wmem_default is 212992 and overhead is 640 bytes per
	 * packet, (256 skb, 64 headroom, 320 shared info), we can reduce the
	 * formula down to
	 *
	 *  (170 * (size + 24)) / (size + 640) = ITR
	 *
	 * We first do some math on the packet size and then finally bitshift
	 * by 8 after rounding up. We also have to account for PCIe link speed
	 * difference as ITR scales based on this.
	 */
	if (avg_wire_size <= 60) {
		/* Start at 50k ints/sec */
		avg_wire_size = 5120;
	} else if (avg_wire_size <= 316) {
		/* 50K ints/sec to 16K ints/sec */
		avg_wire_size *= 40;
		avg_wire_size += 2720;
	} else if (avg_wire_size <= 1084) {
		/* 16K ints/sec to 9.2K ints/sec */
		avg_wire_size *= 15;
		avg_wire_size += 11452;
	} else if (avg_wire_size < 1968) {
		/* 9.2K ints/sec to 8K ints/sec */
		avg_wire_size *= 5;
		avg_wire_size += 22420;
	} else {
		/* plateau at a limit of 8K ints/sec */
		avg_wire_size = 32256;
	}

	/* If we are in low latency mode half our delay which doubles the rate
	 * to somewhere between 100K to 16K ints/sec
	 */
	if (itr & IXGBE_ITR_ADAPTIVE_LATENCY)
		avg_wire_size >>= 1;

	/* Resultant value is 256 times larger than it needs to be. This
	 * gives us room to adjust the value as needed to either increase
	 * or decrease the value based on link speeds of 10G, 2.5G, 1G, etc.
	 *
	 * Use addition as we have already recorded the new latency flag
	 * for the ITR value.
	 */
	switch (q_vector->adapter->link_speed) {
	case IXGBE_LINK_SPEED_10GB_FULL:
	case IXGBE_LINK_SPEED_100_FULL:
	default:
		itr += DIV_ROUND_UP(avg_wire_size,
				    IXGBE_ITR_ADAPTIVE_MIN_INC * 256) *
		       IXGBE_ITR_ADAPTIVE_MIN_INC;
		break;
	case IXGBE_LINK_SPEED_2_5GB_FULL:
	case IXGBE_LINK_SPEED_1GB_FULL:
	case IXGBE_LINK_SPEED_10_FULL:
		if (avg_wire_size > 8064)
			avg_wire_size = 8064;
		itr += DIV_ROUND_UP(avg_wire_size,
				    IXGBE_ITR_ADAPTIVE_MIN_INC * 64) *
		       IXGBE_ITR_ADAPTIVE_MIN_INC;
		break;
	}

clear_counts:
	/* write back value */
	ring_container->itr = itr;

	/* next update should occur within next jiffy */
	ring_container->next_update = next_update + 1;

	ring_container->total_bytes = 0;
	ring_container->total_packets = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_write_eitr - write EITR register in hardware specific way
 * @q_vector: structure containing interrupt and ring information
 *
 * This function is made to be called by ethtool and by the driver
 * when it needs to update EITR registers at runtime.  Hardware
 * specific quirks/differences are taken care of here.
 */
void ixgbe_write_eitr(struct ixgbe_q_vector *q_vector)
{
	struct ixgbe_adapter *adapter = q_vector->adapter;
	struct ixgbe_hw *hw = &adapter->hw;
	int v_idx = q_vector->v_idx;
	u32 itr_reg = q_vector->itr & IXGBE_MAX_EITR;

	switch (adapter->hw.mac.type) {
	case ixgbe_mac_82598EB:
		/* must write high and low 16 bits to reset counter */
		itr_reg |= (itr_reg << 16);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
<<<<<<< HEAD
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * set the WDIS bit to not clear the timer bits and cause an
		 * immediate assertion of the interrupt
		 */
		itr_reg |= IXGBE_EITR_CNT_WDIS;
		break;
	default:
		break;
	}
	IXGBE_WRITE_REG(hw, IXGBE_EITR(v_idx), itr_reg);
}

static void ixgbe_set_itr(struct ixgbe_q_vector *q_vector)
{
<<<<<<< HEAD
	u32 new_itr = q_vector->itr;
	u8 current_itr;
=======
	u32 new_itr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ixgbe_update_itr(q_vector, &q_vector->tx);
	ixgbe_update_itr(q_vector, &q_vector->rx);

<<<<<<< HEAD
	current_itr = max(q_vector->rx.itr, q_vector->tx.itr);

	switch (current_itr) {
	/* counts and packets in update_itr are dependent on these numbers */
	case lowest_latency:
		new_itr = IXGBE_100K_ITR;
		break;
	case low_latency:
		new_itr = IXGBE_20K_ITR;
		break;
	case bulk_latency:
		new_itr = IXGBE_8K_ITR;
		break;
	default:
		break;
	}

	if (new_itr != q_vector->itr) {
		/* do an exponential smoothing */
		new_itr = (10 * new_itr * q_vector->itr) /
			  ((9 * new_itr) + q_vector->itr);

=======
	/* use the smallest value of new ITR delay calculations */
	new_itr = min(q_vector->rx.itr, q_vector->tx.itr);

	/* Clear latency flag if set, shift into correct position */
	new_itr &= ~IXGBE_ITR_ADAPTIVE_LATENCY;
	new_itr <<= 2;

	if (new_itr != q_vector->itr) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* save the algorithm value here */
		q_vector->itr = new_itr;

		ixgbe_write_eitr(q_vector);
	}
}

/**
 * ixgbe_check_overtemp_subtask - check for over temperature
 * @adapter: pointer to adapter
 **/
static void ixgbe_check_overtemp_subtask(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 eicr = adapter->interrupt_event;

	if (test_bit(__IXGBE_DOWN, &adapter->state))
		return;

<<<<<<< HEAD
	if (!(adapter->flags2 & IXGBE_FLAG2_TEMP_SENSOR_CAPABLE) &&
	    !(adapter->flags2 & IXGBE_FLAG2_TEMP_SENSOR_EVENT))
=======
	if (!(adapter->flags2 & IXGBE_FLAG2_TEMP_SENSOR_EVENT))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	adapter->flags2 &= ~IXGBE_FLAG2_TEMP_SENSOR_EVENT;

	switch (hw->device_id) {
	case IXGBE_DEV_ID_82599_T3_LOM:
		/*
		 * Since the warning interrupt is for both ports
		 * we don't have to check if:
		 *  - This interrupt wasn't for our port.
		 *  - We may have missed the interrupt so always have to
		 *    check if we  got a LSC
		 */
<<<<<<< HEAD
		if (!(eicr & IXGBE_EICR_GPI_SDP0) &&
=======
		if (!(eicr & IXGBE_EICR_GPI_SDP0_8259X) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    !(eicr & IXGBE_EICR_LSC))
			return;

		if (!(eicr & IXGBE_EICR_LSC) && hw->mac.ops.check_link) {
<<<<<<< HEAD
			u32 autoneg;
			bool link_up = false;

			hw->mac.ops.check_link(hw, &autoneg, &link_up, false);
=======
			u32 speed;
			bool link_up = false;

			hw->mac.ops.check_link(hw, &speed, &link_up, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			if (link_up)
				return;
		}

		/* Check if this is not due to overtemp */
<<<<<<< HEAD
		if (hw->phy.ops.check_overtemp(hw) != IXGBE_ERR_OVERTEMP)
			return;

		break;
	default:
		if (!(eicr & IXGBE_EICR_GPI_SDP0))
			return;
		break;
	}
	e_crit(drv,
	       "Network adapter has been stopped because it has over heated. "
	       "Restart the computer. If the problem persists, "
	       "power off the system and replace the adapter\n");
=======
		if (!hw->phy.ops.check_overtemp(hw))
			return;

		break;
	case IXGBE_DEV_ID_X550EM_A_1G_T:
	case IXGBE_DEV_ID_X550EM_A_1G_T_L:
		if (!hw->phy.ops.check_overtemp(hw))
			return;
		break;
	default:
		if (adapter->hw.mac.type >= ixgbe_mac_X540)
			return;
		if (!(eicr & IXGBE_EICR_GPI_SDP0(hw)))
			return;
		break;
	}
	e_crit(drv, "%s\n", ixgbe_overheat_msg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	adapter->interrupt_event = 0;
}

static void ixgbe_check_fan_failure(struct ixgbe_adapter *adapter, u32 eicr)
{
	struct ixgbe_hw *hw = &adapter->hw;

	if ((adapter->flags & IXGBE_FLAG_FAN_FAIL_CAPABLE) &&
<<<<<<< HEAD
	    (eicr & IXGBE_EICR_GPI_SDP1)) {
		e_crit(probe, "Fan has stopped, replace the adapter\n");
		/* write to clear the interrupt */
		IXGBE_WRITE_REG(hw, IXGBE_EICR, IXGBE_EICR_GPI_SDP1);
=======
	    (eicr & IXGBE_EICR_GPI_SDP1(hw))) {
		e_crit(probe, "Fan has stopped, replace the adapter\n");
		/* write to clear the interrupt */
		IXGBE_WRITE_REG(hw, IXGBE_EICR, IXGBE_EICR_GPI_SDP1(hw));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void ixgbe_check_overtemp_event(struct ixgbe_adapter *adapter, u32 eicr)
{
<<<<<<< HEAD
=======
	struct ixgbe_hw *hw = &adapter->hw;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!(adapter->flags2 & IXGBE_FLAG2_TEMP_SENSOR_CAPABLE))
		return;

	switch (adapter->hw.mac.type) {
	case ixgbe_mac_82599EB:
		/*
		 * Need to check link state so complete overtemp check
		 * on service task
		 */
<<<<<<< HEAD
		if (((eicr & IXGBE_EICR_GPI_SDP0) || (eicr & IXGBE_EICR_LSC)) &&
=======
		if (((eicr & IXGBE_EICR_GPI_SDP0(hw)) ||
		     (eicr & IXGBE_EICR_LSC)) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    (!test_bit(__IXGBE_DOWN, &adapter->state))) {
			adapter->interrupt_event = eicr;
			adapter->flags2 |= IXGBE_FLAG2_TEMP_SENSOR_EVENT;
			ixgbe_service_event_schedule(adapter);
			return;
		}
		return;
<<<<<<< HEAD
=======
	case ixgbe_mac_x550em_a:
		if (eicr & IXGBE_EICR_GPI_SDP0_X550EM_a) {
			adapter->interrupt_event = eicr;
			adapter->flags2 |= IXGBE_FLAG2_TEMP_SENSOR_EVENT;
			ixgbe_service_event_schedule(adapter);
			IXGBE_WRITE_REG(&adapter->hw, IXGBE_EIMC,
					IXGBE_EICR_GPI_SDP0_X550EM_a);
			IXGBE_WRITE_REG(&adapter->hw, IXGBE_EICR,
					IXGBE_EICR_GPI_SDP0_X550EM_a);
		}
		return;
	case ixgbe_mac_X550:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case ixgbe_mac_X540:
		if (!(eicr & IXGBE_EICR_TS))
			return;
		break;
	default:
		return;
	}

<<<<<<< HEAD
	e_crit(drv,
	       "Network adapter has been stopped because it has over heated. "
	       "Restart the computer. If the problem persists, "
	       "power off the system and replace the adapter\n");
=======
	e_crit(drv, "%s\n", ixgbe_overheat_msg);
}

static inline bool ixgbe_is_sfp(struct ixgbe_hw *hw)
{
	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		if (hw->phy.type == ixgbe_phy_nl)
			return true;
		return false;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
		switch (hw->mac.ops.get_media_type(hw)) {
		case ixgbe_media_type_fiber:
		case ixgbe_media_type_fiber_qsfp:
			return true;
		default:
			return false;
		}
	default:
		return false;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_check_sfp_event(struct ixgbe_adapter *adapter, u32 eicr)
{
	struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD

	if (eicr & IXGBE_EICR_GPI_SDP2) {
		/* Clear the interrupt */
		IXGBE_WRITE_REG(hw, IXGBE_EICR, IXGBE_EICR_GPI_SDP2);
		if (!test_bit(__IXGBE_DOWN, &adapter->state)) {
			adapter->flags2 |= IXGBE_FLAG2_SFP_NEEDS_RESET;
=======
	u32 eicr_mask = IXGBE_EICR_GPI_SDP2(hw);

	if (!ixgbe_is_sfp(hw))
		return;

	/* Later MAC's use different SDP */
	if (hw->mac.type >= ixgbe_mac_X540)
		eicr_mask = IXGBE_EICR_GPI_SDP0_X540;

	if (eicr & eicr_mask) {
		/* Clear the interrupt */
		IXGBE_WRITE_REG(hw, IXGBE_EICR, eicr_mask);
		if (!test_bit(__IXGBE_DOWN, &adapter->state)) {
			adapter->flags2 |= IXGBE_FLAG2_SFP_NEEDS_RESET;
			adapter->sfp_poll_time = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ixgbe_service_event_schedule(adapter);
		}
	}

<<<<<<< HEAD
	if (eicr & IXGBE_EICR_GPI_SDP1) {
		/* Clear the interrupt */
		IXGBE_WRITE_REG(hw, IXGBE_EICR, IXGBE_EICR_GPI_SDP1);
=======
	if (adapter->hw.mac.type == ixgbe_mac_82599EB &&
	    (eicr & IXGBE_EICR_GPI_SDP1(hw))) {
		/* Clear the interrupt */
		IXGBE_WRITE_REG(hw, IXGBE_EICR, IXGBE_EICR_GPI_SDP1(hw));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!test_bit(__IXGBE_DOWN, &adapter->state)) {
			adapter->flags |= IXGBE_FLAG_NEED_LINK_CONFIG;
			ixgbe_service_event_schedule(adapter);
		}
	}
}

static void ixgbe_check_lsc(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;

	adapter->lsc_int++;
	adapter->flags |= IXGBE_FLAG_NEED_LINK_UPDATE;
	adapter->link_check_timeout = jiffies;
	if (!test_bit(__IXGBE_DOWN, &adapter->state)) {
		IXGBE_WRITE_REG(hw, IXGBE_EIMC, IXGBE_EIMC_LSC);
		IXGBE_WRITE_FLUSH(hw);
		ixgbe_service_event_schedule(adapter);
	}
}

static inline void ixgbe_irq_enable_queues(struct ixgbe_adapter *adapter,
					   u64 qmask)
{
<<<<<<< HEAD
	u32 mask;
	struct ixgbe_hw *hw = &adapter->hw;
=======
	struct ixgbe_hw *hw = &adapter->hw;
	u32 mask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		mask = (IXGBE_EIMS_RTX_QUEUE & qmask);
		IXGBE_WRITE_REG(hw, IXGBE_EIMS, mask);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
<<<<<<< HEAD
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mask = (qmask & 0xFFFFFFFF);
		if (mask)
			IXGBE_WRITE_REG(hw, IXGBE_EIMS_EX(0), mask);
		mask = (qmask >> 32);
		if (mask)
			IXGBE_WRITE_REG(hw, IXGBE_EIMS_EX(1), mask);
		break;
	default:
		break;
	}
	/* skip the flush */
}

<<<<<<< HEAD
static inline void ixgbe_irq_disable_queues(struct ixgbe_adapter *adapter,
					    u64 qmask)
{
	u32 mask;
	struct ixgbe_hw *hw = &adapter->hw;

	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		mask = (IXGBE_EIMS_RTX_QUEUE & qmask);
		IXGBE_WRITE_REG(hw, IXGBE_EIMC, mask);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
		mask = (qmask & 0xFFFFFFFF);
		if (mask)
			IXGBE_WRITE_REG(hw, IXGBE_EIMC_EX(0), mask);
		mask = (qmask >> 32);
		if (mask)
			IXGBE_WRITE_REG(hw, IXGBE_EIMC_EX(1), mask);
		break;
	default:
		break;
	}
	/* skip the flush */
}

/**
 * ixgbe_irq_enable - Enable default interrupt generation settings
 * @adapter: board private structure
=======
/**
 * ixgbe_irq_enable - Enable default interrupt generation settings
 * @adapter: board private structure
 * @queues: enable irqs for queues
 * @flush: flush register write
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static inline void ixgbe_irq_enable(struct ixgbe_adapter *adapter, bool queues,
				    bool flush)
{
<<<<<<< HEAD
=======
	struct ixgbe_hw *hw = &adapter->hw;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 mask = (IXGBE_EIMS_ENABLE_MASK & ~IXGBE_EIMS_RTX_QUEUE);

	/* don't reenable LSC while waiting for link */
	if (adapter->flags & IXGBE_FLAG_NEED_LINK_UPDATE)
		mask &= ~IXGBE_EIMS_LSC;

	if (adapter->flags2 & IXGBE_FLAG2_TEMP_SENSOR_CAPABLE)
		switch (adapter->hw.mac.type) {
		case ixgbe_mac_82599EB:
<<<<<<< HEAD
			mask |= IXGBE_EIMS_GPI_SDP0;
			break;
		case ixgbe_mac_X540:
=======
			mask |= IXGBE_EIMS_GPI_SDP0(hw);
			break;
		case ixgbe_mac_X540:
		case ixgbe_mac_X550:
		case ixgbe_mac_X550EM_x:
		case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mask |= IXGBE_EIMS_TS;
			break;
		default:
			break;
		}
	if (adapter->flags & IXGBE_FLAG_FAN_FAIL_CAPABLE)
<<<<<<< HEAD
		mask |= IXGBE_EIMS_GPI_SDP1;
	switch (adapter->hw.mac.type) {
	case ixgbe_mac_82599EB:
		mask |= IXGBE_EIMS_GPI_SDP1;
		mask |= IXGBE_EIMS_GPI_SDP2;
	case ixgbe_mac_X540:
=======
		mask |= IXGBE_EIMS_GPI_SDP1(hw);
	switch (adapter->hw.mac.type) {
	case ixgbe_mac_82599EB:
		mask |= IXGBE_EIMS_GPI_SDP1(hw);
		mask |= IXGBE_EIMS_GPI_SDP2(hw);
		fallthrough;
	case ixgbe_mac_X540:
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
		if (adapter->hw.device_id == IXGBE_DEV_ID_X550EM_X_SFP ||
		    adapter->hw.device_id == IXGBE_DEV_ID_X550EM_A_SFP ||
		    adapter->hw.device_id == IXGBE_DEV_ID_X550EM_A_SFP_N)
			mask |= IXGBE_EIMS_GPI_SDP0(&adapter->hw);
		if (adapter->hw.phy.type == ixgbe_phy_x550em_ext_t)
			mask |= IXGBE_EICR_GPI_SDP0_X540;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mask |= IXGBE_EIMS_ECC;
		mask |= IXGBE_EIMS_MAILBOX;
		break;
	default:
		break;
	}
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((adapter->flags & IXGBE_FLAG_FDIR_HASH_CAPABLE) &&
	    !(adapter->flags2 & IXGBE_FLAG2_FDIR_REQUIRES_REINIT))
		mask |= IXGBE_EIMS_FLOW_DIR;

	IXGBE_WRITE_REG(&adapter->hw, IXGBE_EIMS, mask);
	if (queues)
		ixgbe_irq_enable_queues(adapter, ~0);
	if (flush)
		IXGBE_WRITE_FLUSH(&adapter->hw);
}

static irqreturn_t ixgbe_msix_other(int irq, void *data)
{
	struct ixgbe_adapter *adapter = data;
	struct ixgbe_hw *hw = &adapter->hw;
	u32 eicr;

	/*
	 * Workaround for Silicon errata.  Use clear-by-write instead
	 * of clear-by-read.  Reading with EICS will return the
	 * interrupt causes without clearing, which later be done
	 * with the write to EICR.
	 */
	eicr = IXGBE_READ_REG(hw, IXGBE_EICS);

	/* The lower 16bits of the EICR register are for the queue interrupts
<<<<<<< HEAD
	 * which should be masked here in order to not accidently clear them if
=======
	 * which should be masked here in order to not accidentally clear them if
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * the bits are high when ixgbe_msix_other is called. There is a race
	 * condition otherwise which results in possible performance loss
	 * especially if the ixgbe_msix_other interrupt is triggering
	 * consistently (as it would when PPS is turned on for the X540 device)
	 */
	eicr &= 0xFFFF0000;

	IXGBE_WRITE_REG(hw, IXGBE_EICR, eicr);

	if (eicr & IXGBE_EICR_LSC)
		ixgbe_check_lsc(adapter);

	if (eicr & IXGBE_EICR_MAILBOX)
		ixgbe_msg_task(adapter);

	switch (hw->mac.type) {
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
<<<<<<< HEAD
		if (eicr & IXGBE_EICR_ECC)
			e_info(link, "Received unrecoverable ECC Err, please "
			       "reboot\n");
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
		if (hw->phy.type == ixgbe_phy_x550em_ext_t &&
		    (eicr & IXGBE_EICR_GPI_SDP0_X540)) {
			adapter->flags2 |= IXGBE_FLAG2_PHY_INTERRUPT;
			ixgbe_service_event_schedule(adapter);
			IXGBE_WRITE_REG(hw, IXGBE_EICR,
					IXGBE_EICR_GPI_SDP0_X540);
		}
		if (eicr & IXGBE_EICR_ECC) {
			e_info(link, "Received ECC Err, initiating reset\n");
			set_bit(__IXGBE_RESET_REQUESTED, &adapter->state);
			ixgbe_service_event_schedule(adapter);
			IXGBE_WRITE_REG(hw, IXGBE_EICR, IXGBE_EICR_ECC);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Handle Flow Director Full threshold interrupt */
		if (eicr & IXGBE_EICR_FLOW_DIR) {
			int reinit_count = 0;
			int i;
			for (i = 0; i < adapter->num_tx_queues; i++) {
				struct ixgbe_ring *ring = adapter->tx_ring[i];
				if (test_and_clear_bit(__IXGBE_TX_FDIR_INIT_DONE,
						       &ring->state))
					reinit_count++;
			}
			if (reinit_count) {
				/* no more flow director interrupts until after init */
				IXGBE_WRITE_REG(hw, IXGBE_EIMC, IXGBE_EIMC_FLOW_DIR);
				adapter->flags2 |= IXGBE_FLAG2_FDIR_REQUIRES_REINIT;
				ixgbe_service_event_schedule(adapter);
			}
		}
		ixgbe_check_sfp_event(adapter, eicr);
		ixgbe_check_overtemp_event(adapter, eicr);
		break;
	default:
		break;
	}

	ixgbe_check_fan_failure(adapter, eicr);

<<<<<<< HEAD
=======
	if (unlikely(eicr & IXGBE_EICR_TIMESYNC))
		ixgbe_ptp_check_pps_event(adapter);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* re-enable the original interrupt state, no lsc, no queues */
	if (!test_bit(__IXGBE_DOWN, &adapter->state))
		ixgbe_irq_enable(adapter, false, false);

	return IRQ_HANDLED;
}

static irqreturn_t ixgbe_msix_clean_rings(int irq, void *data)
{
	struct ixgbe_q_vector *q_vector = data;

	/* EIAM disabled interrupts (on this vector) for us */

	if (q_vector->rx.ring || q_vector->tx.ring)
<<<<<<< HEAD
		napi_schedule(&q_vector->napi);
=======
		napi_schedule_irqoff(&q_vector->napi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return IRQ_HANDLED;
}

/**
 * ixgbe_poll - NAPI Rx polling callback
 * @napi: structure for representing this polling device
 * @budget: how many packets driver is allowed to clean
 *
 * This function is used for legacy and MSI, NAPI mode
 **/
int ixgbe_poll(struct napi_struct *napi, int budget)
{
	struct ixgbe_q_vector *q_vector =
				container_of(napi, struct ixgbe_q_vector, napi);
	struct ixgbe_adapter *adapter = q_vector->adapter;
	struct ixgbe_ring *ring;
<<<<<<< HEAD
	int per_ring_budget;
=======
	int per_ring_budget, work_done = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bool clean_complete = true;

#ifdef CONFIG_IXGBE_DCA
	if (adapter->flags & IXGBE_FLAG_DCA_ENABLED)
		ixgbe_update_dca(q_vector);
#endif

<<<<<<< HEAD
	ixgbe_for_each_ring(ring, q_vector->tx)
		clean_complete &= !!ixgbe_clean_tx_irq(q_vector, ring);
=======
	ixgbe_for_each_ring(ring, q_vector->tx) {
		bool wd = ring->xsk_pool ?
			  ixgbe_clean_xdp_tx_irq(q_vector, ring, budget) :
			  ixgbe_clean_tx_irq(q_vector, ring, budget);

		if (!wd)
			clean_complete = false;
	}

	/* Exit if we are called by netpoll */
	if (budget <= 0)
		return budget;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* attempt to distribute budget to each queue fairly, but don't allow
	 * the budget to go below 1 because we'll exit polling */
	if (q_vector->rx.count > 1)
		per_ring_budget = max(budget/q_vector->rx.count, 1);
	else
		per_ring_budget = budget;

<<<<<<< HEAD
	ixgbe_for_each_ring(ring, q_vector->rx)
		clean_complete &= ixgbe_clean_rx_irq(q_vector, ring,
						     per_ring_budget);
=======
	ixgbe_for_each_ring(ring, q_vector->rx) {
		int cleaned = ring->xsk_pool ?
			      ixgbe_clean_rx_irq_zc(q_vector, ring,
						    per_ring_budget) :
			      ixgbe_clean_rx_irq(q_vector, ring,
						 per_ring_budget);

		work_done += cleaned;
		if (cleaned >= per_ring_budget)
			clean_complete = false;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If all work not completed, return budget and keep polling */
	if (!clean_complete)
		return budget;

	/* all work done, exit the polling mode */
<<<<<<< HEAD
	napi_complete(napi);
	if (adapter->rx_itr_setting & 1)
		ixgbe_set_itr(q_vector);
	if (!test_bit(__IXGBE_DOWN, &adapter->state))
		ixgbe_irq_enable_queues(adapter, ((u64)1 << q_vector->v_idx));

	return 0;
=======
	if (likely(napi_complete_done(napi, work_done))) {
		if (adapter->rx_itr_setting & 1)
			ixgbe_set_itr(q_vector);
		if (!test_bit(__IXGBE_DOWN, &adapter->state))
			ixgbe_irq_enable_queues(adapter,
						BIT_ULL(q_vector->v_idx));
	}

	return min(work_done, budget - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_request_msix_irqs - Initialize MSI-X interrupts
 * @adapter: board private structure
 *
 * ixgbe_request_msix_irqs allocates MSI-X vectors and requests
 * interrupts from the kernel.
 **/
static int ixgbe_request_msix_irqs(struct ixgbe_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
<<<<<<< HEAD
	int q_vectors = adapter->num_msix_vectors - NON_Q_VECTORS;
	int vector, err;
	int ri = 0, ti = 0;

	for (vector = 0; vector < q_vectors; vector++) {
=======
	unsigned int ri = 0, ti = 0;
	int vector, err;

	for (vector = 0; vector < adapter->num_q_vectors; vector++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct ixgbe_q_vector *q_vector = adapter->q_vector[vector];
		struct msix_entry *entry = &adapter->msix_entries[vector];

		if (q_vector->tx.ring && q_vector->rx.ring) {
<<<<<<< HEAD
			snprintf(q_vector->name, sizeof(q_vector->name) - 1,
				 "%s-%s-%d", netdev->name, "TxRx", ri++);
			ti++;
		} else if (q_vector->rx.ring) {
			snprintf(q_vector->name, sizeof(q_vector->name) - 1,
				 "%s-%s-%d", netdev->name, "rx", ri++);
		} else if (q_vector->tx.ring) {
			snprintf(q_vector->name, sizeof(q_vector->name) - 1,
				 "%s-%s-%d", netdev->name, "tx", ti++);
=======
			snprintf(q_vector->name, sizeof(q_vector->name),
				 "%s-TxRx-%u", netdev->name, ri++);
			ti++;
		} else if (q_vector->rx.ring) {
			snprintf(q_vector->name, sizeof(q_vector->name),
				 "%s-rx-%u", netdev->name, ri++);
		} else if (q_vector->tx.ring) {
			snprintf(q_vector->name, sizeof(q_vector->name),
				 "%s-tx-%u", netdev->name, ti++);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			/* skip this unused q_vector */
			continue;
		}
		err = request_irq(entry->vector, &ixgbe_msix_clean_rings, 0,
				  q_vector->name, q_vector);
		if (err) {
			e_err(probe, "request_irq failed for MSIX interrupt "
			      "Error: %d\n", err);
			goto free_queue_irqs;
		}
		/* If Flow Director is enabled, set interrupt affinity */
		if (adapter->flags & IXGBE_FLAG_FDIR_HASH_CAPABLE) {
			/* assign the mask for this irq */
<<<<<<< HEAD
			irq_set_affinity_hint(entry->vector,
					      &q_vector->affinity_mask);
=======
			irq_update_affinity_hint(entry->vector,
						 &q_vector->affinity_mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	err = request_irq(adapter->msix_entries[vector].vector,
			  ixgbe_msix_other, 0, netdev->name, adapter);
	if (err) {
		e_err(probe, "request_irq for msix_other failed: %d\n", err);
		goto free_queue_irqs;
	}

	return 0;

free_queue_irqs:
	while (vector) {
		vector--;
<<<<<<< HEAD
		irq_set_affinity_hint(adapter->msix_entries[vector].vector,
				      NULL);
=======
		irq_update_affinity_hint(adapter->msix_entries[vector].vector,
					 NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		free_irq(adapter->msix_entries[vector].vector,
			 adapter->q_vector[vector]);
	}
	adapter->flags &= ~IXGBE_FLAG_MSIX_ENABLED;
	pci_disable_msix(adapter->pdev);
	kfree(adapter->msix_entries);
	adapter->msix_entries = NULL;
	return err;
}

/**
 * ixgbe_intr - legacy mode Interrupt Handler
 * @irq: interrupt number
 * @data: pointer to a network interface device structure
 **/
static irqreturn_t ixgbe_intr(int irq, void *data)
{
	struct ixgbe_adapter *adapter = data;
	struct ixgbe_hw *hw = &adapter->hw;
	struct ixgbe_q_vector *q_vector = adapter->q_vector[0];
	u32 eicr;

	/*
	 * Workaround for silicon errata #26 on 82598.  Mask the interrupt
	 * before the read of EICR.
	 */
	IXGBE_WRITE_REG(hw, IXGBE_EIMC, IXGBE_IRQ_CLEAR_MASK);

	/* for NAPI, using EIAM to auto-mask tx/rx interrupt bits on read
	 * therefore no explicit interrupt disable is necessary */
	eicr = IXGBE_READ_REG(hw, IXGBE_EICR);
	if (!eicr) {
		/*
		 * shared interrupt alert!
		 * make sure interrupts are enabled because the read will
		 * have disabled interrupts due to EIAM
		 * finish the workaround of silicon errata on 82598.  Unmask
		 * the interrupt that we masked before the EICR read.
		 */
		if (!test_bit(__IXGBE_DOWN, &adapter->state))
			ixgbe_irq_enable(adapter, true, true);
		return IRQ_NONE;	/* Not our interrupt */
	}

	if (eicr & IXGBE_EICR_LSC)
		ixgbe_check_lsc(adapter);

	switch (hw->mac.type) {
	case ixgbe_mac_82599EB:
		ixgbe_check_sfp_event(adapter, eicr);
<<<<<<< HEAD
		/* Fall through */
	case ixgbe_mac_X540:
		if (eicr & IXGBE_EICR_ECC)
			e_info(link, "Received unrecoverable ECC err, please "
				     "reboot\n");
=======
		fallthrough;
	case ixgbe_mac_X540:
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
		if (eicr & IXGBE_EICR_ECC) {
			e_info(link, "Received ECC Err, initiating reset\n");
			set_bit(__IXGBE_RESET_REQUESTED, &adapter->state);
			ixgbe_service_event_schedule(adapter);
			IXGBE_WRITE_REG(hw, IXGBE_EICR, IXGBE_EICR_ECC);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ixgbe_check_overtemp_event(adapter, eicr);
		break;
	default:
		break;
	}

	ixgbe_check_fan_failure(adapter, eicr);
<<<<<<< HEAD

	/* would disable interrupts here but EIAM disabled it */
	napi_schedule(&q_vector->napi);
=======
	if (unlikely(eicr & IXGBE_EICR_TIMESYNC))
		ixgbe_ptp_check_pps_event(adapter);

	/* would disable interrupts here but EIAM disabled it */
	napi_schedule_irqoff(&q_vector->napi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * re-enable link(maybe) and non-queue interrupts, no flush.
	 * ixgbe_poll will re-enable the queue interrupts
	 */
	if (!test_bit(__IXGBE_DOWN, &adapter->state))
		ixgbe_irq_enable(adapter, false, false);

	return IRQ_HANDLED;
}

/**
 * ixgbe_request_irq - initialize interrupts
 * @adapter: board private structure
 *
 * Attempts to configure interrupts using the best available
 * capabilities of the hardware and kernel.
 **/
static int ixgbe_request_irq(struct ixgbe_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	int err;

	if (adapter->flags & IXGBE_FLAG_MSIX_ENABLED)
		err = ixgbe_request_msix_irqs(adapter);
	else if (adapter->flags & IXGBE_FLAG_MSI_ENABLED)
		err = request_irq(adapter->pdev->irq, ixgbe_intr, 0,
				  netdev->name, adapter);
	else
		err = request_irq(adapter->pdev->irq, ixgbe_intr, IRQF_SHARED,
				  netdev->name, adapter);

	if (err)
		e_err(probe, "request_irq failed, Error %d\n", err);

	return err;
}

static void ixgbe_free_irq(struct ixgbe_adapter *adapter)
{
<<<<<<< HEAD
	if (adapter->flags & IXGBE_FLAG_MSIX_ENABLED) {
		int i, q_vectors;

		q_vectors = adapter->num_msix_vectors;
		i = q_vectors - 1;
		free_irq(adapter->msix_entries[i].vector, adapter);
		i--;

		for (; i >= 0; i--) {
			/* free only the irqs that were actually requested */
			if (!adapter->q_vector[i]->rx.ring &&
			    !adapter->q_vector[i]->tx.ring)
				continue;

			/* clear the affinity_mask in the IRQ descriptor */
			irq_set_affinity_hint(adapter->msix_entries[i].vector,
					      NULL);

			free_irq(adapter->msix_entries[i].vector,
				 adapter->q_vector[i]);
		}
	} else {
		free_irq(adapter->pdev->irq, adapter);
	}
=======
	int vector;

	if (!(adapter->flags & IXGBE_FLAG_MSIX_ENABLED)) {
		free_irq(adapter->pdev->irq, adapter);
		return;
	}

	if (!adapter->msix_entries)
		return;

	for (vector = 0; vector < adapter->num_q_vectors; vector++) {
		struct ixgbe_q_vector *q_vector = adapter->q_vector[vector];
		struct msix_entry *entry = &adapter->msix_entries[vector];

		/* free only the irqs that were actually requested */
		if (!q_vector->rx.ring && !q_vector->tx.ring)
			continue;

		/* clear the affinity_mask in the IRQ descriptor */
		irq_update_affinity_hint(entry->vector, NULL);

		free_irq(entry->vector, q_vector);
	}

	free_irq(adapter->msix_entries[vector].vector, adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_irq_disable - Mask off interrupt generation on the NIC
 * @adapter: board private structure
 **/
static inline void ixgbe_irq_disable(struct ixgbe_adapter *adapter)
{
	switch (adapter->hw.mac.type) {
	case ixgbe_mac_82598EB:
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_EIMC, ~0);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
<<<<<<< HEAD
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_EIMC, 0xFFFF0000);
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_EIMC_EX(0), ~0);
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_EIMC_EX(1), ~0);
		break;
	default:
		break;
	}
	IXGBE_WRITE_FLUSH(&adapter->hw);
	if (adapter->flags & IXGBE_FLAG_MSIX_ENABLED) {
<<<<<<< HEAD
		int i;
		for (i = 0; i < adapter->num_msix_vectors; i++)
			synchronize_irq(adapter->msix_entries[i].vector);
=======
		int vector;

		for (vector = 0; vector < adapter->num_q_vectors; vector++)
			synchronize_irq(adapter->msix_entries[vector].vector);

		synchronize_irq(adapter->msix_entries[vector++].vector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		synchronize_irq(adapter->pdev->irq);
	}
}

/**
 * ixgbe_configure_msi_and_legacy - Initialize PIN (INTA...) and MSI interrupts
<<<<<<< HEAD
=======
 * @adapter: board private structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 **/
static void ixgbe_configure_msi_and_legacy(struct ixgbe_adapter *adapter)
{
	struct ixgbe_q_vector *q_vector = adapter->q_vector[0];

<<<<<<< HEAD
	/* rx/tx vector */
	if (adapter->rx_itr_setting == 1)
		q_vector->itr = IXGBE_20K_ITR;
	else
		q_vector->itr = adapter->rx_itr_setting;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ixgbe_write_eitr(q_vector);

	ixgbe_set_ivar(adapter, 0, 0, 0);
	ixgbe_set_ivar(adapter, 1, 0, 0);

	e_info(hw, "Legacy interrupt IVAR setup done\n");
}

/**
 * ixgbe_configure_tx_ring - Configure 8259x Tx ring after Reset
 * @adapter: board private structure
 * @ring: structure containing ring specific data
 *
 * Configure the Tx descriptor ring after a reset.
 **/
void ixgbe_configure_tx_ring(struct ixgbe_adapter *adapter,
			     struct ixgbe_ring *ring)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u64 tdba = ring->dma;
	int wait_loop = 10;
	u32 txdctl = IXGBE_TXDCTL_ENABLE;
	u8 reg_idx = ring->reg_idx;

<<<<<<< HEAD
=======
	ring->xsk_pool = NULL;
	if (ring_is_xdp(ring))
		ring->xsk_pool = ixgbe_xsk_pool(adapter, ring);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* disable queue to avoid issues while updating state */
	IXGBE_WRITE_REG(hw, IXGBE_TXDCTL(reg_idx), 0);
	IXGBE_WRITE_FLUSH(hw);

	IXGBE_WRITE_REG(hw, IXGBE_TDBAL(reg_idx),
			(tdba & DMA_BIT_MASK(32)));
	IXGBE_WRITE_REG(hw, IXGBE_TDBAH(reg_idx), (tdba >> 32));
	IXGBE_WRITE_REG(hw, IXGBE_TDLEN(reg_idx),
			ring->count * sizeof(union ixgbe_adv_tx_desc));
	IXGBE_WRITE_REG(hw, IXGBE_TDH(reg_idx), 0);
	IXGBE_WRITE_REG(hw, IXGBE_TDT(reg_idx), 0);
<<<<<<< HEAD
	ring->tail = hw->hw_addr + IXGBE_TDT(reg_idx);

	/*
	 * set WTHRESH to encourage burst writeback, it should not be set
	 * higher than 1 when ITR is 0 as it could cause false TX hangs
=======
	ring->tail = adapter->io_addr + IXGBE_TDT(reg_idx);

	/*
	 * set WTHRESH to encourage burst writeback, it should not be set
	 * higher than 1 when:
	 * - ITR is 0 as it could cause false TX hangs
	 * - ITR is set to > 100k int/sec and BQL is enabled
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 *
	 * In order to avoid issues WTHRESH + PTHRESH should always be equal
	 * to or less than the number of on chip descriptors, which is
	 * currently 40.
	 */
<<<<<<< HEAD
	if (!ring->q_vector || (ring->q_vector->itr < 8))
		txdctl |= (1 << 16);	/* WTHRESH = 1 */
	else
		txdctl |= (8 << 16);	/* WTHRESH = 8 */
=======
	if (!ring->q_vector || (ring->q_vector->itr < IXGBE_100K_ITR))
		txdctl |= 1u << 16;	/* WTHRESH = 1 */
	else
		txdctl |= 8u << 16;	/* WTHRESH = 8 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Setting PTHRESH to 32 both improves performance
	 * and avoids a TX hang with DFP enabled
	 */
<<<<<<< HEAD
	txdctl |= (1 << 8) |	/* HTHRESH = 1 */
		   32;		/* PTHRESH = 32 */

	/* reinitialize flowdirector state */
	if ((adapter->flags & IXGBE_FLAG_FDIR_HASH_CAPABLE) &&
	    adapter->atr_sample_rate) {
=======
	txdctl |= (1u << 8) |	/* HTHRESH = 1 */
		   32;		/* PTHRESH = 32 */

	/* reinitialize flowdirector state */
	if (adapter->flags & IXGBE_FLAG_FDIR_HASH_CAPABLE) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ring->atr_sample_rate = adapter->atr_sample_rate;
		ring->atr_count = 0;
		set_bit(__IXGBE_TX_FDIR_INIT_DONE, &ring->state);
	} else {
		ring->atr_sample_rate = 0;
	}

<<<<<<< HEAD
	clear_bit(__IXGBE_HANG_CHECK_ARMED, &ring->state);

=======
	/* initialize XPS */
	if (!test_and_set_bit(__IXGBE_TX_XPS_INIT_DONE, &ring->state)) {
		struct ixgbe_q_vector *q_vector = ring->q_vector;

		if (q_vector)
			netif_set_xps_queue(ring->netdev,
					    &q_vector->affinity_mask,
					    ring->queue_index);
	}

	clear_bit(__IXGBE_HANG_CHECK_ARMED, &ring->state);

	/* reinitialize tx_buffer_info */
	memset(ring->tx_buffer_info, 0,
	       sizeof(struct ixgbe_tx_buffer) * ring->count);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* enable queue */
	IXGBE_WRITE_REG(hw, IXGBE_TXDCTL(reg_idx), txdctl);

	/* TXDCTL.EN will return 0 on 82598 if link is down, so skip it */
	if (hw->mac.type == ixgbe_mac_82598EB &&
	    !(IXGBE_READ_REG(hw, IXGBE_LINKS) & IXGBE_LINKS_UP))
		return;

	/* poll to verify queue is enabled */
	do {
		usleep_range(1000, 2000);
		txdctl = IXGBE_READ_REG(hw, IXGBE_TXDCTL(reg_idx));
	} while (--wait_loop && !(txdctl & IXGBE_TXDCTL_ENABLE));
	if (!wait_loop)
<<<<<<< HEAD
		e_err(drv, "Could not enable Tx Queue %d\n", reg_idx);
=======
		hw_dbg(hw, "Could not enable Tx Queue %d\n", reg_idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_setup_mtqc(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD
	u32 rttdcs;
	u32 reg;
	u8 tcs = netdev_get_num_tc(adapter->netdev);
=======
	u32 rttdcs, mtqc;
	u8 tcs = adapter->hw_tcs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (hw->mac.type == ixgbe_mac_82598EB)
		return;

	/* disable the arbiter while setting MTQC */
	rttdcs = IXGBE_READ_REG(hw, IXGBE_RTTDCS);
	rttdcs |= IXGBE_RTTDCS_ARBDIS;
	IXGBE_WRITE_REG(hw, IXGBE_RTTDCS, rttdcs);

	/* set transmit pool layout */
<<<<<<< HEAD
	switch (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED) {
	case (IXGBE_FLAG_SRIOV_ENABLED):
		IXGBE_WRITE_REG(hw, IXGBE_MTQC,
				(IXGBE_MTQC_VT_ENA | IXGBE_MTQC_64VF));
		break;
	default:
		if (!tcs)
			reg = IXGBE_MTQC_64Q_1PB;
		else if (tcs <= 4)
			reg = IXGBE_MTQC_RT_ENA | IXGBE_MTQC_4TC_4TQ;
		else
			reg = IXGBE_MTQC_RT_ENA | IXGBE_MTQC_8TC_8TQ;

		IXGBE_WRITE_REG(hw, IXGBE_MTQC, reg);

		/* Enable Security TX Buffer IFG for multiple pb */
		if (tcs) {
			reg = IXGBE_READ_REG(hw, IXGBE_SECTXMINIFG);
			reg |= IXGBE_SECTX_DCB;
			IXGBE_WRITE_REG(hw, IXGBE_SECTXMINIFG, reg);
		}
		break;
=======
	if (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED) {
		mtqc = IXGBE_MTQC_VT_ENA;
		if (tcs > 4)
			mtqc |= IXGBE_MTQC_RT_ENA | IXGBE_MTQC_8TC_8TQ;
		else if (tcs > 1)
			mtqc |= IXGBE_MTQC_RT_ENA | IXGBE_MTQC_4TC_4TQ;
		else if (adapter->ring_feature[RING_F_VMDQ].mask ==
			 IXGBE_82599_VMDQ_4Q_MASK)
			mtqc |= IXGBE_MTQC_32VF;
		else
			mtqc |= IXGBE_MTQC_64VF;
	} else {
		if (tcs > 4) {
			mtqc = IXGBE_MTQC_RT_ENA | IXGBE_MTQC_8TC_8TQ;
		} else if (tcs > 1) {
			mtqc = IXGBE_MTQC_RT_ENA | IXGBE_MTQC_4TC_4TQ;
		} else {
			u8 max_txq = adapter->num_tx_queues +
				adapter->num_xdp_queues;
			if (max_txq > 63)
				mtqc = IXGBE_MTQC_RT_ENA | IXGBE_MTQC_4TC_4TQ;
			else
				mtqc = IXGBE_MTQC_64Q_1PB;
		}
	}

	IXGBE_WRITE_REG(hw, IXGBE_MTQC, mtqc);

	/* Enable Security TX Buffer IFG for multiple pb */
	if (tcs) {
		u32 sectx = IXGBE_READ_REG(hw, IXGBE_SECTXMINIFG);
		sectx |= IXGBE_SECTX_DCB;
		IXGBE_WRITE_REG(hw, IXGBE_SECTXMINIFG, sectx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* re-enable the arbiter */
	rttdcs &= ~IXGBE_RTTDCS_ARBDIS;
	IXGBE_WRITE_REG(hw, IXGBE_RTTDCS, rttdcs);
}

/**
 * ixgbe_configure_tx - Configure 8259x Transmit Unit after Reset
 * @adapter: board private structure
 *
 * Configure the Tx unit of the MAC after a reset.
 **/
static void ixgbe_configure_tx(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 dmatxctl;
	u32 i;

	ixgbe_setup_mtqc(adapter);

	if (hw->mac.type != ixgbe_mac_82598EB) {
		/* DMATXCTL.EN must be before Tx queues are enabled */
		dmatxctl = IXGBE_READ_REG(hw, IXGBE_DMATXCTL);
		dmatxctl |= IXGBE_DMATXCTL_TE;
		IXGBE_WRITE_REG(hw, IXGBE_DMATXCTL, dmatxctl);
	}

	/* Setup the HW Tx Head and Tail descriptor pointers */
	for (i = 0; i < adapter->num_tx_queues; i++)
		ixgbe_configure_tx_ring(adapter, adapter->tx_ring[i]);
<<<<<<< HEAD
=======
	for (i = 0; i < adapter->num_xdp_queues; i++)
		ixgbe_configure_tx_ring(adapter, adapter->xdp_ring[i]);
}

static void ixgbe_enable_rx_drop(struct ixgbe_adapter *adapter,
				 struct ixgbe_ring *ring)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u8 reg_idx = ring->reg_idx;
	u32 srrctl = IXGBE_READ_REG(hw, IXGBE_SRRCTL(reg_idx));

	srrctl |= IXGBE_SRRCTL_DROP_EN;

	IXGBE_WRITE_REG(hw, IXGBE_SRRCTL(reg_idx), srrctl);
}

static void ixgbe_disable_rx_drop(struct ixgbe_adapter *adapter,
				  struct ixgbe_ring *ring)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u8 reg_idx = ring->reg_idx;
	u32 srrctl = IXGBE_READ_REG(hw, IXGBE_SRRCTL(reg_idx));

	srrctl &= ~IXGBE_SRRCTL_DROP_EN;

	IXGBE_WRITE_REG(hw, IXGBE_SRRCTL(reg_idx), srrctl);
}

#ifdef CONFIG_IXGBE_DCB
void ixgbe_set_rx_drop_en(struct ixgbe_adapter *adapter)
#else
static void ixgbe_set_rx_drop_en(struct ixgbe_adapter *adapter)
#endif
{
	int i;
	bool pfc_en = adapter->dcb_cfg.pfc_mode_enable;

	if (adapter->ixgbe_ieee_pfc)
		pfc_en |= !!(adapter->ixgbe_ieee_pfc->pfc_en);

	/*
	 * We should set the drop enable bit if:
	 *  SR-IOV is enabled
	 *   or
	 *  Number of Rx queues > 1 and flow control is disabled
	 *
	 *  This allows us to avoid head of line blocking for security
	 *  and performance reasons.
	 */
	if (adapter->num_vfs || (adapter->num_rx_queues > 1 &&
	    !(adapter->hw.fc.current_mode & ixgbe_fc_tx_pause) && !pfc_en)) {
		for (i = 0; i < adapter->num_rx_queues; i++)
			ixgbe_enable_rx_drop(adapter, adapter->rx_ring[i]);
	} else {
		for (i = 0; i < adapter->num_rx_queues; i++)
			ixgbe_disable_rx_drop(adapter, adapter->rx_ring[i]);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define IXGBE_SRRCTL_BSIZEHDRSIZE_SHIFT 2

static void ixgbe_configure_srrctl(struct ixgbe_adapter *adapter,
				   struct ixgbe_ring *rx_ring)
{
<<<<<<< HEAD
	u32 srrctl;
	u8 reg_idx = rx_ring->reg_idx;

	switch (adapter->hw.mac.type) {
	case ixgbe_mac_82598EB: {
		struct ixgbe_ring_feature *feature = adapter->ring_feature;
		const int mask = feature[RING_F_RSS].mask;
		reg_idx = reg_idx & mask;
	}
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
	default:
		break;
	}

	srrctl = IXGBE_READ_REG(&adapter->hw, IXGBE_SRRCTL(reg_idx));

	srrctl &= ~IXGBE_SRRCTL_BSIZEHDR_MASK;
	srrctl &= ~IXGBE_SRRCTL_BSIZEPKT_MASK;
	if (adapter->num_vfs)
		srrctl |= IXGBE_SRRCTL_DROP_EN;

	srrctl |= (IXGBE_RX_HDR_SIZE << IXGBE_SRRCTL_BSIZEHDRSIZE_SHIFT) &
		  IXGBE_SRRCTL_BSIZEHDR_MASK;

#if PAGE_SIZE > IXGBE_MAX_RXBUFFER
	srrctl |= IXGBE_MAX_RXBUFFER >> IXGBE_SRRCTL_BSIZEPKT_SHIFT;
#else
	srrctl |= ixgbe_rx_bufsz(rx_ring) >> IXGBE_SRRCTL_BSIZEPKT_SHIFT;
#endif
	srrctl |= IXGBE_SRRCTL_DESCTYPE_ADV_ONEBUF;

	IXGBE_WRITE_REG(&adapter->hw, IXGBE_SRRCTL(reg_idx), srrctl);
=======
	struct ixgbe_hw *hw = &adapter->hw;
	u32 srrctl;
	u8 reg_idx = rx_ring->reg_idx;

	if (hw->mac.type == ixgbe_mac_82598EB) {
		u16 mask = adapter->ring_feature[RING_F_RSS].mask;

		/*
		 * if VMDq is not active we must program one srrctl register
		 * per RSS queue since we have enabled RDRXCTL.MVMEN
		 */
		reg_idx &= mask;
	}

	/* configure header buffer length, needed for RSC */
	srrctl = IXGBE_RX_HDR_SIZE << IXGBE_SRRCTL_BSIZEHDRSIZE_SHIFT;

	/* configure the packet buffer length */
	if (rx_ring->xsk_pool) {
		u32 xsk_buf_len = xsk_pool_get_rx_frame_size(rx_ring->xsk_pool);

		/* If the MAC support setting RXDCTL.RLPML, the
		 * SRRCTL[n].BSIZEPKT is set to PAGE_SIZE and
		 * RXDCTL.RLPML is set to the actual UMEM buffer
		 * size. If not, then we are stuck with a 1k buffer
		 * size resolution. In this case frames larger than
		 * the UMEM buffer size viewed in a 1k resolution will
		 * be dropped.
		 */
		if (hw->mac.type != ixgbe_mac_82599EB)
			srrctl |= PAGE_SIZE >> IXGBE_SRRCTL_BSIZEPKT_SHIFT;
		else
			srrctl |= xsk_buf_len >> IXGBE_SRRCTL_BSIZEPKT_SHIFT;
	} else if (test_bit(__IXGBE_RX_3K_BUFFER, &rx_ring->state)) {
		srrctl |= IXGBE_RXBUFFER_3K >> IXGBE_SRRCTL_BSIZEPKT_SHIFT;
	} else {
		srrctl |= IXGBE_RXBUFFER_2K >> IXGBE_SRRCTL_BSIZEPKT_SHIFT;
	}

	/* configure descriptor type */
	srrctl |= IXGBE_SRRCTL_DESCTYPE_ADV_ONEBUF;

	IXGBE_WRITE_REG(hw, IXGBE_SRRCTL(reg_idx), srrctl);
}

/**
 * ixgbe_rss_indir_tbl_entries - Return RSS indirection table entries
 * @adapter: device handle
 *
 *  - 82598/82599/X540:     128
 *  - X550(non-SRIOV mode): 512
 *  - X550(SRIOV mode):     64
 */
u32 ixgbe_rss_indir_tbl_entries(struct ixgbe_adapter *adapter)
{
	if (adapter->hw.mac.type < ixgbe_mac_X550)
		return 128;
	else if (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED)
		return 64;
	else
		return 512;
}

/**
 * ixgbe_store_key - Write the RSS key to HW
 * @adapter: device handle
 *
 * Write the RSS key stored in adapter.rss_key to HW.
 */
void ixgbe_store_key(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	int i;

	for (i = 0; i < 10; i++)
		IXGBE_WRITE_REG(hw, IXGBE_RSSRK(i), adapter->rss_key[i]);
}

/**
 * ixgbe_init_rss_key - Initialize adapter RSS key
 * @adapter: device handle
 *
 * Allocates and initializes the RSS key if it is not allocated.
 **/
static inline int ixgbe_init_rss_key(struct ixgbe_adapter *adapter)
{
	u32 *rss_key;

	if (!adapter->rss_key) {
		rss_key = kzalloc(IXGBE_RSS_KEY_SIZE, GFP_KERNEL);
		if (unlikely(!rss_key))
			return -ENOMEM;

		netdev_rss_key_fill(rss_key, IXGBE_RSS_KEY_SIZE);
		adapter->rss_key = rss_key;
	}

	return 0;
}

/**
 * ixgbe_store_reta - Write the RETA table to HW
 * @adapter: device handle
 *
 * Write the RSS redirection table stored in adapter.rss_indir_tbl[] to HW.
 */
void ixgbe_store_reta(struct ixgbe_adapter *adapter)
{
	u32 i, reta_entries = ixgbe_rss_indir_tbl_entries(adapter);
	struct ixgbe_hw *hw = &adapter->hw;
	u32 reta = 0;
	u32 indices_multi;
	u8 *indir_tbl = adapter->rss_indir_tbl;

	/* Fill out the redirection table as follows:
	 *  - 82598:      8 bit wide entries containing pair of 4 bit RSS
	 *    indices.
	 *  - 82599/X540: 8 bit wide entries containing 4 bit RSS index
	 *  - X550:       8 bit wide entries containing 6 bit RSS index
	 */
	if (adapter->hw.mac.type == ixgbe_mac_82598EB)
		indices_multi = 0x11;
	else
		indices_multi = 0x1;

	/* Write redirection table to HW */
	for (i = 0; i < reta_entries; i++) {
		reta |= indices_multi * indir_tbl[i] << (i & 0x3) * 8;
		if ((i & 3) == 3) {
			if (i < 128)
				IXGBE_WRITE_REG(hw, IXGBE_RETA(i >> 2), reta);
			else
				IXGBE_WRITE_REG(hw, IXGBE_ERETA((i >> 2) - 32),
						reta);
			reta = 0;
		}
	}
}

/**
 * ixgbe_store_vfreta - Write the RETA table to HW (x550 devices in SRIOV mode)
 * @adapter: device handle
 *
 * Write the RSS redirection table stored in adapter.rss_indir_tbl[] to HW.
 */
static void ixgbe_store_vfreta(struct ixgbe_adapter *adapter)
{
	u32 i, reta_entries = ixgbe_rss_indir_tbl_entries(adapter);
	struct ixgbe_hw *hw = &adapter->hw;
	u32 vfreta = 0;

	/* Write redirection table to HW */
	for (i = 0; i < reta_entries; i++) {
		u16 pool = adapter->num_rx_pools;

		vfreta |= (u32)adapter->rss_indir_tbl[i] << (i & 0x3) * 8;
		if ((i & 3) != 3)
			continue;

		while (pool--)
			IXGBE_WRITE_REG(hw,
					IXGBE_PFVFRETA(i >> 2, VMDQ_P(pool)),
					vfreta);
		vfreta = 0;
	}
}

static void ixgbe_setup_reta(struct ixgbe_adapter *adapter)
{
	u32 i, j;
	u32 reta_entries = ixgbe_rss_indir_tbl_entries(adapter);
	u16 rss_i = adapter->ring_feature[RING_F_RSS].indices;

	/* Program table for at least 4 queues w/ SR-IOV so that VFs can
	 * make full use of any rings they may have.  We will use the
	 * PSRTYPE register to control how many rings we use within the PF.
	 */
	if ((adapter->flags & IXGBE_FLAG_SRIOV_ENABLED) && (rss_i < 4))
		rss_i = 4;

	/* Fill out hash function seeds */
	ixgbe_store_key(adapter);

	/* Fill out redirection table */
	memset(adapter->rss_indir_tbl, 0, sizeof(adapter->rss_indir_tbl));

	for (i = 0, j = 0; i < reta_entries; i++, j++) {
		if (j == rss_i)
			j = 0;

		adapter->rss_indir_tbl[i] = j;
	}

	ixgbe_store_reta(adapter);
}

static void ixgbe_setup_vfreta(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u16 rss_i = adapter->ring_feature[RING_F_RSS].indices;
	int i, j;

	/* Fill out hash function seeds */
	for (i = 0; i < 10; i++) {
		u16 pool = adapter->num_rx_pools;

		while (pool--)
			IXGBE_WRITE_REG(hw,
					IXGBE_PFVFRSSRK(i, VMDQ_P(pool)),
					*(adapter->rss_key + i));
	}

	/* Fill out the redirection table */
	for (i = 0, j = 0; i < 64; i++, j++) {
		if (j == rss_i)
			j = 0;

		adapter->rss_indir_tbl[i] = j;
	}

	ixgbe_store_vfreta(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_setup_mrqc(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD
	static const u32 seed[10] = { 0xE291D73D, 0x1805EC6C, 0x2A94B30D,
			  0xA54F2BEC, 0xEA49AF7C, 0xE214AD3D, 0xB855AABE,
			  0x6A3E67EA, 0x14364D17, 0x3BED200D};
	u32 mrqc = 0, reta = 0;
	u32 rxcsum;
	int i, j;
	u8 tcs = netdev_get_num_tc(adapter->netdev);
	int maxq = adapter->ring_feature[RING_F_RSS].indices;

	if (tcs)
		maxq = min(maxq, adapter->num_tx_queues / tcs);

	/* Fill out hash function seeds */
	for (i = 0; i < 10; i++)
		IXGBE_WRITE_REG(hw, IXGBE_RSSRK(i), seed[i]);

	/* Fill out redirection table */
	for (i = 0, j = 0; i < 128; i++, j++) {
		if (j == maxq)
			j = 0;
		/* reta = 4-byte sliding window of
		 * 0x00..(indices-1)(indices-1)00..etc. */
		reta = (reta << 8) | (j * 0x11);
		if ((i & 3) == 3)
			IXGBE_WRITE_REG(hw, IXGBE_RETA(i >> 2), reta);
	}
=======
	u32 mrqc = 0, rss_field = 0, vfmrqc = 0;
	u32 rxcsum;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Disable indicating checksum in descriptor, enables RSS hash */
	rxcsum = IXGBE_READ_REG(hw, IXGBE_RXCSUM);
	rxcsum |= IXGBE_RXCSUM_PCSD;
	IXGBE_WRITE_REG(hw, IXGBE_RXCSUM, rxcsum);

<<<<<<< HEAD
	if (adapter->hw.mac.type == ixgbe_mac_82598EB &&
	    (adapter->flags & IXGBE_FLAG_RSS_ENABLED)) {
		mrqc = IXGBE_MRQC_RSSEN;
	} else {
		int mask = adapter->flags & (IXGBE_FLAG_RSS_ENABLED
					     | IXGBE_FLAG_SRIOV_ENABLED);

		switch (mask) {
		case (IXGBE_FLAG_RSS_ENABLED):
			if (!tcs)
				mrqc = IXGBE_MRQC_RSSEN;
			else if (tcs <= 4)
				mrqc = IXGBE_MRQC_RTRSS4TCEN;
			else
				mrqc = IXGBE_MRQC_RTRSS8TCEN;
			break;
		case (IXGBE_FLAG_SRIOV_ENABLED):
			mrqc = IXGBE_MRQC_VMDQEN;
			break;
		default:
			break;
=======
	if (adapter->hw.mac.type == ixgbe_mac_82598EB) {
		if (adapter->ring_feature[RING_F_RSS].mask)
			mrqc = IXGBE_MRQC_RSSEN;
	} else {
		u8 tcs = adapter->hw_tcs;

		if (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED) {
			if (tcs > 4)
				mrqc = IXGBE_MRQC_VMDQRT8TCEN;	/* 8 TCs */
			else if (tcs > 1)
				mrqc = IXGBE_MRQC_VMDQRT4TCEN;	/* 4 TCs */
			else if (adapter->ring_feature[RING_F_VMDQ].mask ==
				 IXGBE_82599_VMDQ_4Q_MASK)
				mrqc = IXGBE_MRQC_VMDQRSS32EN;
			else
				mrqc = IXGBE_MRQC_VMDQRSS64EN;

			/* Enable L3/L4 for Tx Switched packets only for X550,
			 * older devices do not support this feature
			 */
			if (hw->mac.type >= ixgbe_mac_X550)
				mrqc |= IXGBE_MRQC_L3L4TXSWEN;
		} else {
			if (tcs > 4)
				mrqc = IXGBE_MRQC_RTRSS8TCEN;
			else if (tcs > 1)
				mrqc = IXGBE_MRQC_RTRSS4TCEN;
			else
				mrqc = IXGBE_MRQC_RSSEN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/* Perform hash on these packet types */
<<<<<<< HEAD
	mrqc |= IXGBE_MRQC_RSS_FIELD_IPV4
	      | IXGBE_MRQC_RSS_FIELD_IPV4_TCP
	      | IXGBE_MRQC_RSS_FIELD_IPV6
	      | IXGBE_MRQC_RSS_FIELD_IPV6_TCP;

	if (adapter->flags2 & IXGBE_FLAG2_RSS_FIELD_IPV4_UDP)
		mrqc |= IXGBE_MRQC_RSS_FIELD_IPV4_UDP;
	if (adapter->flags2 & IXGBE_FLAG2_RSS_FIELD_IPV6_UDP)
		mrqc |= IXGBE_MRQC_RSS_FIELD_IPV6_UDP;

	IXGBE_WRITE_REG(hw, IXGBE_MRQC, mrqc);
=======
	rss_field |= IXGBE_MRQC_RSS_FIELD_IPV4 |
		     IXGBE_MRQC_RSS_FIELD_IPV4_TCP |
		     IXGBE_MRQC_RSS_FIELD_IPV6 |
		     IXGBE_MRQC_RSS_FIELD_IPV6_TCP;

	if (adapter->flags2 & IXGBE_FLAG2_RSS_FIELD_IPV4_UDP)
		rss_field |= IXGBE_MRQC_RSS_FIELD_IPV4_UDP;
	if (adapter->flags2 & IXGBE_FLAG2_RSS_FIELD_IPV6_UDP)
		rss_field |= IXGBE_MRQC_RSS_FIELD_IPV6_UDP;

	if ((hw->mac.type >= ixgbe_mac_X550) &&
	    (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED)) {
		u16 pool = adapter->num_rx_pools;

		/* Enable VF RSS mode */
		mrqc |= IXGBE_MRQC_MULTIPLE_RSS;
		IXGBE_WRITE_REG(hw, IXGBE_MRQC, mrqc);

		/* Setup RSS through the VF registers */
		ixgbe_setup_vfreta(adapter);
		vfmrqc = IXGBE_MRQC_RSSEN;
		vfmrqc |= rss_field;

		while (pool--)
			IXGBE_WRITE_REG(hw,
					IXGBE_PFVFMRQC(VMDQ_P(pool)),
					vfmrqc);
	} else {
		ixgbe_setup_reta(adapter);
		mrqc |= rss_field;
		IXGBE_WRITE_REG(hw, IXGBE_MRQC, mrqc);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_configure_rscctl - enable RSC for the indicated ring
<<<<<<< HEAD
 * @adapter:    address of board private structure
 * @index:      index of ring to set
=======
 * @adapter: address of board private structure
 * @ring: structure containing ring specific data
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void ixgbe_configure_rscctl(struct ixgbe_adapter *adapter,
				   struct ixgbe_ring *ring)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 rscctrl;
	u8 reg_idx = ring->reg_idx;

	if (!ring_is_rsc_enabled(ring))
		return;

	rscctrl = IXGBE_READ_REG(hw, IXGBE_RSCCTL(reg_idx));
	rscctrl |= IXGBE_RSCCTL_RSCEN;
	/*
	 * we must limit the number of descriptors so that the
	 * total size of max desc * buf_len is not greater
	 * than 65536
	 */
<<<<<<< HEAD
#if (PAGE_SIZE <= 8192)
	rscctrl |= IXGBE_RSCCTL_MAXDESC_16;
#elif (PAGE_SIZE <= 16384)
	rscctrl |= IXGBE_RSCCTL_MAXDESC_8;
#else
	rscctrl |= IXGBE_RSCCTL_MAXDESC_4;
#endif
	IXGBE_WRITE_REG(hw, IXGBE_RSCCTL(reg_idx), rscctrl);
}

/**
 *  ixgbe_set_uta - Set unicast filter table address
 *  @adapter: board private structure
 *
 *  The unicast table address is a register array of 32-bit registers.
 *  The table is meant to be used in a way similar to how the MTA is used
 *  however due to certain limitations in the hardware it is necessary to
 *  set all the hash bits to 1 and use the VMOLR ROPE bit as a promiscuous
 *  enable bit to allow vlan tag stripping when promiscuous mode is enabled
 **/
static void ixgbe_set_uta(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	int i;

	/* The UTA table only exists on 82599 hardware and newer */
	if (hw->mac.type < ixgbe_mac_82599EB)
		return;

	/* we only need to do this if VMDq is enabled */
	if (!(adapter->flags & IXGBE_FLAG_SRIOV_ENABLED))
		return;

	for (i = 0; i < 128; i++)
		IXGBE_WRITE_REG(hw, IXGBE_UTA(i), ~0);
}

=======
	rscctrl |= IXGBE_RSCCTL_MAXDESC_16;
	IXGBE_WRITE_REG(hw, IXGBE_RSCCTL(reg_idx), rscctrl);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define IXGBE_MAX_RX_DESC_POLL 10
static void ixgbe_rx_desc_queue_enable(struct ixgbe_adapter *adapter,
				       struct ixgbe_ring *ring)
{
	struct ixgbe_hw *hw = &adapter->hw;
	int wait_loop = IXGBE_MAX_RX_DESC_POLL;
	u32 rxdctl;
	u8 reg_idx = ring->reg_idx;

<<<<<<< HEAD
=======
	if (ixgbe_removed(hw->hw_addr))
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* RXDCTL.EN will return 0 on 82598 if link is down, so skip it */
	if (hw->mac.type == ixgbe_mac_82598EB &&
	    !(IXGBE_READ_REG(hw, IXGBE_LINKS) & IXGBE_LINKS_UP))
		return;

	do {
		usleep_range(1000, 2000);
		rxdctl = IXGBE_READ_REG(hw, IXGBE_RXDCTL(reg_idx));
	} while (--wait_loop && !(rxdctl & IXGBE_RXDCTL_ENABLE));

	if (!wait_loop) {
		e_err(drv, "RXDCTL.ENABLE on Rx queue %d not set within "
		      "the polling period\n", reg_idx);
	}
}

<<<<<<< HEAD
void ixgbe_disable_rx_queue(struct ixgbe_adapter *adapter,
			    struct ixgbe_ring *ring)
{
	struct ixgbe_hw *hw = &adapter->hw;
	int wait_loop = IXGBE_MAX_RX_DESC_POLL;
	u32 rxdctl;
	u8 reg_idx = ring->reg_idx;

=======
void ixgbe_configure_rx_ring(struct ixgbe_adapter *adapter,
			     struct ixgbe_ring *ring)
{
	struct ixgbe_hw *hw = &adapter->hw;
	union ixgbe_adv_rx_desc *rx_desc;
	u64 rdba = ring->dma;
	u32 rxdctl;
	u8 reg_idx = ring->reg_idx;

	xdp_rxq_info_unreg_mem_model(&ring->xdp_rxq);
	ring->xsk_pool = ixgbe_xsk_pool(adapter, ring);
	if (ring->xsk_pool) {
		WARN_ON(xdp_rxq_info_reg_mem_model(&ring->xdp_rxq,
						   MEM_TYPE_XSK_BUFF_POOL,
						   NULL));
		xsk_pool_set_rxq_info(ring->xsk_pool, &ring->xdp_rxq);
	} else {
		WARN_ON(xdp_rxq_info_reg_mem_model(&ring->xdp_rxq,
						   MEM_TYPE_PAGE_SHARED, NULL));
	}

	/* disable queue to avoid use of these values while updating state */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rxdctl = IXGBE_READ_REG(hw, IXGBE_RXDCTL(reg_idx));
	rxdctl &= ~IXGBE_RXDCTL_ENABLE;

	/* write value back with RXDCTL.ENABLE bit cleared */
	IXGBE_WRITE_REG(hw, IXGBE_RXDCTL(reg_idx), rxdctl);
<<<<<<< HEAD

	if (hw->mac.type == ixgbe_mac_82598EB &&
	    !(IXGBE_READ_REG(hw, IXGBE_LINKS) & IXGBE_LINKS_UP))
		return;

	/* the hardware may take up to 100us to really disable the rx queue */
	do {
		udelay(10);
		rxdctl = IXGBE_READ_REG(hw, IXGBE_RXDCTL(reg_idx));
	} while (--wait_loop && (rxdctl & IXGBE_RXDCTL_ENABLE));

	if (!wait_loop) {
		e_err(drv, "RXDCTL.ENABLE on Rx queue %d not cleared within "
		      "the polling period\n", reg_idx);
	}
}

void ixgbe_configure_rx_ring(struct ixgbe_adapter *adapter,
			     struct ixgbe_ring *ring)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u64 rdba = ring->dma;
	u32 rxdctl;
	u8 reg_idx = ring->reg_idx;

	/* disable queue to avoid issues while updating state */
	rxdctl = IXGBE_READ_REG(hw, IXGBE_RXDCTL(reg_idx));
	ixgbe_disable_rx_queue(adapter, ring);
=======
	IXGBE_WRITE_FLUSH(hw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	IXGBE_WRITE_REG(hw, IXGBE_RDBAL(reg_idx), (rdba & DMA_BIT_MASK(32)));
	IXGBE_WRITE_REG(hw, IXGBE_RDBAH(reg_idx), (rdba >> 32));
	IXGBE_WRITE_REG(hw, IXGBE_RDLEN(reg_idx),
			ring->count * sizeof(union ixgbe_adv_rx_desc));
<<<<<<< HEAD
	IXGBE_WRITE_REG(hw, IXGBE_RDH(reg_idx), 0);
	IXGBE_WRITE_REG(hw, IXGBE_RDT(reg_idx), 0);
	ring->tail = hw->hw_addr + IXGBE_RDT(reg_idx);
=======
	/* Force flushing of IXGBE_RDLEN to prevent MDD */
	IXGBE_WRITE_FLUSH(hw);

	IXGBE_WRITE_REG(hw, IXGBE_RDH(reg_idx), 0);
	IXGBE_WRITE_REG(hw, IXGBE_RDT(reg_idx), 0);
	ring->tail = adapter->io_addr + IXGBE_RDT(reg_idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ixgbe_configure_srrctl(adapter, ring);
	ixgbe_configure_rscctl(adapter, ring);

<<<<<<< HEAD
	/* If operating in IOV mode set RLPML for X540 */
	if ((adapter->flags & IXGBE_FLAG_SRIOV_ENABLED) &&
	    hw->mac.type == ixgbe_mac_X540) {
		rxdctl &= ~IXGBE_RXDCTL_RLPMLMASK;
		rxdctl |= ((ring->netdev->mtu + ETH_HLEN +
			    ETH_FCS_LEN + VLAN_HLEN) | IXGBE_RXDCTL_RLPML_EN);
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (hw->mac.type == ixgbe_mac_82598EB) {
		/*
		 * enable cache line friendly hardware writes:
		 * PTHRESH=32 descriptors (half the internal cache),
		 * this also removes ugly rx_no_buffer_count increment
		 * HTHRESH=4 descriptors (to minimize latency on fetch)
		 * WTHRESH=8 burst writeback up to two cache lines
		 */
		rxdctl &= ~0x3FFFFF;
		rxdctl |=  0x080420;
<<<<<<< HEAD
	}

=======
#if (PAGE_SIZE < 8192)
	/* RXDCTL.RLPML does not work on 82599 */
	} else if (hw->mac.type != ixgbe_mac_82599EB) {
		rxdctl &= ~(IXGBE_RXDCTL_RLPMLMASK |
			    IXGBE_RXDCTL_RLPML_EN);

		/* Limit the maximum frame size so we don't overrun the skb.
		 * This can happen in SRIOV mode when the MTU of the VF is
		 * higher than the MTU of the PF.
		 */
		if (ring_uses_build_skb(ring) &&
		    !test_bit(__IXGBE_RX_3K_BUFFER, &ring->state))
			rxdctl |= IXGBE_MAX_2K_FRAME_BUILD_SKB |
				  IXGBE_RXDCTL_RLPML_EN;
#endif
	}

	ring->rx_offset = ixgbe_rx_offset(ring);

	if (ring->xsk_pool && hw->mac.type != ixgbe_mac_82599EB) {
		u32 xsk_buf_len = xsk_pool_get_rx_frame_size(ring->xsk_pool);

		rxdctl &= ~(IXGBE_RXDCTL_RLPMLMASK |
			    IXGBE_RXDCTL_RLPML_EN);
		rxdctl |= xsk_buf_len | IXGBE_RXDCTL_RLPML_EN;

		ring->rx_buf_len = xsk_buf_len;
	}

	/* initialize rx_buffer_info */
	memset(ring->rx_buffer_info, 0,
	       sizeof(struct ixgbe_rx_buffer) * ring->count);

	/* initialize Rx descriptor 0 */
	rx_desc = IXGBE_RX_DESC(ring, 0);
	rx_desc->wb.upper.length = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* enable receive descriptor ring */
	rxdctl |= IXGBE_RXDCTL_ENABLE;
	IXGBE_WRITE_REG(hw, IXGBE_RXDCTL(reg_idx), rxdctl);

	ixgbe_rx_desc_queue_enable(adapter, ring);
<<<<<<< HEAD
	ixgbe_alloc_rx_buffers(ring, ixgbe_desc_unused(ring));
=======
	if (ring->xsk_pool)
		ixgbe_alloc_rx_buffers_zc(ring, ixgbe_desc_unused(ring));
	else
		ixgbe_alloc_rx_buffers(ring, ixgbe_desc_unused(ring));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_setup_psrtype(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD
	int p;
=======
	int rss_i = adapter->ring_feature[RING_F_RSS].indices;
	u16 pool = adapter->num_rx_pools;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* PSRTYPE must be initialized in non 82598 adapters */
	u32 psrtype = IXGBE_PSRTYPE_TCPHDR |
		      IXGBE_PSRTYPE_UDPHDR |
		      IXGBE_PSRTYPE_IPV4HDR |
		      IXGBE_PSRTYPE_L2HDR |
		      IXGBE_PSRTYPE_IPV6HDR;

	if (hw->mac.type == ixgbe_mac_82598EB)
		return;

<<<<<<< HEAD
	if (adapter->flags & IXGBE_FLAG_RSS_ENABLED)
		psrtype |= (adapter->num_rx_queues_per_pool << 29);

	for (p = 0; p < adapter->num_rx_pools; p++)
		IXGBE_WRITE_REG(hw, IXGBE_PSRTYPE(adapter->num_vfs + p),
				psrtype);
=======
	if (rss_i > 3)
		psrtype |= 2u << 29;
	else if (rss_i > 1)
		psrtype |= 1u << 29;

	while (pool--)
		IXGBE_WRITE_REG(hw, IXGBE_PSRTYPE(VMDQ_P(pool)), psrtype);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_configure_virtualization(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD
	u32 gcr_ext;
	u32 vt_reg_bits;
	u32 reg_offset, vf_shift;
	u32 vmdctl;
=======
	u16 pool = adapter->num_rx_pools;
	u32 reg_offset, vf_shift, vmolr;
	u32 gcr_ext, vmdctl;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i;

	if (!(adapter->flags & IXGBE_FLAG_SRIOV_ENABLED))
		return;

	vmdctl = IXGBE_READ_REG(hw, IXGBE_VT_CTL);
<<<<<<< HEAD
	vt_reg_bits = IXGBE_VMD_CTL_VMDQ_EN | IXGBE_VT_CTL_REPLEN;
	vt_reg_bits |= (adapter->num_vfs << IXGBE_VT_CTL_POOL_SHIFT);
	IXGBE_WRITE_REG(hw, IXGBE_VT_CTL, vmdctl | vt_reg_bits);

	vf_shift = adapter->num_vfs % 32;
	reg_offset = (adapter->num_vfs >= 32) ? 1 : 0;

	/* Enable only the PF's pool for Tx/Rx */
	IXGBE_WRITE_REG(hw, IXGBE_VFRE(reg_offset), (1 << vf_shift));
	IXGBE_WRITE_REG(hw, IXGBE_VFRE(reg_offset ^ 1), 0);
	IXGBE_WRITE_REG(hw, IXGBE_VFTE(reg_offset), (1 << vf_shift));
	IXGBE_WRITE_REG(hw, IXGBE_VFTE(reg_offset ^ 1), 0);
	IXGBE_WRITE_REG(hw, IXGBE_PFDTXGSWC, IXGBE_PFDTXGSWC_VT_LBEN);

	/* Map PF MAC address in RAR Entry 0 to first pool following VFs */
	hw->mac.ops.set_vmdq(hw, 0, adapter->num_vfs);
=======
	vmdctl |= IXGBE_VMD_CTL_VMDQ_EN;
	vmdctl &= ~IXGBE_VT_CTL_POOL_MASK;
	vmdctl |= VMDQ_P(0) << IXGBE_VT_CTL_POOL_SHIFT;
	vmdctl |= IXGBE_VT_CTL_REPLEN;
	IXGBE_WRITE_REG(hw, IXGBE_VT_CTL, vmdctl);

	/* accept untagged packets until a vlan tag is
	 * specifically set for the VMDQ queue/pool
	 */
	vmolr = IXGBE_VMOLR_AUPE;
	while (pool--)
		IXGBE_WRITE_REG(hw, IXGBE_VMOLR(VMDQ_P(pool)), vmolr);

	vf_shift = VMDQ_P(0) % 32;
	reg_offset = (VMDQ_P(0) >= 32) ? 1 : 0;

	/* Enable only the PF's pool for Tx/Rx */
	IXGBE_WRITE_REG(hw, IXGBE_VFRE(reg_offset), GENMASK(31, vf_shift));
	IXGBE_WRITE_REG(hw, IXGBE_VFRE(reg_offset ^ 1), reg_offset - 1);
	IXGBE_WRITE_REG(hw, IXGBE_VFTE(reg_offset), GENMASK(31, vf_shift));
	IXGBE_WRITE_REG(hw, IXGBE_VFTE(reg_offset ^ 1), reg_offset - 1);
	if (adapter->bridge_mode == BRIDGE_MODE_VEB)
		IXGBE_WRITE_REG(hw, IXGBE_PFDTXGSWC, IXGBE_PFDTXGSWC_VT_LBEN);

	/* Map PF MAC address in RAR Entry 0 to first pool following VFs */
	hw->mac.ops.set_vmdq(hw, 0, VMDQ_P(0));

	/* clear VLAN promisc flag so VFTA will be updated if necessary */
	adapter->flags2 &= ~IXGBE_FLAG2_VLAN_PROMISC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Set up VF register offsets for selected VT Mode,
	 * i.e. 32 or 64 VFs for SR-IOV
	 */
<<<<<<< HEAD
	gcr_ext = IXGBE_READ_REG(hw, IXGBE_GCR_EXT);
	gcr_ext |= IXGBE_GCR_EXT_MSIX_EN;
	gcr_ext |= IXGBE_GCR_EXT_VT_MODE_64;
	IXGBE_WRITE_REG(hw, IXGBE_GCR_EXT, gcr_ext);

	/* enable Tx loopback for VF/PF communication */
	IXGBE_WRITE_REG(hw, IXGBE_PFDTXGSWC, IXGBE_PFDTXGSWC_VT_LBEN);
	/* Enable MAC Anti-Spoofing */
	hw->mac.ops.set_mac_anti_spoofing(hw,
					   (adapter->num_vfs != 0),
					  adapter->num_vfs);
	/* For VFs that have spoof checking turned off */
	for (i = 0; i < adapter->num_vfs; i++) {
		if (!adapter->vfinfo[i].spoofchk_enabled)
			ixgbe_ndo_set_vf_spoofchk(adapter->netdev, i, false);
=======
	switch (adapter->ring_feature[RING_F_VMDQ].mask) {
	case IXGBE_82599_VMDQ_8Q_MASK:
		gcr_ext = IXGBE_GCR_EXT_VT_MODE_16;
		break;
	case IXGBE_82599_VMDQ_4Q_MASK:
		gcr_ext = IXGBE_GCR_EXT_VT_MODE_32;
		break;
	default:
		gcr_ext = IXGBE_GCR_EXT_VT_MODE_64;
		break;
	}

	IXGBE_WRITE_REG(hw, IXGBE_GCR_EXT, gcr_ext);

	for (i = 0; i < adapter->num_vfs; i++) {
		/* configure spoof checking */
		ixgbe_ndo_set_vf_spoofchk(adapter->netdev, i,
					  adapter->vfinfo[i].spoofchk_enabled);

		/* Enable/Disable RSS query feature  */
		ixgbe_ndo_set_vf_rss_query_en(adapter->netdev, i,
					  adapter->vfinfo[i].rss_query_enabled);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void ixgbe_set_rx_buffer_len(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	struct net_device *netdev = adapter->netdev;
	int max_frame = netdev->mtu + ETH_HLEN + ETH_FCS_LEN;
	struct ixgbe_ring *rx_ring;
	int i;
	u32 mhadd, hlreg0;

#ifdef IXGBE_FCOE
	/* adjust max frame to be able to do baby jumbo for FCoE */
	if ((adapter->flags & IXGBE_FLAG_FCOE_ENABLED) &&
	    (max_frame < IXGBE_FCOE_JUMBO_FRAME_SIZE))
		max_frame = IXGBE_FCOE_JUMBO_FRAME_SIZE;

#endif /* IXGBE_FCOE */
<<<<<<< HEAD
=======

	/* adjust max frame to be at least the size of a standard frame */
	if (max_frame < (ETH_FRAME_LEN + ETH_FCS_LEN))
		max_frame = (ETH_FRAME_LEN + ETH_FCS_LEN);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mhadd = IXGBE_READ_REG(hw, IXGBE_MHADD);
	if (max_frame != (mhadd >> IXGBE_MHADD_MFS_SHIFT)) {
		mhadd &= ~IXGBE_MHADD_MFS_MASK;
		mhadd |= max_frame << IXGBE_MHADD_MFS_SHIFT;

		IXGBE_WRITE_REG(hw, IXGBE_MHADD, mhadd);
	}

<<<<<<< HEAD
	/* MHADD will allow an extra 4 bytes past for vlan tagged frames */
	max_frame += VLAN_HLEN;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hlreg0 = IXGBE_READ_REG(hw, IXGBE_HLREG0);
	/* set jumbo enable since MHADD.MFS is keeping size locked at max_frame */
	hlreg0 |= IXGBE_HLREG0_JUMBOEN;
	IXGBE_WRITE_REG(hw, IXGBE_HLREG0, hlreg0);

	/*
	 * Setup the HW Rx Head and Tail Descriptor Pointers and
	 * the Base and Length of the Rx Descriptor Ring
	 */
	for (i = 0; i < adapter->num_rx_queues; i++) {
		rx_ring = adapter->rx_ring[i];
<<<<<<< HEAD
		if (adapter->flags2 & IXGBE_FLAG2_RSC_ENABLED)
			set_ring_rsc_enabled(rx_ring);
		else
			clear_ring_rsc_enabled(rx_ring);
=======

		clear_ring_rsc_enabled(rx_ring);
		clear_bit(__IXGBE_RX_3K_BUFFER, &rx_ring->state);
		clear_bit(__IXGBE_RX_BUILD_SKB_ENABLED, &rx_ring->state);

		if (adapter->flags2 & IXGBE_FLAG2_RSC_ENABLED)
			set_ring_rsc_enabled(rx_ring);

		if (test_bit(__IXGBE_RX_FCOE, &rx_ring->state))
			set_bit(__IXGBE_RX_3K_BUFFER, &rx_ring->state);

		if (adapter->flags2 & IXGBE_FLAG2_RX_LEGACY)
			continue;

		set_bit(__IXGBE_RX_BUILD_SKB_ENABLED, &rx_ring->state);

#if (PAGE_SIZE < 8192)
		if (adapter->flags2 & IXGBE_FLAG2_RSC_ENABLED)
			set_bit(__IXGBE_RX_3K_BUFFER, &rx_ring->state);

		if (IXGBE_2K_TOO_SMALL_WITH_PADDING ||
		    (max_frame > (ETH_FRAME_LEN + ETH_FCS_LEN)))
			set_bit(__IXGBE_RX_3K_BUFFER, &rx_ring->state);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void ixgbe_setup_rdrxctl(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 rdrxctl = IXGBE_READ_REG(hw, IXGBE_RDRXCTL);

	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		/*
		 * For VMDq support of different descriptor types or
		 * buffer sizes through the use of multiple SRRCTL
		 * registers, RDRXCTL.MVMEN must be set to 1
		 *
		 * also, the manual doesn't mention it clearly but DCA hints
		 * will only use queue 0's tags unless this bit is set.  Side
		 * effects of setting this bit are only that SRRCTL must be
		 * fully programmed [0..15]
		 */
		rdrxctl |= IXGBE_RDRXCTL_MVMEN;
		break;
<<<<<<< HEAD
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
		if (adapter->num_vfs)
			rdrxctl |= IXGBE_RDRXCTL_PSP;
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
		/* Disable RSC for ACK packets */
		IXGBE_WRITE_REG(hw, IXGBE_RSCDBU,
		   (IXGBE_RSCDBU_RSCACKDIS | IXGBE_READ_REG(hw, IXGBE_RSCDBU)));
		rdrxctl &= ~IXGBE_RDRXCTL_RSCFRSTSIZE;
		/* hardware requires some bits to be set by default */
		rdrxctl |= (IXGBE_RDRXCTL_RSCACKC | IXGBE_RDRXCTL_FCOE_WRFIX);
		rdrxctl |= IXGBE_RDRXCTL_CRCSTRIP;
		break;
	default:
		/* We should do nothing since we don't know this hardware */
		return;
	}

	IXGBE_WRITE_REG(hw, IXGBE_RDRXCTL, rdrxctl);
}

/**
 * ixgbe_configure_rx - Configure 8259x Receive Unit after Reset
 * @adapter: board private structure
 *
 * Configure the Rx unit of the MAC after a reset.
 **/
static void ixgbe_configure_rx(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	int i;
<<<<<<< HEAD
	u32 rxctrl;

	/* disable receives while setting up the descriptors */
	rxctrl = IXGBE_READ_REG(hw, IXGBE_RXCTRL);
	IXGBE_WRITE_REG(hw, IXGBE_RXCTRL, rxctrl & ~IXGBE_RXCTRL_RXEN);
=======
	u32 rxctrl, rfctl;

	/* disable receives while setting up the descriptors */
	hw->mac.ops.disable_rx(hw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ixgbe_setup_psrtype(adapter);
	ixgbe_setup_rdrxctl(adapter);

<<<<<<< HEAD
	/* Program registers for the distribution of queues */
	ixgbe_setup_mrqc(adapter);

	ixgbe_set_uta(adapter);

=======
	/* RSC Setup */
	rfctl = IXGBE_READ_REG(hw, IXGBE_RFCTL);
	rfctl &= ~IXGBE_RFCTL_RSC_DIS;
	if (!(adapter->flags2 & IXGBE_FLAG2_RSC_ENABLED))
		rfctl |= IXGBE_RFCTL_RSC_DIS;

	/* disable NFS filtering */
	rfctl |= (IXGBE_RFCTL_NFSW_DIS | IXGBE_RFCTL_NFSR_DIS);
	IXGBE_WRITE_REG(hw, IXGBE_RFCTL, rfctl);

	/* Program registers for the distribution of queues */
	ixgbe_setup_mrqc(adapter);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* set_rx_buffer_len must be called before ring initialization */
	ixgbe_set_rx_buffer_len(adapter);

	/*
	 * Setup the HW Rx Head and Tail Descriptor Pointers and
	 * the Base and Length of the Rx Descriptor Ring
	 */
	for (i = 0; i < adapter->num_rx_queues; i++)
		ixgbe_configure_rx_ring(adapter, adapter->rx_ring[i]);

<<<<<<< HEAD
=======
	rxctrl = IXGBE_READ_REG(hw, IXGBE_RXCTRL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* disable drop enable for 82598 parts */
	if (hw->mac.type == ixgbe_mac_82598EB)
		rxctrl |= IXGBE_RXCTRL_DMBYPS;

	/* enable all receives */
	rxctrl |= IXGBE_RXCTRL_RXEN;
	hw->mac.ops.enable_rx_dma(hw, rxctrl);
}

<<<<<<< HEAD
static int ixgbe_vlan_rx_add_vid(struct net_device *netdev, u16 vid)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	struct ixgbe_hw *hw = &adapter->hw;
	int pool_ndx = adapter->num_vfs;

	/* add VID to filter table */
	hw->mac.ops.set_vfta(&adapter->hw, vid, pool_ndx, true);
=======
static int ixgbe_vlan_rx_add_vid(struct net_device *netdev,
				 __be16 proto, u16 vid)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	struct ixgbe_hw *hw = &adapter->hw;

	/* add VID to filter table */
	if (!vid || !(adapter->flags2 & IXGBE_FLAG2_VLAN_PROMISC))
		hw->mac.ops.set_vfta(&adapter->hw, vid, VMDQ_P(0), true, !!vid);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	set_bit(vid, adapter->active_vlans);

	return 0;
}

<<<<<<< HEAD
static int ixgbe_vlan_rx_kill_vid(struct net_device *netdev, u16 vid)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	struct ixgbe_hw *hw = &adapter->hw;
	int pool_ndx = adapter->num_vfs;

	/* remove VID from filter table */
	hw->mac.ops.set_vfta(&adapter->hw, vid, pool_ndx, false);
=======
static int ixgbe_find_vlvf_entry(struct ixgbe_hw *hw, u32 vlan)
{
	u32 vlvf;
	int idx;

	/* short cut the special case */
	if (vlan == 0)
		return 0;

	/* Search for the vlan id in the VLVF entries */
	for (idx = IXGBE_VLVF_ENTRIES; --idx;) {
		vlvf = IXGBE_READ_REG(hw, IXGBE_VLVF(idx));
		if ((vlvf & VLAN_VID_MASK) == vlan)
			break;
	}

	return idx;
}

void ixgbe_update_pf_promisc_vlvf(struct ixgbe_adapter *adapter, u32 vid)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 bits, word;
	int idx;

	idx = ixgbe_find_vlvf_entry(hw, vid);
	if (!idx)
		return;

	/* See if any other pools are set for this VLAN filter
	 * entry other than the PF.
	 */
	word = idx * 2 + (VMDQ_P(0) / 32);
	bits = ~BIT(VMDQ_P(0) % 32);
	bits &= IXGBE_READ_REG(hw, IXGBE_VLVFB(word));

	/* Disable the filter so this falls into the default pool. */
	if (!bits && !IXGBE_READ_REG(hw, IXGBE_VLVFB(word ^ 1))) {
		if (!(adapter->flags2 & IXGBE_FLAG2_VLAN_PROMISC))
			IXGBE_WRITE_REG(hw, IXGBE_VLVFB(word), 0);
		IXGBE_WRITE_REG(hw, IXGBE_VLVF(idx), 0);
	}
}

static int ixgbe_vlan_rx_kill_vid(struct net_device *netdev,
				  __be16 proto, u16 vid)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	struct ixgbe_hw *hw = &adapter->hw;

	/* remove VID from filter table */
	if (vid && !(adapter->flags2 & IXGBE_FLAG2_VLAN_PROMISC))
		hw->mac.ops.set_vfta(hw, vid, VMDQ_P(0), false, true);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clear_bit(vid, adapter->active_vlans);

	return 0;
}

/**
<<<<<<< HEAD
 * ixgbe_vlan_filter_disable - helper to disable hw vlan filtering
 * @adapter: driver data
 */
static void ixgbe_vlan_filter_disable(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 vlnctrl;

	vlnctrl = IXGBE_READ_REG(hw, IXGBE_VLNCTRL);
	vlnctrl &= ~(IXGBE_VLNCTRL_VFE | IXGBE_VLNCTRL_CFIEN);
	IXGBE_WRITE_REG(hw, IXGBE_VLNCTRL, vlnctrl);
}

/**
 * ixgbe_vlan_filter_enable - helper to enable hw vlan filtering
 * @adapter: driver data
 */
static void ixgbe_vlan_filter_enable(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 vlnctrl;

	vlnctrl = IXGBE_READ_REG(hw, IXGBE_VLNCTRL);
	vlnctrl |= IXGBE_VLNCTRL_VFE;
	vlnctrl &= ~IXGBE_VLNCTRL_CFIEN;
	IXGBE_WRITE_REG(hw, IXGBE_VLNCTRL, vlnctrl);
}

/**
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ixgbe_vlan_strip_disable - helper to disable hw vlan stripping
 * @adapter: driver data
 */
static void ixgbe_vlan_strip_disable(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 vlnctrl;
	int i, j;

	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		vlnctrl = IXGBE_READ_REG(hw, IXGBE_VLNCTRL);
		vlnctrl &= ~IXGBE_VLNCTRL_VME;
		IXGBE_WRITE_REG(hw, IXGBE_VLNCTRL, vlnctrl);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
<<<<<<< HEAD
		for (i = 0; i < adapter->num_rx_queues; i++) {
			j = adapter->rx_ring[i]->reg_idx;
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
		for (i = 0; i < adapter->num_rx_queues; i++) {
			struct ixgbe_ring *ring = adapter->rx_ring[i];

			if (!netif_is_ixgbe(ring->netdev))
				continue;

			j = ring->reg_idx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			vlnctrl = IXGBE_READ_REG(hw, IXGBE_RXDCTL(j));
			vlnctrl &= ~IXGBE_RXDCTL_VME;
			IXGBE_WRITE_REG(hw, IXGBE_RXDCTL(j), vlnctrl);
		}
		break;
	default:
		break;
	}
}

/**
 * ixgbe_vlan_strip_enable - helper to enable hw vlan stripping
 * @adapter: driver data
 */
static void ixgbe_vlan_strip_enable(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 vlnctrl;
	int i, j;

	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		vlnctrl = IXGBE_READ_REG(hw, IXGBE_VLNCTRL);
		vlnctrl |= IXGBE_VLNCTRL_VME;
		IXGBE_WRITE_REG(hw, IXGBE_VLNCTRL, vlnctrl);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
<<<<<<< HEAD
		for (i = 0; i < adapter->num_rx_queues; i++) {
			j = adapter->rx_ring[i]->reg_idx;
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
		for (i = 0; i < adapter->num_rx_queues; i++) {
			struct ixgbe_ring *ring = adapter->rx_ring[i];

			if (!netif_is_ixgbe(ring->netdev))
				continue;

			j = ring->reg_idx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			vlnctrl = IXGBE_READ_REG(hw, IXGBE_RXDCTL(j));
			vlnctrl |= IXGBE_RXDCTL_VME;
			IXGBE_WRITE_REG(hw, IXGBE_RXDCTL(j), vlnctrl);
		}
		break;
	default:
		break;
	}
}

<<<<<<< HEAD
static void ixgbe_restore_vlan(struct ixgbe_adapter *adapter)
{
	u16 vid;

	ixgbe_vlan_rx_add_vid(adapter->netdev, 0);

	for_each_set_bit(vid, adapter->active_vlans, VLAN_N_VID)
		ixgbe_vlan_rx_add_vid(adapter->netdev, vid);
}

/**
 * ixgbe_write_uc_addr_list - write unicast addresses to RAR table
 * @netdev: network interface device structure
 *
 * Writes unicast address list to the RAR table.
 * Returns: -ENOMEM on failure/insufficient address space
 *                0 on no addresses written
 *                X on writing X addresses to the RAR table
 **/
static int ixgbe_write_uc_addr_list(struct net_device *netdev)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	struct ixgbe_hw *hw = &adapter->hw;
	unsigned int vfn = adapter->num_vfs;
	unsigned int rar_entries = IXGBE_MAX_PF_MACVLANS;
	int count = 0;

	/* return ENOMEM indicating insufficient memory for addresses */
	if (netdev_uc_count(netdev) > rar_entries)
		return -ENOMEM;

	if (!netdev_uc_empty(netdev) && rar_entries) {
		struct netdev_hw_addr *ha;
		/* return error if we do not support writing to RAR table */
		if (!hw->mac.ops.set_rar)
			return -ENOMEM;

		netdev_for_each_uc_addr(ha, netdev) {
			if (!rar_entries)
				break;
			hw->mac.ops.set_rar(hw, rar_entries--, ha->addr,
					    vfn, IXGBE_RAH_AV);
			count++;
		}
	}
	/* write the addresses in reverse order to avoid write combining */
	for (; rar_entries > 0 ; rar_entries--)
		hw->mac.ops.clear_rar(hw, rar_entries);
=======
static void ixgbe_vlan_promisc_enable(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 vlnctrl, i;

	vlnctrl = IXGBE_READ_REG(hw, IXGBE_VLNCTRL);

	if (adapter->flags & IXGBE_FLAG_VMDQ_ENABLED) {
	/* For VMDq and SR-IOV we must leave VLAN filtering enabled */
		vlnctrl |= IXGBE_VLNCTRL_VFE;
		IXGBE_WRITE_REG(hw, IXGBE_VLNCTRL, vlnctrl);
	} else {
		vlnctrl &= ~IXGBE_VLNCTRL_VFE;
		IXGBE_WRITE_REG(hw, IXGBE_VLNCTRL, vlnctrl);
		return;
	}

	/* Nothing to do for 82598 */
	if (hw->mac.type == ixgbe_mac_82598EB)
		return;

	/* We are already in VLAN promisc, nothing to do */
	if (adapter->flags2 & IXGBE_FLAG2_VLAN_PROMISC)
		return;

	/* Set flag so we don't redo unnecessary work */
	adapter->flags2 |= IXGBE_FLAG2_VLAN_PROMISC;

	/* Add PF to all active pools */
	for (i = IXGBE_VLVF_ENTRIES; --i;) {
		u32 reg_offset = IXGBE_VLVFB(i * 2 + VMDQ_P(0) / 32);
		u32 vlvfb = IXGBE_READ_REG(hw, reg_offset);

		vlvfb |= BIT(VMDQ_P(0) % 32);
		IXGBE_WRITE_REG(hw, reg_offset, vlvfb);
	}

	/* Set all bits in the VLAN filter table array */
	for (i = hw->mac.vft_size; i--;)
		IXGBE_WRITE_REG(hw, IXGBE_VFTA(i), ~0U);
}

#define VFTA_BLOCK_SIZE 8
static void ixgbe_scrub_vfta(struct ixgbe_adapter *adapter, u32 vfta_offset)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 vfta[VFTA_BLOCK_SIZE] = { 0 };
	u32 vid_start = vfta_offset * 32;
	u32 vid_end = vid_start + (VFTA_BLOCK_SIZE * 32);
	u32 i, vid, word, bits;

	for (i = IXGBE_VLVF_ENTRIES; --i;) {
		u32 vlvf = IXGBE_READ_REG(hw, IXGBE_VLVF(i));

		/* pull VLAN ID from VLVF */
		vid = vlvf & VLAN_VID_MASK;

		/* only concern outselves with a certain range */
		if (vid < vid_start || vid >= vid_end)
			continue;

		if (vlvf) {
			/* record VLAN ID in VFTA */
			vfta[(vid - vid_start) / 32] |= BIT(vid % 32);

			/* if PF is part of this then continue */
			if (test_bit(vid, adapter->active_vlans))
				continue;
		}

		/* remove PF from the pool */
		word = i * 2 + VMDQ_P(0) / 32;
		bits = ~BIT(VMDQ_P(0) % 32);
		bits &= IXGBE_READ_REG(hw, IXGBE_VLVFB(word));
		IXGBE_WRITE_REG(hw, IXGBE_VLVFB(word), bits);
	}

	/* extract values from active_vlans and write back to VFTA */
	for (i = VFTA_BLOCK_SIZE; i--;) {
		vid = (vfta_offset + i) * 32;
		word = vid / BITS_PER_LONG;
		bits = vid % BITS_PER_LONG;

		vfta[i] |= adapter->active_vlans[word] >> bits;

		IXGBE_WRITE_REG(hw, IXGBE_VFTA(vfta_offset + i), vfta[i]);
	}
}

static void ixgbe_vlan_promisc_disable(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 vlnctrl, i;

	/* Set VLAN filtering to enabled */
	vlnctrl = IXGBE_READ_REG(hw, IXGBE_VLNCTRL);
	vlnctrl |= IXGBE_VLNCTRL_VFE;
	IXGBE_WRITE_REG(hw, IXGBE_VLNCTRL, vlnctrl);

	if (!(adapter->flags & IXGBE_FLAG_VMDQ_ENABLED) ||
	    hw->mac.type == ixgbe_mac_82598EB)
		return;

	/* We are not in VLAN promisc, nothing to do */
	if (!(adapter->flags2 & IXGBE_FLAG2_VLAN_PROMISC))
		return;

	/* Set flag so we don't redo unnecessary work */
	adapter->flags2 &= ~IXGBE_FLAG2_VLAN_PROMISC;

	for (i = 0; i < hw->mac.vft_size; i += VFTA_BLOCK_SIZE)
		ixgbe_scrub_vfta(adapter, i);
}

static void ixgbe_restore_vlan(struct ixgbe_adapter *adapter)
{
	u16 vid = 1;

	ixgbe_vlan_rx_add_vid(adapter->netdev, htons(ETH_P_8021Q), 0);

	for_each_set_bit_from(vid, adapter->active_vlans, VLAN_N_VID)
		ixgbe_vlan_rx_add_vid(adapter->netdev, htons(ETH_P_8021Q), vid);
}

/**
 * ixgbe_write_mc_addr_list - write multicast addresses to MTA
 * @netdev: network interface device structure
 *
 * Writes multicast address list to the MTA hash table.
 * Returns: -ENOMEM on failure
 *                0 on no addresses written
 *                X on writing X addresses to MTA
 **/
static int ixgbe_write_mc_addr_list(struct net_device *netdev)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	struct ixgbe_hw *hw = &adapter->hw;

	if (!netif_running(netdev))
		return 0;

	if (hw->mac.ops.update_mc_addr_list)
		hw->mac.ops.update_mc_addr_list(hw, netdev);
	else
		return -ENOMEM;

#ifdef CONFIG_PCI_IOV
	ixgbe_restore_vf_multicasts(adapter);
#endif

	return netdev_mc_count(netdev);
}

#ifdef CONFIG_PCI_IOV
void ixgbe_full_sync_mac_table(struct ixgbe_adapter *adapter)
{
	struct ixgbe_mac_addr *mac_table = &adapter->mac_table[0];
	struct ixgbe_hw *hw = &adapter->hw;
	int i;

	for (i = 0; i < hw->mac.num_rar_entries; i++, mac_table++) {
		mac_table->state &= ~IXGBE_MAC_STATE_MODIFIED;

		if (mac_table->state & IXGBE_MAC_STATE_IN_USE)
			hw->mac.ops.set_rar(hw, i,
					    mac_table->addr,
					    mac_table->pool,
					    IXGBE_RAH_AV);
		else
			hw->mac.ops.clear_rar(hw, i);
	}
}

#endif
static void ixgbe_sync_mac_table(struct ixgbe_adapter *adapter)
{
	struct ixgbe_mac_addr *mac_table = &adapter->mac_table[0];
	struct ixgbe_hw *hw = &adapter->hw;
	int i;

	for (i = 0; i < hw->mac.num_rar_entries; i++, mac_table++) {
		if (!(mac_table->state & IXGBE_MAC_STATE_MODIFIED))
			continue;

		mac_table->state &= ~IXGBE_MAC_STATE_MODIFIED;

		if (mac_table->state & IXGBE_MAC_STATE_IN_USE)
			hw->mac.ops.set_rar(hw, i,
					    mac_table->addr,
					    mac_table->pool,
					    IXGBE_RAH_AV);
		else
			hw->mac.ops.clear_rar(hw, i);
	}
}

static void ixgbe_flush_sw_mac_table(struct ixgbe_adapter *adapter)
{
	struct ixgbe_mac_addr *mac_table = &adapter->mac_table[0];
	struct ixgbe_hw *hw = &adapter->hw;
	int i;

	for (i = 0; i < hw->mac.num_rar_entries; i++, mac_table++) {
		mac_table->state |= IXGBE_MAC_STATE_MODIFIED;
		mac_table->state &= ~IXGBE_MAC_STATE_IN_USE;
	}

	ixgbe_sync_mac_table(adapter);
}

static int ixgbe_available_rars(struct ixgbe_adapter *adapter, u16 pool)
{
	struct ixgbe_mac_addr *mac_table = &adapter->mac_table[0];
	struct ixgbe_hw *hw = &adapter->hw;
	int i, count = 0;

	for (i = 0; i < hw->mac.num_rar_entries; i++, mac_table++) {
		/* do not count default RAR as available */
		if (mac_table->state & IXGBE_MAC_STATE_DEFAULT)
			continue;

		/* only count unused and addresses that belong to us */
		if (mac_table->state & IXGBE_MAC_STATE_IN_USE) {
			if (mac_table->pool != pool)
				continue;
		}

		count++;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return count;
}

<<<<<<< HEAD
=======
/* this function destroys the first RAR entry */
static void ixgbe_mac_set_default_filter(struct ixgbe_adapter *adapter)
{
	struct ixgbe_mac_addr *mac_table = &adapter->mac_table[0];
	struct ixgbe_hw *hw = &adapter->hw;

	memcpy(&mac_table->addr, hw->mac.addr, ETH_ALEN);
	mac_table->pool = VMDQ_P(0);

	mac_table->state = IXGBE_MAC_STATE_DEFAULT | IXGBE_MAC_STATE_IN_USE;

	hw->mac.ops.set_rar(hw, 0, mac_table->addr, mac_table->pool,
			    IXGBE_RAH_AV);
}

int ixgbe_add_mac_filter(struct ixgbe_adapter *adapter,
			 const u8 *addr, u16 pool)
{
	struct ixgbe_mac_addr *mac_table = &adapter->mac_table[0];
	struct ixgbe_hw *hw = &adapter->hw;
	int i;

	if (is_zero_ether_addr(addr))
		return -EINVAL;

	for (i = 0; i < hw->mac.num_rar_entries; i++, mac_table++) {
		if (mac_table->state & IXGBE_MAC_STATE_IN_USE)
			continue;

		ether_addr_copy(mac_table->addr, addr);
		mac_table->pool = pool;

		mac_table->state |= IXGBE_MAC_STATE_MODIFIED |
				    IXGBE_MAC_STATE_IN_USE;

		ixgbe_sync_mac_table(adapter);

		return i;
	}

	return -ENOMEM;
}

int ixgbe_del_mac_filter(struct ixgbe_adapter *adapter,
			 const u8 *addr, u16 pool)
{
	struct ixgbe_mac_addr *mac_table = &adapter->mac_table[0];
	struct ixgbe_hw *hw = &adapter->hw;
	int i;

	if (is_zero_ether_addr(addr))
		return -EINVAL;

	/* search table for addr, if found clear IN_USE flag and sync */
	for (i = 0; i < hw->mac.num_rar_entries; i++, mac_table++) {
		/* we can only delete an entry if it is in use */
		if (!(mac_table->state & IXGBE_MAC_STATE_IN_USE))
			continue;
		/* we only care about entries that belong to the given pool */
		if (mac_table->pool != pool)
			continue;
		/* we only care about a specific MAC address */
		if (!ether_addr_equal(addr, mac_table->addr))
			continue;

		mac_table->state |= IXGBE_MAC_STATE_MODIFIED;
		mac_table->state &= ~IXGBE_MAC_STATE_IN_USE;

		ixgbe_sync_mac_table(adapter);

		return 0;
	}

	return -ENOMEM;
}

static int ixgbe_uc_sync(struct net_device *netdev, const unsigned char *addr)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	int ret;

	ret = ixgbe_add_mac_filter(adapter, addr, VMDQ_P(0));

	return min_t(int, ret, 0);
}

static int ixgbe_uc_unsync(struct net_device *netdev, const unsigned char *addr)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);

	ixgbe_del_mac_filter(adapter, addr, VMDQ_P(0));

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * ixgbe_set_rx_mode - Unicast, Multicast and Promiscuous mode set
 * @netdev: network interface device structure
 *
 * The set_rx_method entry point is called whenever the unicast/multicast
 * address list or the network interface flags are updated.  This routine is
 * responsible for configuring the hardware for proper unicast, multicast and
 * promiscuous mode.
 **/
void ixgbe_set_rx_mode(struct net_device *netdev)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	struct ixgbe_hw *hw = &adapter->hw;
	u32 fctrl, vmolr = IXGBE_VMOLR_BAM | IXGBE_VMOLR_AUPE;
<<<<<<< HEAD
	int count;

	/* Check for Promiscuous and All Multicast modes */

=======
	netdev_features_t features = netdev->features;
	int count;

	/* Check for Promiscuous and All Multicast modes */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	fctrl = IXGBE_READ_REG(hw, IXGBE_FCTRL);

	/* set all bits that we expect to always be set */
	fctrl &= ~IXGBE_FCTRL_SBP; /* disable store-bad-packets */
	fctrl |= IXGBE_FCTRL_BAM;
	fctrl |= IXGBE_FCTRL_DPF; /* discard pause frames when FC enabled */
	fctrl |= IXGBE_FCTRL_PMCF;

	/* clear the bits we are changing the status of */
	fctrl &= ~(IXGBE_FCTRL_UPE | IXGBE_FCTRL_MPE);
<<<<<<< HEAD

	if (netdev->flags & IFF_PROMISC) {
		hw->addr_ctrl.user_set_promisc = true;
		fctrl |= (IXGBE_FCTRL_UPE | IXGBE_FCTRL_MPE);
		vmolr |= (IXGBE_VMOLR_ROPE | IXGBE_VMOLR_MPE);
		/* don't hardware filter vlans in promisc mode */
		ixgbe_vlan_filter_disable(adapter);
=======
	if (netdev->flags & IFF_PROMISC) {
		hw->addr_ctrl.user_set_promisc = true;
		fctrl |= (IXGBE_FCTRL_UPE | IXGBE_FCTRL_MPE);
		vmolr |= IXGBE_VMOLR_MPE;
		features &= ~NETIF_F_HW_VLAN_CTAG_FILTER;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		if (netdev->flags & IFF_ALLMULTI) {
			fctrl |= IXGBE_FCTRL_MPE;
			vmolr |= IXGBE_VMOLR_MPE;
<<<<<<< HEAD
		} else {
			/*
			 * Write addresses to the MTA, if the attempt fails
			 * then we should just turn on promiscuous mode so
			 * that we can at least receive multicast traffic
			 */
			hw->mac.ops.update_mc_addr_list(hw, netdev);
			vmolr |= IXGBE_VMOLR_ROMPE;
		}
		ixgbe_vlan_filter_enable(adapter);
		hw->addr_ctrl.user_set_promisc = false;
		/*
		 * Write addresses to available RAR registers, if there is not
		 * sufficient space to store all the addresses then enable
		 * unicast promiscuous mode
		 */
		count = ixgbe_write_uc_addr_list(netdev);
		if (count < 0) {
			fctrl |= IXGBE_FCTRL_UPE;
			vmolr |= IXGBE_VMOLR_ROPE;
		}
	}

	if (adapter->num_vfs) {
		ixgbe_restore_vf_multicasts(adapter);
		vmolr |= IXGBE_READ_REG(hw, IXGBE_VMOLR(adapter->num_vfs)) &
			 ~(IXGBE_VMOLR_MPE | IXGBE_VMOLR_ROMPE |
			   IXGBE_VMOLR_ROPE);
		IXGBE_WRITE_REG(hw, IXGBE_VMOLR(adapter->num_vfs), vmolr);
	}

	/* This is useful for sniffing bad packets. */
	if (adapter->netdev->features & NETIF_F_RXALL) {
=======
		}
		hw->addr_ctrl.user_set_promisc = false;
	}

	/*
	 * Write addresses to available RAR registers, if there is not
	 * sufficient space to store all the addresses then enable
	 * unicast promiscuous mode
	 */
	if (__dev_uc_sync(netdev, ixgbe_uc_sync, ixgbe_uc_unsync)) {
		fctrl |= IXGBE_FCTRL_UPE;
		vmolr |= IXGBE_VMOLR_ROPE;
	}

	/* Write addresses to the MTA, if the attempt fails
	 * then we should just turn on promiscuous mode so
	 * that we can at least receive multicast traffic
	 */
	count = ixgbe_write_mc_addr_list(netdev);
	if (count < 0) {
		fctrl |= IXGBE_FCTRL_MPE;
		vmolr |= IXGBE_VMOLR_MPE;
	} else if (count) {
		vmolr |= IXGBE_VMOLR_ROMPE;
	}

	if (hw->mac.type != ixgbe_mac_82598EB) {
		vmolr |= IXGBE_READ_REG(hw, IXGBE_VMOLR(VMDQ_P(0))) &
			 ~(IXGBE_VMOLR_MPE | IXGBE_VMOLR_ROMPE |
			   IXGBE_VMOLR_ROPE);
		IXGBE_WRITE_REG(hw, IXGBE_VMOLR(VMDQ_P(0)), vmolr);
	}

	/* This is useful for sniffing bad packets. */
	if (features & NETIF_F_RXALL) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* UPE and MPE will be handled by normal PROMISC logic
		 * in e1000e_set_rx_mode */
		fctrl |= (IXGBE_FCTRL_SBP | /* Receive bad packets */
			  IXGBE_FCTRL_BAM | /* RX All Bcast Pkts */
			  IXGBE_FCTRL_PMCF); /* RX All MAC Ctrl Pkts */

		fctrl &= ~(IXGBE_FCTRL_DPF);
		/* NOTE:  VLAN filtering is disabled by setting PROMISC */
	}

	IXGBE_WRITE_REG(hw, IXGBE_FCTRL, fctrl);

<<<<<<< HEAD
	if (netdev->features & NETIF_F_HW_VLAN_RX)
		ixgbe_vlan_strip_enable(adapter);
	else
		ixgbe_vlan_strip_disable(adapter);
=======
	if (features & NETIF_F_HW_VLAN_CTAG_RX)
		ixgbe_vlan_strip_enable(adapter);
	else
		ixgbe_vlan_strip_disable(adapter);

	if (features & NETIF_F_HW_VLAN_CTAG_FILTER)
		ixgbe_vlan_promisc_disable(adapter);
	else
		ixgbe_vlan_promisc_enable(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_napi_enable_all(struct ixgbe_adapter *adapter)
{
	int q_idx;
<<<<<<< HEAD
	struct ixgbe_q_vector *q_vector;
	int q_vectors = adapter->num_msix_vectors - NON_Q_VECTORS;

	/* legacy and MSI only use one vector */
	if (!(adapter->flags & IXGBE_FLAG_MSIX_ENABLED))
		q_vectors = 1;

	for (q_idx = 0; q_idx < q_vectors; q_idx++) {
		q_vector = adapter->q_vector[q_idx];
		napi_enable(&q_vector->napi);
	}
=======

	for (q_idx = 0; q_idx < adapter->num_q_vectors; q_idx++)
		napi_enable(&adapter->q_vector[q_idx]->napi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_napi_disable_all(struct ixgbe_adapter *adapter)
{
	int q_idx;
<<<<<<< HEAD
	struct ixgbe_q_vector *q_vector;
	int q_vectors = adapter->num_msix_vectors - NON_Q_VECTORS;

	/* legacy and MSI only use one vector */
	if (!(adapter->flags & IXGBE_FLAG_MSIX_ENABLED))
		q_vectors = 1;

	for (q_idx = 0; q_idx < q_vectors; q_idx++) {
		q_vector = adapter->q_vector[q_idx];
		napi_disable(&q_vector->napi);
	}
}

#ifdef CONFIG_IXGBE_DCB
/*
=======

	for (q_idx = 0; q_idx < adapter->num_q_vectors; q_idx++)
		napi_disable(&adapter->q_vector[q_idx]->napi);
}

static int ixgbe_udp_tunnel_sync(struct net_device *dev, unsigned int table)
{
	struct ixgbe_adapter *adapter = netdev_priv(dev);
	struct ixgbe_hw *hw = &adapter->hw;
	struct udp_tunnel_info ti;

	udp_tunnel_nic_get_port(dev, table, 0, &ti);
	if (ti.type == UDP_TUNNEL_TYPE_VXLAN)
		adapter->vxlan_port = ti.port;
	else
		adapter->geneve_port = ti.port;

	IXGBE_WRITE_REG(hw, IXGBE_VXLANCTRL,
			ntohs(adapter->vxlan_port) |
			ntohs(adapter->geneve_port) <<
				IXGBE_VXLANCTRL_GENEVE_UDPPORT_SHIFT);
	return 0;
}

static const struct udp_tunnel_nic_info ixgbe_udp_tunnels_x550 = {
	.sync_table	= ixgbe_udp_tunnel_sync,
	.flags		= UDP_TUNNEL_NIC_INFO_IPV4_ONLY,
	.tables		= {
		{ .n_entries = 1, .tunnel_types = UDP_TUNNEL_TYPE_VXLAN,  },
	},
};

static const struct udp_tunnel_nic_info ixgbe_udp_tunnels_x550em_a = {
	.sync_table	= ixgbe_udp_tunnel_sync,
	.flags		= UDP_TUNNEL_NIC_INFO_IPV4_ONLY,
	.tables		= {
		{ .n_entries = 1, .tunnel_types = UDP_TUNNEL_TYPE_VXLAN,  },
		{ .n_entries = 1, .tunnel_types = UDP_TUNNEL_TYPE_GENEVE, },
	},
};

#ifdef CONFIG_IXGBE_DCB
/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ixgbe_configure_dcb - Configure DCB hardware
 * @adapter: ixgbe adapter struct
 *
 * This is called by the driver on open to configure the DCB hardware.
 * This is also called by the gennetlink interface when reconfiguring
 * the DCB state.
 */
static void ixgbe_configure_dcb(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	int max_frame = adapter->netdev->mtu + ETH_HLEN + ETH_FCS_LEN;

	if (!(adapter->flags & IXGBE_FLAG_DCB_ENABLED)) {
		if (hw->mac.type == ixgbe_mac_82598EB)
<<<<<<< HEAD
			netif_set_gso_max_size(adapter->netdev, 65536);
=======
			netif_set_tso_max_size(adapter->netdev, 65536);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	if (hw->mac.type == ixgbe_mac_82598EB)
<<<<<<< HEAD
		netif_set_gso_max_size(adapter->netdev, 32768);


	/* Enable VLAN tag insert/strip */
	adapter->netdev->features |= NETIF_F_HW_VLAN_RX;

	hw->mac.ops.set_vfta(&adapter->hw, 0, 0, true);
=======
		netif_set_tso_max_size(adapter->netdev, 32768);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef IXGBE_FCOE
	if (adapter->netdev->features & NETIF_F_FCOE_MTU)
		max_frame = max(max_frame, IXGBE_FCOE_JUMBO_FRAME_SIZE);
#endif

	/* reconfigure the hardware */
	if (adapter->dcbx_cap & DCB_CAP_DCBX_VER_CEE) {
		ixgbe_dcb_calculate_tc_credits(hw, &adapter->dcb_cfg, max_frame,
						DCB_TX_CONFIG);
		ixgbe_dcb_calculate_tc_credits(hw, &adapter->dcb_cfg, max_frame,
						DCB_RX_CONFIG);
		ixgbe_dcb_hw_config(hw, &adapter->dcb_cfg);
	} else if (adapter->ixgbe_ieee_ets && adapter->ixgbe_ieee_pfc) {
		ixgbe_dcb_hw_ets(&adapter->hw,
				 adapter->ixgbe_ieee_ets,
				 max_frame);
		ixgbe_dcb_hw_pfc_config(&adapter->hw,
					adapter->ixgbe_ieee_pfc->pfc_en,
					adapter->ixgbe_ieee_ets->prio_tc);
	}

	/* Enable RSS Hash per TC */
	if (hw->mac.type != ixgbe_mac_82598EB) {
<<<<<<< HEAD
		int i;
		u32 reg = 0;

		for (i = 0; i < MAX_TRAFFIC_CLASS; i++) {
			u8 msb = 0;
			u8 cnt = adapter->netdev->tc_to_txq[i].count;

			while (cnt >>= 1)
				msb++;

			reg |= msb << IXGBE_RQTC_SHIFT_TC(i);
		}
		IXGBE_WRITE_REG(hw, IXGBE_RQTC, reg);
=======
		u32 msb = 0;
		u16 rss_i = adapter->ring_feature[RING_F_RSS].indices - 1;

		while (rss_i) {
			msb++;
			rss_i >>= 1;
		}

		/* write msb to all 8 TCs in one write */
		IXGBE_WRITE_REG(hw, IXGBE_RQTC, msb * 0x11111111);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
#endif

/* Additional bittime to account for IXGBE framing */
#define IXGBE_ETH_FRAMING 20

<<<<<<< HEAD
/*
 * ixgbe_hpbthresh - calculate high water mark for flow control
 *
 * @adapter: board private structure to calculate for
 * @pb - packet buffer to calculate
=======
/**
 * ixgbe_hpbthresh - calculate high water mark for flow control
 *
 * @adapter: board private structure to calculate for
 * @pb: packet buffer to calculate
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int ixgbe_hpbthresh(struct ixgbe_adapter *adapter, int pb)
{
	struct ixgbe_hw *hw = &adapter->hw;
	struct net_device *dev = adapter->netdev;
	int link, tc, kb, marker;
	u32 dv_id, rx_pba;

	/* Calculate max LAN frame size */
	tc = link = dev->mtu + ETH_HLEN + ETH_FCS_LEN + IXGBE_ETH_FRAMING;

#ifdef IXGBE_FCOE
	/* FCoE traffic class uses FCOE jumbo frames */
<<<<<<< HEAD
	if (dev->features & NETIF_F_FCOE_MTU) {
		int fcoe_pb = 0;

#ifdef CONFIG_IXGBE_DCB
		fcoe_pb = netdev_get_prio_tc_map(dev, adapter->fcoe.up);

#endif
		if (fcoe_pb == pb && tc < IXGBE_FCOE_JUMBO_FRAME_SIZE)
			tc = IXGBE_FCOE_JUMBO_FRAME_SIZE;
	}
=======
	if ((dev->features & NETIF_F_FCOE_MTU) &&
	    (tc < IXGBE_FCOE_JUMBO_FRAME_SIZE) &&
	    (pb == ixgbe_fcoe_get_tc(adapter)))
		tc = IXGBE_FCOE_JUMBO_FRAME_SIZE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

	/* Calculate delay value for device */
	switch (hw->mac.type) {
	case ixgbe_mac_X540:
<<<<<<< HEAD
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dv_id = IXGBE_DV_X540(link, tc);
		break;
	default:
		dv_id = IXGBE_DV(link, tc);
		break;
	}

	/* Loopback switch introduces additional latency */
	if (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED)
		dv_id += IXGBE_B2BT(tc);

	/* Delay value is calculated in bit times convert to KB */
	kb = IXGBE_BT2KB(dv_id);
	rx_pba = IXGBE_READ_REG(hw, IXGBE_RXPBSIZE(pb)) >> 10;

	marker = rx_pba - kb;

	/* It is possible that the packet buffer is not large enough
	 * to provide required headroom. In this case throw an error
	 * to user and a do the best we can.
	 */
	if (marker < 0) {
		e_warn(drv, "Packet Buffer(%i) can not provide enough"
			    "headroom to support flow control."
			    "Decrease MTU or number of traffic classes\n", pb);
		marker = tc + 1;
	}

	return marker;
}

<<<<<<< HEAD
/*
 * ixgbe_lpbthresh - calculate low water mark for for flow control
 *
 * @adapter: board private structure to calculate for
 * @pb - packet buffer to calculate
 */
static int ixgbe_lpbthresh(struct ixgbe_adapter *adapter)
=======
/**
 * ixgbe_lpbthresh - calculate low water mark for flow control
 *
 * @adapter: board private structure to calculate for
 * @pb: packet buffer to calculate
 */
static int ixgbe_lpbthresh(struct ixgbe_adapter *adapter, int pb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ixgbe_hw *hw = &adapter->hw;
	struct net_device *dev = adapter->netdev;
	int tc;
	u32 dv_id;

	/* Calculate max LAN frame size */
	tc = dev->mtu + ETH_HLEN + ETH_FCS_LEN;

<<<<<<< HEAD
	/* Calculate delay value for device */
	switch (hw->mac.type) {
	case ixgbe_mac_X540:
=======
#ifdef IXGBE_FCOE
	/* FCoE traffic class uses FCOE jumbo frames */
	if ((dev->features & NETIF_F_FCOE_MTU) &&
	    (tc < IXGBE_FCOE_JUMBO_FRAME_SIZE) &&
	    (pb == netdev_get_prio_tc_map(dev, adapter->fcoe.up)))
		tc = IXGBE_FCOE_JUMBO_FRAME_SIZE;
#endif

	/* Calculate delay value for device */
	switch (hw->mac.type) {
	case ixgbe_mac_X540:
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dv_id = IXGBE_LOW_DV_X540(tc);
		break;
	default:
		dv_id = IXGBE_LOW_DV(tc);
		break;
	}

	/* Delay value is calculated in bit times convert to KB */
	return IXGBE_BT2KB(dv_id);
}

/*
 * ixgbe_pbthresh_setup - calculate and setup high low water marks
 */
static void ixgbe_pbthresh_setup(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD
	int num_tc = netdev_get_num_tc(adapter->netdev);
=======
	int num_tc = adapter->hw_tcs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i;

	if (!num_tc)
		num_tc = 1;

<<<<<<< HEAD
	hw->fc.low_water = ixgbe_lpbthresh(adapter);

	for (i = 0; i < num_tc; i++) {
		hw->fc.high_water[i] = ixgbe_hpbthresh(adapter, i);

		/* Low water marks must not be larger than high water marks */
		if (hw->fc.low_water > hw->fc.high_water[i])
			hw->fc.low_water = 0;
	}
=======
	for (i = 0; i < num_tc; i++) {
		hw->fc.high_water[i] = ixgbe_hpbthresh(adapter, i);
		hw->fc.low_water[i] = ixgbe_lpbthresh(adapter, i);

		/* Low water marks must not be larger than high water marks */
		if (hw->fc.low_water[i] > hw->fc.high_water[i])
			hw->fc.low_water[i] = 0;
	}

	for (; i < MAX_TRAFFIC_CLASS; i++)
		hw->fc.high_water[i] = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_configure_pb(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	int hdrm;
<<<<<<< HEAD
	u8 tc = netdev_get_num_tc(adapter->netdev);
=======
	u8 tc = adapter->hw_tcs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (adapter->flags & IXGBE_FLAG_FDIR_HASH_CAPABLE ||
	    adapter->flags & IXGBE_FLAG_FDIR_PERFECT_CAPABLE)
		hdrm = 32 << adapter->fdir_pballoc;
	else
		hdrm = 0;

	hw->mac.ops.set_rxpba(hw, tc, hdrm, PBA_STRATEGY_EQUAL);
	ixgbe_pbthresh_setup(adapter);
}

static void ixgbe_fdir_filter_restore(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD
	struct hlist_node *node, *node2;
	struct ixgbe_fdir_filter *filter;
=======
	struct hlist_node *node2;
	struct ixgbe_fdir_filter *filter;
	u8 queue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&adapter->fdir_perfect_lock);

	if (!hlist_empty(&adapter->fdir_filter_list))
		ixgbe_fdir_set_input_mask_82599(hw, &adapter->fdir_mask);

<<<<<<< HEAD
	hlist_for_each_entry_safe(filter, node, node2,
				  &adapter->fdir_filter_list, fdir_node) {
		ixgbe_fdir_write_perfect_filter_82599(hw,
				&filter->filter,
				filter->sw_idx,
				(filter->action == IXGBE_FDIR_DROP_QUEUE) ?
				IXGBE_FDIR_DROP_QUEUE :
				adapter->rx_ring[filter->action]->reg_idx);
=======
	hlist_for_each_entry_safe(filter, node2,
				  &adapter->fdir_filter_list, fdir_node) {
		if (filter->action == IXGBE_FDIR_DROP_QUEUE) {
			queue = IXGBE_FDIR_DROP_QUEUE;
		} else {
			u32 ring = ethtool_get_flow_spec_ring(filter->action);
			u8 vf = ethtool_get_flow_spec_ring_vf(filter->action);

			if (!vf && (ring >= adapter->num_rx_queues)) {
				e_err(drv, "FDIR restore failed without VF, ring: %u\n",
				      ring);
				continue;
			} else if (vf &&
				   ((vf > adapter->num_vfs) ||
				     ring >= adapter->num_rx_queues_per_pool)) {
				e_err(drv, "FDIR restore failed with VF, vf: %hhu, ring: %u\n",
				      vf, ring);
				continue;
			}

			/* Map the ring onto the absolute queue index */
			if (!vf)
				queue = adapter->rx_ring[ring]->reg_idx;
			else
				queue = ((vf - 1) *
					adapter->num_rx_queues_per_pool) + ring;
		}

		ixgbe_fdir_write_perfect_filter_82599(hw,
				&filter->filter, filter->sw_idx, queue);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	spin_unlock(&adapter->fdir_perfect_lock);
}

<<<<<<< HEAD
=======
/**
 * ixgbe_clean_rx_ring - Free Rx Buffers per Queue
 * @rx_ring: ring to free buffers from
 **/
static void ixgbe_clean_rx_ring(struct ixgbe_ring *rx_ring)
{
	u16 i = rx_ring->next_to_clean;
	struct ixgbe_rx_buffer *rx_buffer = &rx_ring->rx_buffer_info[i];

	if (rx_ring->xsk_pool) {
		ixgbe_xsk_clean_rx_ring(rx_ring);
		goto skip_free;
	}

	/* Free all the Rx ring sk_buffs */
	while (i != rx_ring->next_to_alloc) {
		if (rx_buffer->skb) {
			struct sk_buff *skb = rx_buffer->skb;
			if (IXGBE_CB(skb)->page_released)
				dma_unmap_page_attrs(rx_ring->dev,
						     IXGBE_CB(skb)->dma,
						     ixgbe_rx_pg_size(rx_ring),
						     DMA_FROM_DEVICE,
						     IXGBE_RX_DMA_ATTR);
			dev_kfree_skb(skb);
		}

		/* Invalidate cache lines that may have been written to by
		 * device so that we avoid corrupting memory.
		 */
		dma_sync_single_range_for_cpu(rx_ring->dev,
					      rx_buffer->dma,
					      rx_buffer->page_offset,
					      ixgbe_rx_bufsz(rx_ring),
					      DMA_FROM_DEVICE);

		/* free resources associated with mapping */
		dma_unmap_page_attrs(rx_ring->dev, rx_buffer->dma,
				     ixgbe_rx_pg_size(rx_ring),
				     DMA_FROM_DEVICE,
				     IXGBE_RX_DMA_ATTR);
		__page_frag_cache_drain(rx_buffer->page,
					rx_buffer->pagecnt_bias);

		i++;
		rx_buffer++;
		if (i == rx_ring->count) {
			i = 0;
			rx_buffer = rx_ring->rx_buffer_info;
		}
	}

skip_free:
	rx_ring->next_to_alloc = 0;
	rx_ring->next_to_clean = 0;
	rx_ring->next_to_use = 0;
}

static int ixgbe_fwd_ring_up(struct ixgbe_adapter *adapter,
			     struct ixgbe_fwd_adapter *accel)
{
	u16 rss_i = adapter->ring_feature[RING_F_RSS].indices;
	int num_tc = netdev_get_num_tc(adapter->netdev);
	struct net_device *vdev = accel->netdev;
	int i, baseq, err;

	baseq = accel->pool * adapter->num_rx_queues_per_pool;
	netdev_dbg(vdev, "pool %i:%i queues %i:%i\n",
		   accel->pool, adapter->num_rx_pools,
		   baseq, baseq + adapter->num_rx_queues_per_pool);

	accel->rx_base_queue = baseq;
	accel->tx_base_queue = baseq;

	/* record configuration for macvlan interface in vdev */
	for (i = 0; i < num_tc; i++)
		netdev_bind_sb_channel_queue(adapter->netdev, vdev,
					     i, rss_i, baseq + (rss_i * i));

	for (i = 0; i < adapter->num_rx_queues_per_pool; i++)
		adapter->rx_ring[baseq + i]->netdev = vdev;

	/* Guarantee all rings are updated before we update the
	 * MAC address filter.
	 */
	wmb();

	/* ixgbe_add_mac_filter will return an index if it succeeds, so we
	 * need to only treat it as an error value if it is negative.
	 */
	err = ixgbe_add_mac_filter(adapter, vdev->dev_addr,
				   VMDQ_P(accel->pool));
	if (err >= 0)
		return 0;

	/* if we cannot add the MAC rule then disable the offload */
	macvlan_release_l2fw_offload(vdev);

	for (i = 0; i < adapter->num_rx_queues_per_pool; i++)
		adapter->rx_ring[baseq + i]->netdev = NULL;

	netdev_err(vdev, "L2FW offload disabled due to L2 filter error\n");

	/* unbind the queues and drop the subordinate channel config */
	netdev_unbind_sb_channel(adapter->netdev, vdev);
	netdev_set_sb_channel(vdev, 0);

	clear_bit(accel->pool, adapter->fwd_bitmask);
	kfree(accel);

	return err;
}

static int ixgbe_macvlan_up(struct net_device *vdev,
			    struct netdev_nested_priv *priv)
{
	struct ixgbe_adapter *adapter = (struct ixgbe_adapter *)priv->data;
	struct ixgbe_fwd_adapter *accel;

	if (!netif_is_macvlan(vdev))
		return 0;

	accel = macvlan_accel_priv(vdev);
	if (!accel)
		return 0;

	ixgbe_fwd_ring_up(adapter, accel);

	return 0;
}

static void ixgbe_configure_dfwd(struct ixgbe_adapter *adapter)
{
	struct netdev_nested_priv priv = {
		.data = (void *)adapter,
	};

	netdev_walk_all_upper_dev_rcu(adapter->netdev,
				      ixgbe_macvlan_up, &priv);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void ixgbe_configure(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;

	ixgbe_configure_pb(adapter);
#ifdef CONFIG_IXGBE_DCB
	ixgbe_configure_dcb(adapter);
#endif
<<<<<<< HEAD

	ixgbe_set_rx_mode(adapter->netdev);
	ixgbe_restore_vlan(adapter);

#ifdef IXGBE_FCOE
	if (adapter->flags & IXGBE_FLAG_FCOE_ENABLED)
		ixgbe_configure_fcoe(adapter);

#endif /* IXGBE_FCOE */
=======
	/*
	 * We must restore virtualization before VLANs or else
	 * the VLVF registers will not be populated
	 */
	ixgbe_configure_virtualization(adapter);

	ixgbe_set_rx_mode(adapter->netdev);
	ixgbe_restore_vlan(adapter);
	ixgbe_ipsec_restore(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (hw->mac.type) {
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
		hw->mac.ops.disable_rx_buff(hw);
		break;
	default:
		break;
	}

	if (adapter->flags & IXGBE_FLAG_FDIR_HASH_CAPABLE) {
		ixgbe_init_fdir_signature_82599(&adapter->hw,
						adapter->fdir_pballoc);
	} else if (adapter->flags & IXGBE_FLAG_FDIR_PERFECT_CAPABLE) {
		ixgbe_init_fdir_perfect_82599(&adapter->hw,
					      adapter->fdir_pballoc);
		ixgbe_fdir_filter_restore(adapter);
	}

	switch (hw->mac.type) {
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
		hw->mac.ops.enable_rx_buff(hw);
		break;
	default:
		break;
	}

<<<<<<< HEAD
	ixgbe_configure_virtualization(adapter);

	ixgbe_configure_tx(adapter);
	ixgbe_configure_rx(adapter);
}

static inline bool ixgbe_is_sfp(struct ixgbe_hw *hw)
{
	switch (hw->phy.type) {
	case ixgbe_phy_sfp_avago:
	case ixgbe_phy_sfp_ftl:
	case ixgbe_phy_sfp_intel:
	case ixgbe_phy_sfp_unknown:
	case ixgbe_phy_sfp_passive_tyco:
	case ixgbe_phy_sfp_passive_unknown:
	case ixgbe_phy_sfp_active_unknown:
	case ixgbe_phy_sfp_ftl_active:
		return true;
	case ixgbe_phy_nl:
		if (hw->mac.type == ixgbe_mac_82598EB)
			return true;
	default:
		return false;
	}
=======
#ifdef CONFIG_IXGBE_DCA
	/* configure DCA */
	if (adapter->flags & IXGBE_FLAG_DCA_CAPABLE)
		ixgbe_setup_dca(adapter);
#endif /* CONFIG_IXGBE_DCA */

#ifdef IXGBE_FCOE
	/* configure FCoE L2 filters, redirection table, and Rx control */
	ixgbe_configure_fcoe(adapter);

#endif /* IXGBE_FCOE */
	ixgbe_configure_tx(adapter);
	ixgbe_configure_rx(adapter);
	ixgbe_configure_dfwd(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_sfp_link_config - set up SFP+ link
 * @adapter: pointer to private adapter struct
 **/
static void ixgbe_sfp_link_config(struct ixgbe_adapter *adapter)
{
	/*
	 * We are assuming the worst case scenario here, and that
	 * is that an SFP was inserted/removed after the reset
	 * but before SFP detection was enabled.  As such the best
	 * solution is to just start searching as soon as we start
	 */
	if (adapter->hw.mac.type == ixgbe_mac_82598EB)
		adapter->flags2 |= IXGBE_FLAG2_SEARCH_FOR_SFP;

	adapter->flags2 |= IXGBE_FLAG2_SFP_NEEDS_RESET;
<<<<<<< HEAD
=======
	adapter->sfp_poll_time = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_non_sfp_link_config - set up non-SFP+ link
 * @hw: pointer to private hardware struct
 *
 * Returns 0 on success, negative on failure
 **/
static int ixgbe_non_sfp_link_config(struct ixgbe_hw *hw)
{
<<<<<<< HEAD
	u32 autoneg;
	bool negotiation, link_up = false;
	u32 ret = IXGBE_ERR_LINK_SETUP;

	if (hw->mac.ops.check_link)
		ret = hw->mac.ops.check_link(hw, &autoneg, &link_up, false);

	if (ret)
		goto link_cfg_out;

	autoneg = hw->phy.autoneg_advertised;
	if ((!autoneg) && (hw->mac.ops.get_link_capabilities))
		ret = hw->mac.ops.get_link_capabilities(hw, &autoneg,
							&negotiation);
	if (ret)
		goto link_cfg_out;

	if (hw->mac.ops.setup_link)
		ret = hw->mac.ops.setup_link(hw, autoneg, negotiation, link_up);
link_cfg_out:
	return ret;
}

=======
	u32 speed;
	bool autoneg, link_up = false;
	int ret = -EIO;

	if (hw->mac.ops.check_link)
		ret = hw->mac.ops.check_link(hw, &speed, &link_up, false);

	if (ret)
		return ret;

	speed = hw->phy.autoneg_advertised;
	if (!speed && hw->mac.ops.get_link_capabilities) {
		ret = hw->mac.ops.get_link_capabilities(hw, &speed,
							&autoneg);
		/* remove NBASE-T speeds from default autonegotiation
		 * to accommodate broken network switches in the field
		 * which cannot cope with advertised NBASE-T speeds
		 */
		speed &= ~(IXGBE_LINK_SPEED_5GB_FULL |
			   IXGBE_LINK_SPEED_2_5GB_FULL);
	}

	if (ret)
		return ret;

	if (hw->mac.ops.setup_link)
		ret = hw->mac.ops.setup_link(hw, speed, link_up);

	return ret;
}

/**
 * ixgbe_clear_vf_stats_counters - Clear out VF stats after reset
 * @adapter: board private structure
 *
 * On a reset we need to clear out the VF stats or accounting gets
 * messed up because they're not clear on read.
 **/
static void ixgbe_clear_vf_stats_counters(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	int i;

	for (i = 0; i < adapter->num_vfs; i++) {
		adapter->vfinfo[i].last_vfstats.gprc =
			IXGBE_READ_REG(hw, IXGBE_PVFGPRC(i));
		adapter->vfinfo[i].saved_rst_vfstats.gprc +=
			adapter->vfinfo[i].vfstats.gprc;
		adapter->vfinfo[i].vfstats.gprc = 0;
		adapter->vfinfo[i].last_vfstats.gptc =
			IXGBE_READ_REG(hw, IXGBE_PVFGPTC(i));
		adapter->vfinfo[i].saved_rst_vfstats.gptc +=
			adapter->vfinfo[i].vfstats.gptc;
		adapter->vfinfo[i].vfstats.gptc = 0;
		adapter->vfinfo[i].last_vfstats.gorc =
			IXGBE_READ_REG(hw, IXGBE_PVFGORC_LSB(i));
		adapter->vfinfo[i].saved_rst_vfstats.gorc +=
			adapter->vfinfo[i].vfstats.gorc;
		adapter->vfinfo[i].vfstats.gorc = 0;
		adapter->vfinfo[i].last_vfstats.gotc =
			IXGBE_READ_REG(hw, IXGBE_PVFGOTC_LSB(i));
		adapter->vfinfo[i].saved_rst_vfstats.gotc +=
			adapter->vfinfo[i].vfstats.gotc;
		adapter->vfinfo[i].vfstats.gotc = 0;
		adapter->vfinfo[i].last_vfstats.mprc =
			IXGBE_READ_REG(hw, IXGBE_PVFMPRC(i));
		adapter->vfinfo[i].saved_rst_vfstats.mprc +=
			adapter->vfinfo[i].vfstats.mprc;
		adapter->vfinfo[i].vfstats.mprc = 0;
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void ixgbe_setup_gpie(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 gpie = 0;

	if (adapter->flags & IXGBE_FLAG_MSIX_ENABLED) {
		gpie = IXGBE_GPIE_MSIX_MODE | IXGBE_GPIE_PBA_SUPPORT |
		       IXGBE_GPIE_OCD;
		gpie |= IXGBE_GPIE_EIAME;
		/*
		 * use EIAM to auto-mask when MSI-X interrupt is asserted
		 * this saves a register write for every interrupt
		 */
		switch (hw->mac.type) {
		case ixgbe_mac_82598EB:
			IXGBE_WRITE_REG(hw, IXGBE_EIAM, IXGBE_EICS_RTX_QUEUE);
			break;
		case ixgbe_mac_82599EB:
		case ixgbe_mac_X540:
<<<<<<< HEAD
=======
		case ixgbe_mac_X550:
		case ixgbe_mac_X550EM_x:
		case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			IXGBE_WRITE_REG(hw, IXGBE_EIAM_EX(0), 0xFFFFFFFF);
			IXGBE_WRITE_REG(hw, IXGBE_EIAM_EX(1), 0xFFFFFFFF);
			break;
		}
	} else {
		/* legacy interrupts, use EIAM to auto-mask when reading EICR,
		 * specifically only auto mask tx and rx interrupts */
		IXGBE_WRITE_REG(hw, IXGBE_EIAM, IXGBE_EICS_RTX_QUEUE);
	}

	/* XXX: to interrupt immediately for EICS writes, enable this */
	/* gpie |= IXGBE_GPIE_EIMEN; */

	if (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED) {
		gpie &= ~IXGBE_GPIE_VTMODE_MASK;
<<<<<<< HEAD
		gpie |= IXGBE_GPIE_VTMODE_64;
=======

		switch (adapter->ring_feature[RING_F_VMDQ].mask) {
		case IXGBE_82599_VMDQ_8Q_MASK:
			gpie |= IXGBE_GPIE_VTMODE_16;
			break;
		case IXGBE_82599_VMDQ_4Q_MASK:
			gpie |= IXGBE_GPIE_VTMODE_32;
			break;
		default:
			gpie |= IXGBE_GPIE_VTMODE_64;
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Enable Thermal over heat sensor interrupt */
	if (adapter->flags2 & IXGBE_FLAG2_TEMP_SENSOR_CAPABLE) {
		switch (adapter->hw.mac.type) {
		case ixgbe_mac_82599EB:
<<<<<<< HEAD
			gpie |= IXGBE_SDP0_GPIEN;
			break;
		case ixgbe_mac_X540:
			gpie |= IXGBE_EIMS_TS;
=======
			gpie |= IXGBE_SDP0_GPIEN_8259X;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		default:
			break;
		}
	}

	/* Enable fan failure interrupt */
	if (adapter->flags & IXGBE_FLAG_FAN_FAIL_CAPABLE)
<<<<<<< HEAD
		gpie |= IXGBE_SDP1_GPIEN;

	if (hw->mac.type == ixgbe_mac_82599EB) {
		gpie |= IXGBE_SDP1_GPIEN;
		gpie |= IXGBE_SDP2_GPIEN;
=======
		gpie |= IXGBE_SDP1_GPIEN(hw);

	switch (hw->mac.type) {
	case ixgbe_mac_82599EB:
		gpie |= IXGBE_SDP1_GPIEN_8259X | IXGBE_SDP2_GPIEN_8259X;
		break;
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
		gpie |= IXGBE_SDP0_GPIEN_X540;
		break;
	default:
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	IXGBE_WRITE_REG(hw, IXGBE_GPIE, gpie);
}

static void ixgbe_up_complete(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	int err;
	u32 ctrl_ext;

	ixgbe_get_hw_control(adapter);
	ixgbe_setup_gpie(adapter);

	if (adapter->flags & IXGBE_FLAG_MSIX_ENABLED)
		ixgbe_configure_msix(adapter);
	else
		ixgbe_configure_msi_and_legacy(adapter);

<<<<<<< HEAD
	/* enable the optics for both mult-speed fiber and 82599 SFP+ fiber */
	if (hw->mac.ops.enable_tx_laser &&
	    ((hw->phy.multispeed_fiber) ||
	     ((hw->mac.ops.get_media_type(hw) == ixgbe_media_type_fiber) &&
	      (hw->mac.type == ixgbe_mac_82599EB))))
		hw->mac.ops.enable_tx_laser(hw);

=======
	/* enable the optics for 82599 SFP+ fiber */
	if (hw->mac.ops.enable_tx_laser)
		hw->mac.ops.enable_tx_laser(hw);

	if (hw->phy.ops.set_phy_power)
		hw->phy.ops.set_phy_power(hw, true);

	smp_mb__before_atomic();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clear_bit(__IXGBE_DOWN, &adapter->state);
	ixgbe_napi_enable_all(adapter);

	if (ixgbe_is_sfp(hw)) {
		ixgbe_sfp_link_config(adapter);
	} else {
		err = ixgbe_non_sfp_link_config(hw);
		if (err)
			e_err(probe, "link_config FAILED %d\n", err);
	}

	/* clear any pending interrupts, may auto mask */
	IXGBE_READ_REG(hw, IXGBE_EICR);
	ixgbe_irq_enable(adapter, true, true);

	/*
	 * If this adapter has a fan, check to see if we had a failure
	 * before we enabled the interrupt.
	 */
	if (adapter->flags & IXGBE_FLAG_FAN_FAIL_CAPABLE) {
		u32 esdp = IXGBE_READ_REG(hw, IXGBE_ESDP);
		if (esdp & IXGBE_ESDP_SDP1)
			e_crit(drv, "Fan has stopped, replace the adapter\n");
	}

<<<<<<< HEAD
	/* enable transmits */
	netif_tx_start_all_queues(adapter->netdev);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* bring the link up in the watchdog, this could race with our first
	 * link up interrupt but shouldn't be a problem */
	adapter->flags |= IXGBE_FLAG_NEED_LINK_UPDATE;
	adapter->link_check_timeout = jiffies;
	mod_timer(&adapter->service_timer, jiffies);

<<<<<<< HEAD
=======
	ixgbe_clear_vf_stats_counters(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Set PF Reset Done bit so PF/VF Mail Ops can work */
	ctrl_ext = IXGBE_READ_REG(hw, IXGBE_CTRL_EXT);
	ctrl_ext |= IXGBE_CTRL_EXT_PFRSTD;
	IXGBE_WRITE_REG(hw, IXGBE_CTRL_EXT, ctrl_ext);
<<<<<<< HEAD
=======

	/* update setting rx tx for all active vfs */
	ixgbe_set_all_vfs(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void ixgbe_reinit_locked(struct ixgbe_adapter *adapter)
{
<<<<<<< HEAD
	WARN_ON(in_interrupt());
	/* put off any impending NetWatchDogTimeout */
	adapter->netdev->trans_start = jiffies;

	while (test_and_set_bit(__IXGBE_RESETTING, &adapter->state))
		usleep_range(1000, 2000);
=======
	/* put off any impending NetWatchDogTimeout */
	netif_trans_update(adapter->netdev);

	while (test_and_set_bit(__IXGBE_RESETTING, &adapter->state))
		usleep_range(1000, 2000);
	if (adapter->hw.phy.type == ixgbe_phy_fw)
		ixgbe_watchdog_link_is_down(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ixgbe_down(adapter);
	/*
	 * If SR-IOV enabled then wait a bit before bringing the adapter
	 * back up to give the VFs time to respond to the reset.  The
	 * two second wait is based upon the watchdog timer cycle in
	 * the VF driver.
	 */
	if (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED)
		msleep(2000);
	ixgbe_up(adapter);
	clear_bit(__IXGBE_RESETTING, &adapter->state);
}

void ixgbe_up(struct ixgbe_adapter *adapter)
{
	/* hardware has been reset, we need to reload some things */
	ixgbe_configure(adapter);

	ixgbe_up_complete(adapter);
}

<<<<<<< HEAD
void ixgbe_reset(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	int err;

=======
static unsigned long ixgbe_get_completion_timeout(struct ixgbe_adapter *adapter)
{
	u16 devctl2;

	pcie_capability_read_word(adapter->pdev, PCI_EXP_DEVCTL2, &devctl2);

	switch (devctl2 & IXGBE_PCIDEVCTRL2_TIMEO_MASK) {
	case IXGBE_PCIDEVCTRL2_17_34s:
	case IXGBE_PCIDEVCTRL2_4_8s:
		/* For now we cap the upper limit on delay to 2 seconds
		 * as we end up going up to 34 seconds of delay in worst
		 * case timeout value.
		 */
	case IXGBE_PCIDEVCTRL2_1_2s:
		return 2000000ul;	/* 2.0 s */
	case IXGBE_PCIDEVCTRL2_260_520ms:
		return 520000ul;	/* 520 ms */
	case IXGBE_PCIDEVCTRL2_65_130ms:
		return 130000ul;	/* 130 ms */
	case IXGBE_PCIDEVCTRL2_16_32ms:
		return 32000ul;		/* 32 ms */
	case IXGBE_PCIDEVCTRL2_1_2ms:
		return 2000ul;		/* 2 ms */
	case IXGBE_PCIDEVCTRL2_50_100us:
		return 100ul;		/* 100 us */
	case IXGBE_PCIDEVCTRL2_16_32ms_def:
		return 32000ul;		/* 32 ms */
	default:
		break;
	}

	/* We shouldn't need to hit this path, but just in case default as
	 * though completion timeout is not supported and support 32ms.
	 */
	return 32000ul;
}

void ixgbe_disable_rx(struct ixgbe_adapter *adapter)
{
	unsigned long wait_delay, delay_interval;
	struct ixgbe_hw *hw = &adapter->hw;
	int i, wait_loop;
	u32 rxdctl;

	/* disable receives */
	hw->mac.ops.disable_rx(hw);

	if (ixgbe_removed(hw->hw_addr))
		return;

	/* disable all enabled Rx queues */
	for (i = 0; i < adapter->num_rx_queues; i++) {
		struct ixgbe_ring *ring = adapter->rx_ring[i];
		u8 reg_idx = ring->reg_idx;

		rxdctl = IXGBE_READ_REG(hw, IXGBE_RXDCTL(reg_idx));
		rxdctl &= ~IXGBE_RXDCTL_ENABLE;
		rxdctl |= IXGBE_RXDCTL_SWFLSH;

		/* write value back with RXDCTL.ENABLE bit cleared */
		IXGBE_WRITE_REG(hw, IXGBE_RXDCTL(reg_idx), rxdctl);
	}

	/* RXDCTL.EN may not change on 82598 if link is down, so skip it */
	if (hw->mac.type == ixgbe_mac_82598EB &&
	    !(IXGBE_READ_REG(hw, IXGBE_LINKS) & IXGBE_LINKS_UP))
		return;

	/* Determine our minimum delay interval. We will increase this value
	 * with each subsequent test. This way if the device returns quickly
	 * we should spend as little time as possible waiting, however as
	 * the time increases we will wait for larger periods of time.
	 *
	 * The trick here is that we increase the interval using the
	 * following pattern: 1x 3x 5x 7x 9x 11x 13x 15x 17x 19x. The result
	 * of that wait is that it totals up to 100x whatever interval we
	 * choose. Since our minimum wait is 100us we can just divide the
	 * total timeout by 100 to get our minimum delay interval.
	 */
	delay_interval = ixgbe_get_completion_timeout(adapter) / 100;

	wait_loop = IXGBE_MAX_RX_DESC_POLL;
	wait_delay = delay_interval;

	while (wait_loop--) {
		usleep_range(wait_delay, wait_delay + 10);
		wait_delay += delay_interval * 2;
		rxdctl = 0;

		/* OR together the reading of all the active RXDCTL registers,
		 * and then test the result. We need the disable to complete
		 * before we start freeing the memory and invalidating the
		 * DMA mappings.
		 */
		for (i = 0; i < adapter->num_rx_queues; i++) {
			struct ixgbe_ring *ring = adapter->rx_ring[i];
			u8 reg_idx = ring->reg_idx;

			rxdctl |= IXGBE_READ_REG(hw, IXGBE_RXDCTL(reg_idx));
		}

		if (!(rxdctl & IXGBE_RXDCTL_ENABLE))
			return;
	}

	e_err(drv,
	      "RXDCTL.ENABLE for one or more queues not cleared within the polling period\n");
}

void ixgbe_disable_tx(struct ixgbe_adapter *adapter)
{
	unsigned long wait_delay, delay_interval;
	struct ixgbe_hw *hw = &adapter->hw;
	int i, wait_loop;
	u32 txdctl;

	if (ixgbe_removed(hw->hw_addr))
		return;

	/* disable all enabled Tx queues */
	for (i = 0; i < adapter->num_tx_queues; i++) {
		struct ixgbe_ring *ring = adapter->tx_ring[i];
		u8 reg_idx = ring->reg_idx;

		IXGBE_WRITE_REG(hw, IXGBE_TXDCTL(reg_idx), IXGBE_TXDCTL_SWFLSH);
	}

	/* disable all enabled XDP Tx queues */
	for (i = 0; i < adapter->num_xdp_queues; i++) {
		struct ixgbe_ring *ring = adapter->xdp_ring[i];
		u8 reg_idx = ring->reg_idx;

		IXGBE_WRITE_REG(hw, IXGBE_TXDCTL(reg_idx), IXGBE_TXDCTL_SWFLSH);
	}

	/* If the link is not up there shouldn't be much in the way of
	 * pending transactions. Those that are left will be flushed out
	 * when the reset logic goes through the flush sequence to clean out
	 * the pending Tx transactions.
	 */
	if (!(IXGBE_READ_REG(hw, IXGBE_LINKS) & IXGBE_LINKS_UP))
		goto dma_engine_disable;

	/* Determine our minimum delay interval. We will increase this value
	 * with each subsequent test. This way if the device returns quickly
	 * we should spend as little time as possible waiting, however as
	 * the time increases we will wait for larger periods of time.
	 *
	 * The trick here is that we increase the interval using the
	 * following pattern: 1x 3x 5x 7x 9x 11x 13x 15x 17x 19x. The result
	 * of that wait is that it totals up to 100x whatever interval we
	 * choose. Since our minimum wait is 100us we can just divide the
	 * total timeout by 100 to get our minimum delay interval.
	 */
	delay_interval = ixgbe_get_completion_timeout(adapter) / 100;

	wait_loop = IXGBE_MAX_RX_DESC_POLL;
	wait_delay = delay_interval;

	while (wait_loop--) {
		usleep_range(wait_delay, wait_delay + 10);
		wait_delay += delay_interval * 2;
		txdctl = 0;

		/* OR together the reading of all the active TXDCTL registers,
		 * and then test the result. We need the disable to complete
		 * before we start freeing the memory and invalidating the
		 * DMA mappings.
		 */
		for (i = 0; i < adapter->num_tx_queues; i++) {
			struct ixgbe_ring *ring = adapter->tx_ring[i];
			u8 reg_idx = ring->reg_idx;

			txdctl |= IXGBE_READ_REG(hw, IXGBE_TXDCTL(reg_idx));
		}
		for (i = 0; i < adapter->num_xdp_queues; i++) {
			struct ixgbe_ring *ring = adapter->xdp_ring[i];
			u8 reg_idx = ring->reg_idx;

			txdctl |= IXGBE_READ_REG(hw, IXGBE_TXDCTL(reg_idx));
		}

		if (!(txdctl & IXGBE_TXDCTL_ENABLE))
			goto dma_engine_disable;
	}

	e_err(drv,
	      "TXDCTL.ENABLE for one or more queues not cleared within the polling period\n");

dma_engine_disable:
	/* Disable the Tx DMA engine on 82599 and later MAC */
	switch (hw->mac.type) {
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
		IXGBE_WRITE_REG(hw, IXGBE_DMATXCTL,
				(IXGBE_READ_REG(hw, IXGBE_DMATXCTL) &
				 ~IXGBE_DMATXCTL_TE));
		fallthrough;
	default:
		break;
	}
}

void ixgbe_reset(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	struct net_device *netdev = adapter->netdev;
	int err;

	if (ixgbe_removed(hw->hw_addr))
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* lock SFP init bit to prevent race conditions with the watchdog */
	while (test_and_set_bit(__IXGBE_IN_SFP_INIT, &adapter->state))
		usleep_range(1000, 2000);

	/* clear all SFP and link config related flags while holding SFP_INIT */
	adapter->flags2 &= ~(IXGBE_FLAG2_SEARCH_FOR_SFP |
			     IXGBE_FLAG2_SFP_NEEDS_RESET);
	adapter->flags &= ~IXGBE_FLAG_NEED_LINK_CONFIG;

	err = hw->mac.ops.init_hw(hw);
	switch (err) {
	case 0:
<<<<<<< HEAD
	case IXGBE_ERR_SFP_NOT_PRESENT:
	case IXGBE_ERR_SFP_NOT_SUPPORTED:
		break;
	case IXGBE_ERR_MASTER_REQUESTS_PENDING:
		e_dev_err("master disable timed out\n");
		break;
	case IXGBE_ERR_EEPROM_VERSION:
=======
	case -ENOENT:
	case -EOPNOTSUPP:
		break;
	case -EALREADY:
		e_dev_err("primary disable timed out\n");
		break;
	case -EACCES:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* We are running on a pre-production device, log a warning */
		e_dev_warn("This device is a pre-production adapter/LOM. "
			   "Please be aware there may be issues associated with "
			   "your hardware.  If you are experiencing problems "
			   "please contact your Intel or hardware "
			   "representative who provided you with this "
			   "hardware.\n");
		break;
	default:
		e_dev_err("Hardware Error: %d\n", err);
	}

	clear_bit(__IXGBE_IN_SFP_INIT, &adapter->state);

<<<<<<< HEAD
	/* reprogram the RAR[0] in case user changed it. */
	hw->mac.ops.set_rar(hw, 0, hw->mac.addr, adapter->num_vfs,
			    IXGBE_RAH_AV);
}

/**
 * ixgbe_init_rx_page_offset - initialize page offset values for Rx buffers
 * @rx_ring: ring to setup
 *
 * On many IA platforms the L1 cache has a critical stride of 4K, this
 * results in each receive buffer starting in the same cache set.  To help
 * reduce the pressure on this cache set we can interleave the offsets so
 * that only every other buffer will be in the same cache set.
 **/
static void ixgbe_init_rx_page_offset(struct ixgbe_ring *rx_ring)
{
	struct ixgbe_rx_buffer *rx_buffer = rx_ring->rx_buffer_info;
	u16 i;

	for (i = 0; i < rx_ring->count; i += 2) {
		rx_buffer[0].page_offset = 0;
		rx_buffer[1].page_offset = ixgbe_rx_bufsz(rx_ring);
		rx_buffer = &rx_buffer[2];
=======
	/* flush entries out of MAC table */
	ixgbe_flush_sw_mac_table(adapter);
	__dev_uc_unsync(netdev, NULL);

	/* do not flush user set addresses */
	ixgbe_mac_set_default_filter(adapter);

	/* update SAN MAC vmdq pool selection */
	if (hw->mac.san_mac_rar_index)
		hw->mac.ops.set_vmdq_san_mac(hw, VMDQ_P(0));

	if (test_bit(__IXGBE_PTP_RUNNING, &adapter->state))
		ixgbe_ptp_reset(adapter);

	if (hw->phy.ops.set_phy_power) {
		if (!netif_running(adapter->netdev) && !adapter->wol)
			hw->phy.ops.set_phy_power(hw, false);
		else
			hw->phy.ops.set_phy_power(hw, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/**
<<<<<<< HEAD
 * ixgbe_clean_rx_ring - Free Rx Buffers per Queue
 * @rx_ring: ring to free buffers from
 **/
static void ixgbe_clean_rx_ring(struct ixgbe_ring *rx_ring)
{
	struct device *dev = rx_ring->dev;
	unsigned long size;
	u16 i;

	/* ring already cleared, nothing to do */
	if (!rx_ring->rx_buffer_info)
		return;

	/* Free all the Rx ring sk_buffs */
	for (i = 0; i < rx_ring->count; i++) {
		struct ixgbe_rx_buffer *rx_buffer;

		rx_buffer = &rx_ring->rx_buffer_info[i];
		if (rx_buffer->skb) {
			struct sk_buff *skb = rx_buffer->skb;
			if (IXGBE_CB(skb)->page_released) {
				dma_unmap_page(dev,
					       IXGBE_CB(skb)->dma,
					       ixgbe_rx_bufsz(rx_ring),
					       DMA_FROM_DEVICE);
				IXGBE_CB(skb)->page_released = false;
			}
			dev_kfree_skb(skb);
		}
		rx_buffer->skb = NULL;
		if (rx_buffer->dma)
			dma_unmap_page(dev, rx_buffer->dma,
				       ixgbe_rx_pg_size(rx_ring),
				       DMA_FROM_DEVICE);
		rx_buffer->dma = 0;
		if (rx_buffer->page)
			put_page(rx_buffer->page);
		rx_buffer->page = NULL;
	}

	size = sizeof(struct ixgbe_rx_buffer) * rx_ring->count;
	memset(rx_ring->rx_buffer_info, 0, size);

	ixgbe_init_rx_page_offset(rx_ring);

	/* Zero out the descriptor ring */
	memset(rx_ring->desc, 0, rx_ring->size);

	rx_ring->next_to_alloc = 0;
	rx_ring->next_to_clean = 0;
	rx_ring->next_to_use = 0;
}

/**
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ixgbe_clean_tx_ring - Free Tx Buffers
 * @tx_ring: ring to be cleaned
 **/
static void ixgbe_clean_tx_ring(struct ixgbe_ring *tx_ring)
{
<<<<<<< HEAD
	struct ixgbe_tx_buffer *tx_buffer_info;
	unsigned long size;
	u16 i;

	/* ring already cleared, nothing to do */
	if (!tx_ring->tx_buffer_info)
		return;

	/* Free all the Tx ring sk_buffs */
	for (i = 0; i < tx_ring->count; i++) {
		tx_buffer_info = &tx_ring->tx_buffer_info[i];
		ixgbe_unmap_and_free_tx_resource(tx_ring, tx_buffer_info);
	}

	netdev_tx_reset_queue(txring_txq(tx_ring));

	size = sizeof(struct ixgbe_tx_buffer) * tx_ring->count;
	memset(tx_ring->tx_buffer_info, 0, size);

	/* Zero out the descriptor ring */
	memset(tx_ring->desc, 0, tx_ring->size);

=======
	u16 i = tx_ring->next_to_clean;
	struct ixgbe_tx_buffer *tx_buffer = &tx_ring->tx_buffer_info[i];

	if (tx_ring->xsk_pool) {
		ixgbe_xsk_clean_tx_ring(tx_ring);
		goto out;
	}

	while (i != tx_ring->next_to_use) {
		union ixgbe_adv_tx_desc *eop_desc, *tx_desc;

		/* Free all the Tx ring sk_buffs */
		if (ring_is_xdp(tx_ring))
			xdp_return_frame(tx_buffer->xdpf);
		else
			dev_kfree_skb_any(tx_buffer->skb);

		/* unmap skb header data */
		dma_unmap_single(tx_ring->dev,
				 dma_unmap_addr(tx_buffer, dma),
				 dma_unmap_len(tx_buffer, len),
				 DMA_TO_DEVICE);

		/* check for eop_desc to determine the end of the packet */
		eop_desc = tx_buffer->next_to_watch;
		tx_desc = IXGBE_TX_DESC(tx_ring, i);

		/* unmap remaining buffers */
		while (tx_desc != eop_desc) {
			tx_buffer++;
			tx_desc++;
			i++;
			if (unlikely(i == tx_ring->count)) {
				i = 0;
				tx_buffer = tx_ring->tx_buffer_info;
				tx_desc = IXGBE_TX_DESC(tx_ring, 0);
			}

			/* unmap any remaining paged data */
			if (dma_unmap_len(tx_buffer, len))
				dma_unmap_page(tx_ring->dev,
					       dma_unmap_addr(tx_buffer, dma),
					       dma_unmap_len(tx_buffer, len),
					       DMA_TO_DEVICE);
		}

		/* move us one more past the eop_desc for start of next pkt */
		tx_buffer++;
		i++;
		if (unlikely(i == tx_ring->count)) {
			i = 0;
			tx_buffer = tx_ring->tx_buffer_info;
		}
	}

	/* reset BQL for queue */
	if (!ring_is_xdp(tx_ring))
		netdev_tx_reset_queue(txring_txq(tx_ring));

out:
	/* reset next_to_use and next_to_clean */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tx_ring->next_to_use = 0;
	tx_ring->next_to_clean = 0;
}

/**
 * ixgbe_clean_all_rx_rings - Free Rx Buffers for all queues
 * @adapter: board private structure
 **/
static void ixgbe_clean_all_rx_rings(struct ixgbe_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_rx_queues; i++)
		ixgbe_clean_rx_ring(adapter->rx_ring[i]);
}

/**
 * ixgbe_clean_all_tx_rings - Free Tx Buffers for all queues
 * @adapter: board private structure
 **/
static void ixgbe_clean_all_tx_rings(struct ixgbe_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_tx_queues; i++)
		ixgbe_clean_tx_ring(adapter->tx_ring[i]);
<<<<<<< HEAD
=======
	for (i = 0; i < adapter->num_xdp_queues; i++)
		ixgbe_clean_tx_ring(adapter->xdp_ring[i]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_fdir_filter_exit(struct ixgbe_adapter *adapter)
{
<<<<<<< HEAD
	struct hlist_node *node, *node2;
=======
	struct hlist_node *node2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ixgbe_fdir_filter *filter;

	spin_lock(&adapter->fdir_perfect_lock);

<<<<<<< HEAD
	hlist_for_each_entry_safe(filter, node, node2,
=======
	hlist_for_each_entry_safe(filter, node2,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				  &adapter->fdir_filter_list, fdir_node) {
		hlist_del(&filter->fdir_node);
		kfree(filter);
	}
	adapter->fdir_filter_count = 0;

	spin_unlock(&adapter->fdir_perfect_lock);
}

void ixgbe_down(struct ixgbe_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD
	u32 rxctrl;
	int i;

	/* signal that we are down to the interrupt handler */
	set_bit(__IXGBE_DOWN, &adapter->state);

	/* disable receives */
	rxctrl = IXGBE_READ_REG(hw, IXGBE_RXCTRL);
	IXGBE_WRITE_REG(hw, IXGBE_RXCTRL, rxctrl & ~IXGBE_RXCTRL_RXEN);

	/* disable all enabled rx queues */
	for (i = 0; i < adapter->num_rx_queues; i++)
		/* this call also flushes the previous write */
		ixgbe_disable_rx_queue(adapter, adapter->rx_ring[i]);

	usleep_range(10000, 20000);

=======
	int i;

	/* signal that we are down to the interrupt handler */
	if (test_and_set_bit(__IXGBE_DOWN, &adapter->state))
		return; /* do nothing if already down */

	/* Shut off incoming Tx traffic */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	netif_tx_stop_all_queues(netdev);

	/* call carrier off first to avoid false dev_watchdog timeouts */
	netif_carrier_off(netdev);
	netif_tx_disable(netdev);

<<<<<<< HEAD
=======
	/* Disable Rx */
	ixgbe_disable_rx(adapter);

	/* synchronize_rcu() needed for pending XDP buffers to drain */
	if (adapter->xdp_ring[0])
		synchronize_rcu();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ixgbe_irq_disable(adapter);

	ixgbe_napi_disable_all(adapter);

<<<<<<< HEAD
	adapter->flags2 &= ~(IXGBE_FLAG2_FDIR_REQUIRES_REINIT |
			     IXGBE_FLAG2_RESET_REQUESTED);
=======
	clear_bit(__IXGBE_RESET_REQUESTED, &adapter->state);
	adapter->flags2 &= ~IXGBE_FLAG2_FDIR_REQUIRES_REINIT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	adapter->flags &= ~IXGBE_FLAG_NEED_LINK_UPDATE;

	del_timer_sync(&adapter->service_timer);

	if (adapter->num_vfs) {
		/* Clear EITR Select mapping */
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_EITRSEL, 0);

		/* Mark all the VFs as inactive */
		for (i = 0 ; i < adapter->num_vfs; i++)
			adapter->vfinfo[i].clear_to_send = false;

<<<<<<< HEAD
		/* ping all the active vfs to let them know we are going down */
		ixgbe_ping_all_vfs(adapter);

		/* Disable all VFTE/VFRE TX/RX */
		ixgbe_disable_tx_rx(adapter);
	}

	/* disable transmits in the hardware now that interrupts are off */
	for (i = 0; i < adapter->num_tx_queues; i++) {
		u8 reg_idx = adapter->tx_ring[i]->reg_idx;
		IXGBE_WRITE_REG(hw, IXGBE_TXDCTL(reg_idx), IXGBE_TXDCTL_SWFLSH);
	}

	/* Disable the Tx DMA engine on 82599 and X540 */
	switch (hw->mac.type) {
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
		IXGBE_WRITE_REG(hw, IXGBE_DMATXCTL,
				(IXGBE_READ_REG(hw, IXGBE_DMATXCTL) &
				 ~IXGBE_DMATXCTL_TE));
		break;
	default:
		break;
	}
=======
		/* update setting rx tx for all active vfs */
		ixgbe_set_all_vfs(adapter);
	}

	/* disable transmits in the hardware now that interrupts are off */
	ixgbe_disable_tx(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!pci_channel_offline(adapter->pdev))
		ixgbe_reset(adapter);

<<<<<<< HEAD
	/* power down the optics for multispeed fiber and 82599 SFP+ fiber */
	if (hw->mac.ops.disable_tx_laser &&
	    ((hw->phy.multispeed_fiber) ||
	     ((hw->mac.ops.get_media_type(hw) == ixgbe_media_type_fiber) &&
	      (hw->mac.type == ixgbe_mac_82599EB))))
=======
	/* power down the optics for 82599 SFP+ fiber */
	if (hw->mac.ops.disable_tx_laser)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hw->mac.ops.disable_tx_laser(hw);

	ixgbe_clean_all_tx_rings(adapter);
	ixgbe_clean_all_rx_rings(adapter);
<<<<<<< HEAD

#ifdef CONFIG_IXGBE_DCA
	/* since we reset the hardware DCA settings were cleared */
	ixgbe_setup_dca(adapter);
#endif
=======
}

/**
 * ixgbe_set_eee_capable - helper function to determine EEE support on X550
 * @adapter: board private structure
 */
static void ixgbe_set_eee_capable(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;

	switch (hw->device_id) {
	case IXGBE_DEV_ID_X550EM_A_1G_T:
	case IXGBE_DEV_ID_X550EM_A_1G_T_L:
		if (!hw->phy.eee_speeds_supported)
			break;
		adapter->flags2 |= IXGBE_FLAG2_EEE_CAPABLE;
		if (!hw->phy.eee_speeds_advertised)
			break;
		adapter->flags2 |= IXGBE_FLAG2_EEE_ENABLED;
		break;
	default:
		adapter->flags2 &= ~IXGBE_FLAG2_EEE_CAPABLE;
		adapter->flags2 &= ~IXGBE_FLAG2_EEE_ENABLED;
		break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_tx_timeout - Respond to a Tx Hang
 * @netdev: network interface device structure
<<<<<<< HEAD
 **/
static void ixgbe_tx_timeout(struct net_device *netdev)
=======
 * @txqueue: queue number that timed out
 **/
static void ixgbe_tx_timeout(struct net_device *netdev, unsigned int __always_unused txqueue)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);

	/* Do the reset outside of interrupt context */
	ixgbe_tx_timeout_reset(adapter);
}

<<<<<<< HEAD
/**
 * ixgbe_sw_init - Initialize general software structures (struct ixgbe_adapter)
 * @adapter: board private structure to initialize
 *
 * ixgbe_sw_init initializes the Adapter private data structure.
 * Fields are initialized based on PCI device information and
 * OS network device settings (MTU size).
 **/
static int __devinit ixgbe_sw_init(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	struct pci_dev *pdev = adapter->pdev;
	unsigned int rss;
#ifdef CONFIG_IXGBE_DCB
	int j;
	struct tc_configuration *tc;
#endif

	/* PCI config space info */

	hw->vendor_id = pdev->vendor;
	hw->device_id = pdev->device;
	hw->revision_id = pdev->revision;
	hw->subsystem_vendor_id = pdev->subsystem_vendor;
	hw->subsystem_device_id = pdev->subsystem_device;

	/* Set capability flags */
	rss = min_t(int, IXGBE_MAX_RSS_INDICES, num_online_cpus());
	adapter->ring_feature[RING_F_RSS].indices = rss;
	adapter->flags |= IXGBE_FLAG_RSS_ENABLED;
	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		if (hw->device_id == IXGBE_DEV_ID_82598AT)
			adapter->flags |= IXGBE_FLAG_FAN_FAIL_CAPABLE;
		adapter->max_msix_q_vectors = MAX_MSIX_Q_VECTORS_82598;
		break;
	case ixgbe_mac_X540:
		adapter->flags2 |= IXGBE_FLAG2_TEMP_SENSOR_CAPABLE;
	case ixgbe_mac_82599EB:
		adapter->max_msix_q_vectors = MAX_MSIX_Q_VECTORS_82599;
		adapter->flags2 |= IXGBE_FLAG2_RSC_CAPABLE;
		adapter->flags2 |= IXGBE_FLAG2_RSC_ENABLED;
		if (hw->device_id == IXGBE_DEV_ID_82599_T3_LOM)
			adapter->flags2 |= IXGBE_FLAG2_TEMP_SENSOR_CAPABLE;
		/* Flow Director hash filters enabled */
		adapter->flags |= IXGBE_FLAG_FDIR_HASH_CAPABLE;
		adapter->atr_sample_rate = 20;
		adapter->ring_feature[RING_F_FDIR].indices =
							 IXGBE_MAX_FDIR_INDICES;
		adapter->fdir_pballoc = IXGBE_FDIR_PBALLOC_64K;
#ifdef IXGBE_FCOE
		adapter->flags |= IXGBE_FLAG_FCOE_CAPABLE;
		adapter->flags &= ~IXGBE_FLAG_FCOE_ENABLED;
		adapter->ring_feature[RING_F_FCOE].indices = 0;
#ifdef CONFIG_IXGBE_DCB
		/* Default traffic class to use for FCoE */
		adapter->fcoe.up = IXGBE_FCOE_DEFTC;
#endif
#endif /* IXGBE_FCOE */
		break;
	default:
		break;
	}

	/* n-tuple support exists, always init our spinlock */
	spin_lock_init(&adapter->fdir_perfect_lock);

#ifdef CONFIG_IXGBE_DCB
	switch (hw->mac.type) {
	case ixgbe_mac_X540:
		adapter->dcb_cfg.num_tcs.pg_tcs = X540_TRAFFIC_CLASS;
		adapter->dcb_cfg.num_tcs.pfc_tcs = X540_TRAFFIC_CLASS;
		break;
	default:
		adapter->dcb_cfg.num_tcs.pg_tcs = MAX_TRAFFIC_CLASS;
		adapter->dcb_cfg.num_tcs.pfc_tcs = MAX_TRAFFIC_CLASS;
=======
#ifdef CONFIG_IXGBE_DCB
static void ixgbe_init_dcb(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	struct tc_configuration *tc;
	int j;

	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
	case ixgbe_mac_82599EB:
		adapter->dcb_cfg.num_tcs.pg_tcs = MAX_TRAFFIC_CLASS;
		adapter->dcb_cfg.num_tcs.pfc_tcs = MAX_TRAFFIC_CLASS;
		break;
	case ixgbe_mac_X540:
	case ixgbe_mac_X550:
		adapter->dcb_cfg.num_tcs.pg_tcs = X540_TRAFFIC_CLASS;
		adapter->dcb_cfg.num_tcs.pfc_tcs = X540_TRAFFIC_CLASS;
		break;
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
	default:
		adapter->dcb_cfg.num_tcs.pg_tcs = DEF_TRAFFIC_CLASS;
		adapter->dcb_cfg.num_tcs.pfc_tcs = DEF_TRAFFIC_CLASS;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

	/* Configure DCB traffic classes */
	for (j = 0; j < MAX_TRAFFIC_CLASS; j++) {
		tc = &adapter->dcb_cfg.tc_config[j];
		tc->path[DCB_TX_CONFIG].bwg_id = 0;
		tc->path[DCB_TX_CONFIG].bwg_percent = 12 + (j & 1);
		tc->path[DCB_RX_CONFIG].bwg_id = 0;
		tc->path[DCB_RX_CONFIG].bwg_percent = 12 + (j & 1);
		tc->dcb_pfc = pfc_disabled;
	}

	/* Initialize default user to priority mapping, UPx->TC0 */
	tc = &adapter->dcb_cfg.tc_config[0];
	tc->path[DCB_TX_CONFIG].up_to_tc_bitmap = 0xFF;
	tc->path[DCB_RX_CONFIG].up_to_tc_bitmap = 0xFF;

	adapter->dcb_cfg.bw_percentage[DCB_TX_CONFIG][0] = 100;
	adapter->dcb_cfg.bw_percentage[DCB_RX_CONFIG][0] = 100;
	adapter->dcb_cfg.pfc_mode_enable = false;
	adapter->dcb_set_bitmap = 0x00;
<<<<<<< HEAD
	adapter->dcbx_cap = DCB_CAP_DCBX_HOST | DCB_CAP_DCBX_VER_CEE;
	memcpy(&adapter->temp_dcb_cfg, &adapter->dcb_cfg,
	       sizeof(adapter->temp_dcb_cfg));

#endif
=======
	if (adapter->flags & IXGBE_FLAG_DCB_CAPABLE)
		adapter->dcbx_cap = DCB_CAP_DCBX_HOST | DCB_CAP_DCBX_VER_CEE;
	memcpy(&adapter->temp_dcb_cfg, &adapter->dcb_cfg,
	       sizeof(adapter->temp_dcb_cfg));
}
#endif

/**
 * ixgbe_sw_init - Initialize general software structures (struct ixgbe_adapter)
 * @adapter: board private structure to initialize
 * @ii: pointer to ixgbe_info for device
 *
 * ixgbe_sw_init initializes the Adapter private data structure.
 * Fields are initialized based on PCI device information and
 * OS network device settings (MTU size).
 **/
static int ixgbe_sw_init(struct ixgbe_adapter *adapter,
			 const struct ixgbe_info *ii)
{
	struct ixgbe_hw *hw = &adapter->hw;
	struct pci_dev *pdev = adapter->pdev;
	unsigned int rss, fdir;
	u32 fwsm;
	int i;

	/* PCI config space info */

	hw->vendor_id = pdev->vendor;
	hw->device_id = pdev->device;
	hw->revision_id = pdev->revision;
	hw->subsystem_vendor_id = pdev->subsystem_vendor;
	hw->subsystem_device_id = pdev->subsystem_device;

	/* get_invariants needs the device IDs */
	ii->get_invariants(hw);

	/* Set common capability flags and settings */
	rss = min_t(int, ixgbe_max_rss_indices(adapter), num_online_cpus());
	adapter->ring_feature[RING_F_RSS].limit = rss;
	adapter->flags2 |= IXGBE_FLAG2_RSC_CAPABLE;
	adapter->max_q_vectors = MAX_Q_VECTORS_82599;
	adapter->atr_sample_rate = 20;
	fdir = min_t(int, IXGBE_MAX_FDIR_INDICES, num_online_cpus());
	adapter->ring_feature[RING_F_FDIR].limit = fdir;
	adapter->fdir_pballoc = IXGBE_FDIR_PBALLOC_64K;
	adapter->ring_feature[RING_F_VMDQ].limit = 1;
#ifdef CONFIG_IXGBE_DCA
	adapter->flags |= IXGBE_FLAG_DCA_CAPABLE;
#endif
#ifdef CONFIG_IXGBE_DCB
	adapter->flags |= IXGBE_FLAG_DCB_CAPABLE;
	adapter->flags &= ~IXGBE_FLAG_DCB_ENABLED;
#endif
#ifdef IXGBE_FCOE
	adapter->flags |= IXGBE_FLAG_FCOE_CAPABLE;
	adapter->flags &= ~IXGBE_FLAG_FCOE_ENABLED;
#ifdef CONFIG_IXGBE_DCB
	/* Default traffic class to use for FCoE */
	adapter->fcoe.up = IXGBE_FCOE_DEFTC;
#endif /* CONFIG_IXGBE_DCB */
#endif /* IXGBE_FCOE */

	/* initialize static ixgbe jump table entries */
	adapter->jump_tables[0] = kzalloc(sizeof(*adapter->jump_tables[0]),
					  GFP_KERNEL);
	if (!adapter->jump_tables[0])
		return -ENOMEM;
	adapter->jump_tables[0]->mat = ixgbe_ipv4_fields;

	for (i = 1; i < IXGBE_MAX_LINK_HANDLE; i++)
		adapter->jump_tables[i] = NULL;

	adapter->mac_table = kcalloc(hw->mac.num_rar_entries,
				     sizeof(struct ixgbe_mac_addr),
				     GFP_KERNEL);
	if (!adapter->mac_table)
		return -ENOMEM;

	if (ixgbe_init_rss_key(adapter))
		return -ENOMEM;

	adapter->af_xdp_zc_qps = bitmap_zalloc(IXGBE_MAX_XDP_QS, GFP_KERNEL);
	if (!adapter->af_xdp_zc_qps)
		return -ENOMEM;

	/* Set MAC specific capability flags and exceptions */
	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		adapter->flags2 &= ~IXGBE_FLAG2_RSC_CAPABLE;

		if (hw->device_id == IXGBE_DEV_ID_82598AT)
			adapter->flags |= IXGBE_FLAG_FAN_FAIL_CAPABLE;

		adapter->max_q_vectors = MAX_Q_VECTORS_82598;
		adapter->ring_feature[RING_F_FDIR].limit = 0;
		adapter->atr_sample_rate = 0;
		adapter->fdir_pballoc = 0;
#ifdef IXGBE_FCOE
		adapter->flags &= ~IXGBE_FLAG_FCOE_CAPABLE;
		adapter->flags &= ~IXGBE_FLAG_FCOE_ENABLED;
#ifdef CONFIG_IXGBE_DCB
		adapter->fcoe.up = 0;
#endif /* IXGBE_DCB */
#endif /* IXGBE_FCOE */
		break;
	case ixgbe_mac_82599EB:
		if (hw->device_id == IXGBE_DEV_ID_82599_T3_LOM)
			adapter->flags2 |= IXGBE_FLAG2_TEMP_SENSOR_CAPABLE;
		break;
	case ixgbe_mac_X540:
		fwsm = IXGBE_READ_REG(hw, IXGBE_FWSM(hw));
		if (fwsm & IXGBE_FWSM_TS_ENABLED)
			adapter->flags2 |= IXGBE_FLAG2_TEMP_SENSOR_CAPABLE;
		break;
	case ixgbe_mac_x550em_a:
		switch (hw->device_id) {
		case IXGBE_DEV_ID_X550EM_A_1G_T:
		case IXGBE_DEV_ID_X550EM_A_1G_T_L:
			adapter->flags2 |= IXGBE_FLAG2_TEMP_SENSOR_CAPABLE;
			break;
		default:
			break;
		}
		fallthrough;
	case ixgbe_mac_X550EM_x:
#ifdef CONFIG_IXGBE_DCB
		adapter->flags &= ~IXGBE_FLAG_DCB_CAPABLE;
#endif
#ifdef IXGBE_FCOE
		adapter->flags &= ~IXGBE_FLAG_FCOE_CAPABLE;
#ifdef CONFIG_IXGBE_DCB
		adapter->fcoe.up = 0;
#endif /* IXGBE_DCB */
#endif /* IXGBE_FCOE */
		fallthrough;
	case ixgbe_mac_X550:
		if (hw->mac.type == ixgbe_mac_X550)
			adapter->flags2 |= IXGBE_FLAG2_TEMP_SENSOR_CAPABLE;
#ifdef CONFIG_IXGBE_DCA
		adapter->flags &= ~IXGBE_FLAG_DCA_CAPABLE;
#endif
		break;
	default:
		break;
	}

#ifdef IXGBE_FCOE
	/* FCoE support exists, always init the FCoE lock */
	spin_lock_init(&adapter->fcoe.lock);

#endif
	/* n-tuple support exists, always init our spinlock */
	spin_lock_init(&adapter->fdir_perfect_lock);

	/* init spinlock to avoid concurrency of VF resources */
	spin_lock_init(&adapter->vfs_lock);

#ifdef CONFIG_IXGBE_DCB
	ixgbe_init_dcb(adapter);
#endif
	ixgbe_init_ipsec_offload(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* default flow control settings */
	hw->fc.requested_mode = ixgbe_fc_full;
	hw->fc.current_mode = ixgbe_fc_full;	/* init for ethtool output */
<<<<<<< HEAD
#ifdef CONFIG_DCB
	adapter->last_lfc_mode = hw->fc.current_mode;
#endif
	ixgbe_pbthresh_setup(adapter);
	hw->fc.pause_time = IXGBE_DEFAULT_FCPAUSE;
	hw->fc.send_xon = true;
	hw->fc.disable_fc_autoneg = false;
=======
	ixgbe_pbthresh_setup(adapter);
	hw->fc.pause_time = IXGBE_DEFAULT_FCPAUSE;
	hw->fc.send_xon = true;
	hw->fc.disable_fc_autoneg = ixgbe_device_supports_autoneg_fc(hw);

#ifdef CONFIG_PCI_IOV
	if (max_vfs > 0)
		e_dev_warn("Enabling SR-IOV VFs using the max_vfs module parameter is deprecated - please use the pci sysfs interface instead.\n");

	/* assign number of SR-IOV VFs */
	if (hw->mac.type != ixgbe_mac_82598EB) {
		if (max_vfs > IXGBE_MAX_VFS_DRV_LIMIT) {
			max_vfs = 0;
			e_dev_warn("max_vfs parameter out of range. Not assigning any SR-IOV VFs\n");
		}
	}
#endif /* CONFIG_PCI_IOV */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* enable itr by default in dynamic mode */
	adapter->rx_itr_setting = 1;
	adapter->tx_itr_setting = 1;

	/* set default ring sizes */
	adapter->tx_ring_count = IXGBE_DEFAULT_TXD;
	adapter->rx_ring_count = IXGBE_DEFAULT_RXD;

	/* set default work limits */
	adapter->tx_work_limit = IXGBE_DEFAULT_TX_WORK;

	/* initialize eeprom parameters */
	if (ixgbe_init_eeprom_params_generic(hw)) {
		e_dev_err("EEPROM initialization failed\n");
		return -EIO;
	}

<<<<<<< HEAD
	set_bit(__IXGBE_DOWN, &adapter->state);

=======
	/* PF holds first pool slot */
	set_bit(0, adapter->fwd_bitmask);
	set_bit(__IXGBE_DOWN, &adapter->state);

	/* enable locking for XDP_TX if we have more CPUs than queues */
	if (nr_cpu_ids > IXGBE_MAX_XDP_QS)
		static_branch_enable(&ixgbe_xdp_locking_key);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/**
 * ixgbe_setup_tx_resources - allocate Tx resources (Descriptors)
 * @tx_ring:    tx descriptor ring (for a specific queue) to setup
 *
 * Return 0 on success, negative on failure
 **/
int ixgbe_setup_tx_resources(struct ixgbe_ring *tx_ring)
{
	struct device *dev = tx_ring->dev;
	int orig_node = dev_to_node(dev);
<<<<<<< HEAD
	int numa_node = -1;
=======
	int ring_node = NUMA_NO_NODE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int size;

	size = sizeof(struct ixgbe_tx_buffer) * tx_ring->count;

	if (tx_ring->q_vector)
<<<<<<< HEAD
		numa_node = tx_ring->q_vector->numa_node;

	tx_ring->tx_buffer_info = vzalloc_node(size, numa_node);
	if (!tx_ring->tx_buffer_info)
		tx_ring->tx_buffer_info = vzalloc(size);
	if (!tx_ring->tx_buffer_info)
		goto err;

	u64_stats_init(&tx_ring->syncp);

=======
		ring_node = tx_ring->q_vector->numa_node;

	tx_ring->tx_buffer_info = vmalloc_node(size, ring_node);
	if (!tx_ring->tx_buffer_info)
		tx_ring->tx_buffer_info = vmalloc(size);
	if (!tx_ring->tx_buffer_info)
		goto err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* round up to nearest 4K */
	tx_ring->size = tx_ring->count * sizeof(union ixgbe_adv_tx_desc);
	tx_ring->size = ALIGN(tx_ring->size, 4096);

<<<<<<< HEAD
	set_dev_node(dev, numa_node);
=======
	set_dev_node(dev, ring_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tx_ring->desc = dma_alloc_coherent(dev,
					   tx_ring->size,
					   &tx_ring->dma,
					   GFP_KERNEL);
	set_dev_node(dev, orig_node);
	if (!tx_ring->desc)
		tx_ring->desc = dma_alloc_coherent(dev, tx_ring->size,
						   &tx_ring->dma, GFP_KERNEL);
	if (!tx_ring->desc)
		goto err;

	tx_ring->next_to_use = 0;
	tx_ring->next_to_clean = 0;
	return 0;

err:
	vfree(tx_ring->tx_buffer_info);
	tx_ring->tx_buffer_info = NULL;
	dev_err(dev, "Unable to allocate memory for the Tx descriptor ring\n");
	return -ENOMEM;
}

/**
 * ixgbe_setup_all_tx_resources - allocate all queues Tx resources
 * @adapter: board private structure
 *
 * If this function returns with an error, then it's possible one or
 * more of the rings is populated (while the rest are not).  It is the
 * callers duty to clean those orphaned rings.
 *
 * Return 0 on success, negative on failure
 **/
static int ixgbe_setup_all_tx_resources(struct ixgbe_adapter *adapter)
{
<<<<<<< HEAD
	int i, err = 0;
=======
	int i, j = 0, err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < adapter->num_tx_queues; i++) {
		err = ixgbe_setup_tx_resources(adapter->tx_ring[i]);
		if (!err)
			continue;
<<<<<<< HEAD
		e_err(probe, "Allocation for Tx Queue %u failed\n", i);
		break;
	}

	return err;
}

/**
 * ixgbe_setup_rx_resources - allocate Rx resources (Descriptors)
=======

		e_err(probe, "Allocation for Tx Queue %u failed\n", i);
		goto err_setup_tx;
	}
	for (j = 0; j < adapter->num_xdp_queues; j++) {
		err = ixgbe_setup_tx_resources(adapter->xdp_ring[j]);
		if (!err)
			continue;

		e_err(probe, "Allocation for Tx Queue %u failed\n", j);
		goto err_setup_tx;
	}

	return 0;
err_setup_tx:
	/* rewind the index freeing the rings as we go */
	while (j--)
		ixgbe_free_tx_resources(adapter->xdp_ring[j]);
	while (i--)
		ixgbe_free_tx_resources(adapter->tx_ring[i]);
	return err;
}

static int ixgbe_rx_napi_id(struct ixgbe_ring *rx_ring)
{
	struct ixgbe_q_vector *q_vector = rx_ring->q_vector;

	return q_vector ? q_vector->napi.napi_id : 0;
}

/**
 * ixgbe_setup_rx_resources - allocate Rx resources (Descriptors)
 * @adapter: pointer to ixgbe_adapter
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @rx_ring:    rx descriptor ring (for a specific queue) to setup
 *
 * Returns 0 on success, negative on failure
 **/
<<<<<<< HEAD
int ixgbe_setup_rx_resources(struct ixgbe_ring *rx_ring)
{
	struct device *dev = rx_ring->dev;
	int orig_node = dev_to_node(dev);
	int numa_node = -1;
=======
int ixgbe_setup_rx_resources(struct ixgbe_adapter *adapter,
			     struct ixgbe_ring *rx_ring)
{
	struct device *dev = rx_ring->dev;
	int orig_node = dev_to_node(dev);
	int ring_node = NUMA_NO_NODE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int size;

	size = sizeof(struct ixgbe_rx_buffer) * rx_ring->count;

	if (rx_ring->q_vector)
<<<<<<< HEAD
		numa_node = rx_ring->q_vector->numa_node;

	rx_ring->rx_buffer_info = vzalloc_node(size, numa_node);
	if (!rx_ring->rx_buffer_info)
		rx_ring->rx_buffer_info = vzalloc(size);
	if (!rx_ring->rx_buffer_info)
		goto err;

	u64_stats_init(&rx_ring->syncp);

=======
		ring_node = rx_ring->q_vector->numa_node;

	rx_ring->rx_buffer_info = vmalloc_node(size, ring_node);
	if (!rx_ring->rx_buffer_info)
		rx_ring->rx_buffer_info = vmalloc(size);
	if (!rx_ring->rx_buffer_info)
		goto err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Round up to nearest 4K */
	rx_ring->size = rx_ring->count * sizeof(union ixgbe_adv_rx_desc);
	rx_ring->size = ALIGN(rx_ring->size, 4096);

<<<<<<< HEAD
	set_dev_node(dev, numa_node);
=======
	set_dev_node(dev, ring_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rx_ring->desc = dma_alloc_coherent(dev,
					   rx_ring->size,
					   &rx_ring->dma,
					   GFP_KERNEL);
	set_dev_node(dev, orig_node);
	if (!rx_ring->desc)
		rx_ring->desc = dma_alloc_coherent(dev, rx_ring->size,
						   &rx_ring->dma, GFP_KERNEL);
	if (!rx_ring->desc)
		goto err;

	rx_ring->next_to_clean = 0;
	rx_ring->next_to_use = 0;

<<<<<<< HEAD
	ixgbe_init_rx_page_offset(rx_ring);
=======
	/* XDP RX-queue info */
	if (xdp_rxq_info_reg(&rx_ring->xdp_rxq, adapter->netdev,
			     rx_ring->queue_index, ixgbe_rx_napi_id(rx_ring)) < 0)
		goto err;

	WRITE_ONCE(rx_ring->xdp_prog, adapter->xdp_prog);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
err:
	vfree(rx_ring->rx_buffer_info);
	rx_ring->rx_buffer_info = NULL;
	dev_err(dev, "Unable to allocate memory for the Rx descriptor ring\n");
	return -ENOMEM;
}

/**
 * ixgbe_setup_all_rx_resources - allocate all queues Rx resources
 * @adapter: board private structure
 *
 * If this function returns with an error, then it's possible one or
 * more of the rings is populated (while the rest are not).  It is the
 * callers duty to clean those orphaned rings.
 *
 * Return 0 on success, negative on failure
 **/
static int ixgbe_setup_all_rx_resources(struct ixgbe_adapter *adapter)
{
	int i, err = 0;

	for (i = 0; i < adapter->num_rx_queues; i++) {
<<<<<<< HEAD
		err = ixgbe_setup_rx_resources(adapter->rx_ring[i]);
		if (!err)
			continue;
		e_err(probe, "Allocation for Rx Queue %u failed\n", i);
		break;
	}

=======
		err = ixgbe_setup_rx_resources(adapter, adapter->rx_ring[i]);
		if (!err)
			continue;

		e_err(probe, "Allocation for Rx Queue %u failed\n", i);
		goto err_setup_rx;
	}

#ifdef IXGBE_FCOE
	err = ixgbe_setup_fcoe_ddp_resources(adapter);
	if (!err)
#endif
		return 0;
err_setup_rx:
	/* rewind the index freeing the rings as we go */
	while (i--)
		ixgbe_free_rx_resources(adapter->rx_ring[i]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/**
 * ixgbe_free_tx_resources - Free Tx Resources per Queue
 * @tx_ring: Tx descriptor ring for a specific queue
 *
 * Free all transmit software resources
 **/
void ixgbe_free_tx_resources(struct ixgbe_ring *tx_ring)
{
	ixgbe_clean_tx_ring(tx_ring);

	vfree(tx_ring->tx_buffer_info);
	tx_ring->tx_buffer_info = NULL;

	/* if not set, then don't free */
	if (!tx_ring->desc)
		return;

	dma_free_coherent(tx_ring->dev, tx_ring->size,
			  tx_ring->desc, tx_ring->dma);

	tx_ring->desc = NULL;
}

/**
 * ixgbe_free_all_tx_resources - Free Tx Resources for All Queues
 * @adapter: board private structure
 *
 * Free all transmit software resources
 **/
static void ixgbe_free_all_tx_resources(struct ixgbe_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_tx_queues; i++)
		if (adapter->tx_ring[i]->desc)
			ixgbe_free_tx_resources(adapter->tx_ring[i]);
<<<<<<< HEAD
=======
	for (i = 0; i < adapter->num_xdp_queues; i++)
		if (adapter->xdp_ring[i]->desc)
			ixgbe_free_tx_resources(adapter->xdp_ring[i]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_free_rx_resources - Free Rx Resources
 * @rx_ring: ring to clean the resources from
 *
 * Free all receive software resources
 **/
void ixgbe_free_rx_resources(struct ixgbe_ring *rx_ring)
{
	ixgbe_clean_rx_ring(rx_ring);

<<<<<<< HEAD
=======
	rx_ring->xdp_prog = NULL;
	xdp_rxq_info_unreg(&rx_ring->xdp_rxq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vfree(rx_ring->rx_buffer_info);
	rx_ring->rx_buffer_info = NULL;

	/* if not set, then don't free */
	if (!rx_ring->desc)
		return;

	dma_free_coherent(rx_ring->dev, rx_ring->size,
			  rx_ring->desc, rx_ring->dma);

	rx_ring->desc = NULL;
}

/**
 * ixgbe_free_all_rx_resources - Free Rx Resources for All Queues
 * @adapter: board private structure
 *
 * Free all receive software resources
 **/
static void ixgbe_free_all_rx_resources(struct ixgbe_adapter *adapter)
{
	int i;

<<<<<<< HEAD
=======
#ifdef IXGBE_FCOE
	ixgbe_free_fcoe_ddp_resources(adapter);

#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < adapter->num_rx_queues; i++)
		if (adapter->rx_ring[i]->desc)
			ixgbe_free_rx_resources(adapter->rx_ring[i]);
}

/**
<<<<<<< HEAD
=======
 * ixgbe_max_xdp_frame_size - returns the maximum allowed frame size for XDP
 * @adapter: device handle, pointer to adapter
 */
static int ixgbe_max_xdp_frame_size(struct ixgbe_adapter *adapter)
{
	if (PAGE_SIZE >= 8192 || adapter->flags2 & IXGBE_FLAG2_RX_LEGACY)
		return IXGBE_RXBUFFER_2K;
	else
		return IXGBE_RXBUFFER_3K;
}

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ixgbe_change_mtu - Change the Maximum Transfer Unit
 * @netdev: network interface device structure
 * @new_mtu: new value for maximum frame size
 *
 * Returns 0 on success, negative on failure
 **/
static int ixgbe_change_mtu(struct net_device *netdev, int new_mtu)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
<<<<<<< HEAD
	int max_frame = new_mtu + ETH_HLEN + ETH_FCS_LEN;

	/* MTU < 68 is an error and causes problems on some kernels */
	if ((new_mtu < 68) || (max_frame > IXGBE_MAX_JUMBO_FRAME_SIZE))
		return -EINVAL;

	/*
	 * For 82599EB we cannot allow PF to change MTU greater than 1500
	 * in SR-IOV mode as it may cause buffer overruns in guest VFs that
	 * don't allocate and chain buffers correctly.
	 */
	if ((adapter->flags & IXGBE_FLAG_SRIOV_ENABLED) &&
	    (adapter->hw.mac.type == ixgbe_mac_82599EB) &&
	    (max_frame > MAXIMUM_ETHERNET_VLAN_SIZE))
			return -EINVAL;

	e_info(probe, "changing MTU from %d to %d\n", netdev->mtu, new_mtu);
=======

	if (ixgbe_enabled_xdp_adapter(adapter)) {
		int new_frame_size = new_mtu + IXGBE_PKT_HDR_PAD;

		if (new_frame_size > ixgbe_max_xdp_frame_size(adapter)) {
			e_warn(probe, "Requested MTU size is not supported with XDP\n");
			return -EINVAL;
		}
	}

	/*
	 * For 82599EB we cannot allow legacy VFs to enable their receive
	 * paths when MTU greater than 1500 is configured.  So display a
	 * warning that legacy VFs will be disabled.
	 */
	if ((adapter->flags & IXGBE_FLAG_SRIOV_ENABLED) &&
	    (adapter->hw.mac.type == ixgbe_mac_82599EB) &&
	    (new_mtu > ETH_DATA_LEN))
		e_warn(probe, "Setting MTU > 1500 will disable legacy VFs\n");

	netdev_dbg(netdev, "changing MTU from %d to %d\n",
		   netdev->mtu, new_mtu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* must set new MTU before calling down or up */
	netdev->mtu = new_mtu;

	if (netif_running(netdev))
		ixgbe_reinit_locked(adapter);

	return 0;
}

/**
 * ixgbe_open - Called when a network interface is made active
 * @netdev: network interface device structure
 *
 * Returns 0 on success, negative value on failure
 *
 * The open entry point is called when a network interface is made
 * active by the system (IFF_UP).  At this point all resources needed
 * for transmit and receive operations are allocated, the interrupt
 * handler is registered with the OS, the watchdog timer is started,
 * and the stack is notified that the interface is ready.
 **/
<<<<<<< HEAD
static int ixgbe_open(struct net_device *netdev)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	int err;
=======
int ixgbe_open(struct net_device *netdev)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	struct ixgbe_hw *hw = &adapter->hw;
	int err, queues;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* disallow open during test */
	if (test_bit(__IXGBE_TESTING, &adapter->state))
		return -EBUSY;

	netif_carrier_off(netdev);

	/* allocate transmit descriptors */
	err = ixgbe_setup_all_tx_resources(adapter);
	if (err)
		goto err_setup_tx;

	/* allocate receive descriptors */
	err = ixgbe_setup_all_rx_resources(adapter);
	if (err)
		goto err_setup_rx;

	ixgbe_configure(adapter);

	err = ixgbe_request_irq(adapter);
	if (err)
		goto err_req_irq;

<<<<<<< HEAD
	ixgbe_up_complete(adapter);

	return 0;

err_req_irq:
err_setup_rx:
	ixgbe_free_all_rx_resources(adapter);
err_setup_tx:
	ixgbe_free_all_tx_resources(adapter);
=======
	/* Notify the stack of the actual queue counts. */
	queues = adapter->num_tx_queues;
	err = netif_set_real_num_tx_queues(netdev, queues);
	if (err)
		goto err_set_queues;

	queues = adapter->num_rx_queues;
	err = netif_set_real_num_rx_queues(netdev, queues);
	if (err)
		goto err_set_queues;

	ixgbe_ptp_init(adapter);

	ixgbe_up_complete(adapter);

	udp_tunnel_nic_reset_ntf(netdev);

	return 0;

err_set_queues:
	ixgbe_free_irq(adapter);
err_req_irq:
	ixgbe_free_all_rx_resources(adapter);
	if (hw->phy.ops.set_phy_power && !adapter->wol)
		hw->phy.ops.set_phy_power(&adapter->hw, false);
err_setup_rx:
	ixgbe_free_all_tx_resources(adapter);
err_setup_tx:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ixgbe_reset(adapter);

	return err;
}

<<<<<<< HEAD
=======
static void ixgbe_close_suspend(struct ixgbe_adapter *adapter)
{
	ixgbe_ptp_suspend(adapter);

	if (adapter->hw.phy.ops.enter_lplu) {
		adapter->hw.phy.reset_disable = true;
		ixgbe_down(adapter);
		adapter->hw.phy.ops.enter_lplu(&adapter->hw);
		adapter->hw.phy.reset_disable = false;
	} else {
		ixgbe_down(adapter);
	}

	ixgbe_free_irq(adapter);

	ixgbe_free_all_tx_resources(adapter);
	ixgbe_free_all_rx_resources(adapter);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * ixgbe_close - Disables a network interface
 * @netdev: network interface device structure
 *
 * Returns 0, this is not allowed to fail
 *
 * The close entry point is called when an interface is de-activated
 * by the OS.  The hardware is still under the drivers control, but
 * needs to be disabled.  A global MAC reset is issued to stop the
 * hardware, and all transmit and receive resources are freed.
 **/
<<<<<<< HEAD
static int ixgbe_close(struct net_device *netdev)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);

	ixgbe_down(adapter);
	ixgbe_free_irq(adapter);

	ixgbe_fdir_filter_exit(adapter);

	ixgbe_free_all_tx_resources(adapter);
	ixgbe_free_all_rx_resources(adapter);

=======
int ixgbe_close(struct net_device *netdev)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);

	ixgbe_ptp_stop(adapter);

	if (netif_device_present(netdev))
		ixgbe_close_suspend(adapter);

	ixgbe_fdir_filter_exit(adapter);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ixgbe_release_hw_control(adapter);

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM
static int ixgbe_resume(struct pci_dev *pdev)
{
=======
static int __maybe_unused ixgbe_resume(struct device *dev_d)
{
	struct pci_dev *pdev = to_pci_dev(dev_d);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ixgbe_adapter *adapter = pci_get_drvdata(pdev);
	struct net_device *netdev = adapter->netdev;
	u32 err;

<<<<<<< HEAD
	pci_set_power_state(pdev, PCI_D0);
	pci_restore_state(pdev);
	/*
	 * pci_restore_state clears dev->state_saved so call
	 * pci_save_state to restore it.
	 */
	pci_save_state(pdev);
=======
	adapter->hw.hw_addr = adapter->io_addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = pci_enable_device_mem(pdev);
	if (err) {
		e_dev_err("Cannot enable PCI device from suspend\n");
		return err;
	}
<<<<<<< HEAD
	pci_set_master(pdev);

	pci_wake_from_d3(pdev, false);

	rtnl_lock();
	err = ixgbe_init_interrupt_scheme(adapter);
	rtnl_unlock();
	if (err) {
		e_dev_err("Cannot initialize interrupts for device\n");
		return err;
	}
=======
	smp_mb__before_atomic();
	clear_bit(__IXGBE_DISABLED, &adapter->state);
	pci_set_master(pdev);

	device_wakeup_disable(dev_d);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ixgbe_reset(adapter);

	IXGBE_WRITE_REG(&adapter->hw, IXGBE_WUS, ~0);

<<<<<<< HEAD
	if (netif_running(netdev)) {
		err = ixgbe_open(netdev);
		if (err)
			return err;
	}

	netif_device_attach(netdev);

	return 0;
}
#endif /* CONFIG_PM */
=======
	rtnl_lock();
	err = ixgbe_init_interrupt_scheme(adapter);
	if (!err && netif_running(netdev))
		err = ixgbe_open(netdev);


	if (!err)
		netif_device_attach(netdev);
	rtnl_unlock();

	return err;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __ixgbe_shutdown(struct pci_dev *pdev, bool *enable_wake)
{
	struct ixgbe_adapter *adapter = pci_get_drvdata(pdev);
	struct net_device *netdev = adapter->netdev;
	struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD
	u32 ctrl, fctrl;
	u32 wufc = adapter->wol;
#ifdef CONFIG_PM
	int retval = 0;
#endif

	netif_device_detach(netdev);

	if (netif_running(netdev)) {
		rtnl_lock();
		ixgbe_down(adapter);
		ixgbe_free_irq(adapter);
		ixgbe_free_all_tx_resources(adapter);
		ixgbe_free_all_rx_resources(adapter);
		rtnl_unlock();
	}

	ixgbe_clear_interrupt_scheme(adapter);

#ifdef CONFIG_PM
	retval = pci_save_state(pdev);
	if (retval)
		return retval;

#endif
	if (wufc) {
		ixgbe_set_rx_mode(netdev);

		/*
		 * enable the optics for both mult-speed fiber and
		 * 82599 SFP+ fiber as we can WoL.
		 */
		if (hw->mac.ops.enable_tx_laser &&
		    (hw->phy.multispeed_fiber ||
		    (hw->mac.ops.get_media_type(hw) == ixgbe_media_type_fiber &&
		     hw->mac.type == ixgbe_mac_82599EB)))
			hw->mac.ops.enable_tx_laser(hw);

		/* turn on all-multi mode if wake on multicast is enabled */
		if (wufc & IXGBE_WUFC_MC) {
			fctrl = IXGBE_READ_REG(hw, IXGBE_FCTRL);
			fctrl |= IXGBE_FCTRL_MPE;
			IXGBE_WRITE_REG(hw, IXGBE_FCTRL, fctrl);
		}
=======
	u32 ctrl;
	u32 wufc = adapter->wol;

	rtnl_lock();
	netif_device_detach(netdev);

	if (netif_running(netdev))
		ixgbe_close_suspend(adapter);

	ixgbe_clear_interrupt_scheme(adapter);
	rtnl_unlock();

	if (hw->mac.ops.stop_link_on_d3)
		hw->mac.ops.stop_link_on_d3(hw);

	if (wufc) {
		u32 fctrl;

		ixgbe_set_rx_mode(netdev);

		/* enable the optics for 82599 SFP+ fiber as we can WoL */
		if (hw->mac.ops.enable_tx_laser)
			hw->mac.ops.enable_tx_laser(hw);

		/* enable the reception of multicast packets */
		fctrl = IXGBE_READ_REG(hw, IXGBE_FCTRL);
		fctrl |= IXGBE_FCTRL_MPE;
		IXGBE_WRITE_REG(hw, IXGBE_FCTRL, fctrl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		ctrl = IXGBE_READ_REG(hw, IXGBE_CTRL);
		ctrl |= IXGBE_CTRL_GIO_DIS;
		IXGBE_WRITE_REG(hw, IXGBE_CTRL, ctrl);

		IXGBE_WRITE_REG(hw, IXGBE_WUFC, wufc);
	} else {
		IXGBE_WRITE_REG(hw, IXGBE_WUC, 0);
		IXGBE_WRITE_REG(hw, IXGBE_WUFC, 0);
	}

	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		pci_wake_from_d3(pdev, false);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
<<<<<<< HEAD
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pci_wake_from_d3(pdev, !!wufc);
		break;
	default:
		break;
	}

	*enable_wake = !!wufc;
<<<<<<< HEAD

	ixgbe_release_hw_control(adapter);

	pci_disable_device(pdev);
=======
	if (hw->phy.ops.set_phy_power && !*enable_wake)
		hw->phy.ops.set_phy_power(hw, false);

	ixgbe_release_hw_control(adapter);

	if (!test_and_set_bit(__IXGBE_DISABLED, &adapter->state))
		pci_disable_device(pdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM
static int ixgbe_suspend(struct pci_dev *pdev, pm_message_t state)
{
=======
static int __maybe_unused ixgbe_suspend(struct device *dev_d)
{
	struct pci_dev *pdev = to_pci_dev(dev_d);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int retval;
	bool wake;

	retval = __ixgbe_shutdown(pdev, &wake);
<<<<<<< HEAD
	if (retval)
		return retval;

	if (wake) {
		pci_prepare_to_sleep(pdev);
	} else {
		pci_wake_from_d3(pdev, false);
		pci_set_power_state(pdev, PCI_D3hot);
	}

	return 0;
}
#endif /* CONFIG_PM */
=======

	device_set_wakeup_enable(dev_d, wake);

	return retval;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void ixgbe_shutdown(struct pci_dev *pdev)
{
	bool wake;

	__ixgbe_shutdown(pdev, &wake);

	if (system_state == SYSTEM_POWER_OFF) {
		pci_wake_from_d3(pdev, wake);
		pci_set_power_state(pdev, PCI_D3hot);
	}
}

/**
 * ixgbe_update_stats - Update the board statistics counters.
 * @adapter: board private structure
 **/
void ixgbe_update_stats(struct ixgbe_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct ixgbe_hw *hw = &adapter->hw;
	struct ixgbe_hw_stats *hwstats = &adapter->stats;
	u64 total_mpc = 0;
	u32 i, missed_rx = 0, mpc, bprc, lxon, lxoff, xon_off_tot;
	u64 non_eop_descs = 0, restart_queue = 0, tx_busy = 0;
	u64 alloc_rx_page_failed = 0, alloc_rx_buff_failed = 0;
<<<<<<< HEAD
	u64 bytes = 0, packets = 0, hw_csum_rx_error = 0;
#ifdef IXGBE_FCOE
	struct ixgbe_fcoe *fcoe = &adapter->fcoe;
	unsigned int cpu;
	u64 fcoe_noddp_counts_sum = 0, fcoe_noddp_ext_buff_counts_sum = 0;
#endif /* IXGBE_FCOE */
=======
	u64 alloc_rx_page = 0;
	u64 bytes = 0, packets = 0, hw_csum_rx_error = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (test_bit(__IXGBE_DOWN, &adapter->state) ||
	    test_bit(__IXGBE_RESETTING, &adapter->state))
		return;

	if (adapter->flags2 & IXGBE_FLAG2_RSC_ENABLED) {
		u64 rsc_count = 0;
		u64 rsc_flush = 0;
<<<<<<< HEAD
		for (i = 0; i < 16; i++)
			adapter->hw_rx_no_dma_resources +=
				IXGBE_READ_REG(hw, IXGBE_QPRDC(i));
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		for (i = 0; i < adapter->num_rx_queues; i++) {
			rsc_count += adapter->rx_ring[i]->rx_stats.rsc_count;
			rsc_flush += adapter->rx_ring[i]->rx_stats.rsc_flush;
		}
		adapter->rsc_total_count = rsc_count;
		adapter->rsc_total_flush = rsc_flush;
	}

	for (i = 0; i < adapter->num_rx_queues; i++) {
<<<<<<< HEAD
		struct ixgbe_ring *rx_ring = adapter->rx_ring[i];
		non_eop_descs += rx_ring->rx_stats.non_eop_descs;
=======
		struct ixgbe_ring *rx_ring = READ_ONCE(adapter->rx_ring[i]);

		if (!rx_ring)
			continue;
		non_eop_descs += rx_ring->rx_stats.non_eop_descs;
		alloc_rx_page += rx_ring->rx_stats.alloc_rx_page;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		alloc_rx_page_failed += rx_ring->rx_stats.alloc_rx_page_failed;
		alloc_rx_buff_failed += rx_ring->rx_stats.alloc_rx_buff_failed;
		hw_csum_rx_error += rx_ring->rx_stats.csum_err;
		bytes += rx_ring->stats.bytes;
		packets += rx_ring->stats.packets;
	}
	adapter->non_eop_descs = non_eop_descs;
<<<<<<< HEAD
=======
	adapter->alloc_rx_page = alloc_rx_page;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	adapter->alloc_rx_page_failed = alloc_rx_page_failed;
	adapter->alloc_rx_buff_failed = alloc_rx_buff_failed;
	adapter->hw_csum_rx_error = hw_csum_rx_error;
	netdev->stats.rx_bytes = bytes;
	netdev->stats.rx_packets = packets;

	bytes = 0;
	packets = 0;
	/* gather some stats to the adapter struct that are per queue */
	for (i = 0; i < adapter->num_tx_queues; i++) {
<<<<<<< HEAD
		struct ixgbe_ring *tx_ring = adapter->tx_ring[i];
=======
		struct ixgbe_ring *tx_ring = READ_ONCE(adapter->tx_ring[i]);

		if (!tx_ring)
			continue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		restart_queue += tx_ring->tx_stats.restart_queue;
		tx_busy += tx_ring->tx_stats.tx_busy;
		bytes += tx_ring->stats.bytes;
		packets += tx_ring->stats.packets;
	}
<<<<<<< HEAD
=======
	for (i = 0; i < adapter->num_xdp_queues; i++) {
		struct ixgbe_ring *xdp_ring = READ_ONCE(adapter->xdp_ring[i]);

		if (!xdp_ring)
			continue;
		restart_queue += xdp_ring->tx_stats.restart_queue;
		tx_busy += xdp_ring->tx_stats.tx_busy;
		bytes += xdp_ring->stats.bytes;
		packets += xdp_ring->stats.packets;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	adapter->restart_queue = restart_queue;
	adapter->tx_busy = tx_busy;
	netdev->stats.tx_bytes = bytes;
	netdev->stats.tx_packets = packets;

	hwstats->crcerrs += IXGBE_READ_REG(hw, IXGBE_CRCERRS);

	/* 8 register reads */
	for (i = 0; i < 8; i++) {
		/* for packet buffers not used, the register should read 0 */
		mpc = IXGBE_READ_REG(hw, IXGBE_MPC(i));
		missed_rx += mpc;
		hwstats->mpc[i] += mpc;
		total_mpc += hwstats->mpc[i];
		hwstats->pxontxc[i] += IXGBE_READ_REG(hw, IXGBE_PXONTXC(i));
		hwstats->pxofftxc[i] += IXGBE_READ_REG(hw, IXGBE_PXOFFTXC(i));
		switch (hw->mac.type) {
		case ixgbe_mac_82598EB:
			hwstats->rnbc[i] += IXGBE_READ_REG(hw, IXGBE_RNBC(i));
			hwstats->qbtc[i] += IXGBE_READ_REG(hw, IXGBE_QBTC(i));
			hwstats->qbrc[i] += IXGBE_READ_REG(hw, IXGBE_QBRC(i));
			hwstats->pxonrxc[i] +=
				IXGBE_READ_REG(hw, IXGBE_PXONRXC(i));
			break;
		case ixgbe_mac_82599EB:
		case ixgbe_mac_X540:
<<<<<<< HEAD
=======
		case ixgbe_mac_X550:
		case ixgbe_mac_X550EM_x:
		case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			hwstats->pxonrxc[i] +=
				IXGBE_READ_REG(hw, IXGBE_PXONRXCNT(i));
			break;
		default:
			break;
		}
	}

	/*16 register reads */
	for (i = 0; i < 16; i++) {
		hwstats->qptc[i] += IXGBE_READ_REG(hw, IXGBE_QPTC(i));
		hwstats->qprc[i] += IXGBE_READ_REG(hw, IXGBE_QPRC(i));
		if ((hw->mac.type == ixgbe_mac_82599EB) ||
<<<<<<< HEAD
		    (hw->mac.type == ixgbe_mac_X540)) {
=======
		    (hw->mac.type == ixgbe_mac_X540) ||
		    (hw->mac.type == ixgbe_mac_X550) ||
		    (hw->mac.type == ixgbe_mac_X550EM_x) ||
		    (hw->mac.type == ixgbe_mac_x550em_a)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			hwstats->qbtc[i] += IXGBE_READ_REG(hw, IXGBE_QBTC_L(i));
			IXGBE_READ_REG(hw, IXGBE_QBTC_H(i)); /* to clear */
			hwstats->qbrc[i] += IXGBE_READ_REG(hw, IXGBE_QBRC_L(i));
			IXGBE_READ_REG(hw, IXGBE_QBRC_H(i)); /* to clear */
		}
	}

	hwstats->gprc += IXGBE_READ_REG(hw, IXGBE_GPRC);
	/* work around hardware counting issue */
	hwstats->gprc -= missed_rx;

	ixgbe_update_xoff_received(adapter);

	/* 82598 hardware only has a 32 bit counter in the high register */
	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		hwstats->lxonrxc += IXGBE_READ_REG(hw, IXGBE_LXONRXC);
		hwstats->gorc += IXGBE_READ_REG(hw, IXGBE_GORCH);
		hwstats->gotc += IXGBE_READ_REG(hw, IXGBE_GOTCH);
		hwstats->tor += IXGBE_READ_REG(hw, IXGBE_TORH);
		break;
	case ixgbe_mac_X540:
<<<<<<< HEAD
		/* OS2BMC stats are X540 only*/
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
		/* OS2BMC stats are X540 and later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hwstats->o2bgptc += IXGBE_READ_REG(hw, IXGBE_O2BGPTC);
		hwstats->o2bspc += IXGBE_READ_REG(hw, IXGBE_O2BSPC);
		hwstats->b2ospc += IXGBE_READ_REG(hw, IXGBE_B2OSPC);
		hwstats->b2ogprc += IXGBE_READ_REG(hw, IXGBE_B2OGPRC);
<<<<<<< HEAD
	case ixgbe_mac_82599EB:
=======
		fallthrough;
	case ixgbe_mac_82599EB:
		for (i = 0; i < 16; i++)
			adapter->hw_rx_no_dma_resources +=
					     IXGBE_READ_REG(hw, IXGBE_QPRDC(i));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hwstats->gorc += IXGBE_READ_REG(hw, IXGBE_GORCL);
		IXGBE_READ_REG(hw, IXGBE_GORCH); /* to clear */
		hwstats->gotc += IXGBE_READ_REG(hw, IXGBE_GOTCL);
		IXGBE_READ_REG(hw, IXGBE_GOTCH); /* to clear */
		hwstats->tor += IXGBE_READ_REG(hw, IXGBE_TORL);
		IXGBE_READ_REG(hw, IXGBE_TORH); /* to clear */
		hwstats->lxonrxc += IXGBE_READ_REG(hw, IXGBE_LXONRXCNT);
		hwstats->fdirmatch += IXGBE_READ_REG(hw, IXGBE_FDIRMATCH);
		hwstats->fdirmiss += IXGBE_READ_REG(hw, IXGBE_FDIRMISS);
#ifdef IXGBE_FCOE
		hwstats->fccrc += IXGBE_READ_REG(hw, IXGBE_FCCRC);
		hwstats->fcoerpdc += IXGBE_READ_REG(hw, IXGBE_FCOERPDC);
		hwstats->fcoeprc += IXGBE_READ_REG(hw, IXGBE_FCOEPRC);
		hwstats->fcoeptc += IXGBE_READ_REG(hw, IXGBE_FCOEPTC);
		hwstats->fcoedwrc += IXGBE_READ_REG(hw, IXGBE_FCOEDWRC);
		hwstats->fcoedwtc += IXGBE_READ_REG(hw, IXGBE_FCOEDWTC);
		/* Add up per cpu counters for total ddp aloc fail */
<<<<<<< HEAD
		if (fcoe->pcpu_noddp && fcoe->pcpu_noddp_ext_buff) {
			for_each_possible_cpu(cpu) {
				fcoe_noddp_counts_sum +=
					*per_cpu_ptr(fcoe->pcpu_noddp, cpu);
				fcoe_noddp_ext_buff_counts_sum +=
					*per_cpu_ptr(fcoe->
						pcpu_noddp_ext_buff, cpu);
			}
		}
		hwstats->fcoe_noddp = fcoe_noddp_counts_sum;
		hwstats->fcoe_noddp_ext_buff = fcoe_noddp_ext_buff_counts_sum;
=======
		if (adapter->fcoe.ddp_pool) {
			struct ixgbe_fcoe *fcoe = &adapter->fcoe;
			struct ixgbe_fcoe_ddp_pool *ddp_pool;
			unsigned int cpu;
			u64 noddp = 0, noddp_ext_buff = 0;
			for_each_possible_cpu(cpu) {
				ddp_pool = per_cpu_ptr(fcoe->ddp_pool, cpu);
				noddp += ddp_pool->noddp;
				noddp_ext_buff += ddp_pool->noddp_ext_buff;
			}
			hwstats->fcoe_noddp = noddp;
			hwstats->fcoe_noddp_ext_buff = noddp_ext_buff;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* IXGBE_FCOE */
		break;
	default:
		break;
	}
	bprc = IXGBE_READ_REG(hw, IXGBE_BPRC);
	hwstats->bprc += bprc;
	hwstats->mprc += IXGBE_READ_REG(hw, IXGBE_MPRC);
	if (hw->mac.type == ixgbe_mac_82598EB)
		hwstats->mprc -= bprc;
	hwstats->roc += IXGBE_READ_REG(hw, IXGBE_ROC);
	hwstats->prc64 += IXGBE_READ_REG(hw, IXGBE_PRC64);
	hwstats->prc127 += IXGBE_READ_REG(hw, IXGBE_PRC127);
	hwstats->prc255 += IXGBE_READ_REG(hw, IXGBE_PRC255);
	hwstats->prc511 += IXGBE_READ_REG(hw, IXGBE_PRC511);
	hwstats->prc1023 += IXGBE_READ_REG(hw, IXGBE_PRC1023);
	hwstats->prc1522 += IXGBE_READ_REG(hw, IXGBE_PRC1522);
	hwstats->rlec += IXGBE_READ_REG(hw, IXGBE_RLEC);
	lxon = IXGBE_READ_REG(hw, IXGBE_LXONTXC);
	hwstats->lxontxc += lxon;
	lxoff = IXGBE_READ_REG(hw, IXGBE_LXOFFTXC);
	hwstats->lxofftxc += lxoff;
	hwstats->gptc += IXGBE_READ_REG(hw, IXGBE_GPTC);
	hwstats->mptc += IXGBE_READ_REG(hw, IXGBE_MPTC);
	/*
	 * 82598 errata - tx of flow control packets is included in tx counters
	 */
	xon_off_tot = lxon + lxoff;
	hwstats->gptc -= xon_off_tot;
	hwstats->mptc -= xon_off_tot;
	hwstats->gotc -= (xon_off_tot * (ETH_ZLEN + ETH_FCS_LEN));
	hwstats->ruc += IXGBE_READ_REG(hw, IXGBE_RUC);
	hwstats->rfc += IXGBE_READ_REG(hw, IXGBE_RFC);
	hwstats->rjc += IXGBE_READ_REG(hw, IXGBE_RJC);
	hwstats->tpr += IXGBE_READ_REG(hw, IXGBE_TPR);
	hwstats->ptc64 += IXGBE_READ_REG(hw, IXGBE_PTC64);
	hwstats->ptc64 -= xon_off_tot;
	hwstats->ptc127 += IXGBE_READ_REG(hw, IXGBE_PTC127);
	hwstats->ptc255 += IXGBE_READ_REG(hw, IXGBE_PTC255);
	hwstats->ptc511 += IXGBE_READ_REG(hw, IXGBE_PTC511);
	hwstats->ptc1023 += IXGBE_READ_REG(hw, IXGBE_PTC1023);
	hwstats->ptc1522 += IXGBE_READ_REG(hw, IXGBE_PTC1522);
	hwstats->bptc += IXGBE_READ_REG(hw, IXGBE_BPTC);

	/* Fill out the OS statistics structure */
	netdev->stats.multicast = hwstats->mprc;

	/* Rx Errors */
	netdev->stats.rx_errors = hwstats->crcerrs + hwstats->rlec;
	netdev->stats.rx_dropped = 0;
	netdev->stats.rx_length_errors = hwstats->rlec;
	netdev->stats.rx_crc_errors = hwstats->crcerrs;
	netdev->stats.rx_missed_errors = total_mpc;
<<<<<<< HEAD
=======

	/* VF Stats Collection - skip while resetting because these
	 * are not clear on read and otherwise you'll sometimes get
	 * crazy values.
	 */
	if (!test_bit(__IXGBE_RESETTING, &adapter->state)) {
		for (i = 0; i < adapter->num_vfs; i++) {
			UPDATE_VF_COUNTER_32bit(IXGBE_PVFGPRC(i),
						adapter->vfinfo[i].last_vfstats.gprc,
						adapter->vfinfo[i].vfstats.gprc);
			UPDATE_VF_COUNTER_32bit(IXGBE_PVFGPTC(i),
						adapter->vfinfo[i].last_vfstats.gptc,
						adapter->vfinfo[i].vfstats.gptc);
			UPDATE_VF_COUNTER_36bit(IXGBE_PVFGORC_LSB(i),
						IXGBE_PVFGORC_MSB(i),
						adapter->vfinfo[i].last_vfstats.gorc,
						adapter->vfinfo[i].vfstats.gorc);
			UPDATE_VF_COUNTER_36bit(IXGBE_PVFGOTC_LSB(i),
						IXGBE_PVFGOTC_MSB(i),
						adapter->vfinfo[i].last_vfstats.gotc,
						adapter->vfinfo[i].vfstats.gotc);
			UPDATE_VF_COUNTER_32bit(IXGBE_PVFMPRC(i),
						adapter->vfinfo[i].last_vfstats.mprc,
						adapter->vfinfo[i].vfstats.mprc);
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_fdir_reinit_subtask - worker thread to reinit FDIR filter table
<<<<<<< HEAD
 * @adapter - pointer to the device adapter structure
=======
 * @adapter: pointer to the device adapter structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void ixgbe_fdir_reinit_subtask(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	int i;

	if (!(adapter->flags2 & IXGBE_FLAG2_FDIR_REQUIRES_REINIT))
		return;

	adapter->flags2 &= ~IXGBE_FLAG2_FDIR_REQUIRES_REINIT;

	/* if interface is down do nothing */
	if (test_bit(__IXGBE_DOWN, &adapter->state))
		return;

	/* do nothing if we are not using signature filters */
	if (!(adapter->flags & IXGBE_FLAG_FDIR_HASH_CAPABLE))
		return;

	adapter->fdir_overflow++;

	if (ixgbe_reinit_fdir_tables_82599(hw) == 0) {
		for (i = 0; i < adapter->num_tx_queues; i++)
			set_bit(__IXGBE_TX_FDIR_INIT_DONE,
<<<<<<< HEAD
			        &(adapter->tx_ring[i]->state));
=======
				&(adapter->tx_ring[i]->state));
		for (i = 0; i < adapter->num_xdp_queues; i++)
			set_bit(__IXGBE_TX_FDIR_INIT_DONE,
				&adapter->xdp_ring[i]->state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* re-enable flow director interrupts */
		IXGBE_WRITE_REG(hw, IXGBE_EIMS, IXGBE_EIMS_FLOW_DIR);
	} else {
		e_err(probe, "failed to finish FDIR re-initialization, "
		      "ignored adding FDIR ATR filters\n");
	}
}

/**
 * ixgbe_check_hang_subtask - check for hung queues and dropped interrupts
<<<<<<< HEAD
 * @adapter - pointer to the device adapter structure
=======
 * @adapter: pointer to the device adapter structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function serves two purposes.  First it strobes the interrupt lines
 * in order to make certain interrupts are occurring.  Secondly it sets the
 * bits needed to check for TX hangs.  As a result we should immediately
 * determine if a hang has occurred.
 */
static void ixgbe_check_hang_subtask(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u64 eics = 0;
	int i;

<<<<<<< HEAD
	/* If we're down or resetting, just bail */
	if (test_bit(__IXGBE_DOWN, &adapter->state) ||
=======
	/* If we're down, removing or resetting, just bail */
	if (test_bit(__IXGBE_DOWN, &adapter->state) ||
	    test_bit(__IXGBE_REMOVING, &adapter->state) ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    test_bit(__IXGBE_RESETTING, &adapter->state))
		return;

	/* Force detection of hung controller */
	if (netif_carrier_ok(adapter->netdev)) {
		for (i = 0; i < adapter->num_tx_queues; i++)
			set_check_for_tx_hang(adapter->tx_ring[i]);
<<<<<<< HEAD
=======
		for (i = 0; i < adapter->num_xdp_queues; i++)
			set_check_for_tx_hang(adapter->xdp_ring[i]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (!(adapter->flags & IXGBE_FLAG_MSIX_ENABLED)) {
		/*
		 * for legacy and MSI interrupts don't set any bits
		 * that are enabled for EIAM, because this operation
		 * would set *both* EIMS and EICS for any bit in EIAM
		 */
		IXGBE_WRITE_REG(hw, IXGBE_EICS,
			(IXGBE_EICS_TCP_TIMER | IXGBE_EICS_OTHER));
	} else {
		/* get one bit for every active tx/rx interrupt vector */
<<<<<<< HEAD
		for (i = 0; i < adapter->num_msix_vectors - NON_Q_VECTORS; i++) {
			struct ixgbe_q_vector *qv = adapter->q_vector[i];
			if (qv->rx.ring || qv->tx.ring)
				eics |= ((u64)1 << i);
=======
		for (i = 0; i < adapter->num_q_vectors; i++) {
			struct ixgbe_q_vector *qv = adapter->q_vector[i];
			if (qv->rx.ring || qv->tx.ring)
				eics |= BIT_ULL(i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/* Cause software interrupt to ensure rings are cleaned */
	ixgbe_irq_rearm_queues(adapter, eics);
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_watchdog_update_link - update the link status
<<<<<<< HEAD
 * @adapter - pointer to the device adapter structure
 * @link_speed - pointer to a u32 to store the link_speed
=======
 * @adapter: pointer to the device adapter structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void ixgbe_watchdog_update_link(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 link_speed = adapter->link_speed;
	bool link_up = adapter->link_up;
<<<<<<< HEAD
	int i;
=======
	bool pfc_en = adapter->dcb_cfg.pfc_mode_enable;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!(adapter->flags & IXGBE_FLAG_NEED_LINK_UPDATE))
		return;

	if (hw->mac.ops.check_link) {
		hw->mac.ops.check_link(hw, &link_speed, &link_up, false);
	} else {
		/* always assume link is up, if no check link function */
		link_speed = IXGBE_LINK_SPEED_10GB_FULL;
		link_up = true;
	}
<<<<<<< HEAD
	if (link_up) {
		if (adapter->flags & IXGBE_FLAG_DCB_ENABLED) {
			for (i = 0; i < MAX_TRAFFIC_CLASS; i++)
				hw->mac.ops.fc_enable(hw, i);
		} else {
			hw->mac.ops.fc_enable(hw, 0);
		}
=======

	if (adapter->ixgbe_ieee_pfc)
		pfc_en |= !!(adapter->ixgbe_ieee_pfc->pfc_en);

	if (link_up && !((adapter->flags & IXGBE_FLAG_DCB_ENABLED) && pfc_en)) {
		hw->mac.ops.fc_enable(hw);
		ixgbe_set_rx_drop_en(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (link_up ||
	    time_after(jiffies, (adapter->link_check_timeout +
				 IXGBE_TRY_LINK_TIMEOUT))) {
		adapter->flags &= ~IXGBE_FLAG_NEED_LINK_UPDATE;
		IXGBE_WRITE_REG(hw, IXGBE_EIMS, IXGBE_EIMC_LSC);
		IXGBE_WRITE_FLUSH(hw);
	}

	adapter->link_up = link_up;
	adapter->link_speed = link_speed;
}

<<<<<<< HEAD
/**
 * ixgbe_watchdog_link_is_up - update netif_carrier status and
 *                             print link up message
 * @adapter - pointer to the device adapter structure
=======
static void ixgbe_update_default_up(struct ixgbe_adapter *adapter)
{
#ifdef CONFIG_IXGBE_DCB
	struct net_device *netdev = adapter->netdev;
	struct dcb_app app = {
			      .selector = IEEE_8021QAZ_APP_SEL_ETHERTYPE,
			      .protocol = 0,
			     };
	u8 up = 0;

	if (adapter->dcbx_cap & DCB_CAP_DCBX_VER_IEEE)
		up = dcb_ieee_getapp_mask(netdev, &app);

	adapter->default_up = (up > 1) ? (ffs(up) - 1) : 0;
#endif
}

/**
 * ixgbe_watchdog_link_is_up - update netif_carrier status and
 *                             print link up message
 * @adapter: pointer to the device adapter structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void ixgbe_watchdog_link_is_up(struct ixgbe_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct ixgbe_hw *hw = &adapter->hw;
	u32 link_speed = adapter->link_speed;
<<<<<<< HEAD
=======
	const char *speed_str;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bool flow_rx, flow_tx;

	/* only continue if link was previously down */
	if (netif_carrier_ok(netdev))
		return;

	adapter->flags2 &= ~IXGBE_FLAG2_SEARCH_FOR_SFP;

	switch (hw->mac.type) {
	case ixgbe_mac_82598EB: {
		u32 frctl = IXGBE_READ_REG(hw, IXGBE_FCTRL);
		u32 rmcs = IXGBE_READ_REG(hw, IXGBE_RMCS);
		flow_rx = !!(frctl & IXGBE_FCTRL_RFCE);
		flow_tx = !!(rmcs & IXGBE_RMCS_TFCE_802_3X);
	}
		break;
	case ixgbe_mac_X540:
<<<<<<< HEAD
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case ixgbe_mac_82599EB: {
		u32 mflcn = IXGBE_READ_REG(hw, IXGBE_MFLCN);
		u32 fccfg = IXGBE_READ_REG(hw, IXGBE_FCCFG);
		flow_rx = !!(mflcn & IXGBE_MFLCN_RFCE);
		flow_tx = !!(fccfg & IXGBE_FCCFG_TFCE_802_3X);
	}
		break;
	default:
		flow_tx = false;
		flow_rx = false;
		break;
	}
<<<<<<< HEAD
	e_info(drv, "NIC Link is Up %s, Flow Control: %s\n",
	       (link_speed == IXGBE_LINK_SPEED_10GB_FULL ?
	       "10 Gbps" :
	       (link_speed == IXGBE_LINK_SPEED_1GB_FULL ?
	       "1 Gbps" :
	       (link_speed == IXGBE_LINK_SPEED_100_FULL ?
	       "100 Mbps" :
	       "unknown speed"))),
=======

	adapter->last_rx_ptp_check = jiffies;

	if (test_bit(__IXGBE_PTP_RUNNING, &adapter->state))
		ixgbe_ptp_start_cyclecounter(adapter);

	switch (link_speed) {
	case IXGBE_LINK_SPEED_10GB_FULL:
		speed_str = "10 Gbps";
		break;
	case IXGBE_LINK_SPEED_5GB_FULL:
		speed_str = "5 Gbps";
		break;
	case IXGBE_LINK_SPEED_2_5GB_FULL:
		speed_str = "2.5 Gbps";
		break;
	case IXGBE_LINK_SPEED_1GB_FULL:
		speed_str = "1 Gbps";
		break;
	case IXGBE_LINK_SPEED_100_FULL:
		speed_str = "100 Mbps";
		break;
	case IXGBE_LINK_SPEED_10_FULL:
		speed_str = "10 Mbps";
		break;
	default:
		speed_str = "unknown speed";
		break;
	}
	e_info(drv, "NIC Link is Up %s, Flow Control: %s\n", speed_str,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       ((flow_rx && flow_tx) ? "RX/TX" :
	       (flow_rx ? "RX" :
	       (flow_tx ? "TX" : "None"))));

	netif_carrier_on(netdev);
	ixgbe_check_vf_rate_limit(adapter);
<<<<<<< HEAD
=======

	/* enable transmits */
	netif_tx_wake_all_queues(adapter->netdev);

	/* update the default user priority for VFs */
	ixgbe_update_default_up(adapter);

	/* ping all the active vfs to let them know link has changed */
	ixgbe_ping_all_vfs(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_watchdog_link_is_down - update netif_carrier status and
 *                               print link down message
<<<<<<< HEAD
 * @adapter - pointer to the adapter structure
=======
 * @adapter: pointer to the adapter structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void ixgbe_watchdog_link_is_down(struct ixgbe_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct ixgbe_hw *hw = &adapter->hw;

	adapter->link_up = false;
	adapter->link_speed = 0;

	/* only continue if link was up previously */
	if (!netif_carrier_ok(netdev))
		return;

	/* poll for SFP+ cable when link is down */
	if (ixgbe_is_sfp(hw) && hw->mac.type == ixgbe_mac_82598EB)
		adapter->flags2 |= IXGBE_FLAG2_SEARCH_FOR_SFP;

<<<<<<< HEAD
	e_info(drv, "NIC Link is Down\n");
	netif_carrier_off(netdev);
=======
	if (test_bit(__IXGBE_PTP_RUNNING, &adapter->state))
		ixgbe_ptp_start_cyclecounter(adapter);

	e_info(drv, "NIC Link is Down\n");
	netif_carrier_off(netdev);

	/* ping all the active vfs to let them know link has changed */
	ixgbe_ping_all_vfs(adapter);
}

static bool ixgbe_ring_tx_pending(struct ixgbe_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_tx_queues; i++) {
		struct ixgbe_ring *tx_ring = adapter->tx_ring[i];

		if (tx_ring->next_to_use != tx_ring->next_to_clean)
			return true;
	}

	for (i = 0; i < adapter->num_xdp_queues; i++) {
		struct ixgbe_ring *ring = adapter->xdp_ring[i];

		if (ring->next_to_use != ring->next_to_clean)
			return true;
	}

	return false;
}

static bool ixgbe_vf_tx_pending(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	struct ixgbe_ring_feature *vmdq = &adapter->ring_feature[RING_F_VMDQ];
	u32 q_per_pool = __ALIGN_MASK(1, ~vmdq->mask);

	int i, j;

	if (!adapter->num_vfs)
		return false;

	/* resetting the PF is only needed for MAC before X550 */
	if (hw->mac.type >= ixgbe_mac_X550)
		return false;

	for (i = 0; i < adapter->num_vfs; i++) {
		for (j = 0; j < q_per_pool; j++) {
			u32 h, t;

			h = IXGBE_READ_REG(hw, IXGBE_PVFTDHN(q_per_pool, i, j));
			t = IXGBE_READ_REG(hw, IXGBE_PVFTDTN(q_per_pool, i, j));

			if (h != t)
				return true;
		}
	}

	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_watchdog_flush_tx - flush queues on link down
<<<<<<< HEAD
 * @adapter - pointer to the device adapter structure
 **/
static void ixgbe_watchdog_flush_tx(struct ixgbe_adapter *adapter)
{
	int i;
	int some_tx_pending = 0;

	if (!netif_carrier_ok(adapter->netdev)) {
		for (i = 0; i < adapter->num_tx_queues; i++) {
			struct ixgbe_ring *tx_ring = adapter->tx_ring[i];
			if (tx_ring->next_to_use != tx_ring->next_to_clean) {
				some_tx_pending = 1;
				break;
			}
		}

		if (some_tx_pending) {
=======
 * @adapter: pointer to the device adapter structure
 **/
static void ixgbe_watchdog_flush_tx(struct ixgbe_adapter *adapter)
{
	if (!netif_carrier_ok(adapter->netdev)) {
		if (ixgbe_ring_tx_pending(adapter) ||
		    ixgbe_vf_tx_pending(adapter)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* We've lost link, so the controller stops DMA,
			 * but we've got queued Tx work that's never going
			 * to get done, so reset controller to flush Tx.
			 * (Do the reset outside of interrupt context).
			 */
<<<<<<< HEAD
			adapter->flags2 |= IXGBE_FLAG2_RESET_REQUESTED;
=======
			e_warn(drv, "initiating reset to clear Tx work after link loss\n");
			set_bit(__IXGBE_RESET_REQUESTED, &adapter->state);
		}
	}
}

#ifdef CONFIG_PCI_IOV
static void ixgbe_bad_vf_abort(struct ixgbe_adapter *adapter, u32 vf)
{
	struct ixgbe_hw *hw = &adapter->hw;

	if (adapter->hw.mac.type == ixgbe_mac_82599EB &&
	    adapter->flags2 & IXGBE_FLAG2_AUTO_DISABLE_VF) {
		adapter->vfinfo[vf].primary_abort_count++;
		if (adapter->vfinfo[vf].primary_abort_count ==
		    IXGBE_PRIMARY_ABORT_LIMIT) {
			ixgbe_set_vf_link_state(adapter, vf,
						IFLA_VF_LINK_STATE_DISABLE);
			adapter->vfinfo[vf].primary_abort_count = 0;

			e_info(drv,
			       "Malicious Driver Detection event detected on PF %d VF %d MAC: %pM mdd-disable-vf=on",
			       hw->bus.func, vf,
			       adapter->vfinfo[vf].vf_mac_addresses);
		}
	}
}

static void ixgbe_check_for_bad_vf(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	struct pci_dev *pdev = adapter->pdev;
	unsigned int vf;
	u32 gpc;

	if (!(netif_carrier_ok(adapter->netdev)))
		return;

	gpc = IXGBE_READ_REG(hw, IXGBE_TXDGPC);
	if (gpc) /* If incrementing then no need for the check below */
		return;
	/* Check to see if a bad DMA write target from an errant or
	 * malicious VF has caused a PCIe error.  If so then we can
	 * issue a VFLR to the offending VF(s) and then resume without
	 * requesting a full slot reset.
	 */

	if (!pdev)
		return;

	/* check status reg for all VFs owned by this PF */
	for (vf = 0; vf < adapter->num_vfs; ++vf) {
		struct pci_dev *vfdev = adapter->vfinfo[vf].vfdev;
		u16 status_reg;

		if (!vfdev)
			continue;
		pci_read_config_word(vfdev, PCI_STATUS, &status_reg);
		if (status_reg != IXGBE_FAILED_READ_CFG_WORD &&
		    status_reg & PCI_STATUS_REC_MASTER_ABORT) {
			ixgbe_bad_vf_abort(adapter, vf);
			pcie_flr(vfdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

static void ixgbe_spoof_check(struct ixgbe_adapter *adapter)
{
	u32 ssvpc;

<<<<<<< HEAD
	/* Do not perform spoof check for 82598 */
	if (adapter->hw.mac.type == ixgbe_mac_82598EB)
=======
	/* Do not perform spoof check for 82598 or if not in IOV mode */
	if (adapter->hw.mac.type == ixgbe_mac_82598EB ||
	    adapter->num_vfs == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	ssvpc = IXGBE_READ_REG(&adapter->hw, IXGBE_SSVPC);

	/*
	 * ssvpc register is cleared on read, if zero then no
	 * spoofed packets in the last interval.
	 */
	if (!ssvpc)
		return;

<<<<<<< HEAD
	e_warn(drv, "%d Spoofed packets detected\n", ssvpc);
}

/**
 * ixgbe_watchdog_subtask - check and bring link up
 * @adapter - pointer to the device adapter structure
 **/
static void ixgbe_watchdog_subtask(struct ixgbe_adapter *adapter)
{
	/* if interface is down do nothing */
	if (test_bit(__IXGBE_DOWN, &adapter->state) ||
=======
	e_warn(drv, "%u Spoofed packets detected\n", ssvpc);
}
#else
static void ixgbe_spoof_check(struct ixgbe_adapter __always_unused *adapter)
{
}

static void
ixgbe_check_for_bad_vf(struct ixgbe_adapter __always_unused *adapter)
{
}
#endif /* CONFIG_PCI_IOV */


/**
 * ixgbe_watchdog_subtask - check and bring link up
 * @adapter: pointer to the device adapter structure
 **/
static void ixgbe_watchdog_subtask(struct ixgbe_adapter *adapter)
{
	/* if interface is down, removing or resetting, do nothing */
	if (test_bit(__IXGBE_DOWN, &adapter->state) ||
	    test_bit(__IXGBE_REMOVING, &adapter->state) ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    test_bit(__IXGBE_RESETTING, &adapter->state))
		return;

	ixgbe_watchdog_update_link(adapter);

	if (adapter->link_up)
		ixgbe_watchdog_link_is_up(adapter);
	else
		ixgbe_watchdog_link_is_down(adapter);

<<<<<<< HEAD
=======
	ixgbe_check_for_bad_vf(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ixgbe_spoof_check(adapter);
	ixgbe_update_stats(adapter);

	ixgbe_watchdog_flush_tx(adapter);
}

/**
 * ixgbe_sfp_detection_subtask - poll for SFP+ cable
<<<<<<< HEAD
 * @adapter - the ixgbe adapter structure
=======
 * @adapter: the ixgbe adapter structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void ixgbe_sfp_detection_subtask(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD
	s32 err;
=======
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* not searching for SFP so there is nothing to do here */
	if (!(adapter->flags2 & IXGBE_FLAG2_SEARCH_FOR_SFP) &&
	    !(adapter->flags2 & IXGBE_FLAG2_SFP_NEEDS_RESET))
		return;

<<<<<<< HEAD
=======
	if (adapter->sfp_poll_time &&
	    time_after(adapter->sfp_poll_time, jiffies))
		return; /* If not yet time to poll for SFP */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* someone else is in init, wait until next service event */
	if (test_and_set_bit(__IXGBE_IN_SFP_INIT, &adapter->state))
		return;

<<<<<<< HEAD
	err = hw->phy.ops.identify_sfp(hw);
	if (err == IXGBE_ERR_SFP_NOT_SUPPORTED)
		goto sfp_out;

	if (err == IXGBE_ERR_SFP_NOT_PRESENT) {
=======
	adapter->sfp_poll_time = jiffies + IXGBE_SFP_POLL_JIFFIES - 1;

	err = hw->phy.ops.identify_sfp(hw);
	if (err == -EOPNOTSUPP)
		goto sfp_out;

	if (err == -ENOENT) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* If no cable is present, then we need to reset
		 * the next time we find a good cable. */
		adapter->flags2 |= IXGBE_FLAG2_SFP_NEEDS_RESET;
	}

	/* exit on error */
	if (err)
		goto sfp_out;

	/* exit if reset not needed */
	if (!(adapter->flags2 & IXGBE_FLAG2_SFP_NEEDS_RESET))
		goto sfp_out;

	adapter->flags2 &= ~IXGBE_FLAG2_SFP_NEEDS_RESET;

	/*
	 * A module may be identified correctly, but the EEPROM may not have
	 * support for that module.  setup_sfp() will fail in that case, so
	 * we should not allow that module to load.
	 */
	if (hw->mac.type == ixgbe_mac_82598EB)
		err = hw->phy.ops.reset(hw);
	else
		err = hw->mac.ops.setup_sfp(hw);

<<<<<<< HEAD
	if (err == IXGBE_ERR_SFP_NOT_SUPPORTED)
=======
	if (err == -EOPNOTSUPP)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto sfp_out;

	adapter->flags |= IXGBE_FLAG_NEED_LINK_CONFIG;
	e_info(probe, "detected SFP+: %d\n", hw->phy.sfp_type);

sfp_out:
	clear_bit(__IXGBE_IN_SFP_INIT, &adapter->state);

<<<<<<< HEAD
	if ((err == IXGBE_ERR_SFP_NOT_SUPPORTED) &&
	    (adapter->netdev->reg_state == NETREG_REGISTERED)) {
=======
	if (err == -EOPNOTSUPP &&
	    adapter->netdev->reg_state == NETREG_REGISTERED) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		e_dev_err("failed to initialize because an unsupported "
			  "SFP+ module type was detected.\n");
		e_dev_err("Reload the driver after installing a "
			  "supported module.\n");
		unregister_netdev(adapter->netdev);
	}
}

/**
 * ixgbe_sfp_link_config_subtask - set up link SFP after module install
<<<<<<< HEAD
 * @adapter - the ixgbe adapter structure
=======
 * @adapter: the ixgbe adapter structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void ixgbe_sfp_link_config_subtask(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
<<<<<<< HEAD
	u32 autoneg;
	bool negotiation;
=======
	u32 cap_speed;
	u32 speed;
	bool autoneg = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!(adapter->flags & IXGBE_FLAG_NEED_LINK_CONFIG))
		return;

	/* someone else is in init, wait until next service event */
	if (test_and_set_bit(__IXGBE_IN_SFP_INIT, &adapter->state))
		return;

	adapter->flags &= ~IXGBE_FLAG_NEED_LINK_CONFIG;

<<<<<<< HEAD
	autoneg = hw->phy.autoneg_advertised;
	if ((!autoneg) && (hw->mac.ops.get_link_capabilities))
		hw->mac.ops.get_link_capabilities(hw, &autoneg, &negotiation);
	if (hw->mac.ops.setup_link)
		hw->mac.ops.setup_link(hw, autoneg, negotiation, true);
=======
	hw->mac.ops.get_link_capabilities(hw, &cap_speed, &autoneg);

	/* advertise highest capable link speed */
	if (!autoneg && (cap_speed & IXGBE_LINK_SPEED_10GB_FULL))
		speed = IXGBE_LINK_SPEED_10GB_FULL;
	else
		speed = cap_speed & (IXGBE_LINK_SPEED_10GB_FULL |
				     IXGBE_LINK_SPEED_1GB_FULL);

	if (hw->mac.ops.setup_link)
		hw->mac.ops.setup_link(hw, speed, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	adapter->flags |= IXGBE_FLAG_NEED_LINK_UPDATE;
	adapter->link_check_timeout = jiffies;
	clear_bit(__IXGBE_IN_SFP_INIT, &adapter->state);
}

<<<<<<< HEAD
#ifdef CONFIG_PCI_IOV
static void ixgbe_check_for_bad_vf(struct ixgbe_adapter *adapter)
{
	int vf;
	struct ixgbe_hw *hw = &adapter->hw;
	struct net_device *netdev = adapter->netdev;
	u32 gpc;
	u32 ciaa, ciad;

	gpc = IXGBE_READ_REG(hw, IXGBE_TXDGPC);
	if (gpc) /* If incrementing then no need for the check below */
		return;
	/*
	 * Check to see if a bad DMA write target from an errant or
	 * malicious VF has caused a PCIe error.  If so then we can
	 * issue a VFLR to the offending VF(s) and then resume without
	 * requesting a full slot reset.
	 */

	for (vf = 0; vf < adapter->num_vfs; vf++) {
		ciaa = (vf << 16) | 0x80000000;
		/* 32 bit read so align, we really want status at offset 6 */
		ciaa |= PCI_COMMAND;
		IXGBE_WRITE_REG(hw, IXGBE_CIAA_82599, ciaa);
		ciad = IXGBE_READ_REG(hw, IXGBE_CIAD_82599);
		ciaa &= 0x7FFFFFFF;
		/* disable debug mode asap after reading data */
		IXGBE_WRITE_REG(hw, IXGBE_CIAA_82599, ciaa);
		/* Get the upper 16 bits which will be the PCI status reg */
		ciad >>= 16;
		if (ciad & PCI_STATUS_REC_MASTER_ABORT) {
			netdev_err(netdev, "VF %d Hung DMA\n", vf);
			/* Issue VFLR */
			ciaa = (vf << 16) | 0x80000000;
			ciaa |= 0xA8;
			IXGBE_WRITE_REG(hw, IXGBE_CIAA_82599, ciaa);
			ciad = 0x00008000;  /* VFLR */
			IXGBE_WRITE_REG(hw, IXGBE_CIAD_82599, ciad);
			ciaa &= 0x7FFFFFFF;
			IXGBE_WRITE_REG(hw, IXGBE_CIAA_82599, ciaa);
		}
	}
}

#endif
/**
 * ixgbe_service_timer - Timer Call-back
 * @data: pointer to adapter cast into an unsigned long
 **/
static void ixgbe_service_timer(unsigned long data)
{
	struct ixgbe_adapter *adapter = (struct ixgbe_adapter *)data;
	unsigned long next_event_offset;
	bool ready = true;
=======
/**
 * ixgbe_service_timer - Timer Call-back
 * @t: pointer to timer_list structure
 **/
static void ixgbe_service_timer(struct timer_list *t)
{
	struct ixgbe_adapter *adapter = from_timer(adapter, t, service_timer);
	unsigned long next_event_offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* poll faster when waiting for link */
	if (adapter->flags & IXGBE_FLAG_NEED_LINK_UPDATE)
		next_event_offset = HZ / 10;
	else
		next_event_offset = HZ * 2;

<<<<<<< HEAD
#ifdef CONFIG_PCI_IOV
	/*
	 * don't bother with SR-IOV VF DMA hang check if there are
	 * no VFs or the link is down
	 */
	if (!adapter->num_vfs ||
	    (adapter->flags & IXGBE_FLAG_NEED_LINK_UPDATE))
		goto normal_timer_service;

	/* If we have VFs allocated then we must check for DMA hangs */
	ixgbe_check_for_bad_vf(adapter);
	next_event_offset = HZ / 50;
	adapter->timer_event_accumulator++;

	if (adapter->timer_event_accumulator >= 100)
		adapter->timer_event_accumulator = 0;
	else
		ready = false;

normal_timer_service:
#endif
	/* Reset the timer */
	mod_timer(&adapter->service_timer, next_event_offset + jiffies);

	if (ready)
		ixgbe_service_event_schedule(adapter);
=======
	/* Reset the timer */
	mod_timer(&adapter->service_timer, next_event_offset + jiffies);

	ixgbe_service_event_schedule(adapter);
}

static void ixgbe_phy_interrupt_subtask(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	bool overtemp;

	if (!(adapter->flags2 & IXGBE_FLAG2_PHY_INTERRUPT))
		return;

	adapter->flags2 &= ~IXGBE_FLAG2_PHY_INTERRUPT;

	if (!hw->phy.ops.handle_lasi)
		return;

	hw->phy.ops.handle_lasi(&adapter->hw, &overtemp);
	if (overtemp)
		e_crit(drv, "%s\n", ixgbe_overheat_msg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_reset_subtask(struct ixgbe_adapter *adapter)
{
<<<<<<< HEAD
	if (!(adapter->flags2 & IXGBE_FLAG2_RESET_REQUESTED))
		return;

	adapter->flags2 &= ~IXGBE_FLAG2_RESET_REQUESTED;

	/* If we're already down or resetting, just bail */
	if (test_bit(__IXGBE_DOWN, &adapter->state) ||
	    test_bit(__IXGBE_RESETTING, &adapter->state))
		return;
=======
	if (!test_and_clear_bit(__IXGBE_RESET_REQUESTED, &adapter->state))
		return;

	rtnl_lock();
	/* If we're already down, removing or resetting, just bail */
	if (test_bit(__IXGBE_DOWN, &adapter->state) ||
	    test_bit(__IXGBE_REMOVING, &adapter->state) ||
	    test_bit(__IXGBE_RESETTING, &adapter->state)) {
		rtnl_unlock();
		return;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ixgbe_dump(adapter);
	netdev_err(adapter->netdev, "Reset adapter\n");
	adapter->tx_timeout_count++;

	ixgbe_reinit_locked(adapter);
<<<<<<< HEAD
=======
	rtnl_unlock();
}

/**
 * ixgbe_check_fw_error - Check firmware for errors
 * @adapter: the adapter private structure
 *
 * Check firmware errors in register FWSM
 */
static bool ixgbe_check_fw_error(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 fwsm;

	/* read fwsm.ext_err_ind register and log errors */
	fwsm = IXGBE_READ_REG(hw, IXGBE_FWSM(hw));

	if (fwsm & IXGBE_FWSM_EXT_ERR_IND_MASK ||
	    !(fwsm & IXGBE_FWSM_FW_VAL_BIT))
		e_dev_warn("Warning firmware error detected FWSM: 0x%08X\n",
			   fwsm);

	if (hw->mac.ops.fw_recovery_mode && hw->mac.ops.fw_recovery_mode(hw)) {
		e_dev_err("Firmware recovery mode detected. Limiting functionality. Refer to the Intel(R) Ethernet Adapters and Devices User Guide for details on firmware recovery mode.\n");
		return true;
	}

	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_service_task - manages and runs subtasks
 * @work: pointer to work_struct containing our data
 **/
static void ixgbe_service_task(struct work_struct *work)
{
	struct ixgbe_adapter *adapter = container_of(work,
						     struct ixgbe_adapter,
						     service_task);
<<<<<<< HEAD

	ixgbe_reset_subtask(adapter);
=======
	if (ixgbe_removed(adapter->hw.hw_addr)) {
		if (!test_bit(__IXGBE_DOWN, &adapter->state)) {
			rtnl_lock();
			ixgbe_down(adapter);
			rtnl_unlock();
		}
		ixgbe_service_event_complete(adapter);
		return;
	}
	if (ixgbe_check_fw_error(adapter)) {
		if (!test_bit(__IXGBE_DOWN, &adapter->state))
			unregister_netdev(adapter->netdev);
		ixgbe_service_event_complete(adapter);
		return;
	}
	ixgbe_reset_subtask(adapter);
	ixgbe_phy_interrupt_subtask(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ixgbe_sfp_detection_subtask(adapter);
	ixgbe_sfp_link_config_subtask(adapter);
	ixgbe_check_overtemp_subtask(adapter);
	ixgbe_watchdog_subtask(adapter);
	ixgbe_fdir_reinit_subtask(adapter);
	ixgbe_check_hang_subtask(adapter);

<<<<<<< HEAD
=======
	if (test_bit(__IXGBE_PTP_RUNNING, &adapter->state)) {
		ixgbe_ptp_overflow_check(adapter);
		if (adapter->flags & IXGBE_FLAG_RX_HWTSTAMP_IN_REGISTER)
			ixgbe_ptp_rx_hang(adapter);
		ixgbe_ptp_tx_hang(adapter);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ixgbe_service_event_complete(adapter);
}

static int ixgbe_tso(struct ixgbe_ring *tx_ring,
		     struct ixgbe_tx_buffer *first,
<<<<<<< HEAD
		     u8 *hdr_len)
{
	struct sk_buff *skb = first->skb;
	u32 vlan_macip_lens, type_tucmd;
	u32 mss_l4len_idx, l4len;
=======
		     u8 *hdr_len,
		     struct ixgbe_ipsec_tx_data *itd)
{
	u32 vlan_macip_lens, type_tucmd, mss_l4len_idx;
	struct sk_buff *skb = first->skb;
	union {
		struct iphdr *v4;
		struct ipv6hdr *v6;
		unsigned char *hdr;
	} ip;
	union {
		struct tcphdr *tcp;
		struct udphdr *udp;
		unsigned char *hdr;
	} l4;
	u32 paylen, l4_offset;
	u32 fceof_saidx = 0;
	int err;

	if (skb->ip_summed != CHECKSUM_PARTIAL)
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!skb_is_gso(skb))
		return 0;

<<<<<<< HEAD
	if (skb_header_cloned(skb)) {
		int err = pskb_expand_head(skb, 0, 0, GFP_ATOMIC);
		if (err)
			return err;
	}

	/* ADV DTYP TUCMD MKRLOC/ISCSIHEDLEN */
	type_tucmd = IXGBE_ADVTXD_TUCMD_L4T_TCP;

	if (first->protocol == __constant_htons(ETH_P_IP)) {
		struct iphdr *iph = ip_hdr(skb);
		iph->tot_len = 0;
		iph->check = 0;
		tcp_hdr(skb)->check = ~csum_tcpudp_magic(iph->saddr,
							 iph->daddr, 0,
							 IPPROTO_TCP,
							 0);
		type_tucmd |= IXGBE_ADVTXD_TUCMD_IPV4;
		first->tx_flags |= IXGBE_TX_FLAGS_TSO |
				   IXGBE_TX_FLAGS_CSUM |
				   IXGBE_TX_FLAGS_IPV4;
	} else if (skb_is_gso_v6(skb)) {
		ipv6_hdr(skb)->payload_len = 0;
		tcp_hdr(skb)->check =
		    ~csum_ipv6_magic(&ipv6_hdr(skb)->saddr,
				     &ipv6_hdr(skb)->daddr,
				     0, IPPROTO_TCP, 0);
=======
	err = skb_cow_head(skb, 0);
	if (err < 0)
		return err;

	if (eth_p_mpls(first->protocol))
		ip.hdr = skb_inner_network_header(skb);
	else
		ip.hdr = skb_network_header(skb);
	l4.hdr = skb_checksum_start(skb);

	/* ADV DTYP TUCMD MKRLOC/ISCSIHEDLEN */
	type_tucmd = (skb_shinfo(skb)->gso_type & SKB_GSO_UDP_L4) ?
		      IXGBE_ADVTXD_TUCMD_L4T_UDP : IXGBE_ADVTXD_TUCMD_L4T_TCP;

	/* initialize outer IP header fields */
	if (ip.v4->version == 4) {
		unsigned char *csum_start = skb_checksum_start(skb);
		unsigned char *trans_start = ip.hdr + (ip.v4->ihl * 4);
		int len = csum_start - trans_start;

		/* IP header will have to cancel out any data that
		 * is not a part of the outer IP header, so set to
		 * a reverse csum if needed, else init check to 0.
		 */
		ip.v4->check = (skb_shinfo(skb)->gso_type & SKB_GSO_PARTIAL) ?
					   csum_fold(csum_partial(trans_start,
								  len, 0)) : 0;
		type_tucmd |= IXGBE_ADVTXD_TUCMD_IPV4;

		ip.v4->tot_len = 0;
		first->tx_flags |= IXGBE_TX_FLAGS_TSO |
				   IXGBE_TX_FLAGS_CSUM |
				   IXGBE_TX_FLAGS_IPV4;
	} else {
		ip.v6->payload_len = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		first->tx_flags |= IXGBE_TX_FLAGS_TSO |
				   IXGBE_TX_FLAGS_CSUM;
	}

<<<<<<< HEAD
	/* compute header lengths */
	l4len = tcp_hdrlen(skb);
	*hdr_len = skb_transport_offset(skb) + l4len;
=======
	/* determine offset of inner transport header */
	l4_offset = l4.hdr - skb->data;

	/* remove payload length from inner checksum */
	paylen = skb->len - l4_offset;

	if (type_tucmd & IXGBE_ADVTXD_TUCMD_L4T_TCP) {
		/* compute length of segmentation header */
		*hdr_len = (l4.tcp->doff * 4) + l4_offset;
		csum_replace_by_diff(&l4.tcp->check,
				     (__force __wsum)htonl(paylen));
	} else {
		/* compute length of segmentation header */
		*hdr_len = sizeof(*l4.udp) + l4_offset;
		csum_replace_by_diff(&l4.udp->check,
				     (__force __wsum)htonl(paylen));
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* update gso size and bytecount with header size */
	first->gso_segs = skb_shinfo(skb)->gso_segs;
	first->bytecount += (first->gso_segs - 1) * *hdr_len;

<<<<<<< HEAD
	/* mss_l4len_id: use 1 as index for TSO */
	mss_l4len_idx = l4len << IXGBE_ADVTXD_L4LEN_SHIFT;
	mss_l4len_idx |= skb_shinfo(skb)->gso_size << IXGBE_ADVTXD_MSS_SHIFT;
	mss_l4len_idx |= 1 << IXGBE_ADVTXD_IDX_SHIFT;

	/* vlan_macip_lens: HEADLEN, MACLEN, VLAN tag */
	vlan_macip_lens = skb_network_header_len(skb);
	vlan_macip_lens |= skb_network_offset(skb) << IXGBE_ADVTXD_MACLEN_SHIFT;
	vlan_macip_lens |= first->tx_flags & IXGBE_TX_FLAGS_VLAN_MASK;

	ixgbe_tx_ctxtdesc(tx_ring, vlan_macip_lens, 0, type_tucmd,
=======
	/* mss_l4len_id: use 0 as index for TSO */
	mss_l4len_idx = (*hdr_len - l4_offset) << IXGBE_ADVTXD_L4LEN_SHIFT;
	mss_l4len_idx |= skb_shinfo(skb)->gso_size << IXGBE_ADVTXD_MSS_SHIFT;

	fceof_saidx |= itd->sa_idx;
	type_tucmd |= itd->flags | itd->trailer_len;

	/* vlan_macip_lens: HEADLEN, MACLEN, VLAN tag */
	vlan_macip_lens = l4.hdr - ip.hdr;
	vlan_macip_lens |= (ip.hdr - skb->data) << IXGBE_ADVTXD_MACLEN_SHIFT;
	vlan_macip_lens |= first->tx_flags & IXGBE_TX_FLAGS_VLAN_MASK;

	ixgbe_tx_ctxtdesc(tx_ring, vlan_macip_lens, fceof_saidx, type_tucmd,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  mss_l4len_idx);

	return 1;
}

static void ixgbe_tx_csum(struct ixgbe_ring *tx_ring,
<<<<<<< HEAD
			  struct ixgbe_tx_buffer *first)
{
	struct sk_buff *skb = first->skb;
	u32 vlan_macip_lens = 0;
	u32 mss_l4len_idx = 0;
	u32 type_tucmd = 0;

	if (skb->ip_summed != CHECKSUM_PARTIAL) {
		if (!(first->tx_flags & IXGBE_TX_FLAGS_HW_VLAN) &&
		    !(first->tx_flags & IXGBE_TX_FLAGS_TXSW))
			return;
	} else {
		u8 l4_hdr = 0;
		switch (first->protocol) {
		case __constant_htons(ETH_P_IP):
			vlan_macip_lens |= skb_network_header_len(skb);
			type_tucmd |= IXGBE_ADVTXD_TUCMD_IPV4;
			l4_hdr = ip_hdr(skb)->protocol;
			break;
		case __constant_htons(ETH_P_IPV6):
			vlan_macip_lens |= skb_network_header_len(skb);
			l4_hdr = ipv6_hdr(skb)->nexthdr;
			break;
		default:
			if (unlikely(net_ratelimit())) {
				dev_warn(tx_ring->dev,
				 "partial checksum but proto=%x!\n",
				 first->protocol);
			}
			break;
		}

		switch (l4_hdr) {
		case IPPROTO_TCP:
			type_tucmd |= IXGBE_ADVTXD_TUCMD_L4T_TCP;
			mss_l4len_idx = tcp_hdrlen(skb) <<
					IXGBE_ADVTXD_L4LEN_SHIFT;
			break;
		case IPPROTO_SCTP:
			type_tucmd |= IXGBE_ADVTXD_TUCMD_L4T_SCTP;
			mss_l4len_idx = sizeof(struct sctphdr) <<
					IXGBE_ADVTXD_L4LEN_SHIFT;
			break;
		case IPPROTO_UDP:
			mss_l4len_idx = sizeof(struct udphdr) <<
					IXGBE_ADVTXD_L4LEN_SHIFT;
			break;
		default:
			if (unlikely(net_ratelimit())) {
				dev_warn(tx_ring->dev,
				 "partial checksum but l4 proto=%x!\n",
				 l4_hdr);
			}
			break;
		}

		/* update TX checksum flag */
		first->tx_flags |= IXGBE_TX_FLAGS_CSUM;
	}

=======
			  struct ixgbe_tx_buffer *first,
			  struct ixgbe_ipsec_tx_data *itd)
{
	struct sk_buff *skb = first->skb;
	u32 vlan_macip_lens = 0;
	u32 fceof_saidx = 0;
	u32 type_tucmd = 0;

	if (skb->ip_summed != CHECKSUM_PARTIAL) {
csum_failed:
		if (!(first->tx_flags & (IXGBE_TX_FLAGS_HW_VLAN |
					 IXGBE_TX_FLAGS_CC)))
			return;
		goto no_csum;
	}

	switch (skb->csum_offset) {
	case offsetof(struct tcphdr, check):
		type_tucmd = IXGBE_ADVTXD_TUCMD_L4T_TCP;
		fallthrough;
	case offsetof(struct udphdr, check):
		break;
	case offsetof(struct sctphdr, checksum):
		/* validate that this is actually an SCTP request */
		if (skb_csum_is_sctp(skb)) {
			type_tucmd = IXGBE_ADVTXD_TUCMD_L4T_SCTP;
			break;
		}
		fallthrough;
	default:
		skb_checksum_help(skb);
		goto csum_failed;
	}

	/* update TX checksum flag */
	first->tx_flags |= IXGBE_TX_FLAGS_CSUM;
	vlan_macip_lens = skb_checksum_start_offset(skb) -
			  skb_network_offset(skb);
no_csum:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* vlan_macip_lens: MACLEN, VLAN tag */
	vlan_macip_lens |= skb_network_offset(skb) << IXGBE_ADVTXD_MACLEN_SHIFT;
	vlan_macip_lens |= first->tx_flags & IXGBE_TX_FLAGS_VLAN_MASK;

<<<<<<< HEAD
	ixgbe_tx_ctxtdesc(tx_ring, vlan_macip_lens, 0,
			  type_tucmd, mss_l4len_idx);
}

static __le32 ixgbe_tx_cmd_type(u32 tx_flags)
{
	/* set type for advanced descriptor with frame checksum insertion */
	__le32 cmd_type = cpu_to_le32(IXGBE_ADVTXD_DTYP_DATA |
				      IXGBE_ADVTXD_DCMD_IFCS |
				      IXGBE_ADVTXD_DCMD_DEXT);

	/* set HW vlan bit if vlan is present */
	if (tx_flags & IXGBE_TX_FLAGS_HW_VLAN)
		cmd_type |= cpu_to_le32(IXGBE_ADVTXD_DCMD_VLE);

	/* set segmentation enable bits for TSO/FSO */
#ifdef IXGBE_FCOE
	if (tx_flags & (IXGBE_TX_FLAGS_TSO | IXGBE_TX_FLAGS_FSO))
#else
	if (tx_flags & IXGBE_TX_FLAGS_TSO)
#endif
		cmd_type |= cpu_to_le32(IXGBE_ADVTXD_DCMD_TSE);
=======
	fceof_saidx |= itd->sa_idx;
	type_tucmd |= itd->flags | itd->trailer_len;

	ixgbe_tx_ctxtdesc(tx_ring, vlan_macip_lens, fceof_saidx, type_tucmd, 0);
}

#define IXGBE_SET_FLAG(_input, _flag, _result) \
	((_flag <= _result) ? \
	 ((u32)(_input & _flag) * (_result / _flag)) : \
	 ((u32)(_input & _flag) / (_flag / _result)))

static u32 ixgbe_tx_cmd_type(struct sk_buff *skb, u32 tx_flags)
{
	/* set type for advanced descriptor with frame checksum insertion */
	u32 cmd_type = IXGBE_ADVTXD_DTYP_DATA |
		       IXGBE_ADVTXD_DCMD_DEXT |
		       IXGBE_ADVTXD_DCMD_IFCS;

	/* set HW vlan bit if vlan is present */
	cmd_type |= IXGBE_SET_FLAG(tx_flags, IXGBE_TX_FLAGS_HW_VLAN,
				   IXGBE_ADVTXD_DCMD_VLE);

	/* set segmentation enable bits for TSO/FSO */
	cmd_type |= IXGBE_SET_FLAG(tx_flags, IXGBE_TX_FLAGS_TSO,
				   IXGBE_ADVTXD_DCMD_TSE);

	/* set timestamp bit if present */
	cmd_type |= IXGBE_SET_FLAG(tx_flags, IXGBE_TX_FLAGS_TSTAMP,
				   IXGBE_ADVTXD_MAC_TSTAMP);

	/* insert frame checksum */
	cmd_type ^= IXGBE_SET_FLAG(skb->no_fcs, 1, IXGBE_ADVTXD_DCMD_IFCS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return cmd_type;
}

static void ixgbe_tx_olinfo_status(union ixgbe_adv_tx_desc *tx_desc,
				   u32 tx_flags, unsigned int paylen)
{
<<<<<<< HEAD
	__le32 olinfo_status = cpu_to_le32(paylen << IXGBE_ADVTXD_PAYLEN_SHIFT);

	/* enable L4 checksum for TSO and TX checksum offload */
	if (tx_flags & IXGBE_TX_FLAGS_CSUM)
		olinfo_status |= cpu_to_le32(IXGBE_ADVTXD_POPTS_TXSM);

	/* enble IPv4 checksum for TSO */
	if (tx_flags & IXGBE_TX_FLAGS_IPV4)
		olinfo_status |= cpu_to_le32(IXGBE_ADVTXD_POPTS_IXSM);

	/* use index 1 context for TSO/FSO/FCOE */
#ifdef IXGBE_FCOE
	if (tx_flags & (IXGBE_TX_FLAGS_TSO | IXGBE_TX_FLAGS_FCOE))
#else
	if (tx_flags & IXGBE_TX_FLAGS_TSO)
#endif
		olinfo_status |= cpu_to_le32(1 << IXGBE_ADVTXD_IDX_SHIFT);
=======
	u32 olinfo_status = paylen << IXGBE_ADVTXD_PAYLEN_SHIFT;

	/* enable L4 checksum for TSO and TX checksum offload */
	olinfo_status |= IXGBE_SET_FLAG(tx_flags,
					IXGBE_TX_FLAGS_CSUM,
					IXGBE_ADVTXD_POPTS_TXSM);

	/* enable IPv4 checksum for TSO */
	olinfo_status |= IXGBE_SET_FLAG(tx_flags,
					IXGBE_TX_FLAGS_IPV4,
					IXGBE_ADVTXD_POPTS_IXSM);

	/* enable IPsec */
	olinfo_status |= IXGBE_SET_FLAG(tx_flags,
					IXGBE_TX_FLAGS_IPSEC,
					IXGBE_ADVTXD_POPTS_IPSEC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Check Context must be set if Tx switch is enabled, which it
	 * always is for case where virtual functions are running
	 */
<<<<<<< HEAD
#ifdef IXGBE_FCOE
	if (tx_flags & (IXGBE_TX_FLAGS_TXSW | IXGBE_TX_FLAGS_FCOE))
#else
	if (tx_flags & IXGBE_TX_FLAGS_TXSW)
#endif
		olinfo_status |= cpu_to_le32(IXGBE_ADVTXD_CC);

	tx_desc->read.olinfo_status = olinfo_status;
}

#define IXGBE_TXD_CMD (IXGBE_TXD_CMD_EOP | \
		       IXGBE_TXD_CMD_RS)

static void ixgbe_tx_map(struct ixgbe_ring *tx_ring,
			 struct ixgbe_tx_buffer *first,
			 const u8 hdr_len)
{
	dma_addr_t dma;
	struct sk_buff *skb = first->skb;
	struct ixgbe_tx_buffer *tx_buffer;
	union ixgbe_adv_tx_desc *tx_desc;
	struct skb_frag_struct *frag = &skb_shinfo(skb)->frags[0];
	unsigned int data_len = skb->data_len;
	unsigned int size = skb_headlen(skb);
	unsigned int paylen = skb->len - hdr_len;
	u32 tx_flags = first->tx_flags;
	__le32 cmd_type;
=======
	olinfo_status |= IXGBE_SET_FLAG(tx_flags,
					IXGBE_TX_FLAGS_CC,
					IXGBE_ADVTXD_CC);

	tx_desc->read.olinfo_status = cpu_to_le32(olinfo_status);
}

static int __ixgbe_maybe_stop_tx(struct ixgbe_ring *tx_ring, u16 size)
{
	if (!netif_subqueue_try_stop(tx_ring->netdev, tx_ring->queue_index,
				     ixgbe_desc_unused(tx_ring), size))
		return -EBUSY;

	++tx_ring->tx_stats.restart_queue;
	return 0;
}

static inline int ixgbe_maybe_stop_tx(struct ixgbe_ring *tx_ring, u16 size)
{
	if (likely(ixgbe_desc_unused(tx_ring) >= size))
		return 0;

	return __ixgbe_maybe_stop_tx(tx_ring, size);
}

static int ixgbe_tx_map(struct ixgbe_ring *tx_ring,
			struct ixgbe_tx_buffer *first,
			const u8 hdr_len)
{
	struct sk_buff *skb = first->skb;
	struct ixgbe_tx_buffer *tx_buffer;
	union ixgbe_adv_tx_desc *tx_desc;
	skb_frag_t *frag;
	dma_addr_t dma;
	unsigned int data_len, size;
	u32 tx_flags = first->tx_flags;
	u32 cmd_type = ixgbe_tx_cmd_type(skb, tx_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u16 i = tx_ring->next_to_use;

	tx_desc = IXGBE_TX_DESC(tx_ring, i);

<<<<<<< HEAD
	ixgbe_tx_olinfo_status(tx_desc, tx_flags, paylen);
	cmd_type = ixgbe_tx_cmd_type(tx_flags);
=======
	ixgbe_tx_olinfo_status(tx_desc, tx_flags, skb->len - hdr_len);

	size = skb_headlen(skb);
	data_len = skb->data_len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef IXGBE_FCOE
	if (tx_flags & IXGBE_TX_FLAGS_FCOE) {
		if (data_len < sizeof(struct fcoe_crc_eof)) {
			size -= sizeof(struct fcoe_crc_eof) - data_len;
			data_len = 0;
		} else {
			data_len -= sizeof(struct fcoe_crc_eof);
		}
	}

#endif
	dma = dma_map_single(tx_ring->dev, skb->data, size, DMA_TO_DEVICE);
<<<<<<< HEAD
	if (dma_mapping_error(tx_ring->dev, dma))
		goto dma_error;

	/* record length, and DMA address */
	dma_unmap_len_set(first, len, size);
	dma_unmap_addr_set(first, dma, dma);

	tx_desc->read.buffer_addr = cpu_to_le64(dma);

	for (;;) {
		while (unlikely(size > IXGBE_MAX_DATA_PER_TXD)) {
			tx_desc->read.cmd_type_len =
				cmd_type | cpu_to_le32(IXGBE_MAX_DATA_PER_TXD);
=======

	tx_buffer = first;

	for (frag = &skb_shinfo(skb)->frags[0];; frag++) {
		if (dma_mapping_error(tx_ring->dev, dma))
			goto dma_error;

		/* record length, and DMA address */
		dma_unmap_len_set(tx_buffer, len, size);
		dma_unmap_addr_set(tx_buffer, dma, dma);

		tx_desc->read.buffer_addr = cpu_to_le64(dma);

		while (unlikely(size > IXGBE_MAX_DATA_PER_TXD)) {
			tx_desc->read.cmd_type_len =
				cpu_to_le32(cmd_type ^ IXGBE_MAX_DATA_PER_TXD);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			i++;
			tx_desc++;
			if (i == tx_ring->count) {
				tx_desc = IXGBE_TX_DESC(tx_ring, 0);
				i = 0;
			}
<<<<<<< HEAD
=======
			tx_desc->read.olinfo_status = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			dma += IXGBE_MAX_DATA_PER_TXD;
			size -= IXGBE_MAX_DATA_PER_TXD;

			tx_desc->read.buffer_addr = cpu_to_le64(dma);
<<<<<<< HEAD
			tx_desc->read.olinfo_status = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		if (likely(!data_len))
			break;

<<<<<<< HEAD
		if (unlikely(skb->no_fcs))
			cmd_type &= ~(cpu_to_le32(IXGBE_ADVTXD_DCMD_IFCS));
		tx_desc->read.cmd_type_len = cmd_type | cpu_to_le32(size);
=======
		tx_desc->read.cmd_type_len = cpu_to_le32(cmd_type ^ size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		i++;
		tx_desc++;
		if (i == tx_ring->count) {
			tx_desc = IXGBE_TX_DESC(tx_ring, 0);
			i = 0;
		}
<<<<<<< HEAD
=======
		tx_desc->read.olinfo_status = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef IXGBE_FCOE
		size = min_t(unsigned int, data_len, skb_frag_size(frag));
#else
		size = skb_frag_size(frag);
#endif
		data_len -= size;

		dma = skb_frag_dma_map(tx_ring->dev, frag, 0, size,
				       DMA_TO_DEVICE);
<<<<<<< HEAD
		if (dma_mapping_error(tx_ring->dev, dma))
			goto dma_error;

		tx_buffer = &tx_ring->tx_buffer_info[i];
		dma_unmap_len_set(tx_buffer, len, size);
		dma_unmap_addr_set(tx_buffer, dma, dma);

		tx_desc->read.buffer_addr = cpu_to_le64(dma);
		tx_desc->read.olinfo_status = 0;

		frag++;
	}

	/* write last descriptor with RS and EOP bits */
	cmd_type |= cpu_to_le32(size) | cpu_to_le32(IXGBE_TXD_CMD);
	tx_desc->read.cmd_type_len = cmd_type;
=======

		tx_buffer = &tx_ring->tx_buffer_info[i];
	}

	/* write last descriptor with RS and EOP bits */
	cmd_type |= size | IXGBE_TXD_CMD;
	tx_desc->read.cmd_type_len = cpu_to_le32(cmd_type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	netdev_tx_sent_queue(txring_txq(tx_ring), first->bytecount);

	/* set the timestamp */
	first->time_stamp = jiffies;

<<<<<<< HEAD
=======
	skb_tx_timestamp(skb);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Force memory writes to complete before letting h/w know there
	 * are new descriptors to fetch.  (Only applicable for weak-ordered
	 * memory model archs, such as IA-64).
	 *
	 * We also need this memory barrier to make certain all of the
	 * status bits have been updated before next_to_watch is written.
	 */
	wmb();

	/* set next_to_watch value indicating a packet is present */
	first->next_to_watch = tx_desc;

	i++;
	if (i == tx_ring->count)
		i = 0;

	tx_ring->next_to_use = i;

<<<<<<< HEAD
	/* notify HW of packet */
	writel(i, tx_ring->tail);

	return;
=======
	ixgbe_maybe_stop_tx(tx_ring, DESC_NEEDED);

	if (netif_xmit_stopped(txring_txq(tx_ring)) || !netdev_xmit_more()) {
		writel(i, tx_ring->tail);
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
dma_error:
	dev_err(tx_ring->dev, "TX DMA map failed\n");

	/* clear dma mappings for failed tx_buffer_info map */
	for (;;) {
		tx_buffer = &tx_ring->tx_buffer_info[i];
<<<<<<< HEAD
		ixgbe_unmap_and_free_tx_resource(tx_ring, tx_buffer);
		if (tx_buffer == first)
			break;
		if (i == 0)
			i = tx_ring->count;
		i--;
	}

	tx_ring->next_to_use = i;
=======
		if (dma_unmap_len(tx_buffer, len))
			dma_unmap_page(tx_ring->dev,
				       dma_unmap_addr(tx_buffer, dma),
				       dma_unmap_len(tx_buffer, len),
				       DMA_TO_DEVICE);
		dma_unmap_len_set(tx_buffer, len, 0);
		if (tx_buffer == first)
			break;
		if (i == 0)
			i += tx_ring->count;
		i--;
	}

	dev_kfree_skb_any(first->skb);
	first->skb = NULL;

	tx_ring->next_to_use = i;

	return -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ixgbe_atr(struct ixgbe_ring *ring,
		      struct ixgbe_tx_buffer *first)
{
	struct ixgbe_q_vector *q_vector = ring->q_vector;
	union ixgbe_atr_hash_dword input = { .dword = 0 };
	union ixgbe_atr_hash_dword common = { .dword = 0 };
	union {
		unsigned char *network;
		struct iphdr *ipv4;
		struct ipv6hdr *ipv6;
	} hdr;
	struct tcphdr *th;
<<<<<<< HEAD
	__be16 vlan_id;
=======
	unsigned int hlen;
	struct sk_buff *skb;
	__be16 vlan_id;
	int l4_proto;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* if ring doesn't have a interrupt vector, cannot perform ATR */
	if (!q_vector)
		return;

	/* do nothing if sampling is disabled */
	if (!ring->atr_sample_rate)
		return;

	ring->atr_count++;

<<<<<<< HEAD
	/* snag network header to get L4 type and address */
	hdr.network = skb_network_header(first->skb);

	/* Currently only IPv4/IPv6 with TCP is supported */
	if ((first->protocol != __constant_htons(ETH_P_IPV6) ||
	     hdr.ipv6->nexthdr != IPPROTO_TCP) &&
	    (first->protocol != __constant_htons(ETH_P_IP) ||
	     hdr.ipv4->protocol != IPPROTO_TCP))
		return;

	th = tcp_hdr(first->skb);

	/* skip this packet since it is invalid or the socket is closing */
	if (!th || th->fin)
=======
	/* currently only IPv4/IPv6 with TCP is supported */
	if ((first->protocol != htons(ETH_P_IP)) &&
	    (first->protocol != htons(ETH_P_IPV6)))
		return;

	/* snag network header to get L4 type and address */
	skb = first->skb;
	hdr.network = skb_network_header(skb);
	if (unlikely(hdr.network <= skb->data))
		return;
	if (skb->encapsulation &&
	    first->protocol == htons(ETH_P_IP) &&
	    hdr.ipv4->protocol == IPPROTO_UDP) {
		struct ixgbe_adapter *adapter = q_vector->adapter;

		if (unlikely(skb_tail_pointer(skb) < hdr.network +
			     vxlan_headroom(0)))
			return;

		/* verify the port is recognized as VXLAN */
		if (adapter->vxlan_port &&
		    udp_hdr(skb)->dest == adapter->vxlan_port)
			hdr.network = skb_inner_network_header(skb);

		if (adapter->geneve_port &&
		    udp_hdr(skb)->dest == adapter->geneve_port)
			hdr.network = skb_inner_network_header(skb);
	}

	/* Make sure we have at least [minimum IPv4 header + TCP]
	 * or [IPv6 header] bytes
	 */
	if (unlikely(skb_tail_pointer(skb) < hdr.network + 40))
		return;

	/* Currently only IPv4/IPv6 with TCP is supported */
	switch (hdr.ipv4->version) {
	case IPVERSION:
		/* access ihl as u8 to avoid unaligned access on ia64 */
		hlen = (hdr.network[0] & 0x0F) << 2;
		l4_proto = hdr.ipv4->protocol;
		break;
	case 6:
		hlen = hdr.network - skb->data;
		l4_proto = ipv6_find_hdr(skb, &hlen, IPPROTO_TCP, NULL, NULL);
		hlen -= hdr.network - skb->data;
		break;
	default:
		return;
	}

	if (l4_proto != IPPROTO_TCP)
		return;

	if (unlikely(skb_tail_pointer(skb) < hdr.network +
		     hlen + sizeof(struct tcphdr)))
		return;

	th = (struct tcphdr *)(hdr.network + hlen);

	/* skip this packet since the socket is closing */
	if (th->fin)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	/* sample on all syn packets or once every atr sample count */
	if (!th->syn && (ring->atr_count < ring->atr_sample_rate))
		return;

	/* reset sample count */
	ring->atr_count = 0;

	vlan_id = htons(first->tx_flags >> IXGBE_TX_FLAGS_VLAN_SHIFT);

	/*
	 * src and dst are inverted, think how the receiver sees them
	 *
	 * The input is broken into two sections, a non-compressed section
	 * containing vm_pool, vlan_id, and flow_type.  The rest of the data
	 * is XORed together and stored in the compressed dword.
	 */
	input.formatted.vlan_id = vlan_id;

	/*
	 * since src port and flex bytes occupy the same word XOR them together
	 * and write the value to source port portion of compressed dword
	 */
	if (first->tx_flags & (IXGBE_TX_FLAGS_SW_VLAN | IXGBE_TX_FLAGS_HW_VLAN))
<<<<<<< HEAD
		common.port.src ^= th->dest ^ __constant_htons(ETH_P_8021Q);
=======
		common.port.src ^= th->dest ^ htons(ETH_P_8021Q);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		common.port.src ^= th->dest ^ first->protocol;
	common.port.dst ^= th->source;

<<<<<<< HEAD
	if (first->protocol == __constant_htons(ETH_P_IP)) {
		input.formatted.flow_type = IXGBE_ATR_FLOW_TYPE_TCPV4;
		common.ip ^= hdr.ipv4->saddr ^ hdr.ipv4->daddr;
	} else {
=======
	switch (hdr.ipv4->version) {
	case IPVERSION:
		input.formatted.flow_type = IXGBE_ATR_FLOW_TYPE_TCPV4;
		common.ip ^= hdr.ipv4->saddr ^ hdr.ipv4->daddr;
		break;
	case 6:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		input.formatted.flow_type = IXGBE_ATR_FLOW_TYPE_TCPV6;
		common.ip ^= hdr.ipv6->saddr.s6_addr32[0] ^
			     hdr.ipv6->saddr.s6_addr32[1] ^
			     hdr.ipv6->saddr.s6_addr32[2] ^
			     hdr.ipv6->saddr.s6_addr32[3] ^
			     hdr.ipv6->daddr.s6_addr32[0] ^
			     hdr.ipv6->daddr.s6_addr32[1] ^
			     hdr.ipv6->daddr.s6_addr32[2] ^
			     hdr.ipv6->daddr.s6_addr32[3];
<<<<<<< HEAD
	}

=======
		break;
	default:
		break;
	}

	if (hdr.network != skb_network_header(skb))
		input.formatted.flow_type |= IXGBE_ATR_L4TYPE_TUNNEL_MASK;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* This assumes the Rx queue and Tx queue are bound to the same CPU */
	ixgbe_fdir_add_signature_filter_82599(&q_vector->adapter->hw,
					      input, common, ring->queue_index);
}

<<<<<<< HEAD
static int __ixgbe_maybe_stop_tx(struct ixgbe_ring *tx_ring, u16 size)
{
	netif_stop_subqueue(tx_ring->netdev, tx_ring->queue_index);
	/* Herbert's original patch had:
	 *  smp_mb__after_netif_stop_queue();
	 * but since that doesn't exist yet, just open code it. */
	smp_mb();

	/* We need to check again in a case another CPU has just
	 * made room available. */
	if (likely(ixgbe_desc_unused(tx_ring) < size))
		return -EBUSY;

	/* A reprieve! - use start_queue because it doesn't call schedule */
	netif_start_subqueue(tx_ring->netdev, tx_ring->queue_index);
	++tx_ring->tx_stats.restart_queue;
	return 0;
}

static inline int ixgbe_maybe_stop_tx(struct ixgbe_ring *tx_ring, u16 size)
{
	if (likely(ixgbe_desc_unused(tx_ring) >= size))
		return 0;
	return __ixgbe_maybe_stop_tx(tx_ring, size);
}

static u16 ixgbe_select_queue(struct net_device *dev, struct sk_buff *skb)
{
	struct ixgbe_adapter *adapter = netdev_priv(dev);
	int txq = skb_rx_queue_recorded(skb) ? skb_get_rx_queue(skb) :
					       smp_processor_id();
#ifdef IXGBE_FCOE
	__be16 protocol = vlan_get_protocol(skb);

	if (((protocol == htons(ETH_P_FCOE)) ||
	    (protocol == htons(ETH_P_FIP))) &&
	    (adapter->flags & IXGBE_FLAG_FCOE_ENABLED)) {
		txq &= (adapter->ring_feature[RING_F_FCOE].indices - 1);
		txq += adapter->ring_feature[RING_F_FCOE].mask;
		return txq;
	}
#endif

	if (adapter->flags & IXGBE_FLAG_FDIR_HASH_CAPABLE) {
		while (unlikely(txq >= dev->real_num_tx_queues))
			txq -= dev->real_num_tx_queues;
		return txq;
	}

	return skb_tx_hash(dev, skb);
=======
#ifdef IXGBE_FCOE
static u16 ixgbe_select_queue(struct net_device *dev, struct sk_buff *skb,
			      struct net_device *sb_dev)
{
	struct ixgbe_adapter *adapter;
	struct ixgbe_ring_feature *f;
	int txq;

	if (sb_dev) {
		u8 tc = netdev_get_prio_tc_map(dev, skb->priority);
		struct net_device *vdev = sb_dev;

		txq = vdev->tc_to_txq[tc].offset;
		txq += reciprocal_scale(skb_get_hash(skb),
					vdev->tc_to_txq[tc].count);

		return txq;
	}

	/*
	 * only execute the code below if protocol is FCoE
	 * or FIP and we have FCoE enabled on the adapter
	 */
	switch (vlan_get_protocol(skb)) {
	case htons(ETH_P_FCOE):
	case htons(ETH_P_FIP):
		adapter = netdev_priv(dev);

		if (!sb_dev && (adapter->flags & IXGBE_FLAG_FCOE_ENABLED))
			break;
		fallthrough;
	default:
		return netdev_pick_tx(dev, skb, sb_dev);
	}

	f = &adapter->ring_feature[RING_F_FCOE];

	txq = skb_rx_queue_recorded(skb) ? skb_get_rx_queue(skb) :
					   smp_processor_id();

	while (txq >= f->indices)
		txq -= f->indices;

	return txq + f->offset;
}

#endif
int ixgbe_xmit_xdp_ring(struct ixgbe_ring *ring,
			struct xdp_frame *xdpf)
{
	struct skb_shared_info *sinfo = xdp_get_shared_info_from_frame(xdpf);
	u8 nr_frags = unlikely(xdp_frame_has_frags(xdpf)) ? sinfo->nr_frags : 0;
	u16 i = 0, index = ring->next_to_use;
	struct ixgbe_tx_buffer *tx_head = &ring->tx_buffer_info[index];
	struct ixgbe_tx_buffer *tx_buff = tx_head;
	union ixgbe_adv_tx_desc *tx_desc = IXGBE_TX_DESC(ring, index);
	u32 cmd_type, len = xdpf->len;
	void *data = xdpf->data;

	if (unlikely(ixgbe_desc_unused(ring) < 1 + nr_frags))
		return IXGBE_XDP_CONSUMED;

	tx_head->bytecount = xdp_get_frame_len(xdpf);
	tx_head->gso_segs = 1;
	tx_head->xdpf = xdpf;

	tx_desc->read.olinfo_status =
		cpu_to_le32(tx_head->bytecount << IXGBE_ADVTXD_PAYLEN_SHIFT);

	for (;;) {
		dma_addr_t dma;

		dma = dma_map_single(ring->dev, data, len, DMA_TO_DEVICE);
		if (dma_mapping_error(ring->dev, dma))
			goto unmap;

		dma_unmap_len_set(tx_buff, len, len);
		dma_unmap_addr_set(tx_buff, dma, dma);

		cmd_type = IXGBE_ADVTXD_DTYP_DATA | IXGBE_ADVTXD_DCMD_DEXT |
			   IXGBE_ADVTXD_DCMD_IFCS | len;
		tx_desc->read.cmd_type_len = cpu_to_le32(cmd_type);
		tx_desc->read.buffer_addr = cpu_to_le64(dma);
		tx_buff->protocol = 0;

		if (++index == ring->count)
			index = 0;

		if (i == nr_frags)
			break;

		tx_buff = &ring->tx_buffer_info[index];
		tx_desc = IXGBE_TX_DESC(ring, index);
		tx_desc->read.olinfo_status = 0;

		data = skb_frag_address(&sinfo->frags[i]);
		len = skb_frag_size(&sinfo->frags[i]);
		i++;
	}
	/* put descriptor type bits */
	tx_desc->read.cmd_type_len |= cpu_to_le32(IXGBE_TXD_CMD);

	/* Avoid any potential race with xdp_xmit and cleanup */
	smp_wmb();

	tx_head->next_to_watch = tx_desc;
	ring->next_to_use = index;

	return IXGBE_XDP_TX;

unmap:
	for (;;) {
		tx_buff = &ring->tx_buffer_info[index];
		if (dma_unmap_len(tx_buff, len))
			dma_unmap_page(ring->dev, dma_unmap_addr(tx_buff, dma),
				       dma_unmap_len(tx_buff, len),
				       DMA_TO_DEVICE);
		dma_unmap_len_set(tx_buff, len, 0);
		if (tx_buff == tx_head)
			break;

		if (!index)
			index += ring->count;
		index--;
	}

	return IXGBE_XDP_CONSUMED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

netdev_tx_t ixgbe_xmit_frame_ring(struct sk_buff *skb,
			  struct ixgbe_adapter *adapter,
			  struct ixgbe_ring *tx_ring)
{
	struct ixgbe_tx_buffer *first;
	int tso;
	u32 tx_flags = 0;
<<<<<<< HEAD
#if PAGE_SIZE > IXGBE_MAX_DATA_PER_TXD
	unsigned short f;
#endif
	u16 count = TXD_USE_COUNT(skb_headlen(skb));
=======
	unsigned short f;
	u16 count = TXD_USE_COUNT(skb_headlen(skb));
	struct ixgbe_ipsec_tx_data ipsec_tx = { 0 };
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__be16 protocol = skb->protocol;
	u8 hdr_len = 0;

	/*
	 * need: 1 descriptor per page * PAGE_SIZE/IXGBE_MAX_DATA_PER_TXD,
	 *       + 1 desc for skb_headlen/IXGBE_MAX_DATA_PER_TXD,
	 *       + 2 desc gap to keep tail from touching head,
	 *       + 1 desc for context descriptor,
	 * otherwise try next time
	 */
<<<<<<< HEAD
#if PAGE_SIZE > IXGBE_MAX_DATA_PER_TXD
	for (f = 0; f < skb_shinfo(skb)->nr_frags; f++)
		count += TXD_USE_COUNT(skb_shinfo(skb)->frags[f].size);
#else
	count += skb_shinfo(skb)->nr_frags;
#endif
=======
	for (f = 0; f < skb_shinfo(skb)->nr_frags; f++)
		count += TXD_USE_COUNT(skb_frag_size(
						&skb_shinfo(skb)->frags[f]));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ixgbe_maybe_stop_tx(tx_ring, count + 3)) {
		tx_ring->tx_stats.tx_busy++;
		return NETDEV_TX_BUSY;
	}

	/* record the location of the first descriptor for this packet */
	first = &tx_ring->tx_buffer_info[tx_ring->next_to_use];
	first->skb = skb;
	first->bytecount = skb->len;
	first->gso_segs = 1;

	/* if we have a HW VLAN tag being added default to the HW one */
<<<<<<< HEAD
	if (vlan_tx_tag_present(skb)) {
		tx_flags |= vlan_tx_tag_get(skb) << IXGBE_TX_FLAGS_VLAN_SHIFT;
		tx_flags |= IXGBE_TX_FLAGS_HW_VLAN;
	/* else if it is a SW VLAN check the next protocol and store the tag */
	} else if (protocol == __constant_htons(ETH_P_8021Q)) {
=======
	if (skb_vlan_tag_present(skb)) {
		tx_flags |= skb_vlan_tag_get(skb) << IXGBE_TX_FLAGS_VLAN_SHIFT;
		tx_flags |= IXGBE_TX_FLAGS_HW_VLAN;
	/* else if it is a SW VLAN check the next protocol and store the tag */
	} else if (protocol == htons(ETH_P_8021Q)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct vlan_hdr *vhdr, _vhdr;
		vhdr = skb_header_pointer(skb, ETH_HLEN, sizeof(_vhdr), &_vhdr);
		if (!vhdr)
			goto out_drop;

<<<<<<< HEAD
		protocol = vhdr->h_vlan_encapsulated_proto;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tx_flags |= ntohs(vhdr->h_vlan_TCI) <<
				  IXGBE_TX_FLAGS_VLAN_SHIFT;
		tx_flags |= IXGBE_TX_FLAGS_SW_VLAN;
	}
<<<<<<< HEAD
=======
	protocol = vlan_get_protocol(skb);

	if (unlikely(skb_shinfo(skb)->tx_flags & SKBTX_HW_TSTAMP) &&
	    adapter->ptp_clock) {
		if (adapter->tstamp_config.tx_type == HWTSTAMP_TX_ON &&
		    !test_and_set_bit_lock(__IXGBE_PTP_TX_IN_PROGRESS,
					   &adapter->state)) {
			skb_shinfo(skb)->tx_flags |= SKBTX_IN_PROGRESS;
			tx_flags |= IXGBE_TX_FLAGS_TSTAMP;

			/* schedule check for Tx timestamp */
			adapter->ptp_tx_skb = skb_get(skb);
			adapter->ptp_tx_start = jiffies;
			schedule_work(&adapter->ptp_tx_work);
		} else {
			adapter->tx_hwtstamp_skipped++;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_PCI_IOV
	/*
	 * Use the l2switch_enable flag - would be false if the DMA
	 * Tx switch had been disabled.
	 */
	if (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED)
<<<<<<< HEAD
		tx_flags |= IXGBE_TX_FLAGS_TXSW;
=======
		tx_flags |= IXGBE_TX_FLAGS_CC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
	/* DCB maps skb priorities 0-7 onto 3 bit PCP of VLAN tag. */
	if ((adapter->flags & IXGBE_FLAG_DCB_ENABLED) &&
	    ((tx_flags & (IXGBE_TX_FLAGS_HW_VLAN | IXGBE_TX_FLAGS_SW_VLAN)) ||
	     (skb->priority != TC_PRIO_CONTROL))) {
		tx_flags &= ~IXGBE_TX_FLAGS_VLAN_PRIO_MASK;
		tx_flags |= (skb->priority & 0x7) <<
					IXGBE_TX_FLAGS_VLAN_PRIO_SHIFT;
		if (tx_flags & IXGBE_TX_FLAGS_SW_VLAN) {
			struct vlan_ethhdr *vhdr;
<<<<<<< HEAD
			if (skb_header_cloned(skb) &&
			    pskb_expand_head(skb, 0, 0, GFP_ATOMIC))
				goto out_drop;
			vhdr = (struct vlan_ethhdr *)skb->data;
=======

			if (skb_cow_head(skb, 0))
				goto out_drop;
			vhdr = skb_vlan_eth_hdr(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			vhdr->h_vlan_TCI = htons(tx_flags >>
						 IXGBE_TX_FLAGS_VLAN_SHIFT);
		} else {
			tx_flags |= IXGBE_TX_FLAGS_HW_VLAN;
		}
	}

	/* record initial flags and protocol */
	first->tx_flags = tx_flags;
	first->protocol = protocol;

#ifdef IXGBE_FCOE
	/* setup tx offload for FCoE */
<<<<<<< HEAD
	if ((protocol == __constant_htons(ETH_P_FCOE)) &&
	    (adapter->flags & IXGBE_FLAG_FCOE_ENABLED)) {
=======
	if ((protocol == htons(ETH_P_FCOE)) &&
	    (tx_ring->netdev->features & (NETIF_F_FSO | NETIF_F_FCOE_CRC))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tso = ixgbe_fso(tx_ring, first, &hdr_len);
		if (tso < 0)
			goto out_drop;

		goto xmit_fcoe;
	}

#endif /* IXGBE_FCOE */
<<<<<<< HEAD
	tso = ixgbe_tso(tx_ring, first, &hdr_len);
	if (tso < 0)
		goto out_drop;
	else if (!tso)
		ixgbe_tx_csum(tx_ring, first);
=======

#ifdef CONFIG_IXGBE_IPSEC
	if (xfrm_offload(skb) &&
	    !ixgbe_ipsec_tx(tx_ring, first, &ipsec_tx))
		goto out_drop;
#endif
	tso = ixgbe_tso(tx_ring, first, &hdr_len, &ipsec_tx);
	if (tso < 0)
		goto out_drop;
	else if (!tso)
		ixgbe_tx_csum(tx_ring, first, &ipsec_tx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* add the ATR filter if ATR is on */
	if (test_bit(__IXGBE_TX_FDIR_INIT_DONE, &tx_ring->state))
		ixgbe_atr(tx_ring, first);

#ifdef IXGBE_FCOE
xmit_fcoe:
#endif /* IXGBE_FCOE */
<<<<<<< HEAD
	ixgbe_tx_map(tx_ring, first, hdr_len);

	ixgbe_maybe_stop_tx(tx_ring, DESC_NEEDED);
=======
	if (ixgbe_tx_map(tx_ring, first, hdr_len))
		goto cleanup_tx_timestamp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NETDEV_TX_OK;

out_drop:
	dev_kfree_skb_any(first->skb);
	first->skb = NULL;
<<<<<<< HEAD
=======
cleanup_tx_timestamp:
	if (unlikely(tx_flags & IXGBE_TX_FLAGS_TSTAMP)) {
		dev_kfree_skb_any(adapter->ptp_tx_skb);
		adapter->ptp_tx_skb = NULL;
		cancel_work_sync(&adapter->ptp_tx_work);
		clear_bit_unlock(__IXGBE_PTP_TX_IN_PROGRESS, &adapter->state);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NETDEV_TX_OK;
}

<<<<<<< HEAD
static netdev_tx_t ixgbe_xmit_frame(struct sk_buff *skb,
				    struct net_device *netdev)
=======
static netdev_tx_t __ixgbe_xmit_frame(struct sk_buff *skb,
				      struct net_device *netdev,
				      struct ixgbe_ring *ring)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	struct ixgbe_ring *tx_ring;

<<<<<<< HEAD
	if (skb->len <= 0) {
		dev_kfree_skb_any(skb);
		return NETDEV_TX_OK;
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * The minimum packet size for olinfo paylen is 17 so pad the skb
	 * in order to meet this minimum size requirement.
	 */
<<<<<<< HEAD
	if (skb->len < 17) {
		if (skb_padto(skb, 17))
			return NETDEV_TX_OK;
		skb->len = 17;
	}

	tx_ring = adapter->tx_ring[skb->queue_mapping];
	return ixgbe_xmit_frame_ring(skb, adapter, tx_ring);
}

=======
	if (skb_put_padto(skb, 17))
		return NETDEV_TX_OK;

	tx_ring = ring ? ring : adapter->tx_ring[skb_get_queue_mapping(skb)];
	if (unlikely(test_bit(__IXGBE_TX_DISABLED, &tx_ring->state)))
		return NETDEV_TX_BUSY;

	return ixgbe_xmit_frame_ring(skb, adapter, tx_ring);
}

static netdev_tx_t ixgbe_xmit_frame(struct sk_buff *skb,
				    struct net_device *netdev)
{
	return __ixgbe_xmit_frame(skb, netdev, NULL);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * ixgbe_set_mac - Change the Ethernet Address of the NIC
 * @netdev: network interface device structure
 * @p: pointer to an address structure
 *
 * Returns 0 on success, negative on failure
 **/
static int ixgbe_set_mac(struct net_device *netdev, void *p)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	struct ixgbe_hw *hw = &adapter->hw;
	struct sockaddr *addr = p;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

<<<<<<< HEAD
	memcpy(netdev->dev_addr, addr->sa_data, netdev->addr_len);
	memcpy(hw->mac.addr, addr->sa_data, netdev->addr_len);

	hw->mac.ops.set_rar(hw, 0, hw->mac.addr, adapter->num_vfs,
			    IXGBE_RAH_AV);
=======
	eth_hw_addr_set(netdev, addr->sa_data);
	memcpy(hw->mac.addr, addr->sa_data, netdev->addr_len);

	ixgbe_mac_set_default_filter(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int
ixgbe_mdio_read(struct net_device *netdev, int prtad, int devad, u16 addr)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	struct ixgbe_hw *hw = &adapter->hw;
	u16 value;
	int rc;

<<<<<<< HEAD
=======
	if (adapter->mii_bus) {
		int regnum = addr;

		if (devad != MDIO_DEVAD_NONE)
			return mdiobus_c45_read(adapter->mii_bus, prtad,
						devad, regnum);

		return mdiobus_read(adapter->mii_bus, prtad, regnum);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (prtad != hw->phy.mdio.prtad)
		return -EINVAL;
	rc = hw->phy.ops.read_reg(hw, addr, devad, &value);
	if (!rc)
		rc = value;
	return rc;
}

static int ixgbe_mdio_write(struct net_device *netdev, int prtad, int devad,
			    u16 addr, u16 value)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	struct ixgbe_hw *hw = &adapter->hw;

<<<<<<< HEAD
=======
	if (adapter->mii_bus) {
		int regnum = addr;

		if (devad != MDIO_DEVAD_NONE)
			return mdiobus_c45_write(adapter->mii_bus, prtad, devad,
						 regnum, value);

		return mdiobus_write(adapter->mii_bus, prtad, regnum, value);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (prtad != hw->phy.mdio.prtad)
		return -EINVAL;
	return hw->phy.ops.write_reg(hw, addr, devad, value);
}

static int ixgbe_ioctl(struct net_device *netdev, struct ifreq *req, int cmd)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);

<<<<<<< HEAD
	return mdio_mii_ioctl(&adapter->hw.phy.mdio, if_mii(req), cmd);
=======
	switch (cmd) {
	case SIOCSHWTSTAMP:
		return ixgbe_ptp_set_ts_config(adapter, req);
	case SIOCGHWTSTAMP:
		return ixgbe_ptp_get_ts_config(adapter, req);
	case SIOCGMIIPHY:
		if (!adapter->hw.phy.ops.read_reg)
			return -EOPNOTSUPP;
		fallthrough;
	default:
		return mdio_mii_ioctl(&adapter->hw.phy.mdio, if_mii(req), cmd);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_add_sanmac_netdev - Add the SAN MAC address to the corresponding
 * netdev->dev_addrs
<<<<<<< HEAD
 * @netdev: network interface device structure
=======
 * @dev: network interface device structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Returns non-zero on failure
 **/
static int ixgbe_add_sanmac_netdev(struct net_device *dev)
{
	int err = 0;
	struct ixgbe_adapter *adapter = netdev_priv(dev);
<<<<<<< HEAD
	struct ixgbe_mac_info *mac = &adapter->hw.mac;

	if (is_valid_ether_addr(mac->san_addr)) {
		rtnl_lock();
		err = dev_addr_add(dev, mac->san_addr, NETDEV_HW_ADDR_T_SAN);
		rtnl_unlock();
=======
	struct ixgbe_hw *hw = &adapter->hw;

	if (is_valid_ether_addr(hw->mac.san_addr)) {
		rtnl_lock();
		err = dev_addr_add(dev, hw->mac.san_addr, NETDEV_HW_ADDR_T_SAN);
		rtnl_unlock();

		/* update SAN MAC vmdq pool selection */
		hw->mac.ops.set_vmdq_san_mac(hw, VMDQ_P(0));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return err;
}

/**
 * ixgbe_del_sanmac_netdev - Removes the SAN MAC address to the corresponding
 * netdev->dev_addrs
<<<<<<< HEAD
 * @netdev: network interface device structure
=======
 * @dev: network interface device structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Returns non-zero on failure
 **/
static int ixgbe_del_sanmac_netdev(struct net_device *dev)
{
	int err = 0;
	struct ixgbe_adapter *adapter = netdev_priv(dev);
	struct ixgbe_mac_info *mac = &adapter->hw.mac;

	if (is_valid_ether_addr(mac->san_addr)) {
		rtnl_lock();
		err = dev_addr_del(dev, mac->san_addr, NETDEV_HW_ADDR_T_SAN);
		rtnl_unlock();
	}
	return err;
}

<<<<<<< HEAD
#ifdef CONFIG_NET_POLL_CONTROLLER
/*
 * Polling 'interrupt' - used by things like netconsole to send skbs
 * without having to re-enable interrupts. It's not called while
 * the interrupt routine is executing.
 */
static void ixgbe_netpoll(struct net_device *netdev)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	int i;

	/* if interface is down do nothing */
	if (test_bit(__IXGBE_DOWN, &adapter->state))
		return;

	adapter->flags |= IXGBE_FLAG_IN_NETPOLL;
	if (adapter->flags & IXGBE_FLAG_MSIX_ENABLED) {
		int num_q_vectors = adapter->num_msix_vectors - NON_Q_VECTORS;
		for (i = 0; i < num_q_vectors; i++) {
			struct ixgbe_q_vector *q_vector = adapter->q_vector[i];
			ixgbe_msix_clean_rings(0, q_vector);
		}
	} else {
		ixgbe_intr(adapter->pdev->irq, netdev);
	}
	adapter->flags &= ~IXGBE_FLAG_IN_NETPOLL;
}

#endif
static struct rtnl_link_stats64 *ixgbe_get_stats64(struct net_device *netdev,
						   struct rtnl_link_stats64 *stats)
=======
static void ixgbe_get_ring_stats64(struct rtnl_link_stats64 *stats,
				   struct ixgbe_ring *ring)
{
	u64 bytes, packets;
	unsigned int start;

	if (ring) {
		do {
			start = u64_stats_fetch_begin(&ring->syncp);
			packets = ring->stats.packets;
			bytes   = ring->stats.bytes;
		} while (u64_stats_fetch_retry(&ring->syncp, start));
		stats->tx_packets += packets;
		stats->tx_bytes   += bytes;
	}
}

static void ixgbe_get_stats64(struct net_device *netdev,
			      struct rtnl_link_stats64 *stats)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	int i;

	rcu_read_lock();
	for (i = 0; i < adapter->num_rx_queues; i++) {
<<<<<<< HEAD
		struct ixgbe_ring *ring = ACCESS_ONCE(adapter->rx_ring[i]);
=======
		struct ixgbe_ring *ring = READ_ONCE(adapter->rx_ring[i]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		u64 bytes, packets;
		unsigned int start;

		if (ring) {
			do {
<<<<<<< HEAD
				start = u64_stats_fetch_begin_irq(&ring->syncp);
				packets = ring->stats.packets;
				bytes   = ring->stats.bytes;
			} while (u64_stats_fetch_retry_irq(&ring->syncp, start));
=======
				start = u64_stats_fetch_begin(&ring->syncp);
				packets = ring->stats.packets;
				bytes   = ring->stats.bytes;
			} while (u64_stats_fetch_retry(&ring->syncp, start));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			stats->rx_packets += packets;
			stats->rx_bytes   += bytes;
		}
	}

	for (i = 0; i < adapter->num_tx_queues; i++) {
<<<<<<< HEAD
		struct ixgbe_ring *ring = ACCESS_ONCE(adapter->tx_ring[i]);
		u64 bytes, packets;
		unsigned int start;

		if (ring) {
			do {
				start = u64_stats_fetch_begin_irq(&ring->syncp);
				packets = ring->stats.packets;
				bytes   = ring->stats.bytes;
			} while (u64_stats_fetch_retry_irq(&ring->syncp, start));
			stats->tx_packets += packets;
			stats->tx_bytes   += bytes;
		}
	}
	rcu_read_unlock();
=======
		struct ixgbe_ring *ring = READ_ONCE(adapter->tx_ring[i]);

		ixgbe_get_ring_stats64(stats, ring);
	}
	for (i = 0; i < adapter->num_xdp_queues; i++) {
		struct ixgbe_ring *ring = READ_ONCE(adapter->xdp_ring[i]);

		ixgbe_get_ring_stats64(stats, ring);
	}
	rcu_read_unlock();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* following stats updated by ixgbe_watchdog_task() */
	stats->multicast	= netdev->stats.multicast;
	stats->rx_errors	= netdev->stats.rx_errors;
	stats->rx_length_errors	= netdev->stats.rx_length_errors;
	stats->rx_crc_errors	= netdev->stats.rx_crc_errors;
	stats->rx_missed_errors	= netdev->stats.rx_missed_errors;
<<<<<<< HEAD
	return stats;
}

#ifdef CONFIG_IXGBE_DCB
/* ixgbe_validate_rtr - verify 802.1Qp to Rx packet buffer mapping is valid.
 * #adapter: pointer to ixgbe_adapter
=======
}

static int ixgbe_ndo_get_vf_stats(struct net_device *netdev, int vf,
				  struct ifla_vf_stats *vf_stats)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);

	if (vf < 0 || vf >= adapter->num_vfs)
		return -EINVAL;

	vf_stats->rx_packets = adapter->vfinfo[vf].vfstats.gprc;
	vf_stats->rx_bytes   = adapter->vfinfo[vf].vfstats.gorc;
	vf_stats->tx_packets = adapter->vfinfo[vf].vfstats.gptc;
	vf_stats->tx_bytes   = adapter->vfinfo[vf].vfstats.gotc;
	vf_stats->multicast  = adapter->vfinfo[vf].vfstats.mprc;

	return 0;
}

#ifdef CONFIG_IXGBE_DCB
/**
 * ixgbe_validate_rtr - verify 802.1Qp to Rx packet buffer mapping is valid.
 * @adapter: pointer to ixgbe_adapter
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @tc: number of traffic classes currently enabled
 *
 * Configure a valid 802.1Qp to Rx packet buffer mapping ie confirm
 * 802.1Q priority maps to a packet buffer that exists.
 */
static void ixgbe_validate_rtr(struct ixgbe_adapter *adapter, u8 tc)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u32 reg, rsave;
	int i;

	/* 82598 have a static priority to TC mapping that can not
	 * be changed so no validation is needed.
	 */
	if (hw->mac.type == ixgbe_mac_82598EB)
		return;

	reg = IXGBE_READ_REG(hw, IXGBE_RTRUP2TC);
	rsave = reg;

	for (i = 0; i < MAX_TRAFFIC_CLASS; i++) {
		u8 up2tc = reg >> (i * IXGBE_RTRUP2TC_UP_SHIFT);

		/* If up2tc is out of bounds default to zero */
		if (up2tc > tc)
			reg &= ~(0x7 << IXGBE_RTRUP2TC_UP_SHIFT);
	}

	if (reg != rsave)
		IXGBE_WRITE_REG(hw, IXGBE_RTRUP2TC, reg);

	return;
}

<<<<<<< HEAD
/* ixgbe_setup_tc - routine to configure net_device for multiple traffic
 * classes.
 *
 * @netdev: net device to configure
=======
/**
 * ixgbe_set_prio_tc_map - Configure netdev prio tc map
 * @adapter: Pointer to adapter struct
 *
 * Populate the netdev user priority to tc map
 */
static void ixgbe_set_prio_tc_map(struct ixgbe_adapter *adapter)
{
	struct net_device *dev = adapter->netdev;
	struct ixgbe_dcb_config *dcb_cfg = &adapter->dcb_cfg;
	struct ieee_ets *ets = adapter->ixgbe_ieee_ets;
	u8 prio;

	for (prio = 0; prio < MAX_USER_PRIORITY; prio++) {
		u8 tc = 0;

		if (adapter->dcbx_cap & DCB_CAP_DCBX_VER_CEE)
			tc = ixgbe_dcb_get_tc_from_up(dcb_cfg, 0, prio);
		else if (ets)
			tc = ets->prio_tc[prio];

		netdev_set_prio_tc_map(dev, prio, tc);
	}
}

#endif /* CONFIG_IXGBE_DCB */
static int ixgbe_reassign_macvlan_pool(struct net_device *vdev,
				       struct netdev_nested_priv *priv)
{
	struct ixgbe_adapter *adapter = (struct ixgbe_adapter *)priv->data;
	struct ixgbe_fwd_adapter *accel;
	int pool;

	/* we only care about macvlans... */
	if (!netif_is_macvlan(vdev))
		return 0;

	/* that have hardware offload enabled... */
	accel = macvlan_accel_priv(vdev);
	if (!accel)
		return 0;

	/* If we can relocate to a different bit do so */
	pool = find_first_zero_bit(adapter->fwd_bitmask, adapter->num_rx_pools);
	if (pool < adapter->num_rx_pools) {
		set_bit(pool, adapter->fwd_bitmask);
		accel->pool = pool;
		return 0;
	}

	/* if we cannot find a free pool then disable the offload */
	netdev_err(vdev, "L2FW offload disabled due to lack of queue resources\n");
	macvlan_release_l2fw_offload(vdev);

	/* unbind the queues and drop the subordinate channel config */
	netdev_unbind_sb_channel(adapter->netdev, vdev);
	netdev_set_sb_channel(vdev, 0);

	kfree(accel);

	return 0;
}

static void ixgbe_defrag_macvlan_pools(struct net_device *dev)
{
	struct ixgbe_adapter *adapter = netdev_priv(dev);
	struct netdev_nested_priv priv = {
		.data = (void *)adapter,
	};

	/* flush any stale bits out of the fwd bitmask */
	bitmap_clear(adapter->fwd_bitmask, 1, 63);

	/* walk through upper devices reassigning pools */
	netdev_walk_all_upper_dev_rcu(dev, ixgbe_reassign_macvlan_pool,
				      &priv);
}

/**
 * ixgbe_setup_tc - configure net_device for multiple traffic classes
 *
 * @dev: net device to configure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @tc: number of traffic classes to enable
 */
int ixgbe_setup_tc(struct net_device *dev, u8 tc)
{
	struct ixgbe_adapter *adapter = netdev_priv(dev);
	struct ixgbe_hw *hw = &adapter->hw;

<<<<<<< HEAD
	/* Multiple traffic classes requires multiple queues */
	if (!(adapter->flags & IXGBE_FLAG_MSIX_ENABLED)) {
		e_err(drv, "Enable failed, needs MSI-X\n");
		return -EINVAL;
	}

	/* Hardware supports up to 8 traffic classes */
	if (tc > adapter->dcb_cfg.num_tcs.pg_tcs ||
	    (hw->mac.type == ixgbe_mac_82598EB &&
	     tc < MAX_TRAFFIC_CLASS))
=======
	/* Hardware supports up to 8 traffic classes */
	if (tc > adapter->dcb_cfg.num_tcs.pg_tcs)
		return -EINVAL;

	if (hw->mac.type == ixgbe_mac_82598EB && tc && tc < MAX_TRAFFIC_CLASS)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	/* Hardware has to reinitialize queues and interrupts to
	 * match packet buffer alignment. Unfortunately, the
	 * hardware is not flexible enough to do this dynamically.
	 */
	if (netif_running(dev))
		ixgbe_close(dev);
<<<<<<< HEAD
	ixgbe_clear_interrupt_scheme(adapter);

	if (tc) {
		netdev_set_num_tc(dev, tc);
		adapter->last_lfc_mode = adapter->hw.fc.current_mode;
		adapter->flags |= IXGBE_FLAG_DCB_ENABLED;
		adapter->flags &= ~IXGBE_FLAG_FDIR_HASH_CAPABLE;

		if (adapter->hw.mac.type == ixgbe_mac_82598EB)
			adapter->hw.fc.requested_mode = ixgbe_fc_none;
	} else {
		netdev_reset_tc(dev);
		adapter->hw.fc.requested_mode = adapter->last_lfc_mode;

		adapter->flags &= ~IXGBE_FLAG_DCB_ENABLED;
		adapter->flags |= IXGBE_FLAG_FDIR_HASH_CAPABLE;
=======
	else
		ixgbe_reset(adapter);

	ixgbe_clear_interrupt_scheme(adapter);

#ifdef CONFIG_IXGBE_DCB
	if (tc) {
		if (adapter->xdp_prog) {
			e_warn(probe, "DCB is not supported with XDP\n");

			ixgbe_init_interrupt_scheme(adapter);
			if (netif_running(dev))
				ixgbe_open(dev);
			return -EINVAL;
		}

		netdev_set_num_tc(dev, tc);
		ixgbe_set_prio_tc_map(adapter);

		adapter->hw_tcs = tc;
		adapter->flags |= IXGBE_FLAG_DCB_ENABLED;

		if (adapter->hw.mac.type == ixgbe_mac_82598EB) {
			adapter->last_lfc_mode = adapter->hw.fc.requested_mode;
			adapter->hw.fc.requested_mode = ixgbe_fc_none;
		}
	} else {
		netdev_reset_tc(dev);

		if (adapter->hw.mac.type == ixgbe_mac_82598EB)
			adapter->hw.fc.requested_mode = adapter->last_lfc_mode;

		adapter->flags &= ~IXGBE_FLAG_DCB_ENABLED;
		adapter->hw_tcs = tc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		adapter->temp_dcb_cfg.pfc_mode_enable = false;
		adapter->dcb_cfg.pfc_mode_enable = false;
	}

<<<<<<< HEAD
	ixgbe_init_interrupt_scheme(adapter);
	ixgbe_validate_rtr(adapter, tc);
	if (netif_running(dev))
		ixgbe_open(dev);
=======
	ixgbe_validate_rtr(adapter, tc);

#endif /* CONFIG_IXGBE_DCB */
	ixgbe_init_interrupt_scheme(adapter);

	ixgbe_defrag_macvlan_pools(dev);

	if (netif_running(dev))
		return ixgbe_open(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
#endif /* CONFIG_IXGBE_DCB */
=======
static int ixgbe_delete_clsu32(struct ixgbe_adapter *adapter,
			       struct tc_cls_u32_offload *cls)
{
	u32 hdl = cls->knode.handle;
	u32 uhtid = TC_U32_USERHTID(cls->knode.handle);
	u32 loc = cls->knode.handle & 0xfffff;
	int err = 0, i, j;
	struct ixgbe_jump_table *jump = NULL;

	if (loc > IXGBE_MAX_HW_ENTRIES)
		return -EINVAL;

	if ((uhtid != 0x800) && (uhtid >= IXGBE_MAX_LINK_HANDLE))
		return -EINVAL;

	/* Clear this filter in the link data it is associated with */
	if (uhtid != 0x800) {
		jump = adapter->jump_tables[uhtid];
		if (!jump)
			return -EINVAL;
		if (!test_bit(loc - 1, jump->child_loc_map))
			return -EINVAL;
		clear_bit(loc - 1, jump->child_loc_map);
	}

	/* Check if the filter being deleted is a link */
	for (i = 1; i < IXGBE_MAX_LINK_HANDLE; i++) {
		jump = adapter->jump_tables[i];
		if (jump && jump->link_hdl == hdl) {
			/* Delete filters in the hardware in the child hash
			 * table associated with this link
			 */
			for (j = 0; j < IXGBE_MAX_HW_ENTRIES; j++) {
				if (!test_bit(j, jump->child_loc_map))
					continue;
				spin_lock(&adapter->fdir_perfect_lock);
				err = ixgbe_update_ethtool_fdir_entry(adapter,
								      NULL,
								      j + 1);
				spin_unlock(&adapter->fdir_perfect_lock);
				clear_bit(j, jump->child_loc_map);
			}
			/* Remove resources for this link */
			kfree(jump->input);
			kfree(jump->mask);
			kfree(jump);
			adapter->jump_tables[i] = NULL;
			return err;
		}
	}

	spin_lock(&adapter->fdir_perfect_lock);
	err = ixgbe_update_ethtool_fdir_entry(adapter, NULL, loc);
	spin_unlock(&adapter->fdir_perfect_lock);
	return err;
}

static int ixgbe_configure_clsu32_add_hnode(struct ixgbe_adapter *adapter,
					    struct tc_cls_u32_offload *cls)
{
	u32 uhtid = TC_U32_USERHTID(cls->hnode.handle);

	if (uhtid >= IXGBE_MAX_LINK_HANDLE)
		return -EINVAL;

	/* This ixgbe devices do not support hash tables at the moment
	 * so abort when given hash tables.
	 */
	if (cls->hnode.divisor > 0)
		return -EINVAL;

	set_bit(uhtid - 1, &adapter->tables);
	return 0;
}

static int ixgbe_configure_clsu32_del_hnode(struct ixgbe_adapter *adapter,
					    struct tc_cls_u32_offload *cls)
{
	u32 uhtid = TC_U32_USERHTID(cls->hnode.handle);

	if (uhtid >= IXGBE_MAX_LINK_HANDLE)
		return -EINVAL;

	clear_bit(uhtid - 1, &adapter->tables);
	return 0;
}

#ifdef CONFIG_NET_CLS_ACT
struct upper_walk_data {
	struct ixgbe_adapter *adapter;
	u64 action;
	int ifindex;
	u8 queue;
};

static int get_macvlan_queue(struct net_device *upper,
			     struct netdev_nested_priv *priv)
{
	if (netif_is_macvlan(upper)) {
		struct ixgbe_fwd_adapter *vadapter = macvlan_accel_priv(upper);
		struct ixgbe_adapter *adapter;
		struct upper_walk_data *data;
		int ifindex;

		data = (struct upper_walk_data *)priv->data;
		ifindex = data->ifindex;
		adapter = data->adapter;
		if (vadapter && upper->ifindex == ifindex) {
			data->queue = adapter->rx_ring[vadapter->rx_base_queue]->reg_idx;
			data->action = data->queue;
			return 1;
		}
	}

	return 0;
}

static int handle_redirect_action(struct ixgbe_adapter *adapter, int ifindex,
				  u8 *queue, u64 *action)
{
	struct ixgbe_ring_feature *vmdq = &adapter->ring_feature[RING_F_VMDQ];
	unsigned int num_vfs = adapter->num_vfs, vf;
	struct netdev_nested_priv priv;
	struct upper_walk_data data;
	struct net_device *upper;

	/* redirect to a SRIOV VF */
	for (vf = 0; vf < num_vfs; ++vf) {
		upper = pci_get_drvdata(adapter->vfinfo[vf].vfdev);
		if (upper->ifindex == ifindex) {
			*queue = vf * __ALIGN_MASK(1, ~vmdq->mask);
			*action = vf + 1;
			*action <<= ETHTOOL_RX_FLOW_SPEC_RING_VF_OFF;
			return 0;
		}
	}

	/* redirect to a offloaded macvlan netdev */
	data.adapter = adapter;
	data.ifindex = ifindex;
	data.action = 0;
	data.queue = 0;
	priv.data = (void *)&data;
	if (netdev_walk_all_upper_dev_rcu(adapter->netdev,
					  get_macvlan_queue, &priv)) {
		*action = data.action;
		*queue = data.queue;

		return 0;
	}

	return -EINVAL;
}

static int parse_tc_actions(struct ixgbe_adapter *adapter,
			    struct tcf_exts *exts, u64 *action, u8 *queue)
{
	const struct tc_action *a;
	int i;

	if (!tcf_exts_has_actions(exts))
		return -EINVAL;

	tcf_exts_for_each_action(i, a, exts) {
		/* Drop action */
		if (is_tcf_gact_shot(a)) {
			*action = IXGBE_FDIR_DROP_QUEUE;
			*queue = IXGBE_FDIR_DROP_QUEUE;
			return 0;
		}

		/* Redirect to a VF or a offloaded macvlan */
		if (is_tcf_mirred_egress_redirect(a)) {
			struct net_device *dev = tcf_mirred_dev(a);

			if (!dev)
				return -EINVAL;
			return handle_redirect_action(adapter, dev->ifindex,
						      queue, action);
		}

		return -EINVAL;
	}

	return -EINVAL;
}
#else
static int parse_tc_actions(struct ixgbe_adapter *adapter,
			    struct tcf_exts *exts, u64 *action, u8 *queue)
{
	return -EINVAL;
}
#endif /* CONFIG_NET_CLS_ACT */

static int ixgbe_clsu32_build_input(struct ixgbe_fdir_filter *input,
				    union ixgbe_atr_input *mask,
				    struct tc_cls_u32_offload *cls,
				    struct ixgbe_mat_field *field_ptr,
				    struct ixgbe_nexthdr *nexthdr)
{
	int i, j, off;
	__be32 val, m;
	bool found_entry = false, found_jump_field = false;

	for (i = 0; i < cls->knode.sel->nkeys; i++) {
		off = cls->knode.sel->keys[i].off;
		val = cls->knode.sel->keys[i].val;
		m = cls->knode.sel->keys[i].mask;

		for (j = 0; field_ptr[j].val; j++) {
			if (field_ptr[j].off == off) {
				field_ptr[j].val(input, mask, (__force u32)val,
						 (__force u32)m);
				input->filter.formatted.flow_type |=
					field_ptr[j].type;
				found_entry = true;
				break;
			}
		}
		if (nexthdr) {
			if (nexthdr->off == cls->knode.sel->keys[i].off &&
			    nexthdr->val ==
			    (__force u32)cls->knode.sel->keys[i].val &&
			    nexthdr->mask ==
			    (__force u32)cls->knode.sel->keys[i].mask)
				found_jump_field = true;
			else
				continue;
		}
	}

	if (nexthdr && !found_jump_field)
		return -EINVAL;

	if (!found_entry)
		return 0;

	mask->formatted.flow_type = IXGBE_ATR_L4TYPE_IPV6_MASK |
				    IXGBE_ATR_L4TYPE_MASK;

	if (input->filter.formatted.flow_type == IXGBE_ATR_FLOW_TYPE_IPV4)
		mask->formatted.flow_type &= IXGBE_ATR_L4TYPE_IPV6_MASK;

	return 0;
}

static int ixgbe_configure_clsu32(struct ixgbe_adapter *adapter,
				  struct tc_cls_u32_offload *cls)
{
	__be16 protocol = cls->common.protocol;
	u32 loc = cls->knode.handle & 0xfffff;
	struct ixgbe_hw *hw = &adapter->hw;
	struct ixgbe_mat_field *field_ptr;
	struct ixgbe_fdir_filter *input = NULL;
	union ixgbe_atr_input *mask = NULL;
	struct ixgbe_jump_table *jump = NULL;
	int i, err = -EINVAL;
	u8 queue;
	u32 uhtid, link_uhtid;

	uhtid = TC_U32_USERHTID(cls->knode.handle);
	link_uhtid = TC_U32_USERHTID(cls->knode.link_handle);

	/* At the moment cls_u32 jumps to network layer and skips past
	 * L2 headers. The canonical method to match L2 frames is to use
	 * negative values. However this is error prone at best but really
	 * just broken because there is no way to "know" what sort of hdr
	 * is in front of the network layer. Fix cls_u32 to support L2
	 * headers when needed.
	 */
	if (protocol != htons(ETH_P_IP))
		return err;

	if (loc >= ((1024 << adapter->fdir_pballoc) - 2)) {
		e_err(drv, "Location out of range\n");
		return err;
	}

	/* cls u32 is a graph starting at root node 0x800. The driver tracks
	 * links and also the fields used to advance the parser across each
	 * link (e.g. nexthdr/eat parameters from 'tc'). This way we can map
	 * the u32 graph onto the hardware parse graph denoted in ixgbe_model.h
	 * To add support for new nodes update ixgbe_model.h parse structures
	 * this function _should_ be generic try not to hardcode values here.
	 */
	if (uhtid == 0x800) {
		field_ptr = (adapter->jump_tables[0])->mat;
	} else {
		if (uhtid >= IXGBE_MAX_LINK_HANDLE)
			return err;
		if (!adapter->jump_tables[uhtid])
			return err;
		field_ptr = (adapter->jump_tables[uhtid])->mat;
	}

	if (!field_ptr)
		return err;

	/* At this point we know the field_ptr is valid and need to either
	 * build cls_u32 link or attach filter. Because adding a link to
	 * a handle that does not exist is invalid and the same for adding
	 * rules to handles that don't exist.
	 */

	if (link_uhtid) {
		struct ixgbe_nexthdr *nexthdr = ixgbe_ipv4_jumps;

		if (link_uhtid >= IXGBE_MAX_LINK_HANDLE)
			return err;

		if (!test_bit(link_uhtid - 1, &adapter->tables))
			return err;

		/* Multiple filters as links to the same hash table are not
		 * supported. To add a new filter with the same next header
		 * but different match/jump conditions, create a new hash table
		 * and link to it.
		 */
		if (adapter->jump_tables[link_uhtid] &&
		    (adapter->jump_tables[link_uhtid])->link_hdl) {
			e_err(drv, "Link filter exists for link: %x\n",
			      link_uhtid);
			return err;
		}

		for (i = 0; nexthdr[i].jump; i++) {
			if (nexthdr[i].o != cls->knode.sel->offoff ||
			    nexthdr[i].s != cls->knode.sel->offshift ||
			    nexthdr[i].m !=
			    (__force u32)cls->knode.sel->offmask)
				return err;

			jump = kzalloc(sizeof(*jump), GFP_KERNEL);
			if (!jump)
				return -ENOMEM;
			input = kzalloc(sizeof(*input), GFP_KERNEL);
			if (!input) {
				err = -ENOMEM;
				goto free_jump;
			}
			mask = kzalloc(sizeof(*mask), GFP_KERNEL);
			if (!mask) {
				err = -ENOMEM;
				goto free_input;
			}
			jump->input = input;
			jump->mask = mask;
			jump->link_hdl = cls->knode.handle;

			err = ixgbe_clsu32_build_input(input, mask, cls,
						       field_ptr, &nexthdr[i]);
			if (!err) {
				jump->mat = nexthdr[i].jump;
				adapter->jump_tables[link_uhtid] = jump;
				break;
			} else {
				kfree(mask);
				kfree(input);
				kfree(jump);
			}
		}
		return 0;
	}

	input = kzalloc(sizeof(*input), GFP_KERNEL);
	if (!input)
		return -ENOMEM;
	mask = kzalloc(sizeof(*mask), GFP_KERNEL);
	if (!mask) {
		err = -ENOMEM;
		goto free_input;
	}

	if ((uhtid != 0x800) && (adapter->jump_tables[uhtid])) {
		if ((adapter->jump_tables[uhtid])->input)
			memcpy(input, (adapter->jump_tables[uhtid])->input,
			       sizeof(*input));
		if ((adapter->jump_tables[uhtid])->mask)
			memcpy(mask, (adapter->jump_tables[uhtid])->mask,
			       sizeof(*mask));

		/* Lookup in all child hash tables if this location is already
		 * filled with a filter
		 */
		for (i = 1; i < IXGBE_MAX_LINK_HANDLE; i++) {
			struct ixgbe_jump_table *link = adapter->jump_tables[i];

			if (link && (test_bit(loc - 1, link->child_loc_map))) {
				e_err(drv, "Filter exists in location: %x\n",
				      loc);
				err = -EINVAL;
				goto err_out;
			}
		}
	}
	err = ixgbe_clsu32_build_input(input, mask, cls, field_ptr, NULL);
	if (err)
		goto err_out;

	err = parse_tc_actions(adapter, cls->knode.exts, &input->action,
			       &queue);
	if (err < 0)
		goto err_out;

	input->sw_idx = loc;

	spin_lock(&adapter->fdir_perfect_lock);

	if (hlist_empty(&adapter->fdir_filter_list)) {
		memcpy(&adapter->fdir_mask, mask, sizeof(*mask));
		err = ixgbe_fdir_set_input_mask_82599(hw, mask);
		if (err)
			goto err_out_w_lock;
	} else if (memcmp(&adapter->fdir_mask, mask, sizeof(*mask))) {
		err = -EINVAL;
		goto err_out_w_lock;
	}

	ixgbe_atr_compute_perfect_hash_82599(&input->filter, mask);
	err = ixgbe_fdir_write_perfect_filter_82599(hw, &input->filter,
						    input->sw_idx, queue);
	if (err)
		goto err_out_w_lock;

	ixgbe_update_ethtool_fdir_entry(adapter, input, input->sw_idx);
	spin_unlock(&adapter->fdir_perfect_lock);

	if ((uhtid != 0x800) && (adapter->jump_tables[uhtid]))
		set_bit(loc - 1, (adapter->jump_tables[uhtid])->child_loc_map);

	kfree(mask);
	return err;
err_out_w_lock:
	spin_unlock(&adapter->fdir_perfect_lock);
err_out:
	kfree(mask);
free_input:
	kfree(input);
free_jump:
	kfree(jump);
	return err;
}

static int ixgbe_setup_tc_cls_u32(struct ixgbe_adapter *adapter,
				  struct tc_cls_u32_offload *cls_u32)
{
	switch (cls_u32->command) {
	case TC_CLSU32_NEW_KNODE:
	case TC_CLSU32_REPLACE_KNODE:
		return ixgbe_configure_clsu32(adapter, cls_u32);
	case TC_CLSU32_DELETE_KNODE:
		return ixgbe_delete_clsu32(adapter, cls_u32);
	case TC_CLSU32_NEW_HNODE:
	case TC_CLSU32_REPLACE_HNODE:
		return ixgbe_configure_clsu32_add_hnode(adapter, cls_u32);
	case TC_CLSU32_DELETE_HNODE:
		return ixgbe_configure_clsu32_del_hnode(adapter, cls_u32);
	default:
		return -EOPNOTSUPP;
	}
}

static int ixgbe_setup_tc_block_cb(enum tc_setup_type type, void *type_data,
				   void *cb_priv)
{
	struct ixgbe_adapter *adapter = cb_priv;

	if (!tc_cls_can_offload_and_chain0(adapter->netdev, type_data))
		return -EOPNOTSUPP;

	switch (type) {
	case TC_SETUP_CLSU32:
		return ixgbe_setup_tc_cls_u32(adapter, type_data);
	default:
		return -EOPNOTSUPP;
	}
}

static int ixgbe_setup_tc_mqprio(struct net_device *dev,
				 struct tc_mqprio_qopt *mqprio)
{
	mqprio->hw = TC_MQPRIO_HW_OFFLOAD_TCS;
	return ixgbe_setup_tc(dev, mqprio->num_tc);
}

static LIST_HEAD(ixgbe_block_cb_list);

static int __ixgbe_setup_tc(struct net_device *dev, enum tc_setup_type type,
			    void *type_data)
{
	struct ixgbe_adapter *adapter = netdev_priv(dev);

	switch (type) {
	case TC_SETUP_BLOCK:
		return flow_block_cb_setup_simple(type_data,
						  &ixgbe_block_cb_list,
						  ixgbe_setup_tc_block_cb,
						  adapter, adapter, true);
	case TC_SETUP_QDISC_MQPRIO:
		return ixgbe_setup_tc_mqprio(dev, type_data);
	default:
		return -EOPNOTSUPP;
	}
}

#ifdef CONFIG_PCI_IOV
void ixgbe_sriov_reinit(struct ixgbe_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;

	rtnl_lock();
	ixgbe_setup_tc(netdev, adapter->hw_tcs);
	rtnl_unlock();
}

#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void ixgbe_do_reset(struct net_device *netdev)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);

	if (netif_running(netdev))
		ixgbe_reinit_locked(adapter);
	else
		ixgbe_reset(adapter);
}

static netdev_features_t ixgbe_fix_features(struct net_device *netdev,
					    netdev_features_t features)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);

<<<<<<< HEAD
#ifdef CONFIG_DCB
	if (adapter->flags & IXGBE_FLAG_DCB_ENABLED)
		features &= ~NETIF_F_HW_VLAN_RX;
#endif

	/* return error if RXHASH is being enabled when RSS is not supported */
	if (!(adapter->flags & IXGBE_FLAG_RSS_ENABLED))
		features &= ~NETIF_F_RXHASH;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* If Rx checksum is disabled, then RSC/LRO should also be disabled */
	if (!(features & NETIF_F_RXCSUM))
		features &= ~NETIF_F_LRO;

	/* Turn off LRO if not RSC capable */
	if (!(adapter->flags2 & IXGBE_FLAG2_RSC_CAPABLE))
		features &= ~NETIF_F_LRO;
<<<<<<< HEAD
	
=======

	if (adapter->xdp_prog && (features & NETIF_F_LRO)) {
		e_dev_err("LRO is not supported with XDP\n");
		features &= ~NETIF_F_LRO;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return features;
}

<<<<<<< HEAD
=======
static void ixgbe_reset_l2fw_offload(struct ixgbe_adapter *adapter)
{
	int rss = min_t(int, ixgbe_max_rss_indices(adapter),
			num_online_cpus());

	/* go back to full RSS if we're not running SR-IOV */
	if (!adapter->ring_feature[RING_F_VMDQ].offset)
		adapter->flags &= ~(IXGBE_FLAG_VMDQ_ENABLED |
				    IXGBE_FLAG_SRIOV_ENABLED);

	adapter->ring_feature[RING_F_RSS].limit = rss;
	adapter->ring_feature[RING_F_VMDQ].limit = 1;

	ixgbe_setup_tc(adapter->netdev, adapter->hw_tcs);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ixgbe_set_features(struct net_device *netdev,
			      netdev_features_t features)
{
	struct ixgbe_adapter *adapter = netdev_priv(netdev);
	netdev_features_t changed = netdev->features ^ features;
	bool need_reset = false;

	/* Make sure RSC matches LRO, reset if change */
	if (!(features & NETIF_F_LRO)) {
		if (adapter->flags2 & IXGBE_FLAG2_RSC_ENABLED)
			need_reset = true;
		adapter->flags2 &= ~IXGBE_FLAG2_RSC_ENABLED;
	} else if ((adapter->flags2 & IXGBE_FLAG2_RSC_CAPABLE) &&
		   !(adapter->flags2 & IXGBE_FLAG2_RSC_ENABLED)) {
		if (adapter->rx_itr_setting == 1 ||
		    adapter->rx_itr_setting > IXGBE_MIN_RSC_ITR) {
			adapter->flags2 |= IXGBE_FLAG2_RSC_ENABLED;
			need_reset = true;
		} else if ((changed ^ features) & NETIF_F_LRO) {
			e_info(probe, "rx-usecs set too low, "
			       "disabling RSC\n");
		}
	}

	/*
<<<<<<< HEAD
	 * Check if Flow Director n-tuple support was enabled or disabled.  If
	 * the state changed, we need to reset.
	 */
	if (!(features & NETIF_F_NTUPLE)) {
		if (adapter->flags & IXGBE_FLAG_FDIR_PERFECT_CAPABLE) {
			/* turn off Flow Director, set ATR and reset */
			if ((adapter->flags & IXGBE_FLAG_RSS_ENABLED) &&
			    !(adapter->flags & IXGBE_FLAG_DCB_ENABLED))
				adapter->flags |= IXGBE_FLAG_FDIR_HASH_CAPABLE;
			need_reset = true;
		}
		adapter->flags &= ~IXGBE_FLAG_FDIR_PERFECT_CAPABLE;
	} else if (!(adapter->flags & IXGBE_FLAG_FDIR_PERFECT_CAPABLE)) {
		/* turn off ATR, enable perfect filters and reset */
		adapter->flags &= ~IXGBE_FLAG_FDIR_HASH_CAPABLE;
		adapter->flags |= IXGBE_FLAG_FDIR_PERFECT_CAPABLE;
		need_reset = true;
=======
	 * Check if Flow Director n-tuple support or hw_tc support was
	 * enabled or disabled.  If the state changed, we need to reset.
	 */
	if ((features & NETIF_F_NTUPLE) || (features & NETIF_F_HW_TC)) {
		/* turn off ATR, enable perfect filters and reset */
		if (!(adapter->flags & IXGBE_FLAG_FDIR_PERFECT_CAPABLE))
			need_reset = true;

		adapter->flags &= ~IXGBE_FLAG_FDIR_HASH_CAPABLE;
		adapter->flags |= IXGBE_FLAG_FDIR_PERFECT_CAPABLE;
	} else {
		/* turn off perfect filters, enable ATR and reset */
		if (adapter->flags & IXGBE_FLAG_FDIR_PERFECT_CAPABLE)
			need_reset = true;

		adapter->flags &= ~IXGBE_FLAG_FDIR_PERFECT_CAPABLE;

		/* We cannot enable ATR if SR-IOV is enabled */
		if (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED ||
		    /* We cannot enable ATR if we have 2 or more tcs */
		    (adapter->hw_tcs > 1) ||
		    /* We cannot enable ATR if RSS is disabled */
		    (adapter->ring_feature[RING_F_RSS].limit <= 1) ||
		    /* A sample rate of 0 indicates ATR disabled */
		    (!adapter->atr_sample_rate))
			; /* do nothing not supported */
		else /* otherwise supported and set the flag */
			adapter->flags |= IXGBE_FLAG_FDIR_HASH_CAPABLE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (changed & NETIF_F_RXALL)
		need_reset = true;

	netdev->features = features;
<<<<<<< HEAD
	if (need_reset)
		ixgbe_do_reset(netdev);
=======

	if ((changed & NETIF_F_HW_L2FW_DOFFLOAD) && adapter->num_rx_pools > 1)
		ixgbe_reset_l2fw_offload(adapter);
	else if (need_reset)
		ixgbe_do_reset(netdev);
	else if (changed & (NETIF_F_HW_VLAN_CTAG_RX |
			    NETIF_F_HW_VLAN_CTAG_FILTER))
		ixgbe_set_rx_mode(netdev);

	return 1;
}

static int ixgbe_ndo_fdb_add(struct ndmsg *ndm, struct nlattr *tb[],
			     struct net_device *dev,
			     const unsigned char *addr, u16 vid,
			     u16 flags,
			     struct netlink_ext_ack *extack)
{
	/* guarantee we can provide a unique filter for the unicast address */
	if (is_unicast_ether_addr(addr) || is_link_local_ether_addr(addr)) {
		struct ixgbe_adapter *adapter = netdev_priv(dev);
		u16 pool = VMDQ_P(0);

		if (netdev_uc_count(dev) >= ixgbe_available_rars(adapter, pool))
			return -ENOMEM;
	}

	return ndo_dflt_fdb_add(ndm, tb, dev, addr, vid, flags);
}

/**
 * ixgbe_configure_bridge_mode - set various bridge modes
 * @adapter: the private structure
 * @mode: requested bridge mode
 *
 * Configure some settings require for various bridge modes.
 **/
static int ixgbe_configure_bridge_mode(struct ixgbe_adapter *adapter,
				       __u16 mode)
{
	struct ixgbe_hw *hw = &adapter->hw;
	unsigned int p, num_pools;
	u32 vmdctl;

	switch (mode) {
	case BRIDGE_MODE_VEPA:
		/* disable Tx loopback, rely on switch hairpin mode */
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_PFDTXGSWC, 0);

		/* must enable Rx switching replication to allow multicast
		 * packet reception on all VFs, and to enable source address
		 * pruning.
		 */
		vmdctl = IXGBE_READ_REG(hw, IXGBE_VMD_CTL);
		vmdctl |= IXGBE_VT_CTL_REPLEN;
		IXGBE_WRITE_REG(hw, IXGBE_VMD_CTL, vmdctl);

		/* enable Rx source address pruning. Note, this requires
		 * replication to be enabled or else it does nothing.
		 */
		num_pools = adapter->num_vfs + adapter->num_rx_pools;
		for (p = 0; p < num_pools; p++) {
			if (hw->mac.ops.set_source_address_pruning)
				hw->mac.ops.set_source_address_pruning(hw,
								       true,
								       p);
		}
		break;
	case BRIDGE_MODE_VEB:
		/* enable Tx loopback for internal VF/PF communication */
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_PFDTXGSWC,
				IXGBE_PFDTXGSWC_VT_LBEN);

		/* disable Rx switching replication unless we have SR-IOV
		 * virtual functions
		 */
		vmdctl = IXGBE_READ_REG(hw, IXGBE_VMD_CTL);
		if (!adapter->num_vfs)
			vmdctl &= ~IXGBE_VT_CTL_REPLEN;
		IXGBE_WRITE_REG(hw, IXGBE_VMD_CTL, vmdctl);

		/* disable Rx source address pruning, since we don't expect to
		 * be receiving external loopback of our transmitted frames.
		 */
		num_pools = adapter->num_vfs + adapter->num_rx_pools;
		for (p = 0; p < num_pools; p++) {
			if (hw->mac.ops.set_source_address_pruning)
				hw->mac.ops.set_source_address_pruning(hw,
								       false,
								       p);
		}
		break;
	default:
		return -EINVAL;
	}

	adapter->bridge_mode = mode;

	e_info(drv, "enabling bridge mode: %s\n",
	       mode == BRIDGE_MODE_VEPA ? "VEPA" : "VEB");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
=======
static int ixgbe_ndo_bridge_setlink(struct net_device *dev,
				    struct nlmsghdr *nlh, u16 flags,
				    struct netlink_ext_ack *extack)
{
	struct ixgbe_adapter *adapter = netdev_priv(dev);
	struct nlattr *attr, *br_spec;
	int rem;

	if (!(adapter->flags & IXGBE_FLAG_SRIOV_ENABLED))
		return -EOPNOTSUPP;

	br_spec = nlmsg_find_attr(nlh, sizeof(struct ifinfomsg), IFLA_AF_SPEC);
	if (!br_spec)
		return -EINVAL;

	nla_for_each_nested(attr, br_spec, rem) {
		int status;
		__u16 mode;

		if (nla_type(attr) != IFLA_BRIDGE_MODE)
			continue;

		mode = nla_get_u16(attr);
		status = ixgbe_configure_bridge_mode(adapter, mode);
		if (status)
			return status;

		break;
	}

	return 0;
}

static int ixgbe_ndo_bridge_getlink(struct sk_buff *skb, u32 pid, u32 seq,
				    struct net_device *dev,
				    u32 filter_mask, int nlflags)
{
	struct ixgbe_adapter *adapter = netdev_priv(dev);

	if (!(adapter->flags & IXGBE_FLAG_SRIOV_ENABLED))
		return 0;

	return ndo_dflt_bridge_getlink(skb, pid, seq, dev,
				       adapter->bridge_mode, 0, 0, nlflags,
				       filter_mask, NULL);
}

static void *ixgbe_fwd_add(struct net_device *pdev, struct net_device *vdev)
{
	struct ixgbe_adapter *adapter = netdev_priv(pdev);
	struct ixgbe_fwd_adapter *accel;
	int tcs = adapter->hw_tcs ? : 1;
	int pool, err;

	if (adapter->xdp_prog) {
		e_warn(probe, "L2FW offload is not supported with XDP\n");
		return ERR_PTR(-EINVAL);
	}

	/* The hardware supported by ixgbe only filters on the destination MAC
	 * address. In order to avoid issues we only support offloading modes
	 * where the hardware can actually provide the functionality.
	 */
	if (!macvlan_supports_dest_filter(vdev))
		return ERR_PTR(-EMEDIUMTYPE);

	/* We need to lock down the macvlan to be a single queue device so that
	 * we can reuse the tc_to_txq field in the macvlan netdev to represent
	 * the queue mapping to our netdev.
	 */
	if (netif_is_multiqueue(vdev))
		return ERR_PTR(-ERANGE);

	pool = find_first_zero_bit(adapter->fwd_bitmask, adapter->num_rx_pools);
	if (pool == adapter->num_rx_pools) {
		u16 used_pools = adapter->num_vfs + adapter->num_rx_pools;
		u16 reserved_pools;

		if (((adapter->flags & IXGBE_FLAG_DCB_ENABLED) &&
		     adapter->num_rx_pools >= (MAX_TX_QUEUES / tcs)) ||
		    adapter->num_rx_pools > IXGBE_MAX_MACVLANS)
			return ERR_PTR(-EBUSY);

		/* Hardware has a limited number of available pools. Each VF,
		 * and the PF require a pool. Check to ensure we don't
		 * attempt to use more then the available number of pools.
		 */
		if (used_pools >= IXGBE_MAX_VF_FUNCTIONS)
			return ERR_PTR(-EBUSY);

		/* Enable VMDq flag so device will be set in VM mode */
		adapter->flags |= IXGBE_FLAG_VMDQ_ENABLED |
				  IXGBE_FLAG_SRIOV_ENABLED;

		/* Try to reserve as many queues per pool as possible,
		 * we start with the configurations that support 4 queues
		 * per pools, followed by 2, and then by just 1 per pool.
		 */
		if (used_pools < 32 && adapter->num_rx_pools < 16)
			reserved_pools = min_t(u16,
					       32 - used_pools,
					       16 - adapter->num_rx_pools);
		else if (adapter->num_rx_pools < 32)
			reserved_pools = min_t(u16,
					       64 - used_pools,
					       32 - adapter->num_rx_pools);
		else
			reserved_pools = 64 - used_pools;


		if (!reserved_pools)
			return ERR_PTR(-EBUSY);

		adapter->ring_feature[RING_F_VMDQ].limit += reserved_pools;

		/* Force reinit of ring allocation with VMDQ enabled */
		err = ixgbe_setup_tc(pdev, adapter->hw_tcs);
		if (err)
			return ERR_PTR(err);

		if (pool >= adapter->num_rx_pools)
			return ERR_PTR(-ENOMEM);
	}

	accel = kzalloc(sizeof(*accel), GFP_KERNEL);
	if (!accel)
		return ERR_PTR(-ENOMEM);

	set_bit(pool, adapter->fwd_bitmask);
	netdev_set_sb_channel(vdev, pool);
	accel->pool = pool;
	accel->netdev = vdev;

	if (!netif_running(pdev))
		return accel;

	err = ixgbe_fwd_ring_up(adapter, accel);
	if (err)
		return ERR_PTR(err);

	return accel;
}

static void ixgbe_fwd_del(struct net_device *pdev, void *priv)
{
	struct ixgbe_fwd_adapter *accel = priv;
	struct ixgbe_adapter *adapter = netdev_priv(pdev);
	unsigned int rxbase = accel->rx_base_queue;
	unsigned int i;

	/* delete unicast filter associated with offloaded interface */
	ixgbe_del_mac_filter(adapter, accel->netdev->dev_addr,
			     VMDQ_P(accel->pool));

	/* Allow remaining Rx packets to get flushed out of the
	 * Rx FIFO before we drop the netdev for the ring.
	 */
	usleep_range(10000, 20000);

	for (i = 0; i < adapter->num_rx_queues_per_pool; i++) {
		struct ixgbe_ring *ring = adapter->rx_ring[rxbase + i];
		struct ixgbe_q_vector *qv = ring->q_vector;

		/* Make sure we aren't processing any packets and clear
		 * netdev to shut down the ring.
		 */
		if (netif_running(adapter->netdev))
			napi_synchronize(&qv->napi);
		ring->netdev = NULL;
	}

	/* unbind the queues and drop the subordinate channel config */
	netdev_unbind_sb_channel(pdev, accel->netdev);
	netdev_set_sb_channel(accel->netdev, 0);

	clear_bit(accel->pool, adapter->fwd_bitmask);
	kfree(accel);
}

#define IXGBE_MAX_MAC_HDR_LEN		127
#define IXGBE_MAX_NETWORK_HDR_LEN	511

static netdev_features_t
ixgbe_features_check(struct sk_buff *skb, struct net_device *dev,
		     netdev_features_t features)
{
	unsigned int network_hdr_len, mac_hdr_len;

	/* Make certain the headers can be described by a context descriptor */
	mac_hdr_len = skb_network_offset(skb);
	if (unlikely(mac_hdr_len > IXGBE_MAX_MAC_HDR_LEN))
		return features & ~(NETIF_F_HW_CSUM |
				    NETIF_F_SCTP_CRC |
				    NETIF_F_GSO_UDP_L4 |
				    NETIF_F_HW_VLAN_CTAG_TX |
				    NETIF_F_TSO |
				    NETIF_F_TSO6);

	network_hdr_len = skb_checksum_start(skb) - skb_network_header(skb);
	if (unlikely(network_hdr_len >  IXGBE_MAX_NETWORK_HDR_LEN))
		return features & ~(NETIF_F_HW_CSUM |
				    NETIF_F_SCTP_CRC |
				    NETIF_F_GSO_UDP_L4 |
				    NETIF_F_TSO |
				    NETIF_F_TSO6);

	/* We can only support IPV4 TSO in tunnels if we can mangle the
	 * inner IP ID field, so strip TSO if MANGLEID is not supported.
	 * IPsec offoad sets skb->encapsulation but still can handle
	 * the TSO, so it's the exception.
	 */
	if (skb->encapsulation && !(features & NETIF_F_TSO_MANGLEID)) {
#ifdef CONFIG_IXGBE_IPSEC
		if (!secpath_exists(skb))
#endif
			features &= ~NETIF_F_TSO;
	}

	return features;
}

static int ixgbe_xdp_setup(struct net_device *dev, struct bpf_prog *prog)
{
	int i, frame_size = dev->mtu + ETH_HLEN + ETH_FCS_LEN + VLAN_HLEN;
	struct ixgbe_adapter *adapter = netdev_priv(dev);
	struct bpf_prog *old_prog;
	bool need_reset;
	int num_queues;

	if (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED)
		return -EINVAL;

	if (adapter->flags & IXGBE_FLAG_DCB_ENABLED)
		return -EINVAL;

	/* verify ixgbe ring attributes are sufficient for XDP */
	for (i = 0; i < adapter->num_rx_queues; i++) {
		struct ixgbe_ring *ring = adapter->rx_ring[i];

		if (ring_is_rsc_enabled(ring))
			return -EINVAL;

		if (frame_size > ixgbe_rx_bufsz(ring))
			return -EINVAL;
	}

	/* if the number of cpus is much larger than the maximum of queues,
	 * we should stop it and then return with ENOMEM like before.
	 */
	if (nr_cpu_ids > IXGBE_MAX_XDP_QS * 2)
		return -ENOMEM;

	old_prog = xchg(&adapter->xdp_prog, prog);
	need_reset = (!!prog != !!old_prog);

	/* If transitioning XDP modes reconfigure rings */
	if (need_reset) {
		int err;

		if (!prog)
			/* Wait until ndo_xsk_wakeup completes. */
			synchronize_rcu();
		err = ixgbe_setup_tc(dev, adapter->hw_tcs);

		if (err)
			return -EINVAL;
		if (!prog)
			xdp_features_clear_redirect_target(dev);
	} else {
		for (i = 0; i < adapter->num_rx_queues; i++) {
			WRITE_ONCE(adapter->rx_ring[i]->xdp_prog,
				   adapter->xdp_prog);
		}
	}

	if (old_prog)
		bpf_prog_put(old_prog);

	/* Kick start the NAPI context if there is an AF_XDP socket open
	 * on that queue id. This so that receiving will start.
	 */
	if (need_reset && prog) {
		num_queues = min_t(int, adapter->num_rx_queues,
				   adapter->num_xdp_queues);
		for (i = 0; i < num_queues; i++)
			if (adapter->xdp_ring[i]->xsk_pool)
				(void)ixgbe_xsk_wakeup(adapter->netdev, i,
						       XDP_WAKEUP_RX);
		xdp_features_set_redirect_target(dev, true);
	}

	return 0;
}

static int ixgbe_xdp(struct net_device *dev, struct netdev_bpf *xdp)
{
	struct ixgbe_adapter *adapter = netdev_priv(dev);

	switch (xdp->command) {
	case XDP_SETUP_PROG:
		return ixgbe_xdp_setup(dev, xdp->prog);
	case XDP_SETUP_XSK_POOL:
		return ixgbe_xsk_pool_setup(adapter, xdp->xsk.pool,
					    xdp->xsk.queue_id);

	default:
		return -EINVAL;
	}
}

void ixgbe_xdp_ring_update_tail(struct ixgbe_ring *ring)
{
	/* Force memory writes to complete before letting h/w know there
	 * are new descriptors to fetch.
	 */
	wmb();
	writel(ring->next_to_use, ring->tail);
}

void ixgbe_xdp_ring_update_tail_locked(struct ixgbe_ring *ring)
{
	if (static_branch_unlikely(&ixgbe_xdp_locking_key))
		spin_lock(&ring->tx_lock);
	ixgbe_xdp_ring_update_tail(ring);
	if (static_branch_unlikely(&ixgbe_xdp_locking_key))
		spin_unlock(&ring->tx_lock);
}

static int ixgbe_xdp_xmit(struct net_device *dev, int n,
			  struct xdp_frame **frames, u32 flags)
{
	struct ixgbe_adapter *adapter = netdev_priv(dev);
	struct ixgbe_ring *ring;
	int nxmit = 0;
	int i;

	if (unlikely(test_bit(__IXGBE_DOWN, &adapter->state)))
		return -ENETDOWN;

	if (unlikely(flags & ~XDP_XMIT_FLAGS_MASK))
		return -EINVAL;

	/* During program transitions its possible adapter->xdp_prog is assigned
	 * but ring has not been configured yet. In this case simply abort xmit.
	 */
	ring = adapter->xdp_prog ? ixgbe_determine_xdp_ring(adapter) : NULL;
	if (unlikely(!ring))
		return -ENXIO;

	if (unlikely(test_bit(__IXGBE_TX_DISABLED, &ring->state)))
		return -ENXIO;

	if (static_branch_unlikely(&ixgbe_xdp_locking_key))
		spin_lock(&ring->tx_lock);

	for (i = 0; i < n; i++) {
		struct xdp_frame *xdpf = frames[i];
		int err;

		err = ixgbe_xmit_xdp_ring(ring, xdpf);
		if (err != IXGBE_XDP_TX)
			break;
		nxmit++;
	}

	if (unlikely(flags & XDP_XMIT_FLUSH))
		ixgbe_xdp_ring_update_tail(ring);

	if (static_branch_unlikely(&ixgbe_xdp_locking_key))
		spin_unlock(&ring->tx_lock);

	return nxmit;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct net_device_ops ixgbe_netdev_ops = {
	.ndo_open		= ixgbe_open,
	.ndo_stop		= ixgbe_close,
	.ndo_start_xmit		= ixgbe_xmit_frame,
<<<<<<< HEAD
	.ndo_select_queue	= ixgbe_select_queue,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ndo_set_rx_mode	= ixgbe_set_rx_mode,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_set_mac_address	= ixgbe_set_mac,
	.ndo_change_mtu		= ixgbe_change_mtu,
	.ndo_tx_timeout		= ixgbe_tx_timeout,
<<<<<<< HEAD
	.ndo_vlan_rx_add_vid	= ixgbe_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid	= ixgbe_vlan_rx_kill_vid,
	.ndo_do_ioctl		= ixgbe_ioctl,
	.ndo_set_vf_mac		= ixgbe_ndo_set_vf_mac,
	.ndo_set_vf_vlan	= ixgbe_ndo_set_vf_vlan,
	.ndo_set_vf_tx_rate	= ixgbe_ndo_set_vf_bw,
	.ndo_set_vf_spoofchk	= ixgbe_ndo_set_vf_spoofchk,
	.ndo_get_vf_config	= ixgbe_ndo_get_vf_config,
	.ndo_get_stats64	= ixgbe_get_stats64,
#ifdef CONFIG_IXGBE_DCB
	.ndo_setup_tc		= ixgbe_setup_tc,
#endif
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= ixgbe_netpoll,
#endif
#ifdef IXGBE_FCOE
=======
	.ndo_set_tx_maxrate	= ixgbe_tx_maxrate,
	.ndo_vlan_rx_add_vid	= ixgbe_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid	= ixgbe_vlan_rx_kill_vid,
	.ndo_eth_ioctl		= ixgbe_ioctl,
	.ndo_set_vf_mac		= ixgbe_ndo_set_vf_mac,
	.ndo_set_vf_vlan	= ixgbe_ndo_set_vf_vlan,
	.ndo_set_vf_rate	= ixgbe_ndo_set_vf_bw,
	.ndo_set_vf_spoofchk	= ixgbe_ndo_set_vf_spoofchk,
	.ndo_set_vf_link_state	= ixgbe_ndo_set_vf_link_state,
	.ndo_set_vf_rss_query_en = ixgbe_ndo_set_vf_rss_query_en,
	.ndo_set_vf_trust	= ixgbe_ndo_set_vf_trust,
	.ndo_get_vf_config	= ixgbe_ndo_get_vf_config,
	.ndo_get_vf_stats	= ixgbe_ndo_get_vf_stats,
	.ndo_get_stats64	= ixgbe_get_stats64,
	.ndo_setup_tc		= __ixgbe_setup_tc,
#ifdef IXGBE_FCOE
	.ndo_select_queue	= ixgbe_select_queue,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ndo_fcoe_ddp_setup = ixgbe_fcoe_ddp_get,
	.ndo_fcoe_ddp_target = ixgbe_fcoe_ddp_target,
	.ndo_fcoe_ddp_done = ixgbe_fcoe_ddp_put,
	.ndo_fcoe_enable = ixgbe_fcoe_enable,
	.ndo_fcoe_disable = ixgbe_fcoe_disable,
	.ndo_fcoe_get_wwn = ixgbe_fcoe_get_wwn,
	.ndo_fcoe_get_hbainfo = ixgbe_fcoe_get_hbainfo,
#endif /* IXGBE_FCOE */
	.ndo_set_features = ixgbe_set_features,
	.ndo_fix_features = ixgbe_fix_features,
<<<<<<< HEAD
};

static void __devinit ixgbe_probe_vf(struct ixgbe_adapter *adapter,
				     const struct ixgbe_info *ii)
{
#ifdef CONFIG_PCI_IOV
	struct ixgbe_hw *hw = &adapter->hw;

	if (hw->mac.type == ixgbe_mac_82598EB)
		return;

	/* The 82599 supports up to 64 VFs per physical function
	 * but this implementation limits allocation to 63 so that
	 * basic networking resources are still available to the
	 * physical function
	 */
	adapter->num_vfs = (max_vfs > 63) ? 63 : max_vfs;
	ixgbe_enable_sriov(adapter, ii);
#endif /* CONFIG_PCI_IOV */
=======
	.ndo_fdb_add		= ixgbe_ndo_fdb_add,
	.ndo_bridge_setlink	= ixgbe_ndo_bridge_setlink,
	.ndo_bridge_getlink	= ixgbe_ndo_bridge_getlink,
	.ndo_dfwd_add_station	= ixgbe_fwd_add,
	.ndo_dfwd_del_station	= ixgbe_fwd_del,
	.ndo_features_check	= ixgbe_features_check,
	.ndo_bpf		= ixgbe_xdp,
	.ndo_xdp_xmit		= ixgbe_xdp_xmit,
	.ndo_xsk_wakeup         = ixgbe_xsk_wakeup,
};

static void ixgbe_disable_txr_hw(struct ixgbe_adapter *adapter,
				 struct ixgbe_ring *tx_ring)
{
	unsigned long wait_delay, delay_interval;
	struct ixgbe_hw *hw = &adapter->hw;
	u8 reg_idx = tx_ring->reg_idx;
	int wait_loop;
	u32 txdctl;

	IXGBE_WRITE_REG(hw, IXGBE_TXDCTL(reg_idx), IXGBE_TXDCTL_SWFLSH);

	/* delay mechanism from ixgbe_disable_tx */
	delay_interval = ixgbe_get_completion_timeout(adapter) / 100;

	wait_loop = IXGBE_MAX_RX_DESC_POLL;
	wait_delay = delay_interval;

	while (wait_loop--) {
		usleep_range(wait_delay, wait_delay + 10);
		wait_delay += delay_interval * 2;
		txdctl = IXGBE_READ_REG(hw, IXGBE_TXDCTL(reg_idx));

		if (!(txdctl & IXGBE_TXDCTL_ENABLE))
			return;
	}

	e_err(drv, "TXDCTL.ENABLE not cleared within the polling period\n");
}

static void ixgbe_disable_txr(struct ixgbe_adapter *adapter,
			      struct ixgbe_ring *tx_ring)
{
	set_bit(__IXGBE_TX_DISABLED, &tx_ring->state);
	ixgbe_disable_txr_hw(adapter, tx_ring);
}

static void ixgbe_disable_rxr_hw(struct ixgbe_adapter *adapter,
				 struct ixgbe_ring *rx_ring)
{
	unsigned long wait_delay, delay_interval;
	struct ixgbe_hw *hw = &adapter->hw;
	u8 reg_idx = rx_ring->reg_idx;
	int wait_loop;
	u32 rxdctl;

	rxdctl = IXGBE_READ_REG(hw, IXGBE_RXDCTL(reg_idx));
	rxdctl &= ~IXGBE_RXDCTL_ENABLE;
	rxdctl |= IXGBE_RXDCTL_SWFLSH;

	/* write value back with RXDCTL.ENABLE bit cleared */
	IXGBE_WRITE_REG(hw, IXGBE_RXDCTL(reg_idx), rxdctl);

	/* RXDCTL.EN may not change on 82598 if link is down, so skip it */
	if (hw->mac.type == ixgbe_mac_82598EB &&
	    !(IXGBE_READ_REG(hw, IXGBE_LINKS) & IXGBE_LINKS_UP))
		return;

	/* delay mechanism from ixgbe_disable_rx */
	delay_interval = ixgbe_get_completion_timeout(adapter) / 100;

	wait_loop = IXGBE_MAX_RX_DESC_POLL;
	wait_delay = delay_interval;

	while (wait_loop--) {
		usleep_range(wait_delay, wait_delay + 10);
		wait_delay += delay_interval * 2;
		rxdctl = IXGBE_READ_REG(hw, IXGBE_RXDCTL(reg_idx));

		if (!(rxdctl & IXGBE_RXDCTL_ENABLE))
			return;
	}

	e_err(drv, "RXDCTL.ENABLE not cleared within the polling period\n");
}

static void ixgbe_reset_txr_stats(struct ixgbe_ring *tx_ring)
{
	memset(&tx_ring->stats, 0, sizeof(tx_ring->stats));
	memset(&tx_ring->tx_stats, 0, sizeof(tx_ring->tx_stats));
}

static void ixgbe_reset_rxr_stats(struct ixgbe_ring *rx_ring)
{
	memset(&rx_ring->stats, 0, sizeof(rx_ring->stats));
	memset(&rx_ring->rx_stats, 0, sizeof(rx_ring->rx_stats));
}

/**
 * ixgbe_irq_disable_single - Disable single IRQ vector
 * @adapter: adapter structure
 * @ring: ring index
 **/
static void ixgbe_irq_disable_single(struct ixgbe_adapter *adapter, u32 ring)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u64 qmask = BIT_ULL(ring);
	u32 mask;

	switch (adapter->hw.mac.type) {
	case ixgbe_mac_82598EB:
		mask = qmask & IXGBE_EIMC_RTX_QUEUE;
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_EIMC, mask);
		break;
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
		mask = (qmask & 0xFFFFFFFF);
		if (mask)
			IXGBE_WRITE_REG(hw, IXGBE_EIMS_EX(0), mask);
		mask = (qmask >> 32);
		if (mask)
			IXGBE_WRITE_REG(hw, IXGBE_EIMS_EX(1), mask);
		break;
	default:
		break;
	}
	IXGBE_WRITE_FLUSH(&adapter->hw);
	if (adapter->flags & IXGBE_FLAG_MSIX_ENABLED)
		synchronize_irq(adapter->msix_entries[ring].vector);
	else
		synchronize_irq(adapter->pdev->irq);
}

/**
 * ixgbe_txrx_ring_disable - Disable Rx/Tx/XDP Tx rings
 * @adapter: adapter structure
 * @ring: ring index
 *
 * This function disables a certain Rx/Tx/XDP Tx ring. The function
 * assumes that the netdev is running.
 **/
void ixgbe_txrx_ring_disable(struct ixgbe_adapter *adapter, int ring)
{
	struct ixgbe_ring *rx_ring, *tx_ring, *xdp_ring;

	rx_ring = adapter->rx_ring[ring];
	tx_ring = adapter->tx_ring[ring];
	xdp_ring = adapter->xdp_ring[ring];

	ixgbe_irq_disable_single(adapter, ring);

	/* Rx/Tx/XDP Tx share the same napi context. */
	napi_disable(&rx_ring->q_vector->napi);

	ixgbe_disable_txr(adapter, tx_ring);
	if (xdp_ring)
		ixgbe_disable_txr(adapter, xdp_ring);
	ixgbe_disable_rxr_hw(adapter, rx_ring);

	if (xdp_ring)
		synchronize_rcu();

	ixgbe_clean_tx_ring(tx_ring);
	if (xdp_ring)
		ixgbe_clean_tx_ring(xdp_ring);
	ixgbe_clean_rx_ring(rx_ring);

	ixgbe_reset_txr_stats(tx_ring);
	if (xdp_ring)
		ixgbe_reset_txr_stats(xdp_ring);
	ixgbe_reset_rxr_stats(rx_ring);
}

/**
 * ixgbe_txrx_ring_enable - Enable Rx/Tx/XDP Tx rings
 * @adapter: adapter structure
 * @ring: ring index
 *
 * This function enables a certain Rx/Tx/XDP Tx ring. The function
 * assumes that the netdev is running.
 **/
void ixgbe_txrx_ring_enable(struct ixgbe_adapter *adapter, int ring)
{
	struct ixgbe_ring *rx_ring, *tx_ring, *xdp_ring;

	rx_ring = adapter->rx_ring[ring];
	tx_ring = adapter->tx_ring[ring];
	xdp_ring = adapter->xdp_ring[ring];

	ixgbe_configure_tx_ring(adapter, tx_ring);
	if (xdp_ring)
		ixgbe_configure_tx_ring(adapter, xdp_ring);
	ixgbe_configure_rx_ring(adapter, rx_ring);

	clear_bit(__IXGBE_TX_DISABLED, &tx_ring->state);
	if (xdp_ring)
		clear_bit(__IXGBE_TX_DISABLED, &xdp_ring->state);

	/* Rx/Tx/XDP Tx share the same napi context. */
	napi_enable(&rx_ring->q_vector->napi);
	ixgbe_irq_enable_queues(adapter, BIT_ULL(ring));
	IXGBE_WRITE_FLUSH(&adapter->hw);
}

/**
 * ixgbe_enumerate_functions - Get the number of ports this device has
 * @adapter: adapter structure
 *
 * This function enumerates the phsyical functions co-located on a single slot,
 * in order to determine how many ports a device has. This is most useful in
 * determining the required GT/s of PCIe bandwidth necessary for optimal
 * performance.
 **/
static inline int ixgbe_enumerate_functions(struct ixgbe_adapter *adapter)
{
	struct pci_dev *entry, *pdev = adapter->pdev;
	int physfns = 0;

	/* Some cards can not use the generic count PCIe functions method,
	 * because they are behind a parent switch, so we hardcode these with
	 * the correct number of functions.
	 */
	if (ixgbe_pcie_from_parent(&adapter->hw))
		physfns = 4;

	list_for_each_entry(entry, &adapter->pdev->bus->devices, bus_list) {
		/* don't count virtual functions */
		if (entry->is_virtfn)
			continue;

		/* When the devices on the bus don't all match our device ID,
		 * we can't reliably determine the correct number of
		 * functions. This can occur if a function has been direct
		 * attached to a virtual machine using VT-d, for example. In
		 * this case, simply return -1 to indicate this.
		 */
		if ((entry->vendor != pdev->vendor) ||
		    (entry->device != pdev->device))
			return -1;

		physfns++;
	}

	return physfns;
}

/**
 * ixgbe_wol_supported - Check whether device supports WoL
 * @adapter: the adapter private structure
 * @device_id: the device ID
 * @subdevice_id: the subsystem device ID
 *
 * This function is used by probe and ethtool to determine
 * which devices have WoL support
 *
 **/
bool ixgbe_wol_supported(struct ixgbe_adapter *adapter, u16 device_id,
			 u16 subdevice_id)
{
	struct ixgbe_hw *hw = &adapter->hw;
	u16 wol_cap = adapter->eeprom_cap & IXGBE_DEVICE_CAPS_WOL_MASK;

	/* WOL not supported on 82598 */
	if (hw->mac.type == ixgbe_mac_82598EB)
		return false;

	/* check eeprom to see if WOL is enabled for X540 and newer */
	if (hw->mac.type >= ixgbe_mac_X540) {
		if ((wol_cap == IXGBE_DEVICE_CAPS_WOL_PORT0_1) ||
		    ((wol_cap == IXGBE_DEVICE_CAPS_WOL_PORT0) &&
		     (hw->bus.func == 0)))
			return true;
	}

	/* WOL is determined based on device IDs for 82599 MACs */
	switch (device_id) {
	case IXGBE_DEV_ID_82599_SFP:
		/* Only these subdevices could supports WOL */
		switch (subdevice_id) {
		case IXGBE_SUBDEV_ID_82599_560FLR:
		case IXGBE_SUBDEV_ID_82599_LOM_SNAP6:
		case IXGBE_SUBDEV_ID_82599_SFP_WOL0:
		case IXGBE_SUBDEV_ID_82599_SFP_2OCP:
			/* only support first port */
			if (hw->bus.func != 0)
				break;
			fallthrough;
		case IXGBE_SUBDEV_ID_82599_SP_560FLR:
		case IXGBE_SUBDEV_ID_82599_SFP:
		case IXGBE_SUBDEV_ID_82599_RNDC:
		case IXGBE_SUBDEV_ID_82599_ECNA_DP:
		case IXGBE_SUBDEV_ID_82599_SFP_1OCP:
		case IXGBE_SUBDEV_ID_82599_SFP_LOM_OEM1:
		case IXGBE_SUBDEV_ID_82599_SFP_LOM_OEM2:
			return true;
		}
		break;
	case IXGBE_DEV_ID_82599EN_SFP:
		/* Only these subdevices support WOL */
		switch (subdevice_id) {
		case IXGBE_SUBDEV_ID_82599EN_SFP_OCP1:
			return true;
		}
		break;
	case IXGBE_DEV_ID_82599_COMBO_BACKPLANE:
		/* All except this subdevice support WOL */
		if (subdevice_id != IXGBE_SUBDEV_ID_82599_KX4_KR_MEZZ)
			return true;
		break;
	case IXGBE_DEV_ID_82599_KX4:
		return  true;
	default:
		break;
	}

	return false;
}

/**
 * ixgbe_set_fw_version - Set FW version
 * @adapter: the adapter private structure
 *
 * This function is used by probe and ethtool to determine the FW version to
 * format to display. The FW version is taken from the EEPROM/NVM.
 */
static void ixgbe_set_fw_version(struct ixgbe_adapter *adapter)
{
	struct ixgbe_hw *hw = &adapter->hw;
	struct ixgbe_nvm_version nvm_ver;

	ixgbe_get_oem_prod_version(hw, &nvm_ver);
	if (nvm_ver.oem_valid) {
		snprintf(adapter->eeprom_id, sizeof(adapter->eeprom_id),
			 "%x.%x.%x", nvm_ver.oem_major, nvm_ver.oem_minor,
			 nvm_ver.oem_release);
		return;
	}

	ixgbe_get_etk_id(hw, &nvm_ver);
	ixgbe_get_orom_version(hw, &nvm_ver);

	if (nvm_ver.or_valid) {
		snprintf(adapter->eeprom_id, sizeof(adapter->eeprom_id),
			 "0x%08x, %d.%d.%d", nvm_ver.etk_id, nvm_ver.or_major,
			 nvm_ver.or_build, nvm_ver.or_patch);
		return;
	}

	/* Set ETrack ID format */
	snprintf(adapter->eeprom_id, sizeof(adapter->eeprom_id),
		 "0x%08x", nvm_ver.etk_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_probe - Device Initialization Routine
 * @pdev: PCI device information struct
 * @ent: entry in ixgbe_pci_tbl
 *
 * Returns 0 on success, negative on failure
 *
 * ixgbe_probe initializes an adapter identified by a pci_dev structure.
 * The OS initialization, configuring of the adapter private structure,
 * and a hardware reset occur.
 **/
<<<<<<< HEAD
static int __devinit ixgbe_probe(struct pci_dev *pdev,
				 const struct pci_device_id *ent)
=======
static int ixgbe_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net_device *netdev;
	struct ixgbe_adapter *adapter = NULL;
	struct ixgbe_hw *hw;
	const struct ixgbe_info *ii = ixgbe_info_tbl[ent->driver_data];
<<<<<<< HEAD
	static int cards_found;
	int i, err, pci_using_dac;
	u8 part_str[IXGBE_PBANUM_LENGTH];
	unsigned int indices = num_possible_cpus();
=======
	unsigned int indices = MAX_TX_QUEUES;
	u8 part_str[IXGBE_PBANUM_LENGTH];
	int i, err, expected_gts;
	bool disable_dev = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef IXGBE_FCOE
	u16 device_caps;
#endif
	u32 eec;
<<<<<<< HEAD
	u16 wol_cap;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Catch broken hardware that put the wrong VF device ID in
	 * the PCIe SR-IOV capability.
	 */
	if (pdev->is_virtfn) {
		WARN(1, KERN_ERR "%s (%hx:%hx) should not be a VF!\n",
		     pci_name(pdev), pdev->vendor, pdev->device);
		return -EINVAL;
	}

	err = pci_enable_device_mem(pdev);
	if (err)
		return err;

<<<<<<< HEAD
	if (!dma_set_mask(&pdev->dev, DMA_BIT_MASK(64)) &&
	    !dma_set_coherent_mask(&pdev->dev, DMA_BIT_MASK(64))) {
		pci_using_dac = 1;
	} else {
		err = dma_set_mask(&pdev->dev, DMA_BIT_MASK(32));
		if (err) {
			err = dma_set_coherent_mask(&pdev->dev,
						    DMA_BIT_MASK(32));
			if (err) {
				dev_err(&pdev->dev,
					"No usable DMA configuration, aborting\n");
				goto err_dma;
			}
		}
		pci_using_dac = 0;
	}

	err = pci_request_selected_regions(pdev, pci_select_bars(pdev,
					   IORESOURCE_MEM), ixgbe_driver_name);
=======
	err = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(64));
	if (err) {
		dev_err(&pdev->dev,
			"No usable DMA configuration, aborting\n");
		goto err_dma;
	}

	err = pci_request_mem_regions(pdev, ixgbe_driver_name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err) {
		dev_err(&pdev->dev,
			"pci_request_selected_regions failed 0x%x\n", err);
		goto err_pci_reg;
	}

<<<<<<< HEAD
	pci_enable_pcie_error_reporting(pdev);

	pci_set_master(pdev);
	pci_save_state(pdev);

#ifdef CONFIG_IXGBE_DCB
	indices *= MAX_TRAFFIC_CLASS;
#endif

	if (ii->mac == ixgbe_mac_82598EB)
		indices = min_t(unsigned int, indices, IXGBE_MAX_RSS_INDICES);
	else
		indices = min_t(unsigned int, indices, IXGBE_MAX_FDIR_INDICES);

#ifdef IXGBE_FCOE
	indices += min_t(unsigned int, num_possible_cpus(),
			 IXGBE_MAX_FCOE_INDICES);
#endif
=======
	pci_set_master(pdev);
	pci_save_state(pdev);

	if (ii->mac == ixgbe_mac_82598EB) {
#ifdef CONFIG_IXGBE_DCB
		/* 8 TC w/ 4 queues per TC */
		indices = 4 * MAX_TRAFFIC_CLASS;
#else
		indices = IXGBE_MAX_RSS_INDICES;
#endif
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	netdev = alloc_etherdev_mq(sizeof(struct ixgbe_adapter), indices);
	if (!netdev) {
		err = -ENOMEM;
		goto err_alloc_etherdev;
	}

	SET_NETDEV_DEV(netdev, &pdev->dev);

	adapter = netdev_priv(netdev);
<<<<<<< HEAD
	pci_set_drvdata(pdev, adapter);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	adapter->netdev = netdev;
	adapter->pdev = pdev;
	hw = &adapter->hw;
	hw->back = adapter;
	adapter->msg_enable = netif_msg_init(debug, DEFAULT_MSG_ENABLE);

	hw->hw_addr = ioremap(pci_resource_start(pdev, 0),
			      pci_resource_len(pdev, 0));
<<<<<<< HEAD
=======
	adapter->io_addr = hw->hw_addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!hw->hw_addr) {
		err = -EIO;
		goto err_ioremap;
	}

<<<<<<< HEAD
	for (i = 1; i <= 5; i++) {
		if (pci_resource_len(pdev, i) == 0)
			continue;
	}

	netdev->netdev_ops = &ixgbe_netdev_ops;
	ixgbe_set_ethtool_ops(netdev);
	netdev->watchdog_timeo = 5 * HZ;
	strncpy(netdev->name, pci_name(pdev), sizeof(netdev->name) - 1);

	adapter->bd_number = cards_found;

	/* Setup hw api */
	memcpy(&hw->mac.ops, ii->mac_ops, sizeof(hw->mac.ops));
	hw->mac.type  = ii->mac;

	/* EEPROM */
	memcpy(&hw->eeprom.ops, ii->eeprom_ops, sizeof(hw->eeprom.ops));
	eec = IXGBE_READ_REG(hw, IXGBE_EEC);
	/* If EEPROM is valid (bit 8 = 1), use default otherwise use bit bang */
	if (!(eec & (1 << 8)))
		hw->eeprom.ops.read = &ixgbe_read_eeprom_bit_bang_generic;

	/* PHY */
	memcpy(&hw->phy.ops, ii->phy_ops, sizeof(hw->phy.ops));
=======
	netdev->netdev_ops = &ixgbe_netdev_ops;
	ixgbe_set_ethtool_ops(netdev);
	netdev->watchdog_timeo = 5 * HZ;
	strscpy(netdev->name, pci_name(pdev), sizeof(netdev->name));

	/* Setup hw api */
	hw->mac.ops   = *ii->mac_ops;
	hw->mac.type  = ii->mac;
	hw->mvals     = ii->mvals;
	if (ii->link_ops)
		hw->link.ops  = *ii->link_ops;

	/* EEPROM */
	hw->eeprom.ops = *ii->eeprom_ops;
	eec = IXGBE_READ_REG(hw, IXGBE_EEC(hw));
	if (ixgbe_removed(hw->hw_addr)) {
		err = -EIO;
		goto err_ioremap;
	}
	/* If EEPROM is valid (bit 8 = 1), use default otherwise use bit bang */
	if (!(eec & BIT(8)))
		hw->eeprom.ops.read = &ixgbe_read_eeprom_bit_bang_generic;

	/* PHY */
	hw->phy.ops = *ii->phy_ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hw->phy.sfp_type = ixgbe_sfp_type_unknown;
	/* ixgbe_identify_phy_generic will set prtad and mmds properly */
	hw->phy.mdio.prtad = MDIO_PRTAD_NONE;
	hw->phy.mdio.mmds = 0;
	hw->phy.mdio.mode_support = MDIO_SUPPORTS_C45 | MDIO_EMULATE_C22;
	hw->phy.mdio.dev = netdev;
	hw->phy.mdio.mdio_read = ixgbe_mdio_read;
	hw->phy.mdio.mdio_write = ixgbe_mdio_write;

<<<<<<< HEAD
	ii->get_invariants(hw);

	/* setup the private structure */
	err = ixgbe_sw_init(adapter);
	if (err)
		goto err_sw_init;

=======
	/* setup the private structure */
	err = ixgbe_sw_init(adapter, ii);
	if (err)
		goto err_sw_init;

	if (adapter->hw.mac.type == ixgbe_mac_82599EB)
		adapter->flags2 |= IXGBE_FLAG2_AUTO_DISABLE_VF;

	switch (adapter->hw.mac.type) {
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
		netdev->udp_tunnel_nic_info = &ixgbe_udp_tunnels_x550;
		break;
	case ixgbe_mac_x550em_a:
		netdev->udp_tunnel_nic_info = &ixgbe_udp_tunnels_x550em_a;
		break;
	default:
		break;
	}

	/* Make sure the SWFW semaphore is in a valid state */
	if (hw->mac.ops.init_swfw_sync)
		hw->mac.ops.init_swfw_sync(hw);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Make it possible the adapter to be woken up via WOL */
	switch (adapter->hw.mac.type) {
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
<<<<<<< HEAD
=======
	case ixgbe_mac_X550:
	case ixgbe_mac_X550EM_x:
	case ixgbe_mac_x550em_a:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_WUS, ~0);
		break;
	default:
		break;
	}

	/*
	 * If there is a fan on this device and it has failed log the
	 * failure.
	 */
	if (adapter->flags & IXGBE_FLAG_FAN_FAIL_CAPABLE) {
		u32 esdp = IXGBE_READ_REG(hw, IXGBE_ESDP);
		if (esdp & IXGBE_ESDP_SDP1)
			e_crit(probe, "Fan has stopped, replace the adapter\n");
	}

	if (allow_unsupported_sfp)
		hw->allow_unsupported_sfp = allow_unsupported_sfp;

	/* reset_hw fills in the perm_addr as well */
	hw->phy.reset_if_overtemp = true;
	err = hw->mac.ops.reset_hw(hw);
	hw->phy.reset_if_overtemp = false;
<<<<<<< HEAD
	if (err == IXGBE_ERR_SFP_NOT_PRESENT &&
	    hw->mac.type == ixgbe_mac_82598EB) {
		err = 0;
	} else if (err == IXGBE_ERR_SFP_NOT_SUPPORTED) {
		e_dev_err("failed to load because an unsupported SFP+ "
			  "module type was detected.\n");
		e_dev_err("Reload the driver after installing a supported "
			  "module.\n");
=======
	ixgbe_set_eee_capable(adapter);
	if (err == -ENOENT) {
		err = 0;
	} else if (err == -EOPNOTSUPP) {
		e_dev_err("failed to load because an unsupported SFP+ or QSFP module type was detected.\n");
		e_dev_err("Reload the driver after installing a supported module.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err_sw_init;
	} else if (err) {
		e_dev_err("HW Init failed: %d\n", err);
		goto err_sw_init;
	}

<<<<<<< HEAD
	ixgbe_probe_vf(adapter, ii);

	netdev->features = NETIF_F_SG |
			   NETIF_F_IP_CSUM |
			   NETIF_F_IPV6_CSUM |
			   NETIF_F_HW_VLAN_TX |
			   NETIF_F_HW_VLAN_RX |
			   NETIF_F_HW_VLAN_FILTER |
			   NETIF_F_TSO |
			   NETIF_F_TSO6 |
			   NETIF_F_RXHASH |
			   NETIF_F_RXCSUM;

	netdev->hw_features = netdev->features;

	switch (adapter->hw.mac.type) {
	case ixgbe_mac_82599EB:
	case ixgbe_mac_X540:
		netdev->features |= NETIF_F_SCTP_CSUM;
		netdev->hw_features |= NETIF_F_SCTP_CSUM |
				       NETIF_F_NTUPLE;
		break;
	default:
		break;
	}

	netdev->hw_features |= NETIF_F_RXALL;

	netdev->vlan_features |= NETIF_F_TSO;
	netdev->vlan_features |= NETIF_F_TSO6;
	netdev->vlan_features |= NETIF_F_IP_CSUM;
	netdev->vlan_features |= NETIF_F_IPV6_CSUM;
	netdev->vlan_features |= NETIF_F_SG;
=======
#ifdef CONFIG_PCI_IOV
	/* SR-IOV not supported on the 82598 */
	if (adapter->hw.mac.type == ixgbe_mac_82598EB)
		goto skip_sriov;
	/* Mailbox */
	ixgbe_init_mbx_params_pf(hw);
	hw->mbx.ops = ii->mbx_ops;
	pci_sriov_set_totalvfs(pdev, IXGBE_MAX_VFS_DRV_LIMIT);
	ixgbe_enable_sriov(adapter, max_vfs);
skip_sriov:

#endif
	netdev->features = NETIF_F_SG |
			   NETIF_F_TSO |
			   NETIF_F_TSO6 |
			   NETIF_F_RXHASH |
			   NETIF_F_RXCSUM |
			   NETIF_F_HW_CSUM;

#define IXGBE_GSO_PARTIAL_FEATURES (NETIF_F_GSO_GRE | \
				    NETIF_F_GSO_GRE_CSUM | \
				    NETIF_F_GSO_IPXIP4 | \
				    NETIF_F_GSO_IPXIP6 | \
				    NETIF_F_GSO_UDP_TUNNEL | \
				    NETIF_F_GSO_UDP_TUNNEL_CSUM)

	netdev->gso_partial_features = IXGBE_GSO_PARTIAL_FEATURES;
	netdev->features |= NETIF_F_GSO_PARTIAL |
			    IXGBE_GSO_PARTIAL_FEATURES;

	if (hw->mac.type >= ixgbe_mac_82599EB)
		netdev->features |= NETIF_F_SCTP_CRC | NETIF_F_GSO_UDP_L4;

#ifdef CONFIG_IXGBE_IPSEC
#define IXGBE_ESP_FEATURES	(NETIF_F_HW_ESP | \
				 NETIF_F_HW_ESP_TX_CSUM | \
				 NETIF_F_GSO_ESP)

	if (adapter->ipsec)
		netdev->features |= IXGBE_ESP_FEATURES;
#endif
	/* copy netdev features into list of user selectable features */
	netdev->hw_features |= netdev->features |
			       NETIF_F_HW_VLAN_CTAG_FILTER |
			       NETIF_F_HW_VLAN_CTAG_RX |
			       NETIF_F_HW_VLAN_CTAG_TX |
			       NETIF_F_RXALL |
			       NETIF_F_HW_L2FW_DOFFLOAD;

	if (hw->mac.type >= ixgbe_mac_82599EB)
		netdev->hw_features |= NETIF_F_NTUPLE |
				       NETIF_F_HW_TC;

	netdev->features |= NETIF_F_HIGHDMA;

	netdev->vlan_features |= netdev->features | NETIF_F_TSO_MANGLEID;
	netdev->hw_enc_features |= netdev->vlan_features;
	netdev->mpls_features |= NETIF_F_SG |
				 NETIF_F_TSO |
				 NETIF_F_TSO6 |
				 NETIF_F_HW_CSUM;
	netdev->mpls_features |= IXGBE_GSO_PARTIAL_FEATURES;

	/* set this bit last since it cannot be part of vlan_features */
	netdev->features |= NETIF_F_HW_VLAN_CTAG_FILTER |
			    NETIF_F_HW_VLAN_CTAG_RX |
			    NETIF_F_HW_VLAN_CTAG_TX;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	netdev->priv_flags |= IFF_UNICAST_FLT;
	netdev->priv_flags |= IFF_SUPP_NOFCS;

<<<<<<< HEAD
	if (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED)
		adapter->flags &= ~(IXGBE_FLAG_RSS_ENABLED |
				    IXGBE_FLAG_DCB_ENABLED);

#ifdef CONFIG_IXGBE_DCB
	netdev->dcbnl_ops = &dcbnl_ops;
=======
	netdev->xdp_features = NETDEV_XDP_ACT_BASIC | NETDEV_XDP_ACT_REDIRECT |
			       NETDEV_XDP_ACT_XSK_ZEROCOPY;

	/* MTU range: 68 - 9710 */
	netdev->min_mtu = ETH_MIN_MTU;
	netdev->max_mtu = IXGBE_MAX_JUMBO_FRAME_SIZE - (ETH_HLEN + ETH_FCS_LEN);

#ifdef CONFIG_IXGBE_DCB
	if (adapter->flags & IXGBE_FLAG_DCB_CAPABLE)
		netdev->dcbnl_ops = &ixgbe_dcbnl_ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#ifdef IXGBE_FCOE
	if (adapter->flags & IXGBE_FLAG_FCOE_CAPABLE) {
<<<<<<< HEAD
=======
		unsigned int fcoe_l;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (hw->mac.ops.get_device_caps) {
			hw->mac.ops.get_device_caps(hw, &device_caps);
			if (device_caps & IXGBE_DEVICE_CAPS_FCOE_OFFLOADS)
				adapter->flags &= ~IXGBE_FLAG_FCOE_CAPABLE;
		}
<<<<<<< HEAD
	}
	if (adapter->flags & IXGBE_FLAG_FCOE_CAPABLE) {
		netdev->vlan_features |= NETIF_F_FCOE_CRC;
		netdev->vlan_features |= NETIF_F_FSO;
		netdev->vlan_features |= NETIF_F_FCOE_MTU;
	}
#endif /* IXGBE_FCOE */
	if (pci_using_dac) {
		netdev->features |= NETIF_F_HIGHDMA;
		netdev->vlan_features |= NETIF_F_HIGHDMA;
	}

=======


		fcoe_l = min_t(int, IXGBE_FCRETA_SIZE, num_online_cpus());
		adapter->ring_feature[RING_F_FCOE].limit = fcoe_l;

		netdev->features |= NETIF_F_FSO |
				    NETIF_F_FCOE_CRC;

		netdev->vlan_features |= NETIF_F_FSO |
					 NETIF_F_FCOE_CRC |
					 NETIF_F_FCOE_MTU;
	}
#endif /* IXGBE_FCOE */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (adapter->flags2 & IXGBE_FLAG2_RSC_CAPABLE)
		netdev->hw_features |= NETIF_F_LRO;
	if (adapter->flags2 & IXGBE_FLAG2_RSC_ENABLED)
		netdev->features |= NETIF_F_LRO;

<<<<<<< HEAD
=======
	if (ixgbe_check_fw_error(adapter)) {
		err = -EIO;
		goto err_sw_init;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* make sure the EEPROM is good */
	if (hw->eeprom.ops.validate_checksum(hw, NULL) < 0) {
		e_dev_err("The EEPROM Checksum Is Not Valid\n");
		err = -EIO;
		goto err_sw_init;
	}

<<<<<<< HEAD
	memcpy(netdev->dev_addr, hw->mac.perm_addr, netdev->addr_len);
	memcpy(netdev->perm_addr, hw->mac.perm_addr, netdev->addr_len);

	if (ixgbe_validate_mac_addr(netdev->perm_addr)) {
=======
	eth_platform_get_mac_address(&adapter->pdev->dev,
				     adapter->hw.mac.perm_addr);

	eth_hw_addr_set(netdev, hw->mac.perm_addr);

	if (!is_valid_ether_addr(netdev->dev_addr)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		e_dev_err("invalid MAC address\n");
		err = -EIO;
		goto err_sw_init;
	}

<<<<<<< HEAD
	setup_timer(&adapter->service_timer, &ixgbe_service_timer,
		    (unsigned long) adapter);

	INIT_WORK(&adapter->service_task, ixgbe_service_task);
=======
	/* Set hw->mac.addr to permanent MAC address */
	ether_addr_copy(hw->mac.addr, hw->mac.perm_addr);
	ixgbe_mac_set_default_filter(adapter);

	timer_setup(&adapter->service_timer, ixgbe_service_timer, 0);

	if (ixgbe_removed(hw->hw_addr)) {
		err = -EIO;
		goto err_sw_init;
	}
	INIT_WORK(&adapter->service_task, ixgbe_service_task);
	set_bit(__IXGBE_SERVICE_INITED, &adapter->state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clear_bit(__IXGBE_SERVICE_SCHED, &adapter->state);

	err = ixgbe_init_interrupt_scheme(adapter);
	if (err)
		goto err_sw_init;

<<<<<<< HEAD
	if (!(adapter->flags & IXGBE_FLAG_RSS_ENABLED)) {
		netdev->hw_features &= ~NETIF_F_RXHASH;
		netdev->features &= ~NETIF_F_RXHASH;
	}

	/* WOL not supported for all but the following */
	adapter->wol = 0;
	switch (pdev->device) {
	case IXGBE_DEV_ID_82599_SFP:
		/* Only these subdevice supports WOL */
		switch (pdev->subsystem_device) {
		case IXGBE_SUBDEV_ID_82599_560FLR:
			/* only support first port */
			if (hw->bus.func != 0)
				break;
		case IXGBE_SUBDEV_ID_82599_SFP:
			adapter->wol = IXGBE_WUFC_MAG;
			break;
		}
		break;
	case IXGBE_DEV_ID_82599_COMBO_BACKPLANE:
		/* All except this subdevice support WOL */
		if (pdev->subsystem_device != IXGBE_SUBDEV_ID_82599_KX4_KR_MEZZ)
			adapter->wol = IXGBE_WUFC_MAG;
		break;
	case IXGBE_DEV_ID_82599_KX4:
		adapter->wol = IXGBE_WUFC_MAG;
		break;
	case IXGBE_DEV_ID_X540T:
	case IXGBE_DEV_ID_X540T1:
		/* Check eeprom to see if it is enabled */
		hw->eeprom.ops.read(hw, 0x2c, &adapter->eeprom_cap);
		wol_cap = adapter->eeprom_cap & IXGBE_DEVICE_CAPS_WOL_MASK;

		if ((wol_cap == IXGBE_DEVICE_CAPS_WOL_PORT0_1) ||
		    ((wol_cap == IXGBE_DEVICE_CAPS_WOL_PORT0) &&
		     (hw->bus.func == 0)))
			adapter->wol = IXGBE_WUFC_MAG;
		break;
	}
	device_set_wakeup_enable(&adapter->pdev->dev, adapter->wol);

	/* save off EEPROM version number */
	hw->eeprom.ops.read(hw, 0x2e, &adapter->eeprom_verh);
	hw->eeprom.ops.read(hw, 0x2d, &adapter->eeprom_verl);

	/* pick up the PCI bus settings for reporting later */
	hw->mac.ops.get_bus_info(hw);

	/* print bus type/speed/width info */
	e_dev_info("(PCI Express:%s:%s) %pM\n",
		   (hw->bus.speed == ixgbe_bus_speed_5000 ? "5.0GT/s" :
		    hw->bus.speed == ixgbe_bus_speed_2500 ? "2.5GT/s" :
		    "Unknown"),
		   (hw->bus.width == ixgbe_bus_width_pcie_x8 ? "Width x8" :
		    hw->bus.width == ixgbe_bus_width_pcie_x4 ? "Width x4" :
		    hw->bus.width == ixgbe_bus_width_pcie_x1 ? "Width x1" :
		    "Unknown"),
		   netdev->dev_addr);

	err = ixgbe_read_pba_string_generic(hw, part_str, IXGBE_PBANUM_LENGTH);
	if (err)
		strncpy(part_str, "Unknown", IXGBE_PBANUM_LENGTH);
	if (ixgbe_is_sfp(hw) && hw->phy.sfp_type != ixgbe_sfp_type_not_present)
		e_dev_info("MAC: %d, PHY: %d, SFP+: %d, PBA No: %s\n",
			   hw->mac.type, hw->phy.type, hw->phy.sfp_type,
		           part_str);
=======
	for (i = 0; i < adapter->num_rx_queues; i++)
		u64_stats_init(&adapter->rx_ring[i]->syncp);
	for (i = 0; i < adapter->num_tx_queues; i++)
		u64_stats_init(&adapter->tx_ring[i]->syncp);
	for (i = 0; i < adapter->num_xdp_queues; i++)
		u64_stats_init(&adapter->xdp_ring[i]->syncp);

	/* WOL not supported for all devices */
	adapter->wol = 0;
	hw->eeprom.ops.read(hw, 0x2c, &adapter->eeprom_cap);
	hw->wol_enabled = ixgbe_wol_supported(adapter, pdev->device,
						pdev->subsystem_device);
	if (hw->wol_enabled)
		adapter->wol = IXGBE_WUFC_MAG;

	device_set_wakeup_enable(&adapter->pdev->dev, adapter->wol);

	/* save off EEPROM version number */
	ixgbe_set_fw_version(adapter);

	/* pick up the PCI bus settings for reporting later */
	if (ixgbe_pcie_from_parent(hw))
		ixgbe_get_parent_bus_info(adapter);
	else
		 hw->mac.ops.get_bus_info(hw);

	/* calculate the expected PCIe bandwidth required for optimal
	 * performance. Note that some older parts will never have enough
	 * bandwidth due to being older generation PCIe parts. We clamp these
	 * parts to ensure no warning is displayed if it can't be fixed.
	 */
	switch (hw->mac.type) {
	case ixgbe_mac_82598EB:
		expected_gts = min(ixgbe_enumerate_functions(adapter) * 10, 16);
		break;
	default:
		expected_gts = ixgbe_enumerate_functions(adapter) * 10;
		break;
	}

	/* don't check link if we failed to enumerate functions */
	if (expected_gts > 0)
		ixgbe_check_minimum_link(adapter, expected_gts);

	err = ixgbe_read_pba_string_generic(hw, part_str, sizeof(part_str));
	if (err)
		strscpy(part_str, "Unknown", sizeof(part_str));
	if (ixgbe_is_sfp(hw) && hw->phy.sfp_type != ixgbe_sfp_type_not_present)
		e_dev_info("MAC: %d, PHY: %d, SFP+: %d, PBA No: %s\n",
			   hw->mac.type, hw->phy.type, hw->phy.sfp_type,
			   part_str);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		e_dev_info("MAC: %d, PHY: %d, PBA No: %s\n",
			   hw->mac.type, hw->phy.type, part_str);

<<<<<<< HEAD
	if (hw->bus.width <= ixgbe_bus_width_pcie_x4) {
		e_dev_warn("PCI-Express bandwidth available for this card is "
			   "not sufficient for optimal performance.\n");
		e_dev_warn("For optimal performance a x8 PCI-Express slot "
			   "is required.\n");
	}

	/* reset the hardware with the new settings */
	err = hw->mac.ops.start_hw(hw);
	if (err == IXGBE_ERR_EEPROM_VERSION) {
=======
	e_dev_info("%pM\n", netdev->dev_addr);

	/* reset the hardware with the new settings */
	err = hw->mac.ops.start_hw(hw);
	if (err == -EACCES) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* We are running on a pre-production device, log a warning */
		e_dev_warn("This device is a pre-production adapter/LOM. "
			   "Please be aware there may be issues associated "
			   "with your hardware.  If you are experiencing "
			   "problems please contact your Intel or hardware "
			   "representative who provided you with this "
			   "hardware.\n");
	}
	strcpy(netdev->name, "eth%d");
<<<<<<< HEAD
=======
	pci_set_drvdata(pdev, adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = register_netdev(netdev);
	if (err)
		goto err_register;

<<<<<<< HEAD
	/* power down the optics for multispeed fiber and 82599 SFP+ fiber */
	if (hw->mac.ops.disable_tx_laser &&
	    ((hw->phy.multispeed_fiber) ||
	     ((hw->mac.ops.get_media_type(hw) == ixgbe_media_type_fiber) &&
	      (hw->mac.type == ixgbe_mac_82599EB))))
=======

	/* power down the optics for 82599 SFP+ fiber */
	if (hw->mac.ops.disable_tx_laser)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hw->mac.ops.disable_tx_laser(hw);

	/* carrier off reporting is important to ethtool even BEFORE open */
	netif_carrier_off(netdev);

#ifdef CONFIG_IXGBE_DCA
	if (dca_add_requester(&pdev->dev) == 0) {
		adapter->flags |= IXGBE_FLAG_DCA_ENABLED;
		ixgbe_setup_dca(adapter);
	}
#endif
	if (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED) {
		e_info(probe, "IOV is enabled with %d VFs\n", adapter->num_vfs);
		for (i = 0; i < adapter->num_vfs; i++)
			ixgbe_vf_configuration(pdev, (i | 0x10000000));
	}

	/* firmware requires driver version to be 0xFFFFFFFF
	 * since os does not support feature
	 */
	if (hw->mac.ops.set_fw_drv_ver)
<<<<<<< HEAD
		hw->mac.ops.set_fw_drv_ver(hw, 0xFF, 0xFF, 0xFF,
					   0xFF);
=======
		hw->mac.ops.set_fw_drv_ver(hw, 0xFF, 0xFF, 0xFF, 0xFF,
					   sizeof(UTS_RELEASE) - 1,
					   UTS_RELEASE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* add san mac addr to netdev */
	ixgbe_add_sanmac_netdev(netdev);

	e_dev_info("%s\n", ixgbe_default_device_descr);
<<<<<<< HEAD
	cards_found++;
	return 0;

=======

#ifdef CONFIG_IXGBE_HWMON
	if (ixgbe_sysfs_init(adapter))
		e_err(probe, "failed to allocate sysfs resources\n");
#endif /* CONFIG_IXGBE_HWMON */

	ixgbe_dbg_adapter_init(adapter);

	/* setup link for SFP devices with MNG FW, else wait for IXGBE_UP */
	if (ixgbe_mng_enabled(hw) && ixgbe_is_sfp(hw) && hw->mac.ops.setup_link)
		hw->mac.ops.setup_link(hw,
			IXGBE_LINK_SPEED_10GB_FULL | IXGBE_LINK_SPEED_1GB_FULL,
			true);

	err = ixgbe_mii_bus_init(hw);
	if (err)
		goto err_netdev;

	return 0;

err_netdev:
	unregister_netdev(netdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_register:
	ixgbe_release_hw_control(adapter);
	ixgbe_clear_interrupt_scheme(adapter);
err_sw_init:
<<<<<<< HEAD
	if (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED)
		ixgbe_disable_sriov(adapter);
	adapter->flags2 &= ~IXGBE_FLAG2_SEARCH_FOR_SFP;
	iounmap(hw->hw_addr);
err_ioremap:
	free_netdev(netdev);
err_alloc_etherdev:
	pci_release_selected_regions(pdev,
				     pci_select_bars(pdev, IORESOURCE_MEM));
err_pci_reg:
err_dma:
	pci_disable_device(pdev);
=======
	ixgbe_disable_sriov(adapter);
	adapter->flags2 &= ~IXGBE_FLAG2_SEARCH_FOR_SFP;
	iounmap(adapter->io_addr);
	kfree(adapter->jump_tables[0]);
	kfree(adapter->mac_table);
	kfree(adapter->rss_key);
	bitmap_free(adapter->af_xdp_zc_qps);
err_ioremap:
	disable_dev = !test_and_set_bit(__IXGBE_DISABLED, &adapter->state);
	free_netdev(netdev);
err_alloc_etherdev:
	pci_release_mem_regions(pdev);
err_pci_reg:
err_dma:
	if (!adapter || disable_dev)
		pci_disable_device(pdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/**
 * ixgbe_remove - Device Removal Routine
 * @pdev: PCI device information struct
 *
 * ixgbe_remove is called by the PCI subsystem to alert the driver
 * that it should release a PCI device.  The could be caused by a
 * Hot-Plug event, or because the driver is going to be removed from
 * memory.
 **/
<<<<<<< HEAD
static void __devexit ixgbe_remove(struct pci_dev *pdev)
{
	struct ixgbe_adapter *adapter = pci_get_drvdata(pdev);
	struct net_device *netdev = adapter->netdev;

	set_bit(__IXGBE_DOWN, &adapter->state);
	cancel_work_sync(&adapter->service_task);

=======
static void ixgbe_remove(struct pci_dev *pdev)
{
	struct ixgbe_adapter *adapter = pci_get_drvdata(pdev);
	struct net_device *netdev;
	bool disable_dev;
	int i;

	/* if !adapter then we already cleaned up in probe */
	if (!adapter)
		return;

	netdev  = adapter->netdev;
	ixgbe_dbg_adapter_exit(adapter);

	set_bit(__IXGBE_REMOVING, &adapter->state);
	cancel_work_sync(&adapter->service_task);

	if (adapter->mii_bus)
		mdiobus_unregister(adapter->mii_bus);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_IXGBE_DCA
	if (adapter->flags & IXGBE_FLAG_DCA_ENABLED) {
		adapter->flags &= ~IXGBE_FLAG_DCA_ENABLED;
		dca_remove_requester(&pdev->dev);
<<<<<<< HEAD
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_DCA_CTRL, 1);
	}

#endif
#ifdef IXGBE_FCOE
	if (adapter->flags & IXGBE_FLAG_FCOE_ENABLED)
		ixgbe_cleanup_fcoe(adapter);

#endif /* IXGBE_FCOE */
=======
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_DCA_CTRL,
				IXGBE_DCA_CTRL_DCA_DISABLE);
	}

#endif
#ifdef CONFIG_IXGBE_HWMON
	ixgbe_sysfs_exit(adapter);
#endif /* CONFIG_IXGBE_HWMON */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* remove the added san mac */
	ixgbe_del_sanmac_netdev(netdev);

<<<<<<< HEAD
	if (netdev->reg_state == NETREG_REGISTERED)
		unregister_netdev(netdev);

	if (adapter->flags & IXGBE_FLAG_SRIOV_ENABLED) {
		if (!(ixgbe_check_vf_assignment(adapter)))
			ixgbe_disable_sriov(adapter);
		else
			e_dev_warn("Unloading driver while VFs are assigned "
				   "- VFs will not be deallocated\n");
	}

=======
#ifdef CONFIG_PCI_IOV
	ixgbe_disable_sriov(adapter);
#endif
	if (netdev->reg_state == NETREG_REGISTERED)
		unregister_netdev(netdev);

	ixgbe_stop_ipsec_offload(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ixgbe_clear_interrupt_scheme(adapter);

	ixgbe_release_hw_control(adapter);

#ifdef CONFIG_DCB
	kfree(adapter->ixgbe_ieee_pfc);
	kfree(adapter->ixgbe_ieee_ets);

#endif
<<<<<<< HEAD
	iounmap(adapter->hw.hw_addr);
	pci_release_selected_regions(pdev, pci_select_bars(pdev,
				     IORESOURCE_MEM));

	e_dev_info("complete\n");

	free_netdev(netdev);

	pci_disable_pcie_error_reporting(pdev);

	pci_disable_device(pdev);
=======
	iounmap(adapter->io_addr);
	pci_release_mem_regions(pdev);

	e_dev_info("complete\n");

	for (i = 0; i < IXGBE_MAX_LINK_HANDLE; i++) {
		if (adapter->jump_tables[i]) {
			kfree(adapter->jump_tables[i]->input);
			kfree(adapter->jump_tables[i]->mask);
		}
		kfree(adapter->jump_tables[i]);
	}

	kfree(adapter->mac_table);
	kfree(adapter->rss_key);
	bitmap_free(adapter->af_xdp_zc_qps);
	disable_dev = !test_and_set_bit(__IXGBE_DISABLED, &adapter->state);
	free_netdev(netdev);

	if (disable_dev)
		pci_disable_device(pdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ixgbe_io_error_detected - called when PCI error is detected
 * @pdev: Pointer to PCI device
 * @state: The current pci connection state
 *
 * This function is called after a PCI bus error affecting
 * this device has been detected.
 */
static pci_ers_result_t ixgbe_io_error_detected(struct pci_dev *pdev,
						pci_channel_state_t state)
{
	struct ixgbe_adapter *adapter = pci_get_drvdata(pdev);
	struct net_device *netdev = adapter->netdev;

#ifdef CONFIG_PCI_IOV
<<<<<<< HEAD
=======
	struct ixgbe_hw *hw = &adapter->hw;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct pci_dev *bdev, *vfdev;
	u32 dw0, dw1, dw2, dw3;
	int vf, pos;
	u16 req_id, pf_func;

	if (adapter->hw.mac.type == ixgbe_mac_82598EB ||
	    adapter->num_vfs == 0)
		goto skip_bad_vf_detection;

	bdev = pdev->bus->self;
<<<<<<< HEAD
	while (bdev && (bdev->pcie_type != PCI_EXP_TYPE_ROOT_PORT))
=======
	while (bdev && (pci_pcie_type(bdev) != PCI_EXP_TYPE_ROOT_PORT))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bdev = bdev->bus->self;

	if (!bdev)
		goto skip_bad_vf_detection;

	pos = pci_find_ext_capability(bdev, PCI_EXT_CAP_ID_ERR);
	if (!pos)
		goto skip_bad_vf_detection;

<<<<<<< HEAD
	pci_read_config_dword(bdev, pos + PCI_ERR_HEADER_LOG, &dw0);
	pci_read_config_dword(bdev, pos + PCI_ERR_HEADER_LOG + 4, &dw1);
	pci_read_config_dword(bdev, pos + PCI_ERR_HEADER_LOG + 8, &dw2);
	pci_read_config_dword(bdev, pos + PCI_ERR_HEADER_LOG + 12, &dw3);
=======
	dw0 = ixgbe_read_pci_cfg_dword(hw, pos + PCI_ERR_HEADER_LOG);
	dw1 = ixgbe_read_pci_cfg_dword(hw, pos + PCI_ERR_HEADER_LOG + 4);
	dw2 = ixgbe_read_pci_cfg_dword(hw, pos + PCI_ERR_HEADER_LOG + 8);
	dw3 = ixgbe_read_pci_cfg_dword(hw, pos + PCI_ERR_HEADER_LOG + 12);
	if (ixgbe_removed(hw->hw_addr))
		goto skip_bad_vf_detection;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	req_id = dw1 >> 16;
	/* On the 82599 if bit 7 of the requestor ID is set then it's a VF */
	if (!(req_id & 0x0080))
		goto skip_bad_vf_detection;

	pf_func = req_id & 0x01;
	if ((pf_func & 1) == (pdev->devfn & 1)) {
		unsigned int device_id;

<<<<<<< HEAD
		vf = (req_id & 0x7F) >> 1;
=======
		vf = FIELD_GET(0x7F, req_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		e_dev_err("VF %d has caused a PCIe error\n", vf);
		e_dev_err("TLP: dw0: %8.8x\tdw1: %8.8x\tdw2: "
				"%8.8x\tdw3: %8.8x\n",
		dw0, dw1, dw2, dw3);
		switch (adapter->hw.mac.type) {
		case ixgbe_mac_82599EB:
			device_id = IXGBE_82599_VF_DEVICE_ID;
			break;
		case ixgbe_mac_X540:
			device_id = IXGBE_X540_VF_DEVICE_ID;
			break;
<<<<<<< HEAD
=======
		case ixgbe_mac_X550:
			device_id = IXGBE_DEV_ID_X550_VF;
			break;
		case ixgbe_mac_X550EM_x:
			device_id = IXGBE_DEV_ID_X550EM_X_VF;
			break;
		case ixgbe_mac_x550em_a:
			device_id = IXGBE_DEV_ID_X550EM_A_VF;
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			device_id = 0;
			break;
		}

		/* Find the pci device of the offending VF */
<<<<<<< HEAD
		vfdev = pci_get_device(IXGBE_INTEL_VENDOR_ID, device_id, NULL);
		while (vfdev) {
			if (vfdev->devfn == (req_id & 0xFF))
				break;
			vfdev = pci_get_device(IXGBE_INTEL_VENDOR_ID,
=======
		vfdev = pci_get_device(PCI_VENDOR_ID_INTEL, device_id, NULL);
		while (vfdev) {
			if (vfdev->devfn == (req_id & 0xFF))
				break;
			vfdev = pci_get_device(PCI_VENDOR_ID_INTEL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					       device_id, vfdev);
		}
		/*
		 * There's a slim chance the VF could have been hot plugged,
		 * so if it is no longer present we don't need to issue the
		 * VFLR.  Just clean up the AER in that case.
		 */
		if (vfdev) {
<<<<<<< HEAD
			e_dev_err("Issuing VFLR to VF %d\n", vf);
			pci_write_config_dword(vfdev, 0xA8, 0x00008000);
		}

		pci_cleanup_aer_uncorrect_error_status(pdev);
=======
			pcie_flr(vfdev);
			/* Free device reference count */
			pci_dev_put(vfdev);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Even though the error may have occurred on the other port
	 * we still need to increment the vf error reference count for
	 * both ports because the I/O resume function will be called
	 * for both of them.
	 */
	adapter->vferr_refcount++;

	return PCI_ERS_RESULT_RECOVERED;

skip_bad_vf_detection:
#endif /* CONFIG_PCI_IOV */
<<<<<<< HEAD
	netif_device_detach(netdev);

	if (state == pci_channel_io_perm_failure)
		return PCI_ERS_RESULT_DISCONNECT;

	if (netif_running(netdev))
		ixgbe_down(adapter);
	pci_disable_device(pdev);
=======
	if (!test_bit(__IXGBE_SERVICE_INITED, &adapter->state))
		return PCI_ERS_RESULT_DISCONNECT;

	if (!netif_device_present(netdev))
		return PCI_ERS_RESULT_DISCONNECT;

	rtnl_lock();
	netif_device_detach(netdev);

	if (netif_running(netdev))
		ixgbe_close_suspend(adapter);

	if (state == pci_channel_io_perm_failure) {
		rtnl_unlock();
		return PCI_ERS_RESULT_DISCONNECT;
	}

	if (!test_and_set_bit(__IXGBE_DISABLED, &adapter->state))
		pci_disable_device(pdev);
	rtnl_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Request a slot reset. */
	return PCI_ERS_RESULT_NEED_RESET;
}

/**
 * ixgbe_io_slot_reset - called after the pci bus has been reset.
 * @pdev: Pointer to PCI device
 *
 * Restart the card from scratch, as if from a cold-boot.
 */
static pci_ers_result_t ixgbe_io_slot_reset(struct pci_dev *pdev)
{
	struct ixgbe_adapter *adapter = pci_get_drvdata(pdev);
	pci_ers_result_t result;
<<<<<<< HEAD
	int err;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (pci_enable_device_mem(pdev)) {
		e_err(probe, "Cannot re-enable PCI device after reset.\n");
		result = PCI_ERS_RESULT_DISCONNECT;
	} else {
<<<<<<< HEAD
=======
		smp_mb__before_atomic();
		clear_bit(__IXGBE_DISABLED, &adapter->state);
		adapter->hw.hw_addr = adapter->io_addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pci_set_master(pdev);
		pci_restore_state(pdev);
		pci_save_state(pdev);

		pci_wake_from_d3(pdev, false);

		ixgbe_reset(adapter);
		IXGBE_WRITE_REG(&adapter->hw, IXGBE_WUS, ~0);
		result = PCI_ERS_RESULT_RECOVERED;
	}

<<<<<<< HEAD
	err = pci_cleanup_aer_uncorrect_error_status(pdev);
	if (err) {
		e_dev_err("pci_cleanup_aer_uncorrect_error_status "
			  "failed 0x%0x\n", err);
		/* non-fatal, continue */
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return result;
}

/**
 * ixgbe_io_resume - called when traffic can start flowing again.
 * @pdev: Pointer to PCI device
 *
 * This callback is called when the error recovery driver tells us that
 * its OK to resume normal operation.
 */
static void ixgbe_io_resume(struct pci_dev *pdev)
{
	struct ixgbe_adapter *adapter = pci_get_drvdata(pdev);
	struct net_device *netdev = adapter->netdev;

#ifdef CONFIG_PCI_IOV
	if (adapter->vferr_refcount) {
		e_info(drv, "Resuming after VF err\n");
		adapter->vferr_refcount--;
		return;
	}

#endif
<<<<<<< HEAD
	if (netif_running(netdev))
		ixgbe_up(adapter);

	netif_device_attach(netdev);
}

static struct pci_error_handlers ixgbe_err_handler = {
=======
	rtnl_lock();
	if (netif_running(netdev))
		ixgbe_open(netdev);

	netif_device_attach(netdev);
	rtnl_unlock();
}

static const struct pci_error_handlers ixgbe_err_handler = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.error_detected = ixgbe_io_error_detected,
	.slot_reset = ixgbe_io_slot_reset,
	.resume = ixgbe_io_resume,
};

<<<<<<< HEAD
static struct pci_driver ixgbe_driver = {
	.name     = ixgbe_driver_name,
	.id_table = ixgbe_pci_tbl,
	.probe    = ixgbe_probe,
	.remove   = __devexit_p(ixgbe_remove),
#ifdef CONFIG_PM
	.suspend  = ixgbe_suspend,
	.resume   = ixgbe_resume,
#endif
	.shutdown = ixgbe_shutdown,
=======
static SIMPLE_DEV_PM_OPS(ixgbe_pm_ops, ixgbe_suspend, ixgbe_resume);

static struct pci_driver ixgbe_driver = {
	.name      = ixgbe_driver_name,
	.id_table  = ixgbe_pci_tbl,
	.probe     = ixgbe_probe,
	.remove    = ixgbe_remove,
	.driver.pm = &ixgbe_pm_ops,
	.shutdown  = ixgbe_shutdown,
	.sriov_configure = ixgbe_pci_sriov_configure,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.err_handler = &ixgbe_err_handler
};

/**
 * ixgbe_init_module - Driver Registration Routine
 *
 * ixgbe_init_module is the first routine called when the driver is
 * loaded. All it does is register with the PCI subsystem.
 **/
static int __init ixgbe_init_module(void)
{
	int ret;
<<<<<<< HEAD
	pr_info("%s - version %s\n", ixgbe_driver_string, ixgbe_driver_version);
	pr_info("%s\n", ixgbe_copyright);

	ret = pci_register_driver(&ixgbe_driver);
	if (ret)
		return ret;
=======
	pr_info("%s\n", ixgbe_driver_string);
	pr_info("%s\n", ixgbe_copyright);

	ixgbe_wq = create_singlethread_workqueue(ixgbe_driver_name);
	if (!ixgbe_wq) {
		pr_err("%s: Failed to create workqueue\n", ixgbe_driver_name);
		return -ENOMEM;
	}

	ixgbe_dbg_init();

	ret = pci_register_driver(&ixgbe_driver);
	if (ret) {
		destroy_workqueue(ixgbe_wq);
		ixgbe_dbg_exit();
		return ret;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_IXGBE_DCA
	dca_register_notify(&dca_notifier);
#endif

	return 0;
}

module_init(ixgbe_init_module);

/**
 * ixgbe_exit_module - Driver Exit Cleanup Routine
 *
 * ixgbe_exit_module is called just before the driver is removed
 * from memory.
 **/
static void __exit ixgbe_exit_module(void)
{
#ifdef CONFIG_IXGBE_DCA
	dca_unregister_notify(&dca_notifier);
#endif
	pci_unregister_driver(&ixgbe_driver);
<<<<<<< HEAD
	rcu_barrier(); /* Wait for completion of call_rcu()'s */
=======

	ixgbe_dbg_exit();
	if (ixgbe_wq) {
		destroy_workqueue(ixgbe_wq);
		ixgbe_wq = NULL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_IXGBE_DCA
static int ixgbe_notify_dca(struct notifier_block *nb, unsigned long event,
			    void *p)
{
	int ret_val;

	ret_val = driver_for_each_device(&ixgbe_driver.driver, NULL, &event,
					 __ixgbe_notify_dca);

	return ret_val ? NOTIFY_BAD : NOTIFY_DONE;
}

#endif /* CONFIG_IXGBE_DCA */

module_exit(ixgbe_exit_module);

/* ixgbe_main.c */
