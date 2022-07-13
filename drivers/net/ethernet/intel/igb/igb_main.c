<<<<<<< HEAD
/*******************************************************************************

  Intel(R) Gigabit Ethernet Linux driver
  Copyright(c) 2007-2012 Intel Corporation.

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
/* Copyright(c) 2007 - 2018 Intel Corporation. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/vmalloc.h>
#include <linux/pagemap.h>
#include <linux/netdevice.h>
#include <linux/ipv6.h>
#include <linux/slab.h>
#include <net/checksum.h>
#include <net/ip6_checksum.h>
<<<<<<< HEAD
=======
#include <net/pkt_sched.h>
#include <net/pkt_cls.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/net_tstamp.h>
#include <linux/mii.h>
#include <linux/ethtool.h>
#include <linux/if.h>
#include <linux/if_vlan.h>
#include <linux/pci.h>
<<<<<<< HEAD
#include <linux/pci-aspm.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/sctp.h>
#include <linux/if_ether.h>
<<<<<<< HEAD
#include <linux/aer.h>
#include <linux/prefetch.h>
#include <linux/pm_runtime.h>
#ifdef CONFIG_IGB_DCA
#include <linux/dca.h>
#endif
#include "igb.h"

#define MAJ 3
#define MIN 2
#define BUILD 10
#define DRV_VERSION __stringify(MAJ) "." __stringify(MIN) "." \
__stringify(BUILD) "-k"
char igb_driver_name[] = "igb";
char igb_driver_version[] = DRV_VERSION;
static const char igb_driver_string[] =
				"Intel(R) Gigabit Ethernet Network Driver";
static const char igb_copyright[] = "Copyright (c) 2007-2012 Intel Corporation.";
=======
#include <linux/prefetch.h>
#include <linux/bpf.h>
#include <linux/bpf_trace.h>
#include <linux/pm_runtime.h>
#include <linux/etherdevice.h>
#ifdef CONFIG_IGB_DCA
#include <linux/dca.h>
#endif
#include <linux/i2c.h>
#include "igb.h"

enum queue_mode {
	QUEUE_MODE_STRICT_PRIORITY,
	QUEUE_MODE_STREAM_RESERVATION,
};

enum tx_queue_prio {
	TX_QUEUE_PRIO_HIGH,
	TX_QUEUE_PRIO_LOW,
};

char igb_driver_name[] = "igb";
static const char igb_driver_string[] =
				"Intel(R) Gigabit Ethernet Network Driver";
static const char igb_copyright[] =
				"Copyright (c) 2007-2014 Intel Corporation.";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct e1000_info *igb_info_tbl[] = {
	[board_82575] = &e1000_82575_info,
};

<<<<<<< HEAD
static DEFINE_PCI_DEVICE_TABLE(igb_pci_tbl) = {
=======
static const struct pci_device_id igb_pci_tbl[] = {
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I354_BACKPLANE_1GBPS) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I354_SGMII) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I354_BACKPLANE_2_5GBPS) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I211_COPPER), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I210_COPPER), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I210_FIBER), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I210_SERDES), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I210_SGMII), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I210_COPPER_FLASHLESS), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I210_SERDES_FLASHLESS), board_82575 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I350_COPPER), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I350_FIBER), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I350_SERDES), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_I350_SGMII), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82580_COPPER), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82580_FIBER), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82580_QUAD_FIBER), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82580_SERDES), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82580_SGMII), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82580_COPPER_DUAL), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_DH89XXCC_SGMII), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_DH89XXCC_SERDES), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_DH89XXCC_BACKPLANE), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_DH89XXCC_SFP), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_NS), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_NS_SERDES), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_FIBER), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_SERDES), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_SERDES_QUAD), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_QUAD_COPPER_ET2), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82576_QUAD_COPPER), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82575EB_COPPER), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82575EB_FIBER_SERDES), board_82575 },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82575GB_QUAD_COPPER), board_82575 },
	/* required last entry */
	{0, }
};

MODULE_DEVICE_TABLE(pci, igb_pci_tbl);

<<<<<<< HEAD
void igb_reset(struct igb_adapter *);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int igb_setup_all_tx_resources(struct igb_adapter *);
static int igb_setup_all_rx_resources(struct igb_adapter *);
static void igb_free_all_tx_resources(struct igb_adapter *);
static void igb_free_all_rx_resources(struct igb_adapter *);
static void igb_setup_mrqc(struct igb_adapter *);
static int igb_probe(struct pci_dev *, const struct pci_device_id *);
<<<<<<< HEAD
static void __devexit igb_remove(struct pci_dev *pdev);
static void igb_init_hw_timer(struct igb_adapter *adapter);
static int igb_sw_init(struct igb_adapter *);
static int igb_open(struct net_device *);
static int igb_close(struct net_device *);
=======
static void igb_remove(struct pci_dev *pdev);
static void igb_init_queue_configuration(struct igb_adapter *adapter);
static int igb_sw_init(struct igb_adapter *);
int igb_open(struct net_device *);
int igb_close(struct net_device *);
static void igb_configure(struct igb_adapter *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void igb_configure_tx(struct igb_adapter *);
static void igb_configure_rx(struct igb_adapter *);
static void igb_clean_all_tx_rings(struct igb_adapter *);
static void igb_clean_all_rx_rings(struct igb_adapter *);
static void igb_clean_tx_ring(struct igb_ring *);
static void igb_clean_rx_ring(struct igb_ring *);
static void igb_set_rx_mode(struct net_device *);
<<<<<<< HEAD
static void igb_update_phy_info(unsigned long);
static void igb_watchdog(unsigned long);
static void igb_watchdog_task(struct work_struct *);
static netdev_tx_t igb_xmit_frame(struct sk_buff *skb, struct net_device *);
static struct rtnl_link_stats64 *igb_get_stats64(struct net_device *dev,
						 struct rtnl_link_stats64 *stats);
static int igb_change_mtu(struct net_device *, int);
static int igb_set_mac(struct net_device *, void *);
static void igb_set_uta(struct igb_adapter *adapter);
=======
static void igb_update_phy_info(struct timer_list *);
static void igb_watchdog(struct timer_list *);
static void igb_watchdog_task(struct work_struct *);
static netdev_tx_t igb_xmit_frame(struct sk_buff *skb, struct net_device *);
static void igb_get_stats64(struct net_device *dev,
			    struct rtnl_link_stats64 *stats);
static int igb_change_mtu(struct net_device *, int);
static int igb_set_mac(struct net_device *, void *);
static void igb_set_uta(struct igb_adapter *adapter, bool set);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static irqreturn_t igb_intr(int irq, void *);
static irqreturn_t igb_intr_msi(int irq, void *);
static irqreturn_t igb_msix_other(int irq, void *);
static irqreturn_t igb_msix_ring(int irq, void *);
#ifdef CONFIG_IGB_DCA
static void igb_update_dca(struct igb_q_vector *);
static void igb_setup_dca(struct igb_adapter *);
#endif /* CONFIG_IGB_DCA */
static int igb_poll(struct napi_struct *, int);
<<<<<<< HEAD
static bool igb_clean_tx_irq(struct igb_q_vector *);
static bool igb_clean_rx_irq(struct igb_q_vector *, int);
static int igb_ioctl(struct net_device *, struct ifreq *, int cmd);
static void igb_tx_timeout(struct net_device *);
static void igb_reset_task(struct work_struct *);
static void igb_vlan_mode(struct net_device *netdev, netdev_features_t features);
static int igb_vlan_rx_add_vid(struct net_device *, u16);
static int igb_vlan_rx_kill_vid(struct net_device *, u16);
static void igb_restore_vlan(struct igb_adapter *);
static void igb_rar_set_qsel(struct igb_adapter *, u8 *, u32 , u8);
=======
static bool igb_clean_tx_irq(struct igb_q_vector *, int);
static int igb_clean_rx_irq(struct igb_q_vector *, int);
static int igb_ioctl(struct net_device *, struct ifreq *, int cmd);
static void igb_tx_timeout(struct net_device *, unsigned int txqueue);
static void igb_reset_task(struct work_struct *);
static void igb_vlan_mode(struct net_device *netdev,
			  netdev_features_t features);
static int igb_vlan_rx_add_vid(struct net_device *, __be16, u16);
static int igb_vlan_rx_kill_vid(struct net_device *, __be16, u16);
static void igb_restore_vlan(struct igb_adapter *);
static void igb_rar_set_index(struct igb_adapter *, u32);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void igb_ping_all_vfs(struct igb_adapter *);
static void igb_msg_task(struct igb_adapter *);
static void igb_vmm_control(struct igb_adapter *);
static int igb_set_vf_mac(struct igb_adapter *, int, unsigned char *);
<<<<<<< HEAD
static void igb_restore_vf_multicasts(struct igb_adapter *adapter);
static int igb_ndo_set_vf_mac(struct net_device *netdev, int vf, u8 *mac);
static int igb_ndo_set_vf_vlan(struct net_device *netdev,
			       int vf, u16 vlan, u8 qos);
static int igb_ndo_set_vf_bw(struct net_device *netdev, int vf, int tx_rate);
static int igb_ndo_get_vf_config(struct net_device *netdev, int vf,
				 struct ifla_vf_info *ivi);
static void igb_check_vf_rate_limit(struct igb_adapter *);

#ifdef CONFIG_PCI_IOV
static int igb_vf_configure(struct igb_adapter *adapter, int vf);
static int igb_find_enabled_vfs(struct igb_adapter *adapter);
static int igb_check_vf_assignment(struct igb_adapter *adapter);
#endif

#ifdef CONFIG_PM
#ifdef CONFIG_PM_SLEEP
static int igb_suspend(struct device *);
#endif
static int igb_resume(struct device *);
#ifdef CONFIG_PM_RUNTIME
static int igb_runtime_suspend(struct device *dev);
static int igb_runtime_resume(struct device *dev);
static int igb_runtime_idle(struct device *dev);
#endif
=======
static void igb_flush_mac_table(struct igb_adapter *);
static int igb_available_rars(struct igb_adapter *, u8);
static void igb_set_default_mac_filter(struct igb_adapter *);
static int igb_uc_sync(struct net_device *, const unsigned char *);
static int igb_uc_unsync(struct net_device *, const unsigned char *);
static void igb_restore_vf_multicasts(struct igb_adapter *adapter);
static int igb_ndo_set_vf_mac(struct net_device *netdev, int vf, u8 *mac);
static int igb_ndo_set_vf_vlan(struct net_device *netdev,
			       int vf, u16 vlan, u8 qos, __be16 vlan_proto);
static int igb_ndo_set_vf_bw(struct net_device *, int, int, int);
static int igb_ndo_set_vf_spoofchk(struct net_device *netdev, int vf,
				   bool setting);
static int igb_ndo_set_vf_trust(struct net_device *netdev, int vf,
				bool setting);
static int igb_ndo_get_vf_config(struct net_device *netdev, int vf,
				 struct ifla_vf_info *ivi);
static void igb_check_vf_rate_limit(struct igb_adapter *);
static void igb_nfc_filter_exit(struct igb_adapter *adapter);
static void igb_nfc_filter_restore(struct igb_adapter *adapter);

#ifdef CONFIG_PCI_IOV
static int igb_vf_configure(struct igb_adapter *adapter, int vf);
static int igb_disable_sriov(struct pci_dev *dev, bool reinit);
#endif

static int igb_suspend(struct device *);
static int igb_resume(struct device *);
static int igb_runtime_suspend(struct device *dev);
static int igb_runtime_resume(struct device *dev);
static int igb_runtime_idle(struct device *dev);
#ifdef CONFIG_PM
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct dev_pm_ops igb_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(igb_suspend, igb_resume)
	SET_RUNTIME_PM_OPS(igb_runtime_suspend, igb_runtime_resume,
			igb_runtime_idle)
};
#endif
static void igb_shutdown(struct pci_dev *);
<<<<<<< HEAD
=======
static int igb_pci_sriov_configure(struct pci_dev *dev, int num_vfs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_IGB_DCA
static int igb_notify_dca(struct notifier_block *, unsigned long, void *);
static struct notifier_block dca_notifier = {
	.notifier_call	= igb_notify_dca,
	.next		= NULL,
	.priority	= 0
};
#endif
<<<<<<< HEAD
#ifdef CONFIG_NET_POLL_CONTROLLER
/* for netdump / net console */
static void igb_netpoll(struct net_device *);
#endif
#ifdef CONFIG_PCI_IOV
static unsigned int max_vfs = 0;
module_param(max_vfs, uint, 0);
MODULE_PARM_DESC(max_vfs, "Maximum number of virtual functions to allocate "
                 "per physical function");
=======
#ifdef CONFIG_PCI_IOV
static unsigned int max_vfs;
module_param(max_vfs, uint, 0444);
MODULE_PARM_DESC(max_vfs, "Maximum number of virtual functions to allocate per physical function");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_PCI_IOV */

static pci_ers_result_t igb_io_error_detected(struct pci_dev *,
		     pci_channel_state_t);
static pci_ers_result_t igb_io_slot_reset(struct pci_dev *);
static void igb_io_resume(struct pci_dev *);

<<<<<<< HEAD
static struct pci_error_handlers igb_err_handler = {
=======
static const struct pci_error_handlers igb_err_handler = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.error_detected = igb_io_error_detected,
	.slot_reset = igb_io_slot_reset,
	.resume = igb_io_resume,
};

static void igb_init_dmac(struct igb_adapter *adapter, u32 pba);

static struct pci_driver igb_driver = {
	.name     = igb_driver_name,
	.id_table = igb_pci_tbl,
	.probe    = igb_probe,
<<<<<<< HEAD
	.remove   = __devexit_p(igb_remove),
=======
	.remove   = igb_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_PM
	.driver.pm = &igb_pm_ops,
#endif
	.shutdown = igb_shutdown,
<<<<<<< HEAD
=======
	.sriov_configure = igb_pci_sriov_configure,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.err_handler = &igb_err_handler
};

MODULE_AUTHOR("Intel Corporation, <e1000-devel@lists.sourceforge.net>");
MODULE_DESCRIPTION("Intel(R) Gigabit Ethernet Network Driver");
<<<<<<< HEAD
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
=======
MODULE_LICENSE("GPL v2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define DEFAULT_MSG_ENABLE (NETIF_MSG_DRV|NETIF_MSG_PROBE|NETIF_MSG_LINK)
static int debug = -1;
module_param(debug, int, 0);
MODULE_PARM_DESC(debug, "Debug level (0=none,...,16=all)");

struct igb_reg_info {
	u32 ofs;
	char *name;
};

static const struct igb_reg_info igb_reg_info_tbl[] = {

	/* General Registers */
	{E1000_CTRL, "CTRL"},
	{E1000_STATUS, "STATUS"},
	{E1000_CTRL_EXT, "CTRL_EXT"},

	/* Interrupt Registers */
	{E1000_ICR, "ICR"},

	/* RX Registers */
	{E1000_RCTL, "RCTL"},
	{E1000_RDLEN(0), "RDLEN"},
	{E1000_RDH(0), "RDH"},
	{E1000_RDT(0), "RDT"},
	{E1000_RXDCTL(0), "RXDCTL"},
	{E1000_RDBAL(0), "RDBAL"},
	{E1000_RDBAH(0), "RDBAH"},

	/* TX Registers */
	{E1000_TCTL, "TCTL"},
	{E1000_TDBAL(0), "TDBAL"},
	{E1000_TDBAH(0), "TDBAH"},
	{E1000_TDLEN(0), "TDLEN"},
	{E1000_TDH(0), "TDH"},
	{E1000_TDT(0), "TDT"},
	{E1000_TXDCTL(0), "TXDCTL"},
	{E1000_TDFH, "TDFH"},
	{E1000_TDFT, "TDFT"},
	{E1000_TDFHS, "TDFHS"},
	{E1000_TDFPC, "TDFPC"},

	/* List Terminator */
	{}
};

<<<<<<< HEAD
/*
 * igb_regdump - register printout routine
 */
=======
/* igb_regdump - register printout routine */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void igb_regdump(struct e1000_hw *hw, struct igb_reg_info *reginfo)
{
	int n = 0;
	char rname[16];
	u32 regs[8];

	switch (reginfo->ofs) {
	case E1000_RDLEN(0):
		for (n = 0; n < 4; n++)
			regs[n] = rd32(E1000_RDLEN(n));
		break;
	case E1000_RDH(0):
		for (n = 0; n < 4; n++)
			regs[n] = rd32(E1000_RDH(n));
		break;
	case E1000_RDT(0):
		for (n = 0; n < 4; n++)
			regs[n] = rd32(E1000_RDT(n));
		break;
	case E1000_RXDCTL(0):
		for (n = 0; n < 4; n++)
			regs[n] = rd32(E1000_RXDCTL(n));
		break;
	case E1000_RDBAL(0):
		for (n = 0; n < 4; n++)
			regs[n] = rd32(E1000_RDBAL(n));
		break;
	case E1000_RDBAH(0):
		for (n = 0; n < 4; n++)
			regs[n] = rd32(E1000_RDBAH(n));
		break;
	case E1000_TDBAL(0):
		for (n = 0; n < 4; n++)
<<<<<<< HEAD
			regs[n] = rd32(E1000_RDBAL(n));
=======
			regs[n] = rd32(E1000_TDBAL(n));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case E1000_TDBAH(0):
		for (n = 0; n < 4; n++)
			regs[n] = rd32(E1000_TDBAH(n));
		break;
	case E1000_TDLEN(0):
		for (n = 0; n < 4; n++)
			regs[n] = rd32(E1000_TDLEN(n));
		break;
	case E1000_TDH(0):
		for (n = 0; n < 4; n++)
			regs[n] = rd32(E1000_TDH(n));
		break;
	case E1000_TDT(0):
		for (n = 0; n < 4; n++)
			regs[n] = rd32(E1000_TDT(n));
		break;
	case E1000_TXDCTL(0):
		for (n = 0; n < 4; n++)
			regs[n] = rd32(E1000_TXDCTL(n));
		break;
	default:
		pr_info("%-15s %08x\n", reginfo->name, rd32(reginfo->ofs));
		return;
	}

	snprintf(rname, 16, "%s%s", reginfo->name, "[0-3]");
	pr_info("%-15s %08x %08x %08x %08x\n", rname, regs[0], regs[1],
		regs[2], regs[3]);
}

<<<<<<< HEAD
/*
 * igb_dump - Print registers, tx-rings and rx-rings
 */
=======
/* igb_dump - Print registers, Tx-rings and Rx-rings */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void igb_dump(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct e1000_hw *hw = &adapter->hw;
	struct igb_reg_info *reginfo;
	struct igb_ring *tx_ring;
	union e1000_adv_tx_desc *tx_desc;
<<<<<<< HEAD
	struct my_u0 { u64 a; u64 b; } *u0;
=======
	struct my_u0 { __le64 a; __le64 b; } *u0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct igb_ring *rx_ring;
	union e1000_adv_rx_desc *rx_desc;
	u32 staterr;
	u16 i, n;

	if (!netif_msg_hw(adapter))
		return;

	/* Print netdevice Info */
	if (netdev) {
		dev_info(&adapter->pdev->dev, "Net device Info\n");
<<<<<<< HEAD
		pr_info("Device Name     state            trans_start      "
			"last_rx\n");
		pr_info("%-15s %016lX %016lX %016lX\n", netdev->name,
			netdev->state, netdev->trans_start, netdev->last_rx);
=======
		pr_info("Device Name     state            trans_start\n");
		pr_info("%-15s %016lX %016lX\n", netdev->name,
			netdev->state, dev_trans_start(netdev));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Print Registers */
	dev_info(&adapter->pdev->dev, "Register Dump\n");
	pr_info(" Register Name   Value\n");
	for (reginfo = (struct igb_reg_info *)igb_reg_info_tbl;
	     reginfo->name; reginfo++) {
		igb_regdump(hw, reginfo);
	}

	/* Print TX Ring Summary */
	if (!netdev || !netif_running(netdev))
		goto exit;

	dev_info(&adapter->pdev->dev, "TX Rings Summary\n");
	pr_info("Queue [NTU] [NTC] [bi(ntc)->dma  ] leng ntw timestamp\n");
	for (n = 0; n < adapter->num_tx_queues; n++) {
		struct igb_tx_buffer *buffer_info;
		tx_ring = adapter->tx_ring[n];
		buffer_info = &tx_ring->tx_buffer_info[tx_ring->next_to_clean];
		pr_info(" %5d %5X %5X %016llX %04X %p %016llX\n",
			n, tx_ring->next_to_use, tx_ring->next_to_clean,
<<<<<<< HEAD
			(u64)buffer_info->dma,
			buffer_info->length,
=======
			(u64)dma_unmap_addr(buffer_info, dma),
			dma_unmap_len(buffer_info, len),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			buffer_info->next_to_watch,
			(u64)buffer_info->time_stamp);
	}

	/* Print TX Rings */
	if (!netif_msg_tx_done(adapter))
		goto rx_ring_summary;

	dev_info(&adapter->pdev->dev, "TX Rings Dump\n");

	/* Transmit Descriptor Formats
	 *
	 * Advanced Transmit Descriptor
	 *   +--------------------------------------------------------------+
	 * 0 |         Buffer Address [63:0]                                |
	 *   +--------------------------------------------------------------+
	 * 8 | PAYLEN  | PORTS  |CC|IDX | STA | DCMD  |DTYP|MAC|RSV| DTALEN |
	 *   +--------------------------------------------------------------+
	 *   63      46 45    40 39 38 36 35 32 31   24             15       0
	 */

	for (n = 0; n < adapter->num_tx_queues; n++) {
		tx_ring = adapter->tx_ring[n];
		pr_info("------------------------------------\n");
		pr_info("TX QUEUE INDEX = %d\n", tx_ring->queue_index);
		pr_info("------------------------------------\n");
<<<<<<< HEAD
		pr_info("T [desc]     [address 63:0  ] [PlPOCIStDDM Ln] "
			"[bi->dma       ] leng  ntw timestamp        "
			"bi->skb\n");
=======
		pr_info("T [desc]     [address 63:0  ] [PlPOCIStDDM Ln] [bi->dma       ] leng  ntw timestamp        bi->skb\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		for (i = 0; tx_ring->desc && (i < tx_ring->count); i++) {
			const char *next_desc;
			struct igb_tx_buffer *buffer_info;
			tx_desc = IGB_TX_DESC(tx_ring, i);
			buffer_info = &tx_ring->tx_buffer_info[i];
			u0 = (struct my_u0 *)tx_desc;
			if (i == tx_ring->next_to_use &&
			    i == tx_ring->next_to_clean)
				next_desc = " NTC/U";
			else if (i == tx_ring->next_to_use)
				next_desc = " NTU";
			else if (i == tx_ring->next_to_clean)
				next_desc = " NTC";
			else
				next_desc = "";

<<<<<<< HEAD
			pr_info("T [0x%03X]    %016llX %016llX %016llX"
				" %04X  %p %016llX %p%s\n", i,
				le64_to_cpu(u0->a),
				le64_to_cpu(u0->b),
				(u64)buffer_info->dma,
				buffer_info->length,
=======
			pr_info("T [0x%03X]    %016llX %016llX %016llX %04X  %p %016llX %p%s\n",
				i, le64_to_cpu(u0->a),
				le64_to_cpu(u0->b),
				(u64)dma_unmap_addr(buffer_info, dma),
				dma_unmap_len(buffer_info, len),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				buffer_info->next_to_watch,
				(u64)buffer_info->time_stamp,
				buffer_info->skb, next_desc);

<<<<<<< HEAD
			if (netif_msg_pktdata(adapter) && buffer_info->dma != 0)
				print_hex_dump(KERN_INFO, "",
					DUMP_PREFIX_ADDRESS,
					16, 1, phys_to_virt(buffer_info->dma),
					buffer_info->length, true);
=======
			if (netif_msg_pktdata(adapter) && buffer_info->skb)
				print_hex_dump(KERN_INFO, "",
					DUMP_PREFIX_ADDRESS,
					16, 1, buffer_info->skb->data,
					dma_unmap_len(buffer_info, len),
					true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/* Print RX Rings Summary */
rx_ring_summary:
	dev_info(&adapter->pdev->dev, "RX Rings Summary\n");
	pr_info("Queue [NTU] [NTC]\n");
	for (n = 0; n < adapter->num_rx_queues; n++) {
		rx_ring = adapter->rx_ring[n];
		pr_info(" %5d %5X %5X\n",
			n, rx_ring->next_to_use, rx_ring->next_to_clean);
	}

	/* Print RX Rings */
	if (!netif_msg_rx_status(adapter))
		goto exit;

	dev_info(&adapter->pdev->dev, "RX Rings Dump\n");

	/* Advanced Receive Descriptor (Read) Format
	 *    63                                           1        0
	 *    +-----------------------------------------------------+
	 *  0 |       Packet Buffer Address [63:1]           |A0/NSE|
	 *    +----------------------------------------------+------+
	 *  8 |       Header Buffer Address [63:1]           |  DD  |
	 *    +-----------------------------------------------------+
	 *
	 *
	 * Advanced Receive Descriptor (Write-Back) Format
	 *
	 *   63       48 47    32 31  30      21 20 17 16   4 3     0
	 *   +------------------------------------------------------+
	 * 0 | Packet     IP     |SPH| HDR_LEN   | RSV|Packet|  RSS |
	 *   | Checksum   Ident  |   |           |    | Type | Type |
	 *   +------------------------------------------------------+
	 * 8 | VLAN Tag | Length | Extended Error | Extended Status |
	 *   +------------------------------------------------------+
	 *   63       48 47    32 31            20 19               0
	 */

	for (n = 0; n < adapter->num_rx_queues; n++) {
		rx_ring = adapter->rx_ring[n];
		pr_info("------------------------------------\n");
		pr_info("RX QUEUE INDEX = %d\n", rx_ring->queue_index);
		pr_info("------------------------------------\n");
<<<<<<< HEAD
		pr_info("R  [desc]      [ PktBuf     A0] [  HeadBuf   DD] "
			"[bi->dma       ] [bi->skb] <-- Adv Rx Read format\n");
		pr_info("RWB[desc]      [PcsmIpSHl PtRs] [vl er S cks ln] -----"
			"----------- [bi->skb] <-- Adv Rx Write-Back format\n");
=======
		pr_info("R  [desc]      [ PktBuf     A0] [  HeadBuf   DD] [bi->dma       ] [bi->skb] <-- Adv Rx Read format\n");
		pr_info("RWB[desc]      [PcsmIpSHl PtRs] [vl er S cks ln] ---------------- [bi->skb] <-- Adv Rx Write-Back format\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		for (i = 0; i < rx_ring->count; i++) {
			const char *next_desc;
			struct igb_rx_buffer *buffer_info;
			buffer_info = &rx_ring->rx_buffer_info[i];
			rx_desc = IGB_RX_DESC(rx_ring, i);
			u0 = (struct my_u0 *)rx_desc;
			staterr = le32_to_cpu(rx_desc->wb.upper.status_error);

			if (i == rx_ring->next_to_use)
				next_desc = " NTU";
			else if (i == rx_ring->next_to_clean)
				next_desc = " NTC";
			else
				next_desc = "";

			if (staterr & E1000_RXD_STAT_DD) {
				/* Descriptor Done */
<<<<<<< HEAD
				pr_info("%s[0x%03X]     %016llX %016llX -------"
					"--------- %p%s\n", "RWB", i,
					le64_to_cpu(u0->a),
					le64_to_cpu(u0->b),
					buffer_info->skb, next_desc);
			} else {
				pr_info("%s[0x%03X]     %016llX %016llX %016llX"
					" %p%s\n", "R  ", i,
					le64_to_cpu(u0->a),
					le64_to_cpu(u0->b),
					(u64)buffer_info->dma,
					buffer_info->skb, next_desc);

				if (netif_msg_pktdata(adapter)) {
					print_hex_dump(KERN_INFO, "",
						DUMP_PREFIX_ADDRESS,
						16, 1,
						phys_to_virt(buffer_info->dma),
						IGB_RX_HDR_LEN, true);
					print_hex_dump(KERN_INFO, "",
					  DUMP_PREFIX_ADDRESS,
					  16, 1,
					  phys_to_virt(
					    buffer_info->page_dma +
					    buffer_info->page_offset),
					  PAGE_SIZE/2, true);
=======
				pr_info("%s[0x%03X]     %016llX %016llX ---------------- %s\n",
					"RWB", i,
					le64_to_cpu(u0->a),
					le64_to_cpu(u0->b),
					next_desc);
			} else {
				pr_info("%s[0x%03X]     %016llX %016llX %016llX %s\n",
					"R  ", i,
					le64_to_cpu(u0->a),
					le64_to_cpu(u0->b),
					(u64)buffer_info->dma,
					next_desc);

				if (netif_msg_pktdata(adapter) &&
				    buffer_info->dma && buffer_info->page) {
					print_hex_dump(KERN_INFO, "",
					  DUMP_PREFIX_ADDRESS,
					  16, 1,
					  page_address(buffer_info->page) +
						      buffer_info->page_offset,
					  igb_rx_bufsz(rx_ring), true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				}
			}
		}
	}

exit:
	return;
}

<<<<<<< HEAD

/**
 * igb_read_clock - read raw cycle counter (to be used by time counter)
 */
static cycle_t igb_read_clock(const struct cyclecounter *tc)
{
	struct igb_adapter *adapter =
		container_of(tc, struct igb_adapter, cycles);
	struct e1000_hw *hw = &adapter->hw;
	u64 stamp = 0;
	int shift = 0;

	/*
	 * The timestamp latches on lowest register read. For the 82580
	 * the lowest register is SYSTIMR instead of SYSTIML.  However we never
	 * adjusted TIMINCA so SYSTIMR will just read as all 0s so ignore it.
	 */
	if (hw->mac.type >= e1000_82580) {
		stamp = rd32(E1000_SYSTIMR) >> 8;
		shift = IGB_82580_TSYNC_SHIFT;
	}

	stamp |= (u64)rd32(E1000_SYSTIML) << shift;
	stamp |= (u64)rd32(E1000_SYSTIMH) << (shift + 32);
	return stamp;
}

/**
 * igb_get_hw_dev - return device
 * used by hardware layer to print debugging information
=======
/**
 *  igb_get_i2c_data - Reads the I2C SDA data bit
 *  @data: opaque pointer to adapter struct
 *
 *  Returns the I2C data bit value
 **/
static int igb_get_i2c_data(void *data)
{
	struct igb_adapter *adapter = (struct igb_adapter *)data;
	struct e1000_hw *hw = &adapter->hw;
	s32 i2cctl = rd32(E1000_I2CPARAMS);

	return !!(i2cctl & E1000_I2C_DATA_IN);
}

/**
 *  igb_set_i2c_data - Sets the I2C data bit
 *  @data: pointer to hardware structure
 *  @state: I2C data value (0 or 1) to set
 *
 *  Sets the I2C data bit
 **/
static void igb_set_i2c_data(void *data, int state)
{
	struct igb_adapter *adapter = (struct igb_adapter *)data;
	struct e1000_hw *hw = &adapter->hw;
	s32 i2cctl = rd32(E1000_I2CPARAMS);

	if (state) {
		i2cctl |= E1000_I2C_DATA_OUT | E1000_I2C_DATA_OE_N;
	} else {
		i2cctl &= ~E1000_I2C_DATA_OE_N;
		i2cctl &= ~E1000_I2C_DATA_OUT;
	}

	wr32(E1000_I2CPARAMS, i2cctl);
	wrfl();
}

/**
 *  igb_set_i2c_clk - Sets the I2C SCL clock
 *  @data: pointer to hardware structure
 *  @state: state to set clock
 *
 *  Sets the I2C clock line to state
 **/
static void igb_set_i2c_clk(void *data, int state)
{
	struct igb_adapter *adapter = (struct igb_adapter *)data;
	struct e1000_hw *hw = &adapter->hw;
	s32 i2cctl = rd32(E1000_I2CPARAMS);

	if (state) {
		i2cctl |= E1000_I2C_CLK_OUT | E1000_I2C_CLK_OE_N;
	} else {
		i2cctl &= ~E1000_I2C_CLK_OUT;
		i2cctl &= ~E1000_I2C_CLK_OE_N;
	}
	wr32(E1000_I2CPARAMS, i2cctl);
	wrfl();
}

/**
 *  igb_get_i2c_clk - Gets the I2C SCL clock state
 *  @data: pointer to hardware structure
 *
 *  Gets the I2C clock state
 **/
static int igb_get_i2c_clk(void *data)
{
	struct igb_adapter *adapter = (struct igb_adapter *)data;
	struct e1000_hw *hw = &adapter->hw;
	s32 i2cctl = rd32(E1000_I2CPARAMS);

	return !!(i2cctl & E1000_I2C_CLK_IN);
}

static const struct i2c_algo_bit_data igb_i2c_algo = {
	.setsda		= igb_set_i2c_data,
	.setscl		= igb_set_i2c_clk,
	.getsda		= igb_get_i2c_data,
	.getscl		= igb_get_i2c_clk,
	.udelay		= 5,
	.timeout	= 20,
};

/**
 *  igb_get_hw_dev - return device
 *  @hw: pointer to hardware structure
 *
 *  used by hardware layer to print debugging information
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
struct net_device *igb_get_hw_dev(struct e1000_hw *hw)
{
	struct igb_adapter *adapter = hw->back;
	return adapter->netdev;
}

/**
<<<<<<< HEAD
 * igb_init_module - Driver Registration Routine
 *
 * igb_init_module is the first routine called when the driver is
 * loaded. All it does is register with the PCI subsystem.
=======
 *  igb_init_module - Driver Registration Routine
 *
 *  igb_init_module is the first routine called when the driver is
 *  loaded. All it does is register with the PCI subsystem.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static int __init igb_init_module(void)
{
	int ret;
<<<<<<< HEAD
	pr_info("%s - version %s\n",
	       igb_driver_string, igb_driver_version);

=======

	pr_info("%s\n", igb_driver_string);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pr_info("%s\n", igb_copyright);

#ifdef CONFIG_IGB_DCA
	dca_register_notify(&dca_notifier);
#endif
	ret = pci_register_driver(&igb_driver);
	return ret;
}

module_init(igb_init_module);

/**
<<<<<<< HEAD
 * igb_exit_module - Driver Exit Cleanup Routine
 *
 * igb_exit_module is called just before the driver is removed
 * from memory.
=======
 *  igb_exit_module - Driver Exit Cleanup Routine
 *
 *  igb_exit_module is called just before the driver is removed
 *  from memory.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void __exit igb_exit_module(void)
{
#ifdef CONFIG_IGB_DCA
	dca_unregister_notify(&dca_notifier);
#endif
	pci_unregister_driver(&igb_driver);
}

module_exit(igb_exit_module);

#define Q_IDX_82576(i) (((i & 0x1) << 3) + (i >> 1))
/**
<<<<<<< HEAD
 * igb_cache_ring_register - Descriptor ring to register mapping
 * @adapter: board private structure to initialize
 *
 * Once we know the feature-set enabled for the device, we'll cache
 * the register offset the descriptor ring is assigned to.
=======
 *  igb_cache_ring_register - Descriptor ring to register mapping
 *  @adapter: board private structure to initialize
 *
 *  Once we know the feature-set enabled for the device, we'll cache
 *  the register offset the descriptor ring is assigned to.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_cache_ring_register(struct igb_adapter *adapter)
{
	int i = 0, j = 0;
	u32 rbase_offset = adapter->vfs_allocated_count;

	switch (adapter->hw.mac.type) {
	case e1000_82576:
		/* The queues are allocated for virtualization such that VF 0
		 * is allocated queues 0 and 8, VF 1 queues 1 and 9, etc.
		 * In order to avoid collision we start at the first free queue
		 * and continue consuming queues in the same sequence
		 */
		if (adapter->vfs_allocated_count) {
			for (; i < adapter->rss_queues; i++)
				adapter->rx_ring[i]->reg_idx = rbase_offset +
<<<<<<< HEAD
				                               Q_IDX_82576(i);
		}
	case e1000_82575:
	case e1000_82580:
	case e1000_i350:
=======
							       Q_IDX_82576(i);
		}
		fallthrough;
	case e1000_82575:
	case e1000_82580:
	case e1000_i350:
	case e1000_i354:
	case e1000_i210:
	case e1000_i211:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		for (; i < adapter->num_rx_queues; i++)
			adapter->rx_ring[i]->reg_idx = rbase_offset + i;
		for (; j < adapter->num_tx_queues; j++)
			adapter->tx_ring[j]->reg_idx = rbase_offset + j;
		break;
	}
}

<<<<<<< HEAD
static void igb_free_queues(struct igb_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_tx_queues; i++) {
		kfree(adapter->tx_ring[i]);
		adapter->tx_ring[i] = NULL;
	}
	for (i = 0; i < adapter->num_rx_queues; i++) {
		kfree(adapter->rx_ring[i]);
		adapter->rx_ring[i] = NULL;
	}
	adapter->num_rx_queues = 0;
	adapter->num_tx_queues = 0;
}

/**
 * igb_alloc_queues - Allocate memory for all rings
 * @adapter: board private structure to initialize
 *
 * We allocate one ring per queue at run-time since we don't know the
 * number of queues at compile-time.
 **/
static int igb_alloc_queues(struct igb_adapter *adapter)
{
	struct igb_ring *ring;
	int i;
	int orig_node = adapter->node;

	for (i = 0; i < adapter->num_tx_queues; i++) {
		if (orig_node == -1) {
			int cur_node = next_online_node(adapter->node);
			if (cur_node == MAX_NUMNODES)
				cur_node = first_online_node;
			adapter->node = cur_node;
		}
		ring = kzalloc_node(sizeof(struct igb_ring), GFP_KERNEL,
				    adapter->node);
		if (!ring)
			ring = kzalloc(sizeof(struct igb_ring), GFP_KERNEL);
		if (!ring)
			goto err;
		ring->count = adapter->tx_ring_count;
		ring->queue_index = i;
		ring->dev = &adapter->pdev->dev;
		ring->netdev = adapter->netdev;
		ring->numa_node = adapter->node;
		/* For 82575, context index must be unique per ring. */
		if (adapter->hw.mac.type == e1000_82575)
			set_bit(IGB_RING_FLAG_TX_CTX_IDX, &ring->flags);
		u64_stats_init(&ring->tx_syncp);
		u64_stats_init(&ring->tx_syncp2);
		adapter->tx_ring[i] = ring;
	}
	/* Restore the adapter's original node */
	adapter->node = orig_node;

	for (i = 0; i < adapter->num_rx_queues; i++) {
		if (orig_node == -1) {
			int cur_node = next_online_node(adapter->node);
			if (cur_node == MAX_NUMNODES)
				cur_node = first_online_node;
			adapter->node = cur_node;
		}
		ring = kzalloc_node(sizeof(struct igb_ring), GFP_KERNEL,
				    adapter->node);
		if (!ring)
			ring = kzalloc(sizeof(struct igb_ring), GFP_KERNEL);
		if (!ring)
			goto err;
		ring->count = adapter->rx_ring_count;
		ring->queue_index = i;
		ring->dev = &adapter->pdev->dev;
		ring->netdev = adapter->netdev;
		ring->numa_node = adapter->node;
		/* set flag indicating ring supports SCTP checksum offload */
		if (adapter->hw.mac.type >= e1000_82576)
			set_bit(IGB_RING_FLAG_RX_SCTP_CSUM, &ring->flags);

		/* On i350, loopback VLAN packets have the tag byte-swapped. */
		if (adapter->hw.mac.type == e1000_i350)
			set_bit(IGB_RING_FLAG_RX_LB_VLAN_BSWAP, &ring->flags);

		u64_stats_init(&ring->rx_syncp);
		adapter->rx_ring[i] = ring;
	}
	/* Restore the adapter's original node */
	adapter->node = orig_node;

	igb_cache_ring_register(adapter);

	return 0;

err:
	/* Restore the adapter's original node */
	adapter->node = orig_node;
	igb_free_queues(adapter);

	return -ENOMEM;
=======
u32 igb_rd32(struct e1000_hw *hw, u32 reg)
{
	struct igb_adapter *igb = container_of(hw, struct igb_adapter, hw);
	u8 __iomem *hw_addr = READ_ONCE(hw->hw_addr);
	u32 value = 0;

	if (E1000_REMOVED(hw_addr))
		return ~value;

	value = readl(&hw_addr[reg]);

	/* reads should not return all F's */
	if (!(~value) && (!reg || !(~readl(hw_addr)))) {
		struct net_device *netdev = igb->netdev;
		hw->hw_addr = NULL;
		netdev_err(netdev, "PCIe link lost\n");
		WARN(pci_device_is_present(igb->pdev),
		     "igb: Failed to read reg 0x%x!\n", reg);
	}

	return value;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 *  igb_write_ivar - configure ivar for given MSI-X vector
 *  @hw: pointer to the HW structure
 *  @msix_vector: vector number we are allocating to a given ring
 *  @index: row index of IVAR register to write within IVAR table
 *  @offset: column offset of in IVAR, should be multiple of 8
 *
 *  This function is intended to handle the writing of the IVAR register
 *  for adapters 82576 and newer.  The IVAR table consists of 2 columns,
 *  each containing an cause allocation for an Rx and Tx ring, and a
 *  variable number of rows depending on the number of queues supported.
 **/
static void igb_write_ivar(struct e1000_hw *hw, int msix_vector,
			   int index, int offset)
{
	u32 ivar = array_rd32(E1000_IVAR0, index);

	/* clear any bits that are currently set */
	ivar &= ~((u32)0xFF << offset);

	/* write vector and valid bit */
	ivar |= (msix_vector | E1000_IVAR_VALID) << offset;

	array_wr32(E1000_IVAR0, index, ivar);
}

#define IGB_N0_QUEUE -1
static void igb_assign_vector(struct igb_q_vector *q_vector, int msix_vector)
{
	struct igb_adapter *adapter = q_vector->adapter;
	struct e1000_hw *hw = &adapter->hw;
	int rx_queue = IGB_N0_QUEUE;
	int tx_queue = IGB_N0_QUEUE;
	u32 msixbm = 0;

	if (q_vector->rx.ring)
		rx_queue = q_vector->rx.ring->reg_idx;
	if (q_vector->tx.ring)
		tx_queue = q_vector->tx.ring->reg_idx;

	switch (hw->mac.type) {
	case e1000_82575:
		/* The 82575 assigns vectors using a bitmask, which matches the
<<<<<<< HEAD
		   bitmask for the EICR/EIMS/EIMC registers.  To assign one
		   or more queues to a vector, we write the appropriate bits
		   into the MSIXBM register for that vector. */
=======
		 * bitmask for the EICR/EIMS/EIMC registers.  To assign one
		 * or more queues to a vector, we write the appropriate bits
		 * into the MSIXBM register for that vector.
		 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rx_queue > IGB_N0_QUEUE)
			msixbm = E1000_EICR_RX_QUEUE0 << rx_queue;
		if (tx_queue > IGB_N0_QUEUE)
			msixbm |= E1000_EICR_TX_QUEUE0 << tx_queue;
<<<<<<< HEAD
		if (!adapter->msix_entries && msix_vector == 0)
=======
		if (!(adapter->flags & IGB_FLAG_HAS_MSIX) && msix_vector == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			msixbm |= E1000_EIMS_OTHER;
		array_wr32(E1000_MSIXBM(0), msix_vector, msixbm);
		q_vector->eims_value = msixbm;
		break;
	case e1000_82576:
<<<<<<< HEAD
		/*
		 * 82576 uses a table that essentially consists of 2 columns
=======
		/* 82576 uses a table that essentially consists of 2 columns
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * with 8 rows.  The ordering is column-major so we use the
		 * lower 3 bits as the row index, and the 4th bit as the
		 * column offset.
		 */
		if (rx_queue > IGB_N0_QUEUE)
			igb_write_ivar(hw, msix_vector,
				       rx_queue & 0x7,
				       (rx_queue & 0x8) << 1);
		if (tx_queue > IGB_N0_QUEUE)
			igb_write_ivar(hw, msix_vector,
				       tx_queue & 0x7,
				       ((tx_queue & 0x8) << 1) + 8);
<<<<<<< HEAD
		q_vector->eims_value = 1 << msix_vector;
		break;
	case e1000_82580:
	case e1000_i350:
		/*
		 * On 82580 and newer adapters the scheme is similar to 82576
=======
		q_vector->eims_value = BIT(msix_vector);
		break;
	case e1000_82580:
	case e1000_i350:
	case e1000_i354:
	case e1000_i210:
	case e1000_i211:
		/* On 82580 and newer adapters the scheme is similar to 82576
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * however instead of ordering column-major we have things
		 * ordered row-major.  So we traverse the table by using
		 * bit 0 as the column offset, and the remaining bits as the
		 * row index.
		 */
		if (rx_queue > IGB_N0_QUEUE)
			igb_write_ivar(hw, msix_vector,
				       rx_queue >> 1,
				       (rx_queue & 0x1) << 4);
		if (tx_queue > IGB_N0_QUEUE)
			igb_write_ivar(hw, msix_vector,
				       tx_queue >> 1,
				       ((tx_queue & 0x1) << 4) + 8);
<<<<<<< HEAD
		q_vector->eims_value = 1 << msix_vector;
=======
		q_vector->eims_value = BIT(msix_vector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		BUG();
		break;
	}

	/* add q_vector eims value to global eims_enable_mask */
	adapter->eims_enable_mask |= q_vector->eims_value;

	/* configure q_vector to set itr on first interrupt */
	q_vector->set_itr = 1;
}

/**
<<<<<<< HEAD
 * igb_configure_msix - Configure MSI-X hardware
 *
 * igb_configure_msix sets up the hardware to properly
 * generate MSI-X interrupts.
=======
 *  igb_configure_msix - Configure MSI-X hardware
 *  @adapter: board private structure to initialize
 *
 *  igb_configure_msix sets up the hardware to properly
 *  generate MSI-X interrupts.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_configure_msix(struct igb_adapter *adapter)
{
	u32 tmp;
	int i, vector = 0;
	struct e1000_hw *hw = &adapter->hw;

	adapter->eims_enable_mask = 0;

	/* set vector for other causes, i.e. link changes */
	switch (hw->mac.type) {
	case e1000_82575:
		tmp = rd32(E1000_CTRL_EXT);
		/* enable MSI-X PBA support*/
		tmp |= E1000_CTRL_EXT_PBA_CLR;

		/* Auto-Mask interrupts upon ICR read. */
		tmp |= E1000_CTRL_EXT_EIAME;
		tmp |= E1000_CTRL_EXT_IRCA;

		wr32(E1000_CTRL_EXT, tmp);

		/* enable msix_other interrupt */
<<<<<<< HEAD
		array_wr32(E1000_MSIXBM(0), vector++,
		                      E1000_EIMS_OTHER);
=======
		array_wr32(E1000_MSIXBM(0), vector++, E1000_EIMS_OTHER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		adapter->eims_other = E1000_EIMS_OTHER;

		break;

	case e1000_82576:
	case e1000_82580:
	case e1000_i350:
<<<<<<< HEAD
		/* Turn on MSI-X capability first, or our settings
		 * won't stick.  And it will take days to debug. */
		wr32(E1000_GPIE, E1000_GPIE_MSIX_MODE |
		                E1000_GPIE_PBA | E1000_GPIE_EIAME |
		                E1000_GPIE_NSICR);

		/* enable msix_other interrupt */
		adapter->eims_other = 1 << vector;
=======
	case e1000_i354:
	case e1000_i210:
	case e1000_i211:
		/* Turn on MSI-X capability first, or our settings
		 * won't stick.  And it will take days to debug.
		 */
		wr32(E1000_GPIE, E1000_GPIE_MSIX_MODE |
		     E1000_GPIE_PBA | E1000_GPIE_EIAME |
		     E1000_GPIE_NSICR);

		/* enable msix_other interrupt */
		adapter->eims_other = BIT(vector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tmp = (vector++ | E1000_IVAR_VALID) << 8;

		wr32(E1000_IVAR_MISC, tmp);
		break;
	default:
		/* do nothing, since nothing else supports MSI-X */
		break;
	} /* switch (hw->mac.type) */

	adapter->eims_enable_mask |= adapter->eims_other;

	for (i = 0; i < adapter->num_q_vectors; i++)
		igb_assign_vector(adapter->q_vector[i], vector++);

	wrfl();
}

/**
<<<<<<< HEAD
 * igb_request_msix - Initialize MSI-X interrupts
 *
 * igb_request_msix allocates MSI-X vectors and requests interrupts from the
 * kernel.
 **/
static int igb_request_msix(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct e1000_hw *hw = &adapter->hw;
	int i, err = 0, vector = 0, free_vector = 0;

	err = request_irq(adapter->msix_entries[vector].vector,
	                  igb_msix_other, 0, netdev->name, adapter);
	if (err)
		goto err_out;

	for (i = 0; i < adapter->num_q_vectors; i++) {
=======
 *  igb_request_msix - Initialize MSI-X interrupts
 *  @adapter: board private structure to initialize
 *
 *  igb_request_msix allocates MSI-X vectors and requests interrupts from the
 *  kernel.
 **/
static int igb_request_msix(struct igb_adapter *adapter)
{
	unsigned int num_q_vectors = adapter->num_q_vectors;
	struct net_device *netdev = adapter->netdev;
	int i, err = 0, vector = 0, free_vector = 0;

	err = request_irq(adapter->msix_entries[vector].vector,
			  igb_msix_other, 0, netdev->name, adapter);
	if (err)
		goto err_out;

	if (num_q_vectors > MAX_Q_VECTORS) {
		num_q_vectors = MAX_Q_VECTORS;
		dev_warn(&adapter->pdev->dev,
			 "The number of queue vectors (%d) is higher than max allowed (%d)\n",
			 adapter->num_q_vectors, MAX_Q_VECTORS);
	}
	for (i = 0; i < num_q_vectors; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct igb_q_vector *q_vector = adapter->q_vector[i];

		vector++;

<<<<<<< HEAD
		q_vector->itr_register = hw->hw_addr + E1000_EITR(vector);
=======
		q_vector->itr_register = adapter->io_addr + E1000_EITR(vector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (q_vector->rx.ring && q_vector->tx.ring)
			sprintf(q_vector->name, "%s-TxRx-%u", netdev->name,
				q_vector->rx.ring->queue_index);
		else if (q_vector->tx.ring)
			sprintf(q_vector->name, "%s-tx-%u", netdev->name,
				q_vector->tx.ring->queue_index);
		else if (q_vector->rx.ring)
			sprintf(q_vector->name, "%s-rx-%u", netdev->name,
				q_vector->rx.ring->queue_index);
		else
			sprintf(q_vector->name, "%s-unused", netdev->name);

		err = request_irq(adapter->msix_entries[vector].vector,
<<<<<<< HEAD
		                  igb_msix_ring, 0, q_vector->name,
		                  q_vector);
=======
				  igb_msix_ring, 0, q_vector->name,
				  q_vector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			goto err_free;
	}

	igb_configure_msix(adapter);
	return 0;

err_free:
	/* free already assigned IRQs */
	free_irq(adapter->msix_entries[free_vector++].vector, adapter);

	vector--;
	for (i = 0; i < vector; i++) {
		free_irq(adapter->msix_entries[free_vector++].vector,
			 adapter->q_vector[i]);
	}
err_out:
	return err;
}

<<<<<<< HEAD
static void igb_reset_interrupt_capability(struct igb_adapter *adapter)
{
	if (adapter->msix_entries) {
		pci_disable_msix(adapter->pdev);
		kfree(adapter->msix_entries);
		adapter->msix_entries = NULL;
	} else if (adapter->flags & IGB_FLAG_HAS_MSI) {
		pci_disable_msi(adapter->pdev);
	}
}

/**
 * igb_free_q_vectors - Free memory allocated for interrupt vectors
 * @adapter: board private structure to initialize
 *
 * This function frees the memory allocated to the q_vectors.  In addition if
 * NAPI is enabled it will delete any references to the NAPI struct prior
 * to freeing the q_vector.
 **/
static void igb_free_q_vectors(struct igb_adapter *adapter)
{
	int v_idx;

	for (v_idx = 0; v_idx < adapter->num_q_vectors; v_idx++) {
		struct igb_q_vector *q_vector = adapter->q_vector[v_idx];
		adapter->q_vector[v_idx] = NULL;
		if (!q_vector)
			continue;
		netif_napi_del(&q_vector->napi);
		kfree(q_vector);
	}
	adapter->num_q_vectors = 0;
}

/**
 * igb_clear_interrupt_scheme - reset the device to a state of no interrupts
 *
 * This function resets the device so that it has 0 rx queues, tx queues, and
 * MSI-X interrupts allocated.
 */
static void igb_clear_interrupt_scheme(struct igb_adapter *adapter)
{
	igb_free_queues(adapter);
=======
/**
 *  igb_free_q_vector - Free memory allocated for specific interrupt vector
 *  @adapter: board private structure to initialize
 *  @v_idx: Index of vector to be freed
 *
 *  This function frees the memory allocated to the q_vector.
 **/
static void igb_free_q_vector(struct igb_adapter *adapter, int v_idx)
{
	struct igb_q_vector *q_vector = adapter->q_vector[v_idx];

	adapter->q_vector[v_idx] = NULL;

	/* igb_get_stats64() might access the rings on this vector,
	 * we must wait a grace period before freeing it.
	 */
	if (q_vector)
		kfree_rcu(q_vector, rcu);
}

/**
 *  igb_reset_q_vector - Reset config for interrupt vector
 *  @adapter: board private structure to initialize
 *  @v_idx: Index of vector to be reset
 *
 *  If NAPI is enabled it will delete any references to the
 *  NAPI struct. This is preparation for igb_free_q_vector.
 **/
static void igb_reset_q_vector(struct igb_adapter *adapter, int v_idx)
{
	struct igb_q_vector *q_vector = adapter->q_vector[v_idx];

	/* Coming from igb_set_interrupt_capability, the vectors are not yet
	 * allocated. So, q_vector is NULL so we should stop here.
	 */
	if (!q_vector)
		return;

	if (q_vector->tx.ring)
		adapter->tx_ring[q_vector->tx.ring->queue_index] = NULL;

	if (q_vector->rx.ring)
		adapter->rx_ring[q_vector->rx.ring->queue_index] = NULL;

	netif_napi_del(&q_vector->napi);

}

static void igb_reset_interrupt_capability(struct igb_adapter *adapter)
{
	int v_idx = adapter->num_q_vectors;

	if (adapter->flags & IGB_FLAG_HAS_MSIX)
		pci_disable_msix(adapter->pdev);
	else if (adapter->flags & IGB_FLAG_HAS_MSI)
		pci_disable_msi(adapter->pdev);

	while (v_idx--)
		igb_reset_q_vector(adapter, v_idx);
}

/**
 *  igb_free_q_vectors - Free memory allocated for interrupt vectors
 *  @adapter: board private structure to initialize
 *
 *  This function frees the memory allocated to the q_vectors.  In addition if
 *  NAPI is enabled it will delete any references to the NAPI struct prior
 *  to freeing the q_vector.
 **/
static void igb_free_q_vectors(struct igb_adapter *adapter)
{
	int v_idx = adapter->num_q_vectors;

	adapter->num_tx_queues = 0;
	adapter->num_rx_queues = 0;
	adapter->num_q_vectors = 0;

	while (v_idx--) {
		igb_reset_q_vector(adapter, v_idx);
		igb_free_q_vector(adapter, v_idx);
	}
}

/**
 *  igb_clear_interrupt_scheme - reset the device to a state of no interrupts
 *  @adapter: board private structure to initialize
 *
 *  This function resets the device so that it has 0 Rx queues, Tx queues, and
 *  MSI-X interrupts allocated.
 */
static void igb_clear_interrupt_scheme(struct igb_adapter *adapter)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	igb_free_q_vectors(adapter);
	igb_reset_interrupt_capability(adapter);
}

/**
<<<<<<< HEAD
 * igb_set_interrupt_capability - set MSI or MSI-X if supported
 *
 * Attempt to configure interrupts using the best available
 * capabilities of the hardware and kernel.
 **/
static int igb_set_interrupt_capability(struct igb_adapter *adapter)
=======
 *  igb_set_interrupt_capability - set MSI or MSI-X if supported
 *  @adapter: board private structure to initialize
 *  @msix: boolean value of MSIX capability
 *
 *  Attempt to configure interrupts using the best available
 *  capabilities of the hardware and kernel.
 **/
static void igb_set_interrupt_capability(struct igb_adapter *adapter, bool msix)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;
	int numvecs, i;

<<<<<<< HEAD
=======
	if (!msix)
		goto msi_only;
	adapter->flags |= IGB_FLAG_HAS_MSIX;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Number of supported queues. */
	adapter->num_rx_queues = adapter->rss_queues;
	if (adapter->vfs_allocated_count)
		adapter->num_tx_queues = 1;
	else
		adapter->num_tx_queues = adapter->rss_queues;

<<<<<<< HEAD
	/* start with one vector for every rx queue */
	numvecs = adapter->num_rx_queues;

	/* if tx handler is separate add 1 for every tx queue */
=======
	/* start with one vector for every Rx queue */
	numvecs = adapter->num_rx_queues;

	/* if Tx handler is separate add 1 for every Tx queue */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!(adapter->flags & IGB_FLAG_QUEUE_PAIRS))
		numvecs += adapter->num_tx_queues;

	/* store the number of vectors reserved for queues */
	adapter->num_q_vectors = numvecs;

	/* add 1 vector for link status interrupts */
	numvecs++;
<<<<<<< HEAD
	adapter->msix_entries = kcalloc(numvecs, sizeof(struct msix_entry),
					GFP_KERNEL);
	if (!adapter->msix_entries)
		goto msi_only;

	for (i = 0; i < numvecs; i++)
		adapter->msix_entries[i].entry = i;

	err = pci_enable_msix(adapter->pdev,
			      adapter->msix_entries,
			      numvecs);
	if (err == 0)
		goto out;
=======
	for (i = 0; i < numvecs; i++)
		adapter->msix_entries[i].entry = i;

	err = pci_enable_msix_range(adapter->pdev,
				    adapter->msix_entries,
				    numvecs,
				    numvecs);
	if (err > 0)
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	igb_reset_interrupt_capability(adapter);

	/* If we can't do MSI-X, try MSI */
msi_only:
<<<<<<< HEAD
=======
	adapter->flags &= ~IGB_FLAG_HAS_MSIX;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_PCI_IOV
	/* disable SR-IOV for non MSI-X configurations */
	if (adapter->vf_data) {
		struct e1000_hw *hw = &adapter->hw;
		/* disable iov and allow time for transactions to clear */
		pci_disable_sriov(adapter->pdev);
		msleep(500);

<<<<<<< HEAD
=======
		kfree(adapter->vf_mac_list);
		adapter->vf_mac_list = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree(adapter->vf_data);
		adapter->vf_data = NULL;
		wr32(E1000_IOVCTL, E1000_IOVCTL_REUSE_VFQ);
		wrfl();
		msleep(100);
		dev_info(&adapter->pdev->dev, "IOV Disabled\n");
	}
#endif
	adapter->vfs_allocated_count = 0;
	adapter->rss_queues = 1;
	adapter->flags |= IGB_FLAG_QUEUE_PAIRS;
	adapter->num_rx_queues = 1;
	adapter->num_tx_queues = 1;
	adapter->num_q_vectors = 1;
	if (!pci_enable_msi(adapter->pdev))
		adapter->flags |= IGB_FLAG_HAS_MSI;
<<<<<<< HEAD
out:
	/* Notify the stack of the (possibly) reduced queue counts. */
	rtnl_lock();
	netif_set_real_num_tx_queues(adapter->netdev, adapter->num_tx_queues);
	err = netif_set_real_num_rx_queues(adapter->netdev,
		adapter->num_rx_queues);
	rtnl_unlock();
	return err;
}

/**
 * igb_alloc_q_vectors - Allocate memory for interrupt vectors
 * @adapter: board private structure to initialize
 *
 * We allocate one q_vector per queue interrupt.  If allocation fails we
 * return -ENOMEM.
 **/
static int igb_alloc_q_vectors(struct igb_adapter *adapter)
{
	struct igb_q_vector *q_vector;
	struct e1000_hw *hw = &adapter->hw;
	int v_idx;
	int orig_node = adapter->node;

	for (v_idx = 0; v_idx < adapter->num_q_vectors; v_idx++) {
		if ((adapter->num_q_vectors == (adapter->num_rx_queues +
						adapter->num_tx_queues)) &&
		    (adapter->num_rx_queues == v_idx))
			adapter->node = orig_node;
		if (orig_node == -1) {
			int cur_node = next_online_node(adapter->node);
			if (cur_node == MAX_NUMNODES)
				cur_node = first_online_node;
			adapter->node = cur_node;
		}
		q_vector = kzalloc_node(sizeof(struct igb_q_vector), GFP_KERNEL,
					adapter->node);
		if (!q_vector)
			q_vector = kzalloc(sizeof(struct igb_q_vector),
					   GFP_KERNEL);
		if (!q_vector)
			goto err_out;
		q_vector->adapter = adapter;
		q_vector->itr_register = hw->hw_addr + E1000_EITR(0);
		q_vector->itr_val = IGB_START_ITR;
		netif_napi_add(adapter->netdev, &q_vector->napi, igb_poll, 64);
		adapter->q_vector[v_idx] = q_vector;
	}
	/* Restore the adapter's original node */
	adapter->node = orig_node;
=======
}

static void igb_add_ring(struct igb_ring *ring,
			 struct igb_ring_container *head)
{
	head->ring = ring;
	head->count++;
}

/**
 *  igb_alloc_q_vector - Allocate memory for a single interrupt vector
 *  @adapter: board private structure to initialize
 *  @v_count: q_vectors allocated on adapter, used for ring interleaving
 *  @v_idx: index of vector in adapter struct
 *  @txr_count: total number of Tx rings to allocate
 *  @txr_idx: index of first Tx ring to allocate
 *  @rxr_count: total number of Rx rings to allocate
 *  @rxr_idx: index of first Rx ring to allocate
 *
 *  We allocate one q_vector.  If allocation fails we return -ENOMEM.
 **/
static int igb_alloc_q_vector(struct igb_adapter *adapter,
			      int v_count, int v_idx,
			      int txr_count, int txr_idx,
			      int rxr_count, int rxr_idx)
{
	struct igb_q_vector *q_vector;
	struct igb_ring *ring;
	int ring_count;
	size_t size;

	/* igb only supports 1 Tx and/or 1 Rx queue per vector */
	if (txr_count > 1 || rxr_count > 1)
		return -ENOMEM;

	ring_count = txr_count + rxr_count;
	size = kmalloc_size_roundup(struct_size(q_vector, ring, ring_count));

	/* allocate q_vector and rings */
	q_vector = adapter->q_vector[v_idx];
	if (!q_vector) {
		q_vector = kzalloc(size, GFP_KERNEL);
	} else if (size > ksize(q_vector)) {
		struct igb_q_vector *new_q_vector;

		new_q_vector = kzalloc(size, GFP_KERNEL);
		if (new_q_vector)
			kfree_rcu(q_vector, rcu);
		q_vector = new_q_vector;
	} else {
		memset(q_vector, 0, size);
	}
	if (!q_vector)
		return -ENOMEM;

	/* initialize NAPI */
	netif_napi_add(adapter->netdev, &q_vector->napi, igb_poll);

	/* tie q_vector and adapter together */
	adapter->q_vector[v_idx] = q_vector;
	q_vector->adapter = adapter;

	/* initialize work limits */
	q_vector->tx.work_limit = adapter->tx_work_limit;

	/* initialize ITR configuration */
	q_vector->itr_register = adapter->io_addr + E1000_EITR(0);
	q_vector->itr_val = IGB_START_ITR;

	/* initialize pointer to rings */
	ring = q_vector->ring;

	/* intialize ITR */
	if (rxr_count) {
		/* rx or rx/tx vector */
		if (!adapter->rx_itr_setting || adapter->rx_itr_setting > 3)
			q_vector->itr_val = adapter->rx_itr_setting;
	} else {
		/* tx only vector */
		if (!adapter->tx_itr_setting || adapter->tx_itr_setting > 3)
			q_vector->itr_val = adapter->tx_itr_setting;
	}

	if (txr_count) {
		/* assign generic ring traits */
		ring->dev = &adapter->pdev->dev;
		ring->netdev = adapter->netdev;

		/* configure backlink on ring */
		ring->q_vector = q_vector;

		/* update q_vector Tx values */
		igb_add_ring(ring, &q_vector->tx);

		/* For 82575, context index must be unique per ring. */
		if (adapter->hw.mac.type == e1000_82575)
			set_bit(IGB_RING_FLAG_TX_CTX_IDX, &ring->flags);

		/* apply Tx specific ring traits */
		ring->count = adapter->tx_ring_count;
		ring->queue_index = txr_idx;

		ring->cbs_enable = false;
		ring->idleslope = 0;
		ring->sendslope = 0;
		ring->hicredit = 0;
		ring->locredit = 0;

		u64_stats_init(&ring->tx_syncp);
		u64_stats_init(&ring->tx_syncp2);

		/* assign ring to adapter */
		adapter->tx_ring[txr_idx] = ring;

		/* push pointer to next ring */
		ring++;
	}

	if (rxr_count) {
		/* assign generic ring traits */
		ring->dev = &adapter->pdev->dev;
		ring->netdev = adapter->netdev;

		/* configure backlink on ring */
		ring->q_vector = q_vector;

		/* update q_vector Rx values */
		igb_add_ring(ring, &q_vector->rx);

		/* set flag indicating ring supports SCTP checksum offload */
		if (adapter->hw.mac.type >= e1000_82576)
			set_bit(IGB_RING_FLAG_RX_SCTP_CSUM, &ring->flags);

		/* On i350, i354, i210, and i211, loopback VLAN packets
		 * have the tag byte-swapped.
		 */
		if (adapter->hw.mac.type >= e1000_i350)
			set_bit(IGB_RING_FLAG_RX_LB_VLAN_BSWAP, &ring->flags);

		/* apply Rx specific ring traits */
		ring->count = adapter->rx_ring_count;
		ring->queue_index = rxr_idx;

		u64_stats_init(&ring->rx_syncp);

		/* assign ring to adapter */
		adapter->rx_ring[rxr_idx] = ring;
	}

	return 0;
}


/**
 *  igb_alloc_q_vectors - Allocate memory for interrupt vectors
 *  @adapter: board private structure to initialize
 *
 *  We allocate one q_vector per queue interrupt.  If allocation fails we
 *  return -ENOMEM.
 **/
static int igb_alloc_q_vectors(struct igb_adapter *adapter)
{
	int q_vectors = adapter->num_q_vectors;
	int rxr_remaining = adapter->num_rx_queues;
	int txr_remaining = adapter->num_tx_queues;
	int rxr_idx = 0, txr_idx = 0, v_idx = 0;
	int err;

	if (q_vectors >= (rxr_remaining + txr_remaining)) {
		for (; rxr_remaining; v_idx++) {
			err = igb_alloc_q_vector(adapter, q_vectors, v_idx,
						 0, 0, 1, rxr_idx);

			if (err)
				goto err_out;

			/* update counts and index */
			rxr_remaining--;
			rxr_idx++;
		}
	}

	for (; v_idx < q_vectors; v_idx++) {
		int rqpv = DIV_ROUND_UP(rxr_remaining, q_vectors - v_idx);
		int tqpv = DIV_ROUND_UP(txr_remaining, q_vectors - v_idx);

		err = igb_alloc_q_vector(adapter, q_vectors, v_idx,
					 tqpv, txr_idx, rqpv, rxr_idx);

		if (err)
			goto err_out;

		/* update counts and index */
		rxr_remaining -= rqpv;
		txr_remaining -= tqpv;
		rxr_idx++;
		txr_idx++;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;

err_out:
<<<<<<< HEAD
	/* Restore the adapter's original node */
	adapter->node = orig_node;
	igb_free_q_vectors(adapter);
	return -ENOMEM;
}

static void igb_map_rx_ring_to_vector(struct igb_adapter *adapter,
                                      int ring_idx, int v_idx)
{
	struct igb_q_vector *q_vector = adapter->q_vector[v_idx];

	q_vector->rx.ring = adapter->rx_ring[ring_idx];
	q_vector->rx.ring->q_vector = q_vector;
	q_vector->rx.count++;
	q_vector->itr_val = adapter->rx_itr_setting;
	if (q_vector->itr_val && q_vector->itr_val <= 3)
		q_vector->itr_val = IGB_START_ITR;
}

static void igb_map_tx_ring_to_vector(struct igb_adapter *adapter,
                                      int ring_idx, int v_idx)
{
	struct igb_q_vector *q_vector = adapter->q_vector[v_idx];

	q_vector->tx.ring = adapter->tx_ring[ring_idx];
	q_vector->tx.ring->q_vector = q_vector;
	q_vector->tx.count++;
	q_vector->itr_val = adapter->tx_itr_setting;
	q_vector->tx.work_limit = adapter->tx_work_limit;
	if (q_vector->itr_val && q_vector->itr_val <= 3)
		q_vector->itr_val = IGB_START_ITR;
}

/**
 * igb_map_ring_to_vector - maps allocated queues to vectors
 *
 * This function maps the recently allocated queues to vectors.
 **/
static int igb_map_ring_to_vector(struct igb_adapter *adapter)
{
	int i;
	int v_idx = 0;

	if ((adapter->num_q_vectors < adapter->num_rx_queues) ||
	    (adapter->num_q_vectors < adapter->num_tx_queues))
		return -ENOMEM;

	if (adapter->num_q_vectors >=
	    (adapter->num_rx_queues + adapter->num_tx_queues)) {
		for (i = 0; i < adapter->num_rx_queues; i++)
			igb_map_rx_ring_to_vector(adapter, i, v_idx++);
		for (i = 0; i < adapter->num_tx_queues; i++)
			igb_map_tx_ring_to_vector(adapter, i, v_idx++);
	} else {
		for (i = 0; i < adapter->num_rx_queues; i++) {
			if (i < adapter->num_tx_queues)
				igb_map_tx_ring_to_vector(adapter, i, v_idx);
			igb_map_rx_ring_to_vector(adapter, i, v_idx++);
		}
		for (; i < adapter->num_tx_queues; i++)
			igb_map_tx_ring_to_vector(adapter, i, v_idx++);
	}
	return 0;
}

/**
 * igb_init_interrupt_scheme - initialize interrupts, allocate queues/vectors
 *
 * This function initializes the interrupts and allocates all of the queues.
 **/
static int igb_init_interrupt_scheme(struct igb_adapter *adapter)
=======
	adapter->num_tx_queues = 0;
	adapter->num_rx_queues = 0;
	adapter->num_q_vectors = 0;

	while (v_idx--)
		igb_free_q_vector(adapter, v_idx);

	return -ENOMEM;
}

/**
 *  igb_init_interrupt_scheme - initialize interrupts, allocate queues/vectors
 *  @adapter: board private structure to initialize
 *  @msix: boolean value of MSIX capability
 *
 *  This function initializes the interrupts and allocates all of the queues.
 **/
static int igb_init_interrupt_scheme(struct igb_adapter *adapter, bool msix)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct pci_dev *pdev = adapter->pdev;
	int err;

<<<<<<< HEAD
	err = igb_set_interrupt_capability(adapter);
	if (err)
		return err;
=======
	igb_set_interrupt_capability(adapter, msix);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = igb_alloc_q_vectors(adapter);
	if (err) {
		dev_err(&pdev->dev, "Unable to allocate memory for vectors\n");
		goto err_alloc_q_vectors;
	}

<<<<<<< HEAD
	err = igb_alloc_queues(adapter);
	if (err) {
		dev_err(&pdev->dev, "Unable to allocate memory for queues\n");
		goto err_alloc_queues;
	}

	err = igb_map_ring_to_vector(adapter);
	if (err) {
		dev_err(&pdev->dev, "Invalid q_vector to ring mapping\n");
		goto err_map_queues;
	}


	return 0;
err_map_queues:
	igb_free_queues(adapter);
err_alloc_queues:
	igb_free_q_vectors(adapter);
=======
	igb_cache_ring_register(adapter);

	return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_alloc_q_vectors:
	igb_reset_interrupt_capability(adapter);
	return err;
}

/**
<<<<<<< HEAD
 * igb_request_irq - initialize interrupts
 *
 * Attempts to configure interrupts using the best available
 * capabilities of the hardware and kernel.
=======
 *  igb_request_irq - initialize interrupts
 *  @adapter: board private structure to initialize
 *
 *  Attempts to configure interrupts using the best available
 *  capabilities of the hardware and kernel.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static int igb_request_irq(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct pci_dev *pdev = adapter->pdev;
	int err = 0;

<<<<<<< HEAD
	if (adapter->msix_entries) {
=======
	if (adapter->flags & IGB_FLAG_HAS_MSIX) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = igb_request_msix(adapter);
		if (!err)
			goto request_done;
		/* fall back to MSI */
<<<<<<< HEAD
		igb_clear_interrupt_scheme(adapter);
		if (!pci_enable_msi(pdev))
			adapter->flags |= IGB_FLAG_HAS_MSI;
		igb_free_all_tx_resources(adapter);
		igb_free_all_rx_resources(adapter);
		adapter->num_tx_queues = 1;
		adapter->num_rx_queues = 1;
		adapter->num_q_vectors = 1;
		err = igb_alloc_q_vectors(adapter);
		if (err) {
			dev_err(&pdev->dev,
			        "Unable to allocate memory for vectors\n");
			goto request_done;
		}
		err = igb_alloc_queues(adapter);
		if (err) {
			dev_err(&pdev->dev,
			        "Unable to allocate memory for queues\n");
			igb_free_q_vectors(adapter);
			goto request_done;
		}
		igb_setup_all_tx_resources(adapter);
		igb_setup_all_rx_resources(adapter);
=======
		igb_free_all_tx_resources(adapter);
		igb_free_all_rx_resources(adapter);

		igb_clear_interrupt_scheme(adapter);
		err = igb_init_interrupt_scheme(adapter, false);
		if (err)
			goto request_done;

		igb_setup_all_tx_resources(adapter);
		igb_setup_all_rx_resources(adapter);
		igb_configure(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	igb_assign_vector(adapter->q_vector[0], 0);

	if (adapter->flags & IGB_FLAG_HAS_MSI) {
		err = request_irq(pdev->irq, igb_intr_msi, 0,
				  netdev->name, adapter);
		if (!err)
			goto request_done;

		/* fall back to legacy interrupts */
		igb_reset_interrupt_capability(adapter);
		adapter->flags &= ~IGB_FLAG_HAS_MSI;
	}

	err = request_irq(pdev->irq, igb_intr, IRQF_SHARED,
			  netdev->name, adapter);

	if (err)
		dev_err(&pdev->dev, "Error %d getting interrupt\n",
			err);

request_done:
	return err;
}

static void igb_free_irq(struct igb_adapter *adapter)
{
<<<<<<< HEAD
	if (adapter->msix_entries) {
=======
	if (adapter->flags & IGB_FLAG_HAS_MSIX) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		int vector = 0, i;

		free_irq(adapter->msix_entries[vector++].vector, adapter);

		for (i = 0; i < adapter->num_q_vectors; i++)
			free_irq(adapter->msix_entries[vector++].vector,
				 adapter->q_vector[i]);
	} else {
		free_irq(adapter->pdev->irq, adapter);
	}
}

/**
<<<<<<< HEAD
 * igb_irq_disable - Mask off interrupt generation on the NIC
 * @adapter: board private structure
=======
 *  igb_irq_disable - Mask off interrupt generation on the NIC
 *  @adapter: board private structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_irq_disable(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;

<<<<<<< HEAD
	/*
	 * we need to be careful when disabling interrupts.  The VFs are also
	 * mapped into these registers and so clearing the bits can cause
	 * issues on the VF drivers so we only need to clear what we set
	 */
	if (adapter->msix_entries) {
		u32 regval = rd32(E1000_EIAM);
=======
	/* we need to be careful when disabling interrupts.  The VFs are also
	 * mapped into these registers and so clearing the bits can cause
	 * issues on the VF drivers so we only need to clear what we set
	 */
	if (adapter->flags & IGB_FLAG_HAS_MSIX) {
		u32 regval = rd32(E1000_EIAM);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		wr32(E1000_EIAM, regval & ~adapter->eims_enable_mask);
		wr32(E1000_EIMC, adapter->eims_enable_mask);
		regval = rd32(E1000_EIAC);
		wr32(E1000_EIAC, regval & ~adapter->eims_enable_mask);
	}

	wr32(E1000_IAM, 0);
	wr32(E1000_IMC, ~0);
	wrfl();
<<<<<<< HEAD
	if (adapter->msix_entries) {
		int i;
=======
	if (adapter->flags & IGB_FLAG_HAS_MSIX) {
		int i;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		for (i = 0; i < adapter->num_q_vectors; i++)
			synchronize_irq(adapter->msix_entries[i].vector);
	} else {
		synchronize_irq(adapter->pdev->irq);
	}
}

/**
<<<<<<< HEAD
 * igb_irq_enable - Enable default interrupt generation settings
 * @adapter: board private structure
=======
 *  igb_irq_enable - Enable default interrupt generation settings
 *  @adapter: board private structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_irq_enable(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;

<<<<<<< HEAD
	if (adapter->msix_entries) {
		u32 ims = E1000_IMS_LSC | E1000_IMS_DOUTSYNC | E1000_IMS_DRSTA;
		u32 regval = rd32(E1000_EIAC);
=======
	if (adapter->flags & IGB_FLAG_HAS_MSIX) {
		u32 ims = E1000_IMS_LSC | E1000_IMS_DOUTSYNC | E1000_IMS_DRSTA;
		u32 regval = rd32(E1000_EIAC);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		wr32(E1000_EIAC, regval | adapter->eims_enable_mask);
		regval = rd32(E1000_EIAM);
		wr32(E1000_EIAM, regval | adapter->eims_enable_mask);
		wr32(E1000_EIMS, adapter->eims_enable_mask);
		if (adapter->vfs_allocated_count) {
			wr32(E1000_MBVFIMR, 0xFF);
			ims |= E1000_IMS_VMMB;
		}
		wr32(E1000_IMS, ims);
	} else {
		wr32(E1000_IMS, IMS_ENABLE_MASK |
				E1000_IMS_DRSTA);
		wr32(E1000_IAM, IMS_ENABLE_MASK |
				E1000_IMS_DRSTA);
	}
}

static void igb_update_mng_vlan(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
<<<<<<< HEAD
=======
	u16 pf_id = adapter->vfs_allocated_count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u16 vid = adapter->hw.mng_cookie.vlan_id;
	u16 old_vid = adapter->mng_vlan_id;

	if (hw->mng_cookie.status & E1000_MNG_DHCP_COOKIE_STATUS_VLAN) {
		/* add VID to filter table */
<<<<<<< HEAD
		igb_vfta_set(hw, vid, true);
=======
		igb_vfta_set(hw, vid, pf_id, true, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		adapter->mng_vlan_id = vid;
	} else {
		adapter->mng_vlan_id = IGB_MNG_VLAN_NONE;
	}

	if ((old_vid != (u16)IGB_MNG_VLAN_NONE) &&
	    (vid != old_vid) &&
	    !test_bit(old_vid, adapter->active_vlans)) {
		/* remove VID from filter table */
<<<<<<< HEAD
		igb_vfta_set(hw, old_vid, false);
=======
		igb_vfta_set(hw, vid, pf_id, false, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/**
<<<<<<< HEAD
 * igb_release_hw_control - release control of the h/w to f/w
 * @adapter: address of board private structure
 *
 * igb_release_hw_control resets CTRL_EXT:DRV_LOAD bit.
 * For ASF and Pass Through versions of f/w this means that the
 * driver is no longer loaded.
 *
=======
 *  igb_release_hw_control - release control of the h/w to f/w
 *  @adapter: address of board private structure
 *
 *  igb_release_hw_control resets CTRL_EXT:DRV_LOAD bit.
 *  For ASF and Pass Through versions of f/w this means that the
 *  driver is no longer loaded.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_release_hw_control(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 ctrl_ext;

	/* Let firmware take over control of h/w */
	ctrl_ext = rd32(E1000_CTRL_EXT);
	wr32(E1000_CTRL_EXT,
			ctrl_ext & ~E1000_CTRL_EXT_DRV_LOAD);
}

/**
<<<<<<< HEAD
 * igb_get_hw_control - get control of the h/w from f/w
 * @adapter: address of board private structure
 *
 * igb_get_hw_control sets CTRL_EXT:DRV_LOAD bit.
 * For ASF and Pass Through versions of f/w this means that
 * the driver is loaded.
 *
=======
 *  igb_get_hw_control - get control of the h/w from f/w
 *  @adapter: address of board private structure
 *
 *  igb_get_hw_control sets CTRL_EXT:DRV_LOAD bit.
 *  For ASF and Pass Through versions of f/w this means that
 *  the driver is loaded.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_get_hw_control(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 ctrl_ext;

	/* Let firmware know the driver has taken over */
	ctrl_ext = rd32(E1000_CTRL_EXT);
	wr32(E1000_CTRL_EXT,
			ctrl_ext | E1000_CTRL_EXT_DRV_LOAD);
}

<<<<<<< HEAD
/**
 * igb_configure - configure the hardware for RX and TX
 * @adapter: private board structure
=======
static void enable_fqtss(struct igb_adapter *adapter, bool enable)
{
	struct net_device *netdev = adapter->netdev;
	struct e1000_hw *hw = &adapter->hw;

	WARN_ON(hw->mac.type != e1000_i210);

	if (enable)
		adapter->flags |= IGB_FLAG_FQTSS;
	else
		adapter->flags &= ~IGB_FLAG_FQTSS;

	if (netif_running(netdev))
		schedule_work(&adapter->reset_task);
}

static bool is_fqtss_enabled(struct igb_adapter *adapter)
{
	return (adapter->flags & IGB_FLAG_FQTSS) ? true : false;
}

static void set_tx_desc_fetch_prio(struct e1000_hw *hw, int queue,
				   enum tx_queue_prio prio)
{
	u32 val;

	WARN_ON(hw->mac.type != e1000_i210);
	WARN_ON(queue < 0 || queue > 4);

	val = rd32(E1000_I210_TXDCTL(queue));

	if (prio == TX_QUEUE_PRIO_HIGH)
		val |= E1000_TXDCTL_PRIORITY;
	else
		val &= ~E1000_TXDCTL_PRIORITY;

	wr32(E1000_I210_TXDCTL(queue), val);
}

static void set_queue_mode(struct e1000_hw *hw, int queue, enum queue_mode mode)
{
	u32 val;

	WARN_ON(hw->mac.type != e1000_i210);
	WARN_ON(queue < 0 || queue > 1);

	val = rd32(E1000_I210_TQAVCC(queue));

	if (mode == QUEUE_MODE_STREAM_RESERVATION)
		val |= E1000_TQAVCC_QUEUEMODE;
	else
		val &= ~E1000_TQAVCC_QUEUEMODE;

	wr32(E1000_I210_TQAVCC(queue), val);
}

static bool is_any_cbs_enabled(struct igb_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_tx_queues; i++) {
		if (adapter->tx_ring[i]->cbs_enable)
			return true;
	}

	return false;
}

static bool is_any_txtime_enabled(struct igb_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_tx_queues; i++) {
		if (adapter->tx_ring[i]->launchtime_enable)
			return true;
	}

	return false;
}

/**
 *  igb_config_tx_modes - Configure "Qav Tx mode" features on igb
 *  @adapter: pointer to adapter struct
 *  @queue: queue number
 *
 *  Configure CBS and Launchtime for a given hardware queue.
 *  Parameters are retrieved from the correct Tx ring, so
 *  igb_save_cbs_params() and igb_save_txtime_params() should be used
 *  for setting those correctly prior to this function being called.
 **/
static void igb_config_tx_modes(struct igb_adapter *adapter, int queue)
{
	struct net_device *netdev = adapter->netdev;
	struct e1000_hw *hw = &adapter->hw;
	struct igb_ring *ring;
	u32 tqavcc, tqavctrl;
	u16 value;

	WARN_ON(hw->mac.type != e1000_i210);
	WARN_ON(queue < 0 || queue > 1);
	ring = adapter->tx_ring[queue];

	/* If any of the Qav features is enabled, configure queues as SR and
	 * with HIGH PRIO. If none is, then configure them with LOW PRIO and
	 * as SP.
	 */
	if (ring->cbs_enable || ring->launchtime_enable) {
		set_tx_desc_fetch_prio(hw, queue, TX_QUEUE_PRIO_HIGH);
		set_queue_mode(hw, queue, QUEUE_MODE_STREAM_RESERVATION);
	} else {
		set_tx_desc_fetch_prio(hw, queue, TX_QUEUE_PRIO_LOW);
		set_queue_mode(hw, queue, QUEUE_MODE_STRICT_PRIORITY);
	}

	/* If CBS is enabled, set DataTranARB and config its parameters. */
	if (ring->cbs_enable || queue == 0) {
		/* i210 does not allow the queue 0 to be in the Strict
		 * Priority mode while the Qav mode is enabled, so,
		 * instead of disabling strict priority mode, we give
		 * queue 0 the maximum of credits possible.
		 *
		 * See section 8.12.19 of the i210 datasheet, "Note:
		 * Queue0 QueueMode must be set to 1b when
		 * TransmitMode is set to Qav."
		 */
		if (queue == 0 && !ring->cbs_enable) {
			/* max "linkspeed" idleslope in kbps */
			ring->idleslope = 1000000;
			ring->hicredit = ETH_FRAME_LEN;
		}

		/* Always set data transfer arbitration to credit-based
		 * shaper algorithm on TQAVCTRL if CBS is enabled for any of
		 * the queues.
		 */
		tqavctrl = rd32(E1000_I210_TQAVCTRL);
		tqavctrl |= E1000_TQAVCTRL_DATATRANARB;
		wr32(E1000_I210_TQAVCTRL, tqavctrl);

		/* According to i210 datasheet section 7.2.7.7, we should set
		 * the 'idleSlope' field from TQAVCC register following the
		 * equation:
		 *
		 * For 100 Mbps link speed:
		 *
		 *     value = BW * 0x7735 * 0.2                          (E1)
		 *
		 * For 1000Mbps link speed:
		 *
		 *     value = BW * 0x7735 * 2                            (E2)
		 *
		 * E1 and E2 can be merged into one equation as shown below.
		 * Note that 'link-speed' is in Mbps.
		 *
		 *     value = BW * 0x7735 * 2 * link-speed
		 *                           --------------               (E3)
		 *                                1000
		 *
		 * 'BW' is the percentage bandwidth out of full link speed
		 * which can be found with the following equation. Note that
		 * idleSlope here is the parameter from this function which
		 * is in kbps.
		 *
		 *     BW =     idleSlope
		 *          -----------------                             (E4)
		 *          link-speed * 1000
		 *
		 * That said, we can come up with a generic equation to
		 * calculate the value we should set it TQAVCC register by
		 * replacing 'BW' in E3 by E4. The resulting equation is:
		 *
		 * value =     idleSlope     * 0x7735 * 2 * link-speed
		 *         -----------------            --------------    (E5)
		 *         link-speed * 1000                 1000
		 *
		 * 'link-speed' is present in both sides of the fraction so
		 * it is canceled out. The final equation is the following:
		 *
		 *     value = idleSlope * 61034
		 *             -----------------                          (E6)
		 *                  1000000
		 *
		 * NOTE: For i210, given the above, we can see that idleslope
		 *       is represented in 16.38431 kbps units by the value at
		 *       the TQAVCC register (1Gbps / 61034), which reduces
		 *       the granularity for idleslope increments.
		 *       For instance, if you want to configure a 2576kbps
		 *       idleslope, the value to be written on the register
		 *       would have to be 157.23. If rounded down, you end
		 *       up with less bandwidth available than originally
		 *       required (~2572 kbps). If rounded up, you end up
		 *       with a higher bandwidth (~2589 kbps). Below the
		 *       approach we take is to always round up the
		 *       calculated value, so the resulting bandwidth might
		 *       be slightly higher for some configurations.
		 */
		value = DIV_ROUND_UP_ULL(ring->idleslope * 61034ULL, 1000000);

		tqavcc = rd32(E1000_I210_TQAVCC(queue));
		tqavcc &= ~E1000_TQAVCC_IDLESLOPE_MASK;
		tqavcc |= value;
		wr32(E1000_I210_TQAVCC(queue), tqavcc);

		wr32(E1000_I210_TQAVHC(queue),
		     0x80000000 + ring->hicredit * 0x7735);
	} else {

		/* Set idleSlope to zero. */
		tqavcc = rd32(E1000_I210_TQAVCC(queue));
		tqavcc &= ~E1000_TQAVCC_IDLESLOPE_MASK;
		wr32(E1000_I210_TQAVCC(queue), tqavcc);

		/* Set hiCredit to zero. */
		wr32(E1000_I210_TQAVHC(queue), 0);

		/* If CBS is not enabled for any queues anymore, then return to
		 * the default state of Data Transmission Arbitration on
		 * TQAVCTRL.
		 */
		if (!is_any_cbs_enabled(adapter)) {
			tqavctrl = rd32(E1000_I210_TQAVCTRL);
			tqavctrl &= ~E1000_TQAVCTRL_DATATRANARB;
			wr32(E1000_I210_TQAVCTRL, tqavctrl);
		}
	}

	/* If LaunchTime is enabled, set DataTranTIM. */
	if (ring->launchtime_enable) {
		/* Always set DataTranTIM on TQAVCTRL if LaunchTime is enabled
		 * for any of the SR queues, and configure fetchtime delta.
		 * XXX NOTE:
		 *     - LaunchTime will be enabled for all SR queues.
		 *     - A fixed offset can be added relative to the launch
		 *       time of all packets if configured at reg LAUNCH_OS0.
		 *       We are keeping it as 0 for now (default value).
		 */
		tqavctrl = rd32(E1000_I210_TQAVCTRL);
		tqavctrl |= E1000_TQAVCTRL_DATATRANTIM |
		       E1000_TQAVCTRL_FETCHTIME_DELTA;
		wr32(E1000_I210_TQAVCTRL, tqavctrl);
	} else {
		/* If Launchtime is not enabled for any SR queues anymore,
		 * then clear DataTranTIM on TQAVCTRL and clear fetchtime delta,
		 * effectively disabling Launchtime.
		 */
		if (!is_any_txtime_enabled(adapter)) {
			tqavctrl = rd32(E1000_I210_TQAVCTRL);
			tqavctrl &= ~E1000_TQAVCTRL_DATATRANTIM;
			tqavctrl &= ~E1000_TQAVCTRL_FETCHTIME_DELTA;
			wr32(E1000_I210_TQAVCTRL, tqavctrl);
		}
	}

	/* XXX: In i210 controller the sendSlope and loCredit parameters from
	 * CBS are not configurable by software so we don't do any 'controller
	 * configuration' in respect to these parameters.
	 */

	netdev_dbg(netdev, "Qav Tx mode: cbs %s, launchtime %s, queue %d idleslope %d sendslope %d hiCredit %d locredit %d\n",
		   ring->cbs_enable ? "enabled" : "disabled",
		   ring->launchtime_enable ? "enabled" : "disabled",
		   queue,
		   ring->idleslope, ring->sendslope,
		   ring->hicredit, ring->locredit);
}

static int igb_save_txtime_params(struct igb_adapter *adapter, int queue,
				  bool enable)
{
	struct igb_ring *ring;

	if (queue < 0 || queue > adapter->num_tx_queues)
		return -EINVAL;

	ring = adapter->tx_ring[queue];
	ring->launchtime_enable = enable;

	return 0;
}

static int igb_save_cbs_params(struct igb_adapter *adapter, int queue,
			       bool enable, int idleslope, int sendslope,
			       int hicredit, int locredit)
{
	struct igb_ring *ring;

	if (queue < 0 || queue > adapter->num_tx_queues)
		return -EINVAL;

	ring = adapter->tx_ring[queue];

	ring->cbs_enable = enable;
	ring->idleslope = idleslope;
	ring->sendslope = sendslope;
	ring->hicredit = hicredit;
	ring->locredit = locredit;

	return 0;
}

/**
 *  igb_setup_tx_mode - Switch to/from Qav Tx mode when applicable
 *  @adapter: pointer to adapter struct
 *
 *  Configure TQAVCTRL register switching the controller's Tx mode
 *  if FQTSS mode is enabled or disabled. Additionally, will issue
 *  a call to igb_config_tx_modes() per queue so any previously saved
 *  Tx parameters are applied.
 **/
static void igb_setup_tx_mode(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct e1000_hw *hw = &adapter->hw;
	u32 val;

	/* Only i210 controller supports changing the transmission mode. */
	if (hw->mac.type != e1000_i210)
		return;

	if (is_fqtss_enabled(adapter)) {
		int i, max_queue;

		/* Configure TQAVCTRL register: set transmit mode to 'Qav',
		 * set data fetch arbitration to 'round robin', set SP_WAIT_SR
		 * so SP queues wait for SR ones.
		 */
		val = rd32(E1000_I210_TQAVCTRL);
		val |= E1000_TQAVCTRL_XMIT_MODE | E1000_TQAVCTRL_SP_WAIT_SR;
		val &= ~E1000_TQAVCTRL_DATAFETCHARB;
		wr32(E1000_I210_TQAVCTRL, val);

		/* Configure Tx and Rx packet buffers sizes as described in
		 * i210 datasheet section 7.2.7.7.
		 */
		val = rd32(E1000_TXPBS);
		val &= ~I210_TXPBSIZE_MASK;
		val |= I210_TXPBSIZE_PB0_6KB | I210_TXPBSIZE_PB1_6KB |
			I210_TXPBSIZE_PB2_6KB | I210_TXPBSIZE_PB3_6KB;
		wr32(E1000_TXPBS, val);

		val = rd32(E1000_RXPBS);
		val &= ~I210_RXPBSIZE_MASK;
		val |= I210_RXPBSIZE_PB_30KB;
		wr32(E1000_RXPBS, val);

		/* Section 8.12.9 states that MAX_TPKT_SIZE from DTXMXPKTSZ
		 * register should not exceed the buffer size programmed in
		 * TXPBS. The smallest buffer size programmed in TXPBS is 4kB
		 * so according to the datasheet we should set MAX_TPKT_SIZE to
		 * 4kB / 64.
		 *
		 * However, when we do so, no frame from queue 2 and 3 are
		 * transmitted.  It seems the MAX_TPKT_SIZE should not be great
		 * or _equal_ to the buffer size programmed in TXPBS. For this
		 * reason, we set MAX_ TPKT_SIZE to (4kB - 1) / 64.
		 */
		val = (4096 - 1) / 64;
		wr32(E1000_I210_DTXMXPKTSZ, val);

		/* Since FQTSS mode is enabled, apply any CBS configuration
		 * previously set. If no previous CBS configuration has been
		 * done, then the initial configuration is applied, which means
		 * CBS is disabled.
		 */
		max_queue = (adapter->num_tx_queues < I210_SR_QUEUES_NUM) ?
			    adapter->num_tx_queues : I210_SR_QUEUES_NUM;

		for (i = 0; i < max_queue; i++) {
			igb_config_tx_modes(adapter, i);
		}
	} else {
		wr32(E1000_RXPBS, I210_RXPBSIZE_DEFAULT);
		wr32(E1000_TXPBS, I210_TXPBSIZE_DEFAULT);
		wr32(E1000_I210_DTXMXPKTSZ, I210_DTXMXPKTSZ_DEFAULT);

		val = rd32(E1000_I210_TQAVCTRL);
		/* According to Section 8.12.21, the other flags we've set when
		 * enabling FQTSS are not relevant when disabling FQTSS so we
		 * don't set they here.
		 */
		val &= ~E1000_TQAVCTRL_XMIT_MODE;
		wr32(E1000_I210_TQAVCTRL, val);
	}

	netdev_dbg(netdev, "FQTSS %s\n", (is_fqtss_enabled(adapter)) ?
		   "enabled" : "disabled");
}

/**
 *  igb_configure - configure the hardware for RX and TX
 *  @adapter: private board structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_configure(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	int i;

	igb_get_hw_control(adapter);
	igb_set_rx_mode(netdev);
<<<<<<< HEAD
=======
	igb_setup_tx_mode(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	igb_restore_vlan(adapter);

	igb_setup_tctl(adapter);
	igb_setup_mrqc(adapter);
	igb_setup_rctl(adapter);

<<<<<<< HEAD
=======
	igb_nfc_filter_restore(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	igb_configure_tx(adapter);
	igb_configure_rx(adapter);

	igb_rx_fifo_flush_82575(&adapter->hw);

	/* call igb_desc_unused which always leaves
	 * at least 1 descriptor unused to make sure
<<<<<<< HEAD
	 * next_to_use != next_to_clean */
=======
	 * next_to_use != next_to_clean
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < adapter->num_rx_queues; i++) {
		struct igb_ring *ring = adapter->rx_ring[i];
		igb_alloc_rx_buffers(ring, igb_desc_unused(ring));
	}
}

/**
<<<<<<< HEAD
 * igb_power_up_link - Power up the phy/serdes link
 * @adapter: address of board private structure
 **/
void igb_power_up_link(struct igb_adapter *adapter)
{
=======
 *  igb_power_up_link - Power up the phy/serdes link
 *  @adapter: address of board private structure
 **/
void igb_power_up_link(struct igb_adapter *adapter)
{
	igb_reset_phy(&adapter->hw);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (adapter->hw.phy.media_type == e1000_media_type_copper)
		igb_power_up_phy_copper(&adapter->hw);
	else
		igb_power_up_serdes_link_82575(&adapter->hw);
<<<<<<< HEAD
	igb_reset_phy(&adapter->hw);
}

/**
 * igb_power_down_link - Power down the phy/serdes link
 * @adapter: address of board private structure
=======

	igb_setup_link(&adapter->hw);
}

/**
 *  igb_power_down_link - Power down the phy/serdes link
 *  @adapter: address of board private structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static void igb_power_down_link(struct igb_adapter *adapter)
{
	if (adapter->hw.phy.media_type == e1000_media_type_copper)
		igb_power_down_phy_copper_82575(&adapter->hw);
	else
		igb_shutdown_serdes_link_82575(&adapter->hw);
}

/**
<<<<<<< HEAD
 * igb_up - Open the interface and prepare it to handle traffic
 * @adapter: board private structure
=======
 * igb_check_swap_media -  Detect and switch function for Media Auto Sense
 * @adapter: address of the board private structure
 **/
static void igb_check_swap_media(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 ctrl_ext, connsw;
	bool swap_now = false;

	ctrl_ext = rd32(E1000_CTRL_EXT);
	connsw = rd32(E1000_CONNSW);

	/* need to live swap if current media is copper and we have fiber/serdes
	 * to go to.
	 */

	if ((hw->phy.media_type == e1000_media_type_copper) &&
	    (!(connsw & E1000_CONNSW_AUTOSENSE_EN))) {
		swap_now = true;
	} else if ((hw->phy.media_type != e1000_media_type_copper) &&
		   !(connsw & E1000_CONNSW_SERDESD)) {
		/* copper signal takes time to appear */
		if (adapter->copper_tries < 4) {
			adapter->copper_tries++;
			connsw |= E1000_CONNSW_AUTOSENSE_CONF;
			wr32(E1000_CONNSW, connsw);
			return;
		} else {
			adapter->copper_tries = 0;
			if ((connsw & E1000_CONNSW_PHYSD) &&
			    (!(connsw & E1000_CONNSW_PHY_PDN))) {
				swap_now = true;
				connsw &= ~E1000_CONNSW_AUTOSENSE_CONF;
				wr32(E1000_CONNSW, connsw);
			}
		}
	}

	if (!swap_now)
		return;

	switch (hw->phy.media_type) {
	case e1000_media_type_copper:
		netdev_info(adapter->netdev,
			"MAS: changing media to fiber/serdes\n");
		ctrl_ext |=
			E1000_CTRL_EXT_LINK_MODE_PCIE_SERDES;
		adapter->flags |= IGB_FLAG_MEDIA_RESET;
		adapter->copper_tries = 0;
		break;
	case e1000_media_type_internal_serdes:
	case e1000_media_type_fiber:
		netdev_info(adapter->netdev,
			"MAS: changing media to copper\n");
		ctrl_ext &=
			~E1000_CTRL_EXT_LINK_MODE_PCIE_SERDES;
		adapter->flags |= IGB_FLAG_MEDIA_RESET;
		break;
	default:
		/* shouldn't get here during regular operation */
		netdev_err(adapter->netdev,
			"AMS: Invalid media type found, returning\n");
		break;
	}
	wr32(E1000_CTRL_EXT, ctrl_ext);
}

/**
 *  igb_up - Open the interface and prepare it to handle traffic
 *  @adapter: board private structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
int igb_up(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	int i;

	/* hardware has been reset, we need to reload some things */
	igb_configure(adapter);

	clear_bit(__IGB_DOWN, &adapter->state);

	for (i = 0; i < adapter->num_q_vectors; i++)
		napi_enable(&(adapter->q_vector[i]->napi));

<<<<<<< HEAD
	if (adapter->msix_entries)
=======
	if (adapter->flags & IGB_FLAG_HAS_MSIX)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		igb_configure_msix(adapter);
	else
		igb_assign_vector(adapter->q_vector[0], 0);

	/* Clear any pending interrupts. */
<<<<<<< HEAD
=======
	rd32(E1000_TSICR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rd32(E1000_ICR);
	igb_irq_enable(adapter);

	/* notify VFs that reset has been completed */
	if (adapter->vfs_allocated_count) {
		u32 reg_data = rd32(E1000_CTRL_EXT);
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		reg_data |= E1000_CTRL_EXT_PFRSTD;
		wr32(E1000_CTRL_EXT, reg_data);
	}

	netif_tx_start_all_queues(adapter->netdev);

	/* start the watchdog. */
	hw->mac.get_link_status = 1;
	schedule_work(&adapter->watchdog_task);

<<<<<<< HEAD
=======
	if ((adapter->flags & IGB_FLAG_EEE) &&
	    (!hw->dev_spec._82575.eee_disable))
		adapter->eee_advert = MDIO_EEE_100TX | MDIO_EEE_1000T;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

void igb_down(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct e1000_hw *hw = &adapter->hw;
	u32 tctl, rctl;
	int i;

	/* signal that we're down so the interrupt handler does not
<<<<<<< HEAD
	 * reschedule our watchdog timer */
=======
	 * reschedule our watchdog timer
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	set_bit(__IGB_DOWN, &adapter->state);

	/* disable receives in the hardware */
	rctl = rd32(E1000_RCTL);
	wr32(E1000_RCTL, rctl & ~E1000_RCTL_EN);
	/* flush and sleep below */

<<<<<<< HEAD
=======
	igb_nfc_filter_exit(adapter);

	netif_carrier_off(netdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	netif_tx_stop_all_queues(netdev);

	/* disable transmits in the hardware */
	tctl = rd32(E1000_TCTL);
	tctl &= ~E1000_TCTL_EN;
	wr32(E1000_TCTL, tctl);
	/* flush both disables and wait for them to finish */
	wrfl();
<<<<<<< HEAD
	msleep(10);

	for (i = 0; i < adapter->num_q_vectors; i++)
		napi_disable(&(adapter->q_vector[i]->napi));

	igb_irq_disable(adapter);

	del_timer_sync(&adapter->watchdog_timer);
	del_timer_sync(&adapter->phy_info_timer);

	netif_carrier_off(netdev);

	/* record the stats before reset*/
	spin_lock(&adapter->stats64_lock);
	igb_update_stats(adapter, &adapter->stats64);
=======
	usleep_range(10000, 11000);

	igb_irq_disable(adapter);

	adapter->flags &= ~IGB_FLAG_NEED_LINK_UPDATE;

	for (i = 0; i < adapter->num_q_vectors; i++) {
		if (adapter->q_vector[i]) {
			napi_synchronize(&adapter->q_vector[i]->napi);
			napi_disable(&adapter->q_vector[i]->napi);
		}
	}

	del_timer_sync(&adapter->watchdog_timer);
	del_timer_sync(&adapter->phy_info_timer);

	/* record the stats before reset*/
	spin_lock(&adapter->stats64_lock);
	igb_update_stats(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&adapter->stats64_lock);

	adapter->link_speed = 0;
	adapter->link_duplex = 0;

	if (!pci_channel_offline(adapter->pdev))
		igb_reset(adapter);
<<<<<<< HEAD
=======

	/* clear VLAN promisc flag so VFTA will be updated if necessary */
	adapter->flags &= ~IGB_FLAG_VLAN_PROMISC;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	igb_clean_all_tx_rings(adapter);
	igb_clean_all_rx_rings(adapter);
#ifdef CONFIG_IGB_DCA

	/* since we reset the hardware DCA settings were cleared */
	igb_setup_dca(adapter);
#endif
}

void igb_reinit_locked(struct igb_adapter *adapter)
{
<<<<<<< HEAD
	WARN_ON(in_interrupt());
	while (test_and_set_bit(__IGB_RESETTING, &adapter->state))
		msleep(1);
=======
	while (test_and_set_bit(__IGB_RESETTING, &adapter->state))
		usleep_range(1000, 2000);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	igb_down(adapter);
	igb_up(adapter);
	clear_bit(__IGB_RESETTING, &adapter->state);
}

<<<<<<< HEAD
=======
/** igb_enable_mas - Media Autosense re-enable after swap
 *
 * @adapter: adapter struct
 **/
static void igb_enable_mas(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 connsw = rd32(E1000_CONNSW);

	/* configure for SerDes media detect */
	if ((hw->phy.media_type == e1000_media_type_copper) &&
	    (!(connsw & E1000_CONNSW_SERDESD))) {
		connsw |= E1000_CONNSW_ENRGSRC;
		connsw |= E1000_CONNSW_AUTOSENSE_EN;
		wr32(E1000_CONNSW, connsw);
		wrfl();
	}
}

#ifdef CONFIG_IGB_HWMON
/**
 *  igb_set_i2c_bb - Init I2C interface
 *  @hw: pointer to hardware structure
 **/
static void igb_set_i2c_bb(struct e1000_hw *hw)
{
	u32 ctrl_ext;
	s32 i2cctl;

	ctrl_ext = rd32(E1000_CTRL_EXT);
	ctrl_ext |= E1000_CTRL_I2C_ENA;
	wr32(E1000_CTRL_EXT, ctrl_ext);
	wrfl();

	i2cctl = rd32(E1000_I2CPARAMS);
	i2cctl |= E1000_I2CBB_EN
		| E1000_I2C_CLK_OE_N
		| E1000_I2C_DATA_OE_N;
	wr32(E1000_I2CPARAMS, i2cctl);
	wrfl();
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void igb_reset(struct igb_adapter *adapter)
{
	struct pci_dev *pdev = adapter->pdev;
	struct e1000_hw *hw = &adapter->hw;
	struct e1000_mac_info *mac = &hw->mac;
	struct e1000_fc_info *fc = &hw->fc;
<<<<<<< HEAD
	u32 pba = 0, tx_space, min_tx_space, min_rx_space;
	u16 hwm;
=======
	u32 pba, hwm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Repartition Pba for greater than 9k mtu
	 * To take effect CTRL.RST is required.
	 */
	switch (mac->type) {
	case e1000_i350:
<<<<<<< HEAD
=======
	case e1000_i354:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case e1000_82580:
		pba = rd32(E1000_RXPBS);
		pba = igb_rxpbs_adjust_82580(pba);
		break;
	case e1000_82576:
		pba = rd32(E1000_RXPBS);
		pba &= E1000_RXPBS_SIZE_MASK_82576;
		break;
	case e1000_82575:
<<<<<<< HEAD
=======
	case e1000_i210:
	case e1000_i211:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		pba = E1000_PBA_34K;
		break;
	}

<<<<<<< HEAD
	if ((adapter->max_frame_size > ETH_FRAME_LEN + ETH_FCS_LEN) &&
	    (mac->type < e1000_82576)) {
		/* adjust PBA for jumbo frames */
=======
	if (mac->type == e1000_82575) {
		u32 min_rx_space, min_tx_space, needed_tx_space;

		/* write Rx PBA so that hardware can report correct Tx PBA */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		wr32(E1000_PBA, pba);

		/* To maintain wire speed transmits, the Tx FIFO should be
		 * large enough to accommodate two full transmit packets,
		 * rounded up to the next 1KB and expressed in KB.  Likewise,
		 * the Rx FIFO should be large enough to accommodate at least
		 * one full receive packet and is similarly rounded up and
<<<<<<< HEAD
		 * expressed in KB. */
		pba = rd32(E1000_PBA);
		/* upper 16 bits has Tx packet buffer allocation size in KB */
		tx_space = pba >> 16;
		/* lower 16 bits has Rx packet buffer allocation size in KB */
		pba &= 0xffff;
		/* the tx fifo also stores 16 bytes of information about the tx
		 * but don't include ethernet FCS because hardware appends it */
		min_tx_space = (adapter->max_frame_size +
				sizeof(union e1000_adv_tx_desc) -
				ETH_FCS_LEN) * 2;
		min_tx_space = ALIGN(min_tx_space, 1024);
		min_tx_space >>= 10;
		/* software strips receive CRC, so leave room for it */
		min_rx_space = adapter->max_frame_size;
		min_rx_space = ALIGN(min_rx_space, 1024);
		min_rx_space >>= 10;

		/* If current Tx allocation is less than the min Tx FIFO size,
		 * and the min Tx FIFO size is less than the current Rx FIFO
		 * allocation, take space away from current Rx allocation */
		if (tx_space < min_tx_space &&
		    ((min_tx_space - tx_space) < pba)) {
			pba = pba - (min_tx_space - tx_space);

			/* if short on rx space, rx wins and must trump tx
			 * adjustment */
			if (pba < min_rx_space)
				pba = min_rx_space;
		}
		wr32(E1000_PBA, pba);
	}

	/* flow control settings */
	/* The high water mark must be low enough to fit one full frame
	 * (or the size used for early receive) above it in the Rx FIFO.
	 * Set it to the lower of:
	 * - 90% of the Rx FIFO size, or
	 * - the full Rx FIFO size minus one full frame */
	hwm = min(((pba << 10) * 9 / 10),
			((pba << 10) - 2 * adapter->max_frame_size));

	fc->high_water = hwm & 0xFFF0;	/* 16-byte granularity */
=======
		 * expressed in KB.
		 */
		min_rx_space = DIV_ROUND_UP(MAX_JUMBO_FRAME_SIZE, 1024);

		/* The Tx FIFO also stores 16 bytes of information about the Tx
		 * but don't include Ethernet FCS because hardware appends it.
		 * We only need to round down to the nearest 512 byte block
		 * count since the value we care about is 2 frames, not 1.
		 */
		min_tx_space = adapter->max_frame_size;
		min_tx_space += sizeof(union e1000_adv_tx_desc) - ETH_FCS_LEN;
		min_tx_space = DIV_ROUND_UP(min_tx_space, 512);

		/* upper 16 bits has Tx packet buffer allocation size in KB */
		needed_tx_space = min_tx_space - (rd32(E1000_PBA) >> 16);

		/* If current Tx allocation is less than the min Tx FIFO size,
		 * and the min Tx FIFO size is less than the current Rx FIFO
		 * allocation, take space away from current Rx allocation.
		 */
		if (needed_tx_space < pba) {
			pba -= needed_tx_space;

			/* if short on Rx space, Rx wins and must trump Tx
			 * adjustment
			 */
			if (pba < min_rx_space)
				pba = min_rx_space;
		}

		/* adjust PBA for jumbo frames */
		wr32(E1000_PBA, pba);
	}

	/* flow control settings
	 * The high water mark must be low enough to fit one full frame
	 * after transmitting the pause frame.  As such we must have enough
	 * space to allow for us to complete our current transmit and then
	 * receive the frame that is in progress from the link partner.
	 * Set it to:
	 * - the full Rx FIFO size minus one full Tx plus one full Rx frame
	 */
	hwm = (pba << 10) - (adapter->max_frame_size + MAX_JUMBO_FRAME_SIZE);

	fc->high_water = hwm & 0xFFFFFFF0;	/* 16-byte granularity */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	fc->low_water = fc->high_water - 16;
	fc->pause_time = 0xFFFF;
	fc->send_xon = 1;
	fc->current_mode = fc->requested_mode;

	/* disable receive for all VFs and wait one second */
	if (adapter->vfs_allocated_count) {
		int i;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		for (i = 0 ; i < adapter->vfs_allocated_count; i++)
			adapter->vf_data[i].flags &= IGB_VF_FLAG_PF_SET_MAC;

		/* ping all the active vfs to let them know we are going down */
		igb_ping_all_vfs(adapter);

		/* disable transmits and receives */
		wr32(E1000_VFRE, 0);
		wr32(E1000_VFTE, 0);
	}

	/* Allow time for pending master requests to run */
	hw->mac.ops.reset_hw(hw);
	wr32(E1000_WUC, 0);

<<<<<<< HEAD
	if (hw->mac.ops.init_hw(hw))
		dev_err(&pdev->dev, "Hardware Error\n");

	igb_init_dmac(adapter, pba);
=======
	if (adapter->flags & IGB_FLAG_MEDIA_RESET) {
		/* need to resetup here after media swap */
		adapter->ei.get_invariants(hw);
		adapter->flags &= ~IGB_FLAG_MEDIA_RESET;
	}
	if ((mac->type == e1000_82575 || mac->type == e1000_i350) &&
	    (adapter->flags & IGB_FLAG_MAS_ENABLE)) {
		igb_enable_mas(adapter);
	}
	if (hw->mac.ops.init_hw(hw))
		dev_err(&pdev->dev, "Hardware Error\n");

	/* RAR registers were cleared during init_hw, clear mac table */
	igb_flush_mac_table(adapter);
	__dev_uc_unsync(adapter->netdev, NULL);

	/* Recover default RAR entry */
	igb_set_default_mac_filter(adapter);

	/* Flow control settings reset on hardware reset, so guarantee flow
	 * control is off when forcing speed.
	 */
	if (!hw->mac.autoneg)
		igb_force_mac_fc(hw);

	igb_init_dmac(adapter, pba);
#ifdef CONFIG_IGB_HWMON
	/* Re-initialize the thermal sensor on i350 devices. */
	if (!test_bit(__IGB_DOWN, &adapter->state)) {
		if (mac->type == e1000_i350 && hw->bus.func == 0) {
			/* If present, re-initialize the external thermal sensor
			 * interface.
			 */
			if (adapter->ets)
				igb_set_i2c_bb(hw);
			mac->ops.init_thermal_sensor_thresh(hw);
		}
	}
#endif
	/* Re-establish EEE setting */
	if (hw->phy.media_type == e1000_media_type_copper) {
		switch (mac->type) {
		case e1000_i350:
		case e1000_i210:
		case e1000_i211:
			igb_set_eee_i350(hw, true, true);
			break;
		case e1000_i354:
			igb_set_eee_i354(hw, true, true);
			break;
		default:
			break;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!netif_running(adapter->netdev))
		igb_power_down_link(adapter);

	igb_update_mng_vlan(adapter);

	/* Enable h/w to recognize an 802.1Q VLAN Ethernet packet */
	wr32(E1000_VET, ETHERNET_IEEE_VLAN_TYPE);

<<<<<<< HEAD
=======
	/* Re-enable PTP, where applicable. */
	if (adapter->ptp_flags & IGB_PTP_ENABLED)
		igb_ptp_reset(adapter);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	igb_get_phy_info(hw);
}

static netdev_features_t igb_fix_features(struct net_device *netdev,
	netdev_features_t features)
{
<<<<<<< HEAD
	/*
	 * Since there is no support for separate rx/tx vlan accel
	 * enable/disable make sure tx flag is always in same state as rx.
	 */
	if (features & NETIF_F_HW_VLAN_RX)
		features |= NETIF_F_HW_VLAN_TX;
	else
		features &= ~NETIF_F_HW_VLAN_TX;
=======
	/* Since there is no support for separate Rx/Tx vlan accel
	 * enable/disable make sure Tx flag is always in same state as Rx.
	 */
	if (features & NETIF_F_HW_VLAN_CTAG_RX)
		features |= NETIF_F_HW_VLAN_CTAG_TX;
	else
		features &= ~NETIF_F_HW_VLAN_CTAG_TX;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return features;
}

static int igb_set_features(struct net_device *netdev,
	netdev_features_t features)
{
	netdev_features_t changed = netdev->features ^ features;
	struct igb_adapter *adapter = netdev_priv(netdev);

<<<<<<< HEAD
	if (changed & NETIF_F_HW_VLAN_RX)
		igb_vlan_mode(netdev, features);

	if (!(changed & NETIF_F_RXALL))
		return 0;

=======
	if (changed & NETIF_F_HW_VLAN_CTAG_RX)
		igb_vlan_mode(netdev, features);

	if (!(changed & (NETIF_F_RXALL | NETIF_F_NTUPLE)))
		return 0;

	if (!(features & NETIF_F_NTUPLE)) {
		struct hlist_node *node2;
		struct igb_nfc_filter *rule;

		spin_lock(&adapter->nfc_lock);
		hlist_for_each_entry_safe(rule, node2,
					  &adapter->nfc_filter_list, nfc_node) {
			igb_erase_filter(adapter, rule);
			hlist_del(&rule->nfc_node);
			kfree(rule);
		}
		spin_unlock(&adapter->nfc_lock);
		adapter->nfc_filter_count = 0;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	netdev->features = features;

	if (netif_running(netdev))
		igb_reinit_locked(adapter);
	else
		igb_reset(adapter);

<<<<<<< HEAD
	return 0;
}

=======
	return 1;
}

static int igb_ndo_fdb_add(struct ndmsg *ndm, struct nlattr *tb[],
			   struct net_device *dev,
			   const unsigned char *addr, u16 vid,
			   u16 flags,
			   struct netlink_ext_ack *extack)
{
	/* guarantee we can provide a unique filter for the unicast address */
	if (is_unicast_ether_addr(addr) || is_link_local_ether_addr(addr)) {
		struct igb_adapter *adapter = netdev_priv(dev);
		int vfn = adapter->vfs_allocated_count;

		if (netdev_uc_count(dev) >= igb_available_rars(adapter, vfn))
			return -ENOMEM;
	}

	return ndo_dflt_fdb_add(ndm, tb, dev, addr, vid, flags);
}

#define IGB_MAX_MAC_HDR_LEN	127
#define IGB_MAX_NETWORK_HDR_LEN	511

static netdev_features_t
igb_features_check(struct sk_buff *skb, struct net_device *dev,
		   netdev_features_t features)
{
	unsigned int network_hdr_len, mac_hdr_len;

	/* Make certain the headers can be described by a context descriptor */
	mac_hdr_len = skb_network_offset(skb);
	if (unlikely(mac_hdr_len > IGB_MAX_MAC_HDR_LEN))
		return features & ~(NETIF_F_HW_CSUM |
				    NETIF_F_SCTP_CRC |
				    NETIF_F_GSO_UDP_L4 |
				    NETIF_F_HW_VLAN_CTAG_TX |
				    NETIF_F_TSO |
				    NETIF_F_TSO6);

	network_hdr_len = skb_checksum_start(skb) - skb_network_header(skb);
	if (unlikely(network_hdr_len >  IGB_MAX_NETWORK_HDR_LEN))
		return features & ~(NETIF_F_HW_CSUM |
				    NETIF_F_SCTP_CRC |
				    NETIF_F_GSO_UDP_L4 |
				    NETIF_F_TSO |
				    NETIF_F_TSO6);

	/* We can only support IPV4 TSO in tunnels if we can mangle the
	 * inner IP ID field, so strip TSO if MANGLEID is not supported.
	 */
	if (skb->encapsulation && !(features & NETIF_F_TSO_MANGLEID))
		features &= ~NETIF_F_TSO;

	return features;
}

static void igb_offload_apply(struct igb_adapter *adapter, s32 queue)
{
	if (!is_fqtss_enabled(adapter)) {
		enable_fqtss(adapter, true);
		return;
	}

	igb_config_tx_modes(adapter, queue);

	if (!is_any_cbs_enabled(adapter) && !is_any_txtime_enabled(adapter))
		enable_fqtss(adapter, false);
}

static int igb_offload_cbs(struct igb_adapter *adapter,
			   struct tc_cbs_qopt_offload *qopt)
{
	struct e1000_hw *hw = &adapter->hw;
	int err;

	/* CBS offloading is only supported by i210 controller. */
	if (hw->mac.type != e1000_i210)
		return -EOPNOTSUPP;

	/* CBS offloading is only supported by queue 0 and queue 1. */
	if (qopt->queue < 0 || qopt->queue > 1)
		return -EINVAL;

	err = igb_save_cbs_params(adapter, qopt->queue, qopt->enable,
				  qopt->idleslope, qopt->sendslope,
				  qopt->hicredit, qopt->locredit);
	if (err)
		return err;

	igb_offload_apply(adapter, qopt->queue);

	return 0;
}

#define ETHER_TYPE_FULL_MASK ((__force __be16)~0)
#define VLAN_PRIO_FULL_MASK (0x07)

static int igb_parse_cls_flower(struct igb_adapter *adapter,
				struct flow_cls_offload *f,
				int traffic_class,
				struct igb_nfc_filter *input)
{
	struct flow_rule *rule = flow_cls_offload_flow_rule(f);
	struct flow_dissector *dissector = rule->match.dissector;
	struct netlink_ext_ack *extack = f->common.extack;

	if (dissector->used_keys &
	    ~(BIT_ULL(FLOW_DISSECTOR_KEY_BASIC) |
	      BIT_ULL(FLOW_DISSECTOR_KEY_CONTROL) |
	      BIT_ULL(FLOW_DISSECTOR_KEY_ETH_ADDRS) |
	      BIT_ULL(FLOW_DISSECTOR_KEY_VLAN))) {
		NL_SET_ERR_MSG_MOD(extack,
				   "Unsupported key used, only BASIC, CONTROL, ETH_ADDRS and VLAN are supported");
		return -EOPNOTSUPP;
	}

	if (flow_rule_match_key(rule, FLOW_DISSECTOR_KEY_ETH_ADDRS)) {
		struct flow_match_eth_addrs match;

		flow_rule_match_eth_addrs(rule, &match);
		if (!is_zero_ether_addr(match.mask->dst)) {
			if (!is_broadcast_ether_addr(match.mask->dst)) {
				NL_SET_ERR_MSG_MOD(extack, "Only full masks are supported for destination MAC address");
				return -EINVAL;
			}

			input->filter.match_flags |=
				IGB_FILTER_FLAG_DST_MAC_ADDR;
			ether_addr_copy(input->filter.dst_addr, match.key->dst);
		}

		if (!is_zero_ether_addr(match.mask->src)) {
			if (!is_broadcast_ether_addr(match.mask->src)) {
				NL_SET_ERR_MSG_MOD(extack, "Only full masks are supported for source MAC address");
				return -EINVAL;
			}

			input->filter.match_flags |=
				IGB_FILTER_FLAG_SRC_MAC_ADDR;
			ether_addr_copy(input->filter.src_addr, match.key->src);
		}
	}

	if (flow_rule_match_key(rule, FLOW_DISSECTOR_KEY_BASIC)) {
		struct flow_match_basic match;

		flow_rule_match_basic(rule, &match);
		if (match.mask->n_proto) {
			if (match.mask->n_proto != ETHER_TYPE_FULL_MASK) {
				NL_SET_ERR_MSG_MOD(extack, "Only full mask is supported for EtherType filter");
				return -EINVAL;
			}

			input->filter.match_flags |= IGB_FILTER_FLAG_ETHER_TYPE;
			input->filter.etype = match.key->n_proto;
		}
	}

	if (flow_rule_match_key(rule, FLOW_DISSECTOR_KEY_VLAN)) {
		struct flow_match_vlan match;

		flow_rule_match_vlan(rule, &match);
		if (match.mask->vlan_priority) {
			if (match.mask->vlan_priority != VLAN_PRIO_FULL_MASK) {
				NL_SET_ERR_MSG_MOD(extack, "Only full mask is supported for VLAN priority");
				return -EINVAL;
			}

			input->filter.match_flags |= IGB_FILTER_FLAG_VLAN_TCI;
			input->filter.vlan_tci =
				(__force __be16)match.key->vlan_priority;
		}
	}

	input->action = traffic_class;
	input->cookie = f->cookie;

	return 0;
}

static int igb_configure_clsflower(struct igb_adapter *adapter,
				   struct flow_cls_offload *cls_flower)
{
	struct netlink_ext_ack *extack = cls_flower->common.extack;
	struct igb_nfc_filter *filter, *f;
	int err, tc;

	tc = tc_classid_to_hwtc(adapter->netdev, cls_flower->classid);
	if (tc < 0) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid traffic class");
		return -EINVAL;
	}

	filter = kzalloc(sizeof(*filter), GFP_KERNEL);
	if (!filter)
		return -ENOMEM;

	err = igb_parse_cls_flower(adapter, cls_flower, tc, filter);
	if (err < 0)
		goto err_parse;

	spin_lock(&adapter->nfc_lock);

	hlist_for_each_entry(f, &adapter->nfc_filter_list, nfc_node) {
		if (!memcmp(&f->filter, &filter->filter, sizeof(f->filter))) {
			err = -EEXIST;
			NL_SET_ERR_MSG_MOD(extack,
					   "This filter is already set in ethtool");
			goto err_locked;
		}
	}

	hlist_for_each_entry(f, &adapter->cls_flower_list, nfc_node) {
		if (!memcmp(&f->filter, &filter->filter, sizeof(f->filter))) {
			err = -EEXIST;
			NL_SET_ERR_MSG_MOD(extack,
					   "This filter is already set in cls_flower");
			goto err_locked;
		}
	}

	err = igb_add_filter(adapter, filter);
	if (err < 0) {
		NL_SET_ERR_MSG_MOD(extack, "Could not add filter to the adapter");
		goto err_locked;
	}

	hlist_add_head(&filter->nfc_node, &adapter->cls_flower_list);

	spin_unlock(&adapter->nfc_lock);

	return 0;

err_locked:
	spin_unlock(&adapter->nfc_lock);

err_parse:
	kfree(filter);

	return err;
}

static int igb_delete_clsflower(struct igb_adapter *adapter,
				struct flow_cls_offload *cls_flower)
{
	struct igb_nfc_filter *filter;
	int err;

	spin_lock(&adapter->nfc_lock);

	hlist_for_each_entry(filter, &adapter->cls_flower_list, nfc_node)
		if (filter->cookie == cls_flower->cookie)
			break;

	if (!filter) {
		err = -ENOENT;
		goto out;
	}

	err = igb_erase_filter(adapter, filter);
	if (err < 0)
		goto out;

	hlist_del(&filter->nfc_node);
	kfree(filter);

out:
	spin_unlock(&adapter->nfc_lock);

	return err;
}

static int igb_setup_tc_cls_flower(struct igb_adapter *adapter,
				   struct flow_cls_offload *cls_flower)
{
	switch (cls_flower->command) {
	case FLOW_CLS_REPLACE:
		return igb_configure_clsflower(adapter, cls_flower);
	case FLOW_CLS_DESTROY:
		return igb_delete_clsflower(adapter, cls_flower);
	case FLOW_CLS_STATS:
		return -EOPNOTSUPP;
	default:
		return -EOPNOTSUPP;
	}
}

static int igb_setup_tc_block_cb(enum tc_setup_type type, void *type_data,
				 void *cb_priv)
{
	struct igb_adapter *adapter = cb_priv;

	if (!tc_cls_can_offload_and_chain0(adapter->netdev, type_data))
		return -EOPNOTSUPP;

	switch (type) {
	case TC_SETUP_CLSFLOWER:
		return igb_setup_tc_cls_flower(adapter, type_data);

	default:
		return -EOPNOTSUPP;
	}
}

static int igb_offload_txtime(struct igb_adapter *adapter,
			      struct tc_etf_qopt_offload *qopt)
{
	struct e1000_hw *hw = &adapter->hw;
	int err;

	/* Launchtime offloading is only supported by i210 controller. */
	if (hw->mac.type != e1000_i210)
		return -EOPNOTSUPP;

	/* Launchtime offloading is only supported by queues 0 and 1. */
	if (qopt->queue < 0 || qopt->queue > 1)
		return -EINVAL;

	err = igb_save_txtime_params(adapter, qopt->queue, qopt->enable);
	if (err)
		return err;

	igb_offload_apply(adapter, qopt->queue);

	return 0;
}

static int igb_tc_query_caps(struct igb_adapter *adapter,
			     struct tc_query_caps_base *base)
{
	switch (base->type) {
	case TC_SETUP_QDISC_TAPRIO: {
		struct tc_taprio_caps *caps = base->caps;

		caps->broken_mqprio = true;

		return 0;
	}
	default:
		return -EOPNOTSUPP;
	}
}

static LIST_HEAD(igb_block_cb_list);

static int igb_setup_tc(struct net_device *dev, enum tc_setup_type type,
			void *type_data)
{
	struct igb_adapter *adapter = netdev_priv(dev);

	switch (type) {
	case TC_QUERY_CAPS:
		return igb_tc_query_caps(adapter, type_data);
	case TC_SETUP_QDISC_CBS:
		return igb_offload_cbs(adapter, type_data);
	case TC_SETUP_BLOCK:
		return flow_block_cb_setup_simple(type_data,
						  &igb_block_cb_list,
						  igb_setup_tc_block_cb,
						  adapter, adapter, true);

	case TC_SETUP_QDISC_ETF:
		return igb_offload_txtime(adapter, type_data);

	default:
		return -EOPNOTSUPP;
	}
}

static int igb_xdp_setup(struct net_device *dev, struct netdev_bpf *bpf)
{
	int i, frame_size = dev->mtu + IGB_ETH_PKT_HDR_PAD;
	struct igb_adapter *adapter = netdev_priv(dev);
	struct bpf_prog *prog = bpf->prog, *old_prog;
	bool running = netif_running(dev);
	bool need_reset;

	/* verify igb ring attributes are sufficient for XDP */
	for (i = 0; i < adapter->num_rx_queues; i++) {
		struct igb_ring *ring = adapter->rx_ring[i];

		if (frame_size > igb_rx_bufsz(ring)) {
			NL_SET_ERR_MSG_MOD(bpf->extack,
					   "The RX buffer size is too small for the frame size");
			netdev_warn(dev, "XDP RX buffer size %d is too small for the frame size %d\n",
				    igb_rx_bufsz(ring), frame_size);
			return -EINVAL;
		}
	}

	old_prog = xchg(&adapter->xdp_prog, prog);
	need_reset = (!!prog != !!old_prog);

	/* device is up and bpf is added/removed, must setup the RX queues */
	if (need_reset && running) {
		igb_close(dev);
	} else {
		for (i = 0; i < adapter->num_rx_queues; i++)
			(void)xchg(&adapter->rx_ring[i]->xdp_prog,
			    adapter->xdp_prog);
	}

	if (old_prog)
		bpf_prog_put(old_prog);

	/* bpf is just replaced, RXQ and MTU are already setup */
	if (!need_reset) {
		return 0;
	} else {
		if (prog)
			xdp_features_set_redirect_target(dev, true);
		else
			xdp_features_clear_redirect_target(dev);
	}

	if (running)
		igb_open(dev);

	return 0;
}

static int igb_xdp(struct net_device *dev, struct netdev_bpf *xdp)
{
	switch (xdp->command) {
	case XDP_SETUP_PROG:
		return igb_xdp_setup(dev, xdp);
	default:
		return -EINVAL;
	}
}

static void igb_xdp_ring_update_tail(struct igb_ring *ring)
{
	/* Force memory writes to complete before letting h/w know there
	 * are new descriptors to fetch.
	 */
	wmb();
	writel(ring->next_to_use, ring->tail);
}

static struct igb_ring *igb_xdp_tx_queue_mapping(struct igb_adapter *adapter)
{
	unsigned int r_idx = smp_processor_id();

	if (r_idx >= adapter->num_tx_queues)
		r_idx = r_idx % adapter->num_tx_queues;

	return adapter->tx_ring[r_idx];
}

static int igb_xdp_xmit_back(struct igb_adapter *adapter, struct xdp_buff *xdp)
{
	struct xdp_frame *xdpf = xdp_convert_buff_to_frame(xdp);
	int cpu = smp_processor_id();
	struct igb_ring *tx_ring;
	struct netdev_queue *nq;
	u32 ret;

	if (unlikely(!xdpf))
		return IGB_XDP_CONSUMED;

	/* During program transitions its possible adapter->xdp_prog is assigned
	 * but ring has not been configured yet. In this case simply abort xmit.
	 */
	tx_ring = adapter->xdp_prog ? igb_xdp_tx_queue_mapping(adapter) : NULL;
	if (unlikely(!tx_ring))
		return IGB_XDP_CONSUMED;

	nq = txring_txq(tx_ring);
	__netif_tx_lock(nq, cpu);
	/* Avoid transmit queue timeout since we share it with the slow path */
	txq_trans_cond_update(nq);
	ret = igb_xmit_xdp_ring(adapter, tx_ring, xdpf);
	__netif_tx_unlock(nq);

	return ret;
}

static int igb_xdp_xmit(struct net_device *dev, int n,
			struct xdp_frame **frames, u32 flags)
{
	struct igb_adapter *adapter = netdev_priv(dev);
	int cpu = smp_processor_id();
	struct igb_ring *tx_ring;
	struct netdev_queue *nq;
	int nxmit = 0;
	int i;

	if (unlikely(test_bit(__IGB_DOWN, &adapter->state)))
		return -ENETDOWN;

	if (unlikely(flags & ~XDP_XMIT_FLAGS_MASK))
		return -EINVAL;

	/* During program transitions its possible adapter->xdp_prog is assigned
	 * but ring has not been configured yet. In this case simply abort xmit.
	 */
	tx_ring = adapter->xdp_prog ? igb_xdp_tx_queue_mapping(adapter) : NULL;
	if (unlikely(!tx_ring))
		return -ENXIO;

	nq = txring_txq(tx_ring);
	__netif_tx_lock(nq, cpu);

	/* Avoid transmit queue timeout since we share it with the slow path */
	txq_trans_cond_update(nq);

	for (i = 0; i < n; i++) {
		struct xdp_frame *xdpf = frames[i];
		int err;

		err = igb_xmit_xdp_ring(adapter, tx_ring, xdpf);
		if (err != IGB_XDP_TX)
			break;
		nxmit++;
	}

	__netif_tx_unlock(nq);

	if (unlikely(flags & XDP_XMIT_FLUSH))
		igb_xdp_ring_update_tail(tx_ring);

	return nxmit;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct net_device_ops igb_netdev_ops = {
	.ndo_open		= igb_open,
	.ndo_stop		= igb_close,
	.ndo_start_xmit		= igb_xmit_frame,
	.ndo_get_stats64	= igb_get_stats64,
	.ndo_set_rx_mode	= igb_set_rx_mode,
	.ndo_set_mac_address	= igb_set_mac,
	.ndo_change_mtu		= igb_change_mtu,
<<<<<<< HEAD
	.ndo_do_ioctl		= igb_ioctl,
=======
	.ndo_eth_ioctl		= igb_ioctl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ndo_tx_timeout		= igb_tx_timeout,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_vlan_rx_add_vid	= igb_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid	= igb_vlan_rx_kill_vid,
	.ndo_set_vf_mac		= igb_ndo_set_vf_mac,
	.ndo_set_vf_vlan	= igb_ndo_set_vf_vlan,
<<<<<<< HEAD
	.ndo_set_vf_tx_rate	= igb_ndo_set_vf_bw,
	.ndo_get_vf_config	= igb_ndo_get_vf_config,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= igb_netpoll,
#endif
	.ndo_fix_features	= igb_fix_features,
	.ndo_set_features	= igb_set_features,
};

/**
 * igb_probe - Device Initialization Routine
 * @pdev: PCI device information struct
 * @ent: entry in igb_pci_tbl
 *
 * Returns 0 on success, negative on failure
 *
 * igb_probe initializes an adapter identified by a pci_dev structure.
 * The OS initialization, configuring of the adapter private structure,
 * and a hardware reset occur.
 **/
static int __devinit igb_probe(struct pci_dev *pdev,
			       const struct pci_device_id *ent)
=======
	.ndo_set_vf_rate	= igb_ndo_set_vf_bw,
	.ndo_set_vf_spoofchk	= igb_ndo_set_vf_spoofchk,
	.ndo_set_vf_trust	= igb_ndo_set_vf_trust,
	.ndo_get_vf_config	= igb_ndo_get_vf_config,
	.ndo_fix_features	= igb_fix_features,
	.ndo_set_features	= igb_set_features,
	.ndo_fdb_add		= igb_ndo_fdb_add,
	.ndo_features_check	= igb_features_check,
	.ndo_setup_tc		= igb_setup_tc,
	.ndo_bpf		= igb_xdp,
	.ndo_xdp_xmit		= igb_xdp_xmit,
};

/**
 * igb_set_fw_version - Configure version string for ethtool
 * @adapter: adapter struct
 **/
void igb_set_fw_version(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	struct e1000_fw_version fw;

	igb_get_fw_version(hw, &fw);

	switch (hw->mac.type) {
	case e1000_i210:
	case e1000_i211:
		if (!(igb_get_flash_presence_i210(hw))) {
			snprintf(adapter->fw_version,
				 sizeof(adapter->fw_version),
				 "%2d.%2d-%d",
				 fw.invm_major, fw.invm_minor,
				 fw.invm_img_type);
			break;
		}
		fallthrough;
	default:
		/* if option rom is valid, display its version too */
		if (fw.or_valid) {
			snprintf(adapter->fw_version,
				 sizeof(adapter->fw_version),
				 "%d.%d, 0x%08x, %d.%d.%d",
				 fw.eep_major, fw.eep_minor, fw.etrack_id,
				 fw.or_major, fw.or_build, fw.or_patch);
		/* no option rom */
		} else if (fw.etrack_id != 0X0000) {
			snprintf(adapter->fw_version,
				 sizeof(adapter->fw_version),
				 "%d.%d, 0x%08x",
				 fw.eep_major, fw.eep_minor, fw.etrack_id);
		} else {
			snprintf(adapter->fw_version,
				 sizeof(adapter->fw_version),
				 "%d.%d.%d",
				 fw.eep_major, fw.eep_minor, fw.eep_build);
		}
		break;
	}
}

/**
 * igb_init_mas - init Media Autosense feature if enabled in the NVM
 *
 * @adapter: adapter struct
 **/
static void igb_init_mas(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u16 eeprom_data;

	hw->nvm.ops.read(hw, NVM_COMPAT, 1, &eeprom_data);
	switch (hw->bus.func) {
	case E1000_FUNC_0:
		if (eeprom_data & IGB_MAS_ENABLE_0) {
			adapter->flags |= IGB_FLAG_MAS_ENABLE;
			netdev_info(adapter->netdev,
				"MAS: Enabling Media Autosense for port %d\n",
				hw->bus.func);
		}
		break;
	case E1000_FUNC_1:
		if (eeprom_data & IGB_MAS_ENABLE_1) {
			adapter->flags |= IGB_FLAG_MAS_ENABLE;
			netdev_info(adapter->netdev,
				"MAS: Enabling Media Autosense for port %d\n",
				hw->bus.func);
		}
		break;
	case E1000_FUNC_2:
		if (eeprom_data & IGB_MAS_ENABLE_2) {
			adapter->flags |= IGB_FLAG_MAS_ENABLE;
			netdev_info(adapter->netdev,
				"MAS: Enabling Media Autosense for port %d\n",
				hw->bus.func);
		}
		break;
	case E1000_FUNC_3:
		if (eeprom_data & IGB_MAS_ENABLE_3) {
			adapter->flags |= IGB_FLAG_MAS_ENABLE;
			netdev_info(adapter->netdev,
				"MAS: Enabling Media Autosense for port %d\n",
				hw->bus.func);
		}
		break;
	default:
		/* Shouldn't get here */
		netdev_err(adapter->netdev,
			"MAS: Invalid port configuration, returning\n");
		break;
	}
}

/**
 *  igb_init_i2c - Init I2C interface
 *  @adapter: pointer to adapter structure
 **/
static s32 igb_init_i2c(struct igb_adapter *adapter)
{
	s32 status = 0;

	/* I2C interface supported on i350 devices */
	if (adapter->hw.mac.type != e1000_i350)
		return 0;

	/* Initialize the i2c bus which is controlled by the registers.
	 * This bus will use the i2c_algo_bit structure that implements
	 * the protocol through toggling of the 4 bits in the register.
	 */
	adapter->i2c_adap.owner = THIS_MODULE;
	adapter->i2c_algo = igb_i2c_algo;
	adapter->i2c_algo.data = adapter;
	adapter->i2c_adap.algo_data = &adapter->i2c_algo;
	adapter->i2c_adap.dev.parent = &adapter->pdev->dev;
	strscpy(adapter->i2c_adap.name, "igb BB",
		sizeof(adapter->i2c_adap.name));
	status = i2c_bit_add_bus(&adapter->i2c_adap);
	return status;
}

/**
 *  igb_probe - Device Initialization Routine
 *  @pdev: PCI device information struct
 *  @ent: entry in igb_pci_tbl
 *
 *  Returns 0 on success, negative on failure
 *
 *  igb_probe initializes an adapter identified by a pci_dev structure.
 *  The OS initialization, configuring of the adapter private structure,
 *  and a hardware reset occur.
 **/
static int igb_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net_device *netdev;
	struct igb_adapter *adapter;
	struct e1000_hw *hw;
	u16 eeprom_data = 0;
	s32 ret_val;
	static int global_quad_port_a; /* global quad port a indication */
	const struct e1000_info *ei = igb_info_tbl[ent->driver_data];
<<<<<<< HEAD
	unsigned long mmio_start, mmio_len;
	int err, pci_using_dac;
	u16 eeprom_apme_mask = IGB_EEPROM_APME;
	u8 part_str[E1000_PBANUM_LENGTH];
=======
	u8 part_str[E1000_PBANUM_LENGTH];
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Catch broken hardware that put the wrong VF device ID in
	 * the PCIe SR-IOV capability.
	 */
	if (pdev->is_virtfn) {
<<<<<<< HEAD
		WARN(1, KERN_ERR "%s (%hx:%hx) should not be a VF!\n",
		     pci_name(pdev), pdev->vendor, pdev->device);
=======
		WARN(1, KERN_ERR "%s (%x:%x) should not be a VF!\n",
			pci_name(pdev), pdev->vendor, pdev->device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	err = pci_enable_device_mem(pdev);
	if (err)
		return err;

<<<<<<< HEAD
	pci_using_dac = 0;
	err = dma_set_mask(&pdev->dev, DMA_BIT_MASK(64));
	if (!err) {
		err = dma_set_coherent_mask(&pdev->dev, DMA_BIT_MASK(64));
		if (!err)
			pci_using_dac = 1;
	} else {
		err = dma_set_mask(&pdev->dev, DMA_BIT_MASK(32));
		if (err) {
			err = dma_set_coherent_mask(&pdev->dev, DMA_BIT_MASK(32));
			if (err) {
				dev_err(&pdev->dev, "No usable DMA "
					"configuration, aborting\n");
				goto err_dma;
			}
		}
	}

	err = pci_request_selected_regions(pdev, pci_select_bars(pdev,
	                                   IORESOURCE_MEM),
	                                   igb_driver_name);
	if (err)
		goto err_pci_reg;

	pci_enable_pcie_error_reporting(pdev);

=======
	err = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(64));
	if (err) {
		dev_err(&pdev->dev,
			"No usable DMA configuration, aborting\n");
		goto err_dma;
	}

	err = pci_request_mem_regions(pdev, igb_driver_name);
	if (err)
		goto err_pci_reg;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pci_set_master(pdev);
	pci_save_state(pdev);

	err = -ENOMEM;
	netdev = alloc_etherdev_mq(sizeof(struct igb_adapter),
				   IGB_MAX_TX_QUEUES);
	if (!netdev)
		goto err_alloc_etherdev;

	SET_NETDEV_DEV(netdev, &pdev->dev);

	pci_set_drvdata(pdev, netdev);
	adapter = netdev_priv(netdev);
	adapter->netdev = netdev;
	adapter->pdev = pdev;
	hw = &adapter->hw;
	hw->back = adapter;
	adapter->msg_enable = netif_msg_init(debug, DEFAULT_MSG_ENABLE);

<<<<<<< HEAD
	mmio_start = pci_resource_start(pdev, 0);
	mmio_len = pci_resource_len(pdev, 0);

	err = -EIO;
	hw->hw_addr = ioremap(mmio_start, mmio_len);
	if (!hw->hw_addr)
		goto err_ioremap;
=======
	err = -EIO;
	adapter->io_addr = pci_iomap(pdev, 0, 0);
	if (!adapter->io_addr)
		goto err_ioremap;
	/* hw->hw_addr can be altered, we'll use adapter->io_addr for unmap */
	hw->hw_addr = adapter->io_addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	netdev->netdev_ops = &igb_netdev_ops;
	igb_set_ethtool_ops(netdev);
	netdev->watchdog_timeo = 5 * HZ;

<<<<<<< HEAD
	strncpy(netdev->name, pci_name(pdev), sizeof(netdev->name) - 1);

	netdev->mem_start = mmio_start;
	netdev->mem_end = mmio_start + mmio_len;
=======
	strscpy(netdev->name, pci_name(pdev), sizeof(netdev->name));

	netdev->mem_start = pci_resource_start(pdev, 0);
	netdev->mem_end = pci_resource_end(pdev, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* PCI config space info */
	hw->vendor_id = pdev->vendor;
	hw->device_id = pdev->device;
	hw->revision_id = pdev->revision;
	hw->subsystem_vendor_id = pdev->subsystem_vendor;
	hw->subsystem_device_id = pdev->subsystem_device;

	/* Copy the default MAC, PHY and NVM function pointers */
	memcpy(&hw->mac.ops, ei->mac_ops, sizeof(hw->mac.ops));
	memcpy(&hw->phy.ops, ei->phy_ops, sizeof(hw->phy.ops));
	memcpy(&hw->nvm.ops, ei->nvm_ops, sizeof(hw->nvm.ops));
	/* Initialize skew-specific constants */
	err = ei->get_invariants(hw);
	if (err)
		goto err_sw_init;

	/* setup the private structure */
	err = igb_sw_init(adapter);
	if (err)
		goto err_sw_init;

	igb_get_bus_info_pcie(hw);

	hw->phy.autoneg_wait_to_complete = false;

	/* Copper options */
	if (hw->phy.media_type == e1000_media_type_copper) {
		hw->phy.mdix = AUTO_ALL_MODES;
		hw->phy.disable_polarity_correction = false;
		hw->phy.ms_type = e1000_ms_hw_default;
	}

	if (igb_check_reset_block(hw))
		dev_info(&pdev->dev,
			"PHY reset is blocked due to SOL/IDER session.\n");

<<<<<<< HEAD
	/*
	 * features is initialized to 0 in allocation, it might have bits
=======
	/* features is initialized to 0 in allocation, it might have bits
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * set by igb_sw_init so we should use an or instead of an
	 * assignment.
	 */
	netdev->features |= NETIF_F_SG |
<<<<<<< HEAD
			    NETIF_F_IP_CSUM |
			    NETIF_F_IPV6_CSUM |
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    NETIF_F_TSO |
			    NETIF_F_TSO6 |
			    NETIF_F_RXHASH |
			    NETIF_F_RXCSUM |
<<<<<<< HEAD
			    NETIF_F_HW_VLAN_RX |
			    NETIF_F_HW_VLAN_TX;

	/* copy netdev features into list of user selectable features */
	netdev->hw_features |= netdev->features;
	netdev->hw_features |= NETIF_F_RXALL;

	/* set this bit last since it cannot be part of hw_features */
	netdev->features |= NETIF_F_HW_VLAN_FILTER;

	netdev->vlan_features |= NETIF_F_TSO |
				 NETIF_F_TSO6 |
				 NETIF_F_IP_CSUM |
				 NETIF_F_IPV6_CSUM |
				 NETIF_F_SG;

	netdev->priv_flags |= IFF_SUPP_NOFCS;

	if (pci_using_dac) {
		netdev->features |= NETIF_F_HIGHDMA;
		netdev->vlan_features |= NETIF_F_HIGHDMA;
	}

	if (hw->mac.type >= e1000_82576) {
		netdev->hw_features |= NETIF_F_SCTP_CSUM;
		netdev->features |= NETIF_F_SCTP_CSUM;
	}

	netdev->priv_flags |= IFF_UNICAST_FLT;
=======
			    NETIF_F_HW_CSUM;

	if (hw->mac.type >= e1000_82576)
		netdev->features |= NETIF_F_SCTP_CRC | NETIF_F_GSO_UDP_L4;

	if (hw->mac.type >= e1000_i350)
		netdev->features |= NETIF_F_HW_TC;

#define IGB_GSO_PARTIAL_FEATURES (NETIF_F_GSO_GRE | \
				  NETIF_F_GSO_GRE_CSUM | \
				  NETIF_F_GSO_IPXIP4 | \
				  NETIF_F_GSO_IPXIP6 | \
				  NETIF_F_GSO_UDP_TUNNEL | \
				  NETIF_F_GSO_UDP_TUNNEL_CSUM)

	netdev->gso_partial_features = IGB_GSO_PARTIAL_FEATURES;
	netdev->features |= NETIF_F_GSO_PARTIAL | IGB_GSO_PARTIAL_FEATURES;

	/* copy netdev features into list of user selectable features */
	netdev->hw_features |= netdev->features |
			       NETIF_F_HW_VLAN_CTAG_RX |
			       NETIF_F_HW_VLAN_CTAG_TX |
			       NETIF_F_RXALL;

	if (hw->mac.type >= e1000_i350)
		netdev->hw_features |= NETIF_F_NTUPLE;

	netdev->features |= NETIF_F_HIGHDMA;

	netdev->vlan_features |= netdev->features | NETIF_F_TSO_MANGLEID;
	netdev->mpls_features |= NETIF_F_HW_CSUM;
	netdev->hw_enc_features |= netdev->vlan_features;

	/* set this bit last since it cannot be part of vlan_features */
	netdev->features |= NETIF_F_HW_VLAN_CTAG_FILTER |
			    NETIF_F_HW_VLAN_CTAG_RX |
			    NETIF_F_HW_VLAN_CTAG_TX;

	netdev->priv_flags |= IFF_SUPP_NOFCS;

	netdev->priv_flags |= IFF_UNICAST_FLT;
	netdev->xdp_features = NETDEV_XDP_ACT_BASIC | NETDEV_XDP_ACT_REDIRECT;

	/* MTU range: 68 - 9216 */
	netdev->min_mtu = ETH_MIN_MTU;
	netdev->max_mtu = MAX_STD_JUMBO_FRAME_SIZE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	adapter->en_mng_pt = igb_enable_mng_pass_thru(hw);

	/* before reading the NVM, reset the controller to put the device in a
<<<<<<< HEAD
	 * known good starting state */
	hw->mac.ops.reset_hw(hw);

	/* make sure the NVM is good */
	if (hw->nvm.ops.validate(hw) < 0) {
		dev_err(&pdev->dev, "The NVM Checksum Is Not Valid\n");
		err = -EIO;
		goto err_eeprom;
	}

	/* copy the MAC address out of the NVM */
	if (hw->mac.ops.read_mac_addr(hw))
		dev_err(&pdev->dev, "NVM Read Error\n");

	memcpy(netdev->dev_addr, hw->mac.addr, netdev->addr_len);
	memcpy(netdev->perm_addr, hw->mac.addr, netdev->addr_len);

	if (!is_valid_ether_addr(netdev->perm_addr)) {
=======
	 * known good starting state
	 */
	hw->mac.ops.reset_hw(hw);

	/* make sure the NVM is good , i211/i210 parts can have special NVM
	 * that doesn't contain a checksum
	 */
	switch (hw->mac.type) {
	case e1000_i210:
	case e1000_i211:
		if (igb_get_flash_presence_i210(hw)) {
			if (hw->nvm.ops.validate(hw) < 0) {
				dev_err(&pdev->dev,
					"The NVM Checksum Is Not Valid\n");
				err = -EIO;
				goto err_eeprom;
			}
		}
		break;
	default:
		if (hw->nvm.ops.validate(hw) < 0) {
			dev_err(&pdev->dev, "The NVM Checksum Is Not Valid\n");
			err = -EIO;
			goto err_eeprom;
		}
		break;
	}

	if (eth_platform_get_mac_address(&pdev->dev, hw->mac.addr)) {
		/* copy the MAC address out of the NVM */
		if (hw->mac.ops.read_mac_addr(hw))
			dev_err(&pdev->dev, "NVM Read Error\n");
	}

	eth_hw_addr_set(netdev, hw->mac.addr);

	if (!is_valid_ether_addr(netdev->dev_addr)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_err(&pdev->dev, "Invalid MAC Address\n");
		err = -EIO;
		goto err_eeprom;
	}

<<<<<<< HEAD
	setup_timer(&adapter->watchdog_timer, igb_watchdog,
	            (unsigned long) adapter);
	setup_timer(&adapter->phy_info_timer, igb_update_phy_info,
	            (unsigned long) adapter);
=======
	igb_set_default_mac_filter(adapter);

	/* get firmware version for ethtool -i */
	igb_set_fw_version(adapter);

	/* configure RXPBSIZE and TXPBSIZE */
	if (hw->mac.type == e1000_i210) {
		wr32(E1000_RXPBS, I210_RXPBSIZE_DEFAULT);
		wr32(E1000_TXPBS, I210_TXPBSIZE_DEFAULT);
	}

	timer_setup(&adapter->watchdog_timer, igb_watchdog, 0);
	timer_setup(&adapter->phy_info_timer, igb_update_phy_info, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	INIT_WORK(&adapter->reset_task, igb_reset_task);
	INIT_WORK(&adapter->watchdog_task, igb_watchdog_task);

	/* Initialize link properties that are user-changeable */
	adapter->fc_autoneg = true;
	hw->mac.autoneg = true;
	hw->phy.autoneg_advertised = 0x2f;

	hw->fc.requested_mode = e1000_fc_default;
	hw->fc.current_mode = e1000_fc_default;

	igb_validate_mdi_setting(hw);

<<<<<<< HEAD
	/* Initial Wake on LAN setting If APM wake is enabled in the EEPROM,
	 * enable the ACPI Magic Packet filter
	 */

	if (hw->bus.func == 0)
		hw->nvm.ops.read(hw, NVM_INIT_CONTROL3_PORT_A, 1, &eeprom_data);
	else if (hw->mac.type >= e1000_82580)
		hw->nvm.ops.read(hw, NVM_INIT_CONTROL3_PORT_A +
		                 NVM_82580_LAN_FUNC_OFFSET(hw->bus.func), 1,
		                 &eeprom_data);
	else if (hw->bus.func == 1)
		hw->nvm.ops.read(hw, NVM_INIT_CONTROL3_PORT_B, 1, &eeprom_data);

	if (eeprom_data & eeprom_apme_mask)
		adapter->eeprom_wol |= E1000_WUFC_MAG;

	/* now that we have the eeprom settings, apply the special cases where
	 * the eeprom may be wrong or the board simply won't support wake on
	 * lan on a particular port */
	switch (pdev->device) {
	case E1000_DEV_ID_82575GB_QUAD_COPPER:
		adapter->eeprom_wol = 0;
=======
	/* By default, support wake on port A */
	if (hw->bus.func == 0)
		adapter->flags |= IGB_FLAG_WOL_SUPPORTED;

	/* Check the NVM for wake support on non-port A ports */
	if (hw->mac.type >= e1000_82580)
		hw->nvm.ops.read(hw, NVM_INIT_CONTROL3_PORT_A +
				 NVM_82580_LAN_FUNC_OFFSET(hw->bus.func), 1,
				 &eeprom_data);
	else if (hw->bus.func == 1)
		hw->nvm.ops.read(hw, NVM_INIT_CONTROL3_PORT_B, 1, &eeprom_data);

	if (eeprom_data & IGB_EEPROM_APME)
		adapter->flags |= IGB_FLAG_WOL_SUPPORTED;

	/* now that we have the eeprom settings, apply the special cases where
	 * the eeprom may be wrong or the board simply won't support wake on
	 * lan on a particular port
	 */
	switch (pdev->device) {
	case E1000_DEV_ID_82575GB_QUAD_COPPER:
		adapter->flags &= ~IGB_FLAG_WOL_SUPPORTED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case E1000_DEV_ID_82575EB_FIBER_SERDES:
	case E1000_DEV_ID_82576_FIBER:
	case E1000_DEV_ID_82576_SERDES:
		/* Wake events only supported on port A for dual fiber
<<<<<<< HEAD
		 * regardless of eeprom setting */
		if (rd32(E1000_STATUS) & E1000_STATUS_FUNC_1)
			adapter->eeprom_wol = 0;
=======
		 * regardless of eeprom setting
		 */
		if (rd32(E1000_STATUS) & E1000_STATUS_FUNC_1)
			adapter->flags &= ~IGB_FLAG_WOL_SUPPORTED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case E1000_DEV_ID_82576_QUAD_COPPER:
	case E1000_DEV_ID_82576_QUAD_COPPER_ET2:
		/* if quad port adapter, disable WoL on all but port A */
		if (global_quad_port_a != 0)
<<<<<<< HEAD
			adapter->eeprom_wol = 0;
=======
			adapter->flags &= ~IGB_FLAG_WOL_SUPPORTED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else
			adapter->flags |= IGB_FLAG_QUAD_PORT_A;
		/* Reset for multiple quad port adapters */
		if (++global_quad_port_a == 4)
			global_quad_port_a = 0;
		break;
<<<<<<< HEAD
	}

	/* initialize the wol settings based on the eeprom settings */
	adapter->wol = adapter->eeprom_wol;
	device_set_wakeup_enable(&adapter->pdev->dev, adapter->wol);
=======
	default:
		/* If the device can't wake, don't set software support */
		if (!device_can_wakeup(&adapter->pdev->dev))
			adapter->flags &= ~IGB_FLAG_WOL_SUPPORTED;
	}

	/* initialize the wol settings based on the eeprom settings */
	if (adapter->flags & IGB_FLAG_WOL_SUPPORTED)
		adapter->wol |= E1000_WUFC_MAG;

	/* Some vendors want WoL disabled by default, but still supported */
	if ((hw->mac.type == e1000_i350) &&
	    (pdev->subsystem_vendor == PCI_VENDOR_ID_HP)) {
		adapter->flags |= IGB_FLAG_WOL_SUPPORTED;
		adapter->wol = 0;
	}

	/* Some vendors want the ability to Use the EEPROM setting as
	 * enable/disable only, and not for capability
	 */
	if (((hw->mac.type == e1000_i350) ||
	     (hw->mac.type == e1000_i354)) &&
	    (pdev->subsystem_vendor == PCI_VENDOR_ID_DELL)) {
		adapter->flags |= IGB_FLAG_WOL_SUPPORTED;
		adapter->wol = 0;
	}
	if (hw->mac.type == e1000_i350) {
		if (((pdev->subsystem_device == 0x5001) ||
		     (pdev->subsystem_device == 0x5002)) &&
				(hw->bus.func == 0)) {
			adapter->flags |= IGB_FLAG_WOL_SUPPORTED;
			adapter->wol = 0;
		}
		if (pdev->subsystem_device == 0x1F52)
			adapter->flags |= IGB_FLAG_WOL_SUPPORTED;
	}

	device_set_wakeup_enable(&adapter->pdev->dev,
				 adapter->flags & IGB_FLAG_WOL_SUPPORTED);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* reset the hardware with the new settings */
	igb_reset(adapter);

<<<<<<< HEAD
	/* let the f/w know that the h/w is now under the control of the
	 * driver. */
=======
	/* Init the I2C interface */
	err = igb_init_i2c(adapter);
	if (err) {
		dev_err(&pdev->dev, "failed to init i2c interface\n");
		goto err_eeprom;
	}

	/* let the f/w know that the h/w is now under the control of the
	 * driver.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	igb_get_hw_control(adapter);

	strcpy(netdev->name, "eth%d");
	err = register_netdev(netdev);
	if (err)
		goto err_register;

	/* carrier off reporting is important to ethtool even BEFORE open */
	netif_carrier_off(netdev);

#ifdef CONFIG_IGB_DCA
	if (dca_add_requester(&pdev->dev) == 0) {
		adapter->flags |= IGB_FLAG_DCA_ENABLED;
		dev_info(&pdev->dev, "DCA enabled\n");
		igb_setup_dca(adapter);
	}

#endif
<<<<<<< HEAD
	/* do hw tstamp init after resetting */
	igb_init_hw_timer(adapter);

	dev_info(&pdev->dev, "Intel(R) Gigabit Ethernet Network Connection\n");
	/* print bus type/speed/width info */
	dev_info(&pdev->dev, "%s: (PCIe:%s:%s) %pM\n",
		 netdev->name,
		 ((hw->bus.speed == e1000_bus_speed_2500) ? "2.5Gb/s" :
		  (hw->bus.speed == e1000_bus_speed_5000) ? "5.0Gb/s" :
		                                            "unknown"),
		 ((hw->bus.width == e1000_bus_width_pcie_x4) ? "Width x4" :
		  (hw->bus.width == e1000_bus_width_pcie_x2) ? "Width x2" :
		  (hw->bus.width == e1000_bus_width_pcie_x1) ? "Width x1" :
		   "unknown"),
		 netdev->dev_addr);

	ret_val = igb_read_part_string(hw, part_str, E1000_PBANUM_LENGTH);
=======
#ifdef CONFIG_IGB_HWMON
	/* Initialize the thermal sensor on i350 devices. */
	if (hw->mac.type == e1000_i350 && hw->bus.func == 0) {
		u16 ets_word;

		/* Read the NVM to determine if this i350 device supports an
		 * external thermal sensor.
		 */
		hw->nvm.ops.read(hw, NVM_ETS_CFG, 1, &ets_word);
		if (ets_word != 0x0000 && ets_word != 0xFFFF)
			adapter->ets = true;
		else
			adapter->ets = false;
		/* Only enable I2C bit banging if an external thermal
		 * sensor is supported.
		 */
		if (adapter->ets)
			igb_set_i2c_bb(hw);
		hw->mac.ops.init_thermal_sensor_thresh(hw);
		if (igb_sysfs_init(adapter))
			dev_err(&pdev->dev,
				"failed to allocate sysfs resources\n");
	} else {
		adapter->ets = false;
	}
#endif
	/* Check if Media Autosense is enabled */
	adapter->ei = *ei;
	if (hw->dev_spec._82575.mas_capable)
		igb_init_mas(adapter);

	/* do hw tstamp init after resetting */
	igb_ptp_init(adapter);

	dev_info(&pdev->dev, "Intel(R) Gigabit Ethernet Network Connection\n");
	/* print bus type/speed/width info, not applicable to i354 */
	if (hw->mac.type != e1000_i354) {
		dev_info(&pdev->dev, "%s: (PCIe:%s:%s) %pM\n",
			 netdev->name,
			 ((hw->bus.speed == e1000_bus_speed_2500) ? "2.5Gb/s" :
			  (hw->bus.speed == e1000_bus_speed_5000) ? "5.0Gb/s" :
			   "unknown"),
			 ((hw->bus.width == e1000_bus_width_pcie_x4) ?
			  "Width x4" :
			  (hw->bus.width == e1000_bus_width_pcie_x2) ?
			  "Width x2" :
			  (hw->bus.width == e1000_bus_width_pcie_x1) ?
			  "Width x1" : "unknown"), netdev->dev_addr);
	}

	if ((hw->mac.type == e1000_82576 &&
	     rd32(E1000_EECD) & E1000_EECD_PRES) ||
	    (hw->mac.type >= e1000_i210 ||
	     igb_get_flash_presence_i210(hw))) {
		ret_val = igb_read_part_string(hw, part_str,
					       E1000_PBANUM_LENGTH);
	} else {
		ret_val = -E1000_ERR_INVM_VALUE_NOT_FOUND;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret_val)
		strcpy(part_str, "Unknown");
	dev_info(&pdev->dev, "%s: PBA No: %s\n", netdev->name, part_str);
	dev_info(&pdev->dev,
		"Using %s interrupts. %d rx queue(s), %d tx queue(s)\n",
<<<<<<< HEAD
		adapter->msix_entries ? "MSI-X" :
		(adapter->flags & IGB_FLAG_HAS_MSI) ? "MSI" : "legacy",
		adapter->num_rx_queues, adapter->num_tx_queues);
	switch (hw->mac.type) {
	case e1000_i350:
		igb_set_eee_i350(hw);
		break;
	default:
		break;
	}

=======
		(adapter->flags & IGB_FLAG_HAS_MSIX) ? "MSI-X" :
		(adapter->flags & IGB_FLAG_HAS_MSI) ? "MSI" : "legacy",
		adapter->num_rx_queues, adapter->num_tx_queues);
	if (hw->phy.media_type == e1000_media_type_copper) {
		switch (hw->mac.type) {
		case e1000_i350:
		case e1000_i210:
		case e1000_i211:
			/* Enable EEE for internal copper PHY devices */
			err = igb_set_eee_i350(hw, true, true);
			if ((!err) &&
			    (!hw->dev_spec._82575.eee_disable)) {
				adapter->eee_advert =
					MDIO_EEE_100TX | MDIO_EEE_1000T;
				adapter->flags |= IGB_FLAG_EEE;
			}
			break;
		case e1000_i354:
			if ((rd32(E1000_CTRL_EXT) &
			    E1000_CTRL_EXT_LINK_MODE_SGMII)) {
				err = igb_set_eee_i354(hw, true, true);
				if ((!err) &&
					(!hw->dev_spec._82575.eee_disable)) {
					adapter->eee_advert =
					   MDIO_EEE_100TX | MDIO_EEE_1000T;
					adapter->flags |= IGB_FLAG_EEE;
				}
			}
			break;
		default:
			break;
		}
	}

	dev_pm_set_driver_flags(&pdev->dev, DPM_FLAG_NO_DIRECT_COMPLETE);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pm_runtime_put_noidle(&pdev->dev);
	return 0;

err_register:
	igb_release_hw_control(adapter);
<<<<<<< HEAD
=======
	memset(&adapter->i2c_adap, 0, sizeof(adapter->i2c_adap));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_eeprom:
	if (!igb_check_reset_block(hw))
		igb_reset_phy(hw);

	if (hw->flash_address)
		iounmap(hw->flash_address);
err_sw_init:
<<<<<<< HEAD
	igb_clear_interrupt_scheme(adapter);
	iounmap(hw->hw_addr);
err_ioremap:
	free_netdev(netdev);
err_alloc_etherdev:
	pci_release_selected_regions(pdev,
	                             pci_select_bars(pdev, IORESOURCE_MEM));
=======
	kfree(adapter->mac_table);
	kfree(adapter->shadow_vfta);
	igb_clear_interrupt_scheme(adapter);
#ifdef CONFIG_PCI_IOV
	igb_disable_sriov(pdev, false);
#endif
	pci_iounmap(pdev, adapter->io_addr);
err_ioremap:
	free_netdev(netdev);
err_alloc_etherdev:
	pci_release_mem_regions(pdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_pci_reg:
err_dma:
	pci_disable_device(pdev);
	return err;
}

<<<<<<< HEAD
/**
 * igb_remove - Device Removal Routine
 * @pdev: PCI device information struct
 *
 * igb_remove is called by the PCI subsystem to alert the driver
 * that it should release a PCI device.  The could be caused by a
 * Hot-Plug event, or because the driver is going to be removed from
 * memory.
 **/
static void __devexit igb_remove(struct pci_dev *pdev)
=======
#ifdef CONFIG_PCI_IOV
static int igb_sriov_reinit(struct pci_dev *dev)
{
	struct net_device *netdev = pci_get_drvdata(dev);
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct pci_dev *pdev = adapter->pdev;

	rtnl_lock();

	if (netif_running(netdev))
		igb_close(netdev);
	else
		igb_reset(adapter);

	igb_clear_interrupt_scheme(adapter);

	igb_init_queue_configuration(adapter);

	if (igb_init_interrupt_scheme(adapter, true)) {
		rtnl_unlock();
		dev_err(&pdev->dev, "Unable to allocate memory for queues\n");
		return -ENOMEM;
	}

	if (netif_running(netdev))
		igb_open(netdev);

	rtnl_unlock();

	return 0;
}

static int igb_disable_sriov(struct pci_dev *pdev, bool reinit)
{
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	unsigned long flags;

	/* reclaim resources allocated to VFs */
	if (adapter->vf_data) {
		/* disable iov and allow time for transactions to clear */
		if (pci_vfs_assigned(pdev)) {
			dev_warn(&pdev->dev,
				 "Cannot deallocate SR-IOV virtual functions while they are assigned - VFs will not be deallocated\n");
			return -EPERM;
		} else {
			pci_disable_sriov(pdev);
			msleep(500);
		}
		spin_lock_irqsave(&adapter->vfs_lock, flags);
		kfree(adapter->vf_mac_list);
		adapter->vf_mac_list = NULL;
		kfree(adapter->vf_data);
		adapter->vf_data = NULL;
		adapter->vfs_allocated_count = 0;
		spin_unlock_irqrestore(&adapter->vfs_lock, flags);
		wr32(E1000_IOVCTL, E1000_IOVCTL_REUSE_VFQ);
		wrfl();
		msleep(100);
		dev_info(&pdev->dev, "IOV Disabled\n");

		/* Re-enable DMA Coalescing flag since IOV is turned off */
		adapter->flags |= IGB_FLAG_DMAC;
	}

	return reinit ? igb_sriov_reinit(pdev) : 0;
}

static int igb_enable_sriov(struct pci_dev *pdev, int num_vfs, bool reinit)
{
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);
	int old_vfs = pci_num_vf(pdev);
	struct vf_mac_filter *mac_list;
	int err = 0;
	int num_vf_mac_filters, i;

	if (!(adapter->flags & IGB_FLAG_HAS_MSIX) || num_vfs > 7) {
		err = -EPERM;
		goto out;
	}
	if (!num_vfs)
		goto out;

	if (old_vfs) {
		dev_info(&pdev->dev, "%d pre-allocated VFs found - override max_vfs setting of %d\n",
			 old_vfs, max_vfs);
		adapter->vfs_allocated_count = old_vfs;
	} else
		adapter->vfs_allocated_count = num_vfs;

	adapter->vf_data = kcalloc(adapter->vfs_allocated_count,
				sizeof(struct vf_data_storage), GFP_KERNEL);

	/* if allocation failed then we do not support SR-IOV */
	if (!adapter->vf_data) {
		adapter->vfs_allocated_count = 0;
		err = -ENOMEM;
		goto out;
	}

	/* Due to the limited number of RAR entries calculate potential
	 * number of MAC filters available for the VFs. Reserve entries
	 * for PF default MAC, PF MAC filters and at least one RAR entry
	 * for each VF for VF MAC.
	 */
	num_vf_mac_filters = adapter->hw.mac.rar_entry_count -
			     (1 + IGB_PF_MAC_FILTERS_RESERVED +
			      adapter->vfs_allocated_count);

	adapter->vf_mac_list = kcalloc(num_vf_mac_filters,
				       sizeof(struct vf_mac_filter),
				       GFP_KERNEL);

	mac_list = adapter->vf_mac_list;
	INIT_LIST_HEAD(&adapter->vf_macs.l);

	if (adapter->vf_mac_list) {
		/* Initialize list of VF MAC filters */
		for (i = 0; i < num_vf_mac_filters; i++) {
			mac_list->vf = -1;
			mac_list->free = true;
			list_add(&mac_list->l, &adapter->vf_macs.l);
			mac_list++;
		}
	} else {
		/* If we could not allocate memory for the VF MAC filters
		 * we can continue without this feature but warn user.
		 */
		dev_err(&pdev->dev,
			"Unable to allocate memory for VF MAC filter list\n");
	}

	dev_info(&pdev->dev, "%d VFs allocated\n",
		 adapter->vfs_allocated_count);
	for (i = 0; i < adapter->vfs_allocated_count; i++)
		igb_vf_configure(adapter, i);

	/* DMA Coalescing is not supported in IOV mode. */
	adapter->flags &= ~IGB_FLAG_DMAC;

	if (reinit) {
		err = igb_sriov_reinit(pdev);
		if (err)
			goto err_out;
	}

	/* only call pci_enable_sriov() if no VFs are allocated already */
	if (!old_vfs) {
		err = pci_enable_sriov(pdev, adapter->vfs_allocated_count);
		if (err)
			goto err_out;
	}

	goto out;

err_out:
	kfree(adapter->vf_mac_list);
	adapter->vf_mac_list = NULL;
	kfree(adapter->vf_data);
	adapter->vf_data = NULL;
	adapter->vfs_allocated_count = 0;
out:
	return err;
}

#endif
/**
 *  igb_remove_i2c - Cleanup  I2C interface
 *  @adapter: pointer to adapter structure
 **/
static void igb_remove_i2c(struct igb_adapter *adapter)
{
	/* free the adapter bus structure */
	i2c_del_adapter(&adapter->i2c_adap);
}

/**
 *  igb_remove - Device Removal Routine
 *  @pdev: PCI device information struct
 *
 *  igb_remove is called by the PCI subsystem to alert the driver
 *  that it should release a PCI device.  The could be caused by a
 *  Hot-Plug event, or because the driver is going to be removed from
 *  memory.
 **/
static void igb_remove(struct pci_dev *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;

	pm_runtime_get_noresume(&pdev->dev);
<<<<<<< HEAD

	/*
	 * The watchdog timer may be rescheduled, so explicitly
=======
#ifdef CONFIG_IGB_HWMON
	igb_sysfs_exit(adapter);
#endif
	igb_remove_i2c(adapter);
	igb_ptp_stop(adapter);
	/* The watchdog timer may be rescheduled, so explicitly
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * disable watchdog from being rescheduled.
	 */
	set_bit(__IGB_DOWN, &adapter->state);
	del_timer_sync(&adapter->watchdog_timer);
	del_timer_sync(&adapter->phy_info_timer);

	cancel_work_sync(&adapter->reset_task);
	cancel_work_sync(&adapter->watchdog_task);

#ifdef CONFIG_IGB_DCA
	if (adapter->flags & IGB_FLAG_DCA_ENABLED) {
		dev_info(&pdev->dev, "DCA disabled\n");
		dca_remove_requester(&pdev->dev);
		adapter->flags &= ~IGB_FLAG_DCA_ENABLED;
		wr32(E1000_DCA_CTRL, E1000_DCA_CTRL_DCA_MODE_DISABLE);
	}
#endif

	/* Release control of h/w to f/w.  If f/w is AMT enabled, this
<<<<<<< HEAD
	 * would have already happened in close and is redundant. */
	igb_release_hw_control(adapter);

=======
	 * would have already happened in close and is redundant.
	 */
	igb_release_hw_control(adapter);

#ifdef CONFIG_PCI_IOV
	igb_disable_sriov(pdev, false);
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unregister_netdev(netdev);

	igb_clear_interrupt_scheme(adapter);

<<<<<<< HEAD
#ifdef CONFIG_PCI_IOV
	/* reclaim resources allocated to VFs */
	if (adapter->vf_data) {
		/* disable iov and allow time for transactions to clear */
		if (!igb_check_vf_assignment(adapter)) {
			pci_disable_sriov(pdev);
			msleep(500);
		} else {
			dev_info(&pdev->dev, "VF(s) assigned to guests!\n");
		}

		kfree(adapter->vf_data);
		adapter->vf_data = NULL;
		wr32(E1000_IOVCTL, E1000_IOVCTL_REUSE_VFQ);
		wrfl();
		msleep(100);
		dev_info(&pdev->dev, "IOV Disabled\n");
	}
#endif

	iounmap(hw->hw_addr);
	if (hw->flash_address)
		iounmap(hw->flash_address);
	pci_release_selected_regions(pdev,
	                             pci_select_bars(pdev, IORESOURCE_MEM));

	kfree(adapter->shadow_vfta);
	free_netdev(netdev);

	pci_disable_pcie_error_reporting(pdev);

=======
	pci_iounmap(pdev, adapter->io_addr);
	if (hw->flash_address)
		iounmap(hw->flash_address);
	pci_release_mem_regions(pdev);

	kfree(adapter->mac_table);
	kfree(adapter->shadow_vfta);
	free_netdev(netdev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pci_disable_device(pdev);
}

/**
<<<<<<< HEAD
 * igb_probe_vfs - Initialize vf data storage and add VFs to pci config space
 * @adapter: board private structure to initialize
 *
 * This function initializes the vf specific data storage and then attempts to
 * allocate the VFs.  The reason for ordering it this way is because it is much
 * mor expensive time wise to disable SR-IOV than it is to allocate and free
 * the memory for the VFs.
 **/
static void __devinit igb_probe_vfs(struct igb_adapter * adapter)
{
#ifdef CONFIG_PCI_IOV
	struct pci_dev *pdev = adapter->pdev;
	int old_vfs = igb_find_enabled_vfs(adapter);
	int i;

	if (old_vfs) {
		dev_info(&pdev->dev, "%d pre-allocated VFs found - override "
			 "max_vfs setting of %d\n", old_vfs, max_vfs);
		adapter->vfs_allocated_count = old_vfs;
	}

	if (!adapter->vfs_allocated_count)
		return;

	adapter->vf_data = kcalloc(adapter->vfs_allocated_count,
				sizeof(struct vf_data_storage), GFP_KERNEL);
	/* if allocation failed then we do not support SR-IOV */
	if (!adapter->vf_data) {
		adapter->vfs_allocated_count = 0;
		dev_err(&pdev->dev, "Unable to allocate memory for VF "
			"Data Storage\n");
		goto out;
	}

	if (!old_vfs) {
		if (pci_enable_sriov(pdev, adapter->vfs_allocated_count))
			goto err_out;
	}
	dev_info(&pdev->dev, "%d VFs allocated\n",
		 adapter->vfs_allocated_count);
	for (i = 0; i < adapter->vfs_allocated_count; i++)
		igb_vf_configure(adapter, i);

	/* DMA Coalescing is not supported in IOV mode. */
	adapter->flags &= ~IGB_FLAG_DMAC;
	goto out;
err_out:
	kfree(adapter->vf_data);
	adapter->vf_data = NULL;
	adapter->vfs_allocated_count = 0;
out:
	return;
#endif /* CONFIG_PCI_IOV */
}

/**
 * igb_init_hw_timer - Initialize hardware timer used with IEEE 1588 timestamp
 * @adapter: board private structure to initialize
 *
 * igb_init_hw_timer initializes the function pointer and values for the hw
 * timer found in hardware.
 **/
static void igb_init_hw_timer(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;

	switch (hw->mac.type) {
	case e1000_i350:
	case e1000_82580:
		memset(&adapter->cycles, 0, sizeof(adapter->cycles));
		adapter->cycles.read = igb_read_clock;
		adapter->cycles.mask = CLOCKSOURCE_MASK(64);
		adapter->cycles.mult = 1;
		/*
		 * The 82580 timesync updates the system timer every 8ns by 8ns
		 * and the value cannot be shifted.  Instead we need to shift
		 * the registers to generate a 64bit timer value.  As a result
		 * SYSTIMR/L/H, TXSTMPL/H, RXSTMPL/H all have to be shifted by
		 * 24 in order to generate a larger value for synchronization.
		 */
		adapter->cycles.shift = IGB_82580_TSYNC_SHIFT;
		/* disable system timer temporarily by setting bit 31 */
		wr32(E1000_TSAUXC, 0x80000000);
		wrfl();

		/* Set registers so that rollover occurs soon to test this. */
		wr32(E1000_SYSTIMR, 0x00000000);
		wr32(E1000_SYSTIML, 0x80000000);
		wr32(E1000_SYSTIMH, 0x000000FF);
		wrfl();

		/* enable system timer by clearing bit 31 */
		wr32(E1000_TSAUXC, 0x0);
		wrfl();

		timecounter_init(&adapter->clock,
				 &adapter->cycles,
				 ktime_to_ns(ktime_get_real()));
		/*
		 * Synchronize our NIC clock against system wall clock. NIC
		 * time stamp reading requires ~3us per sample, each sample
		 * was pretty stable even under load => only require 10
		 * samples for each offset comparison.
		 */
		memset(&adapter->compare, 0, sizeof(adapter->compare));
		adapter->compare.source = &adapter->clock;
		adapter->compare.target = ktime_get_real;
		adapter->compare.num_samples = 10;
		timecompare_update(&adapter->compare, 0);
		break;
	case e1000_82576:
		/*
		 * Initialize hardware timer: we keep it running just in case
		 * that some program needs it later on.
		 */
		memset(&adapter->cycles, 0, sizeof(adapter->cycles));
		adapter->cycles.read = igb_read_clock;
		adapter->cycles.mask = CLOCKSOURCE_MASK(64);
		adapter->cycles.mult = 1;
		/**
		 * Scale the NIC clock cycle by a large factor so that
		 * relatively small clock corrections can be added or
		 * subtracted at each clock tick. The drawbacks of a large
		 * factor are a) that the clock register overflows more quickly
		 * (not such a big deal) and b) that the increment per tick has
		 * to fit into 24 bits.  As a result we need to use a shift of
		 * 19 so we can fit a value of 16 into the TIMINCA register.
		 */
		adapter->cycles.shift = IGB_82576_TSYNC_SHIFT;
		wr32(E1000_TIMINCA,
		                (1 << E1000_TIMINCA_16NS_SHIFT) |
		                (16 << IGB_82576_TSYNC_SHIFT));

		/* Set registers so that rollover occurs soon to test this. */
		wr32(E1000_SYSTIML, 0x00000000);
		wr32(E1000_SYSTIMH, 0xFF800000);
		wrfl();

		timecounter_init(&adapter->clock,
				 &adapter->cycles,
				 ktime_to_ns(ktime_get_real()));
		/*
		 * Synchronize our NIC clock against system wall clock. NIC
		 * time stamp reading requires ~3us per sample, each sample
		 * was pretty stable even under load => only require 10
		 * samples for each offset comparison.
		 */
		memset(&adapter->compare, 0, sizeof(adapter->compare));
		adapter->compare.source = &adapter->clock;
		adapter->compare.target = ktime_get_real;
		adapter->compare.num_samples = 10;
		timecompare_update(&adapter->compare, 0);
		break;
	case e1000_82575:
		/* 82575 does not support timesync */
	default:
		break;
	}

}

/**
 * igb_sw_init - Initialize general software structures (struct igb_adapter)
 * @adapter: board private structure to initialize
 *
 * igb_sw_init initializes the Adapter private data structure.
 * Fields are initialized based on PCI device information and
 * OS network device settings (MTU size).
 **/
static int __devinit igb_sw_init(struct igb_adapter *adapter)
=======
 *  igb_probe_vfs - Initialize vf data storage and add VFs to pci config space
 *  @adapter: board private structure to initialize
 *
 *  This function initializes the vf specific data storage and then attempts to
 *  allocate the VFs.  The reason for ordering it this way is because it is much
 *  mor expensive time wise to disable SR-IOV than it is to allocate and free
 *  the memory for the VFs.
 **/
static void igb_probe_vfs(struct igb_adapter *adapter)
{
#ifdef CONFIG_PCI_IOV
	struct pci_dev *pdev = adapter->pdev;
	struct e1000_hw *hw = &adapter->hw;

	/* Virtualization features not supported on i210 and 82580 family. */
	if ((hw->mac.type == e1000_i210) || (hw->mac.type == e1000_i211) ||
	    (hw->mac.type == e1000_82580))
		return;

	/* Of the below we really only want the effect of getting
	 * IGB_FLAG_HAS_MSIX set (if available), without which
	 * igb_enable_sriov() has no effect.
	 */
	igb_set_interrupt_capability(adapter, true);
	igb_reset_interrupt_capability(adapter);

	pci_sriov_set_totalvfs(pdev, 7);
	igb_enable_sriov(pdev, max_vfs, false);

#endif /* CONFIG_PCI_IOV */
}

unsigned int igb_get_max_rss_queues(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	unsigned int max_rss_queues;

	/* Determine the maximum number of RSS queues supported. */
	switch (hw->mac.type) {
	case e1000_i211:
		max_rss_queues = IGB_MAX_RX_QUEUES_I211;
		break;
	case e1000_82575:
	case e1000_i210:
		max_rss_queues = IGB_MAX_RX_QUEUES_82575;
		break;
	case e1000_i350:
		/* I350 cannot do RSS and SR-IOV at the same time */
		if (!!adapter->vfs_allocated_count) {
			max_rss_queues = 1;
			break;
		}
		fallthrough;
	case e1000_82576:
		if (!!adapter->vfs_allocated_count) {
			max_rss_queues = 2;
			break;
		}
		fallthrough;
	case e1000_82580:
	case e1000_i354:
	default:
		max_rss_queues = IGB_MAX_RX_QUEUES;
		break;
	}

	return max_rss_queues;
}

static void igb_init_queue_configuration(struct igb_adapter *adapter)
{
	u32 max_rss_queues;

	max_rss_queues = igb_get_max_rss_queues(adapter);
	adapter->rss_queues = min_t(u32, max_rss_queues, num_online_cpus());

	igb_set_flag_queue_pairs(adapter, max_rss_queues);
}

void igb_set_flag_queue_pairs(struct igb_adapter *adapter,
			      const u32 max_rss_queues)
{
	struct e1000_hw *hw = &adapter->hw;

	/* Determine if we need to pair queues. */
	switch (hw->mac.type) {
	case e1000_82575:
	case e1000_i211:
		/* Device supports enough interrupts without queue pairing. */
		break;
	case e1000_82576:
	case e1000_82580:
	case e1000_i350:
	case e1000_i354:
	case e1000_i210:
	default:
		/* If rss_queues > half of max_rss_queues, pair the queues in
		 * order to conserve interrupts due to limited supply.
		 */
		if (adapter->rss_queues > (max_rss_queues / 2))
			adapter->flags |= IGB_FLAG_QUEUE_PAIRS;
		else
			adapter->flags &= ~IGB_FLAG_QUEUE_PAIRS;
		break;
	}
}

/**
 *  igb_sw_init - Initialize general software structures (struct igb_adapter)
 *  @adapter: board private structure to initialize
 *
 *  igb_sw_init initializes the Adapter private data structure.
 *  Fields are initialized based on PCI device information and
 *  OS network device settings (MTU size).
 **/
static int igb_sw_init(struct igb_adapter *adapter)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct e1000_hw *hw = &adapter->hw;
	struct net_device *netdev = adapter->netdev;
	struct pci_dev *pdev = adapter->pdev;

	pci_read_config_word(pdev, PCI_COMMAND, &hw->bus.pci_cmd_word);

	/* set default ring sizes */
	adapter->tx_ring_count = IGB_DEFAULT_TXD;
	adapter->rx_ring_count = IGB_DEFAULT_RXD;

	/* set default ITR values */
	adapter->rx_itr_setting = IGB_DEFAULT_ITR;
	adapter->tx_itr_setting = IGB_DEFAULT_ITR;

	/* set default work limits */
	adapter->tx_work_limit = IGB_DEFAULT_TX_WORK;

<<<<<<< HEAD
	adapter->max_frame_size = netdev->mtu + ETH_HLEN + ETH_FCS_LEN +
				  VLAN_HLEN;
	adapter->min_frame_size = ETH_ZLEN + ETH_FCS_LEN;

	adapter->node = -1;

	spin_lock_init(&adapter->stats64_lock);
=======
	adapter->max_frame_size = netdev->mtu + IGB_ETH_PKT_HDR_PAD;
	adapter->min_frame_size = ETH_ZLEN + ETH_FCS_LEN;

	spin_lock_init(&adapter->nfc_lock);
	spin_lock_init(&adapter->stats64_lock);

	/* init spinlock to avoid concurrency of VF resources */
	spin_lock_init(&adapter->vfs_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_PCI_IOV
	switch (hw->mac.type) {
	case e1000_82576:
	case e1000_i350:
		if (max_vfs > 7) {
			dev_warn(&pdev->dev,
				 "Maximum of 7 VFs per PF, using max\n");
<<<<<<< HEAD
			adapter->vfs_allocated_count = 7;
		} else
			adapter->vfs_allocated_count = max_vfs;
=======
			max_vfs = adapter->vfs_allocated_count = 7;
		} else
			adapter->vfs_allocated_count = max_vfs;
		if (adapter->vfs_allocated_count)
			dev_warn(&pdev->dev,
				 "Enabling SR-IOV VFs using the module parameter is deprecated - please use the pci sysfs interface.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		break;
	}
#endif /* CONFIG_PCI_IOV */
<<<<<<< HEAD
	adapter->rss_queues = min_t(u32, IGB_MAX_RX_QUEUES, num_online_cpus());
	/* i350 cannot do RSS and SR-IOV at the same time */
	if (hw->mac.type == e1000_i350 && adapter->vfs_allocated_count)
		adapter->rss_queues = 1;

	/*
	 * if rss_queues > 4 or vfs are going to be allocated with rss_queues
	 * then we should combine the queues into a queue pair in order to
	 * conserve interrupts due to limited supply
	 */
	if ((adapter->rss_queues > 4) ||
	    ((adapter->rss_queues > 1) && (adapter->vfs_allocated_count > 6)))
		adapter->flags |= IGB_FLAG_QUEUE_PAIRS;

	/* Setup and initialize a copy of the hw vlan table array */
	adapter->shadow_vfta = kzalloc(sizeof(u32) *
				E1000_VLAN_FILTER_TBL_SIZE,
				GFP_ATOMIC);

	/* This call may decrease the number of queues */
	if (igb_init_interrupt_scheme(adapter)) {
=======

	/* Assume MSI-X interrupts, will be checked during IRQ allocation */
	adapter->flags |= IGB_FLAG_HAS_MSIX;

	adapter->mac_table = kcalloc(hw->mac.rar_entry_count,
				     sizeof(struct igb_mac_addr),
				     GFP_KERNEL);
	if (!adapter->mac_table)
		return -ENOMEM;

	igb_probe_vfs(adapter);

	igb_init_queue_configuration(adapter);

	/* Setup and initialize a copy of the hw vlan table array */
	adapter->shadow_vfta = kcalloc(E1000_VLAN_FILTER_TBL_SIZE, sizeof(u32),
				       GFP_KERNEL);
	if (!adapter->shadow_vfta)
		return -ENOMEM;

	/* This call may decrease the number of queues */
	if (igb_init_interrupt_scheme(adapter, true)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_err(&pdev->dev, "Unable to allocate memory for queues\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	igb_probe_vfs(adapter);

	/* Explicitly disable IRQ since the NIC can be in any state. */
	igb_irq_disable(adapter);

	if (hw->mac.type == e1000_i350)
=======
	/* Explicitly disable IRQ since the NIC can be in any state. */
	igb_irq_disable(adapter);

	if (hw->mac.type >= e1000_i350)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		adapter->flags &= ~IGB_FLAG_DMAC;

	set_bit(__IGB_DOWN, &adapter->state);
	return 0;
}

/**
<<<<<<< HEAD
 * igb_open - Called when a network interface is made active
 * @netdev: network interface device structure
 *
 * Returns 0 on success, negative value on failure
 *
 * The open entry point is called when a network interface is made
 * active by the system (IFF_UP).  At this point all resources needed
 * for transmit and receive operations are allocated, the interrupt
 * handler is registered with the OS, the watchdog timer is started,
 * and the stack is notified that the interface is ready.
=======
 *  __igb_open - Called when a network interface is made active
 *  @netdev: network interface device structure
 *  @resuming: indicates whether we are in a resume call
 *
 *  Returns 0 on success, negative value on failure
 *
 *  The open entry point is called when a network interface is made
 *  active by the system (IFF_UP).  At this point all resources needed
 *  for transmit and receive operations are allocated, the interrupt
 *  handler is registered with the OS, the watchdog timer is started,
 *  and the stack is notified that the interface is ready.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static int __igb_open(struct net_device *netdev, bool resuming)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	struct pci_dev *pdev = adapter->pdev;
	int err;
	int i;

	/* disallow open during test */
	if (test_bit(__IGB_TESTING, &adapter->state)) {
		WARN_ON(resuming);
		return -EBUSY;
	}

	if (!resuming)
		pm_runtime_get_sync(&pdev->dev);

	netif_carrier_off(netdev);

	/* allocate transmit descriptors */
	err = igb_setup_all_tx_resources(adapter);
	if (err)
		goto err_setup_tx;

	/* allocate receive descriptors */
	err = igb_setup_all_rx_resources(adapter);
	if (err)
		goto err_setup_rx;

	igb_power_up_link(adapter);

	/* before we allocate an interrupt, we must be ready to handle it.
	 * Setting DEBUG_SHIRQ in the kernel makes it fire an interrupt
	 * as soon as we call pci_request_irq, so we have to setup our
<<<<<<< HEAD
	 * clean_rx handler before we do so.  */
=======
	 * clean_rx handler before we do so.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	igb_configure(adapter);

	err = igb_request_irq(adapter);
	if (err)
		goto err_req_irq;

<<<<<<< HEAD
=======
	/* Notify the stack of the actual queue counts. */
	err = netif_set_real_num_tx_queues(adapter->netdev,
					   adapter->num_tx_queues);
	if (err)
		goto err_set_queues;

	err = netif_set_real_num_rx_queues(adapter->netdev,
					   adapter->num_rx_queues);
	if (err)
		goto err_set_queues;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* From here on the code is the same as igb_up() */
	clear_bit(__IGB_DOWN, &adapter->state);

	for (i = 0; i < adapter->num_q_vectors; i++)
		napi_enable(&(adapter->q_vector[i]->napi));

	/* Clear any pending interrupts. */
<<<<<<< HEAD
=======
	rd32(E1000_TSICR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rd32(E1000_ICR);

	igb_irq_enable(adapter);

	/* notify VFs that reset has been completed */
	if (adapter->vfs_allocated_count) {
		u32 reg_data = rd32(E1000_CTRL_EXT);
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		reg_data |= E1000_CTRL_EXT_PFRSTD;
		wr32(E1000_CTRL_EXT, reg_data);
	}

	netif_tx_start_all_queues(netdev);

	if (!resuming)
		pm_runtime_put(&pdev->dev);

	/* start the watchdog. */
	hw->mac.get_link_status = 1;
	schedule_work(&adapter->watchdog_task);

	return 0;

<<<<<<< HEAD
=======
err_set_queues:
	igb_free_irq(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_req_irq:
	igb_release_hw_control(adapter);
	igb_power_down_link(adapter);
	igb_free_all_rx_resources(adapter);
err_setup_rx:
	igb_free_all_tx_resources(adapter);
err_setup_tx:
	igb_reset(adapter);
	if (!resuming)
		pm_runtime_put(&pdev->dev);

	return err;
}

<<<<<<< HEAD
static int igb_open(struct net_device *netdev)
=======
int igb_open(struct net_device *netdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return __igb_open(netdev, false);
}

/**
<<<<<<< HEAD
 * igb_close - Disables a network interface
 * @netdev: network interface device structure
 *
 * Returns 0, this is not allowed to fail
 *
 * The close entry point is called when an interface is de-activated
 * by the OS.  The hardware is still under the driver's control, but
 * needs to be disabled.  A global MAC reset is issued to stop the
 * hardware, and all transmit and receive resources are freed.
=======
 *  __igb_close - Disables a network interface
 *  @netdev: network interface device structure
 *  @suspending: indicates we are in a suspend call
 *
 *  Returns 0, this is not allowed to fail
 *
 *  The close entry point is called when an interface is de-activated
 *  by the OS.  The hardware is still under the driver's control, but
 *  needs to be disabled.  A global MAC reset is issued to stop the
 *  hardware, and all transmit and receive resources are freed.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static int __igb_close(struct net_device *netdev, bool suspending)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct pci_dev *pdev = adapter->pdev;

	WARN_ON(test_bit(__IGB_RESETTING, &adapter->state));

	if (!suspending)
		pm_runtime_get_sync(&pdev->dev);

	igb_down(adapter);
	igb_free_irq(adapter);

	igb_free_all_tx_resources(adapter);
	igb_free_all_rx_resources(adapter);

	if (!suspending)
		pm_runtime_put_sync(&pdev->dev);
	return 0;
}

<<<<<<< HEAD
static int igb_close(struct net_device *netdev)
{
	return __igb_close(netdev, false);
}

/**
 * igb_setup_tx_resources - allocate Tx resources (Descriptors)
 * @tx_ring: tx descriptor ring (for a specific queue) to setup
 *
 * Return 0 on success, negative on failure
=======
int igb_close(struct net_device *netdev)
{
	if (netif_device_present(netdev) || netdev->dismantle)
		return __igb_close(netdev, false);
	return 0;
}

/**
 *  igb_setup_tx_resources - allocate Tx resources (Descriptors)
 *  @tx_ring: tx descriptor ring (for a specific queue) to setup
 *
 *  Return 0 on success, negative on failure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
int igb_setup_tx_resources(struct igb_ring *tx_ring)
{
	struct device *dev = tx_ring->dev;
<<<<<<< HEAD
	int orig_node = dev_to_node(dev);
	int size;

	size = sizeof(struct igb_tx_buffer) * tx_ring->count;
	tx_ring->tx_buffer_info = vzalloc_node(size, tx_ring->numa_node);
	if (!tx_ring->tx_buffer_info)
		tx_ring->tx_buffer_info = vzalloc(size);
=======
	int size;

	size = sizeof(struct igb_tx_buffer) * tx_ring->count;

	tx_ring->tx_buffer_info = vmalloc(size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!tx_ring->tx_buffer_info)
		goto err;

	/* round up to nearest 4K */
	tx_ring->size = tx_ring->count * sizeof(union e1000_adv_tx_desc);
	tx_ring->size = ALIGN(tx_ring->size, 4096);

<<<<<<< HEAD
	set_dev_node(dev, tx_ring->numa_node);
	tx_ring->desc = dma_alloc_coherent(dev,
					   tx_ring->size,
					   &tx_ring->dma,
					   GFP_KERNEL);
	set_dev_node(dev, orig_node);
	if (!tx_ring->desc)
		tx_ring->desc = dma_alloc_coherent(dev,
						   tx_ring->size,
						   &tx_ring->dma,
						   GFP_KERNEL);

=======
	tx_ring->desc = dma_alloc_coherent(dev, tx_ring->size,
					   &tx_ring->dma, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!tx_ring->desc)
		goto err;

	tx_ring->next_to_use = 0;
	tx_ring->next_to_clean = 0;

	return 0;

err:
	vfree(tx_ring->tx_buffer_info);
<<<<<<< HEAD
	dev_err(dev,
		"Unable to allocate memory for the transmit descriptor ring\n");
=======
	tx_ring->tx_buffer_info = NULL;
	dev_err(dev, "Unable to allocate memory for the Tx descriptor ring\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -ENOMEM;
}

/**
<<<<<<< HEAD
 * igb_setup_all_tx_resources - wrapper to allocate Tx resources
 *				  (Descriptors) for all queues
 * @adapter: board private structure
 *
 * Return 0 on success, negative on failure
=======
 *  igb_setup_all_tx_resources - wrapper to allocate Tx resources
 *				 (Descriptors) for all queues
 *  @adapter: board private structure
 *
 *  Return 0 on success, negative on failure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static int igb_setup_all_tx_resources(struct igb_adapter *adapter)
{
	struct pci_dev *pdev = adapter->pdev;
	int i, err = 0;

	for (i = 0; i < adapter->num_tx_queues; i++) {
		err = igb_setup_tx_resources(adapter->tx_ring[i]);
		if (err) {
			dev_err(&pdev->dev,
				"Allocation for Tx Queue %u failed\n", i);
			for (i--; i >= 0; i--)
				igb_free_tx_resources(adapter->tx_ring[i]);
			break;
		}
	}

	return err;
}

/**
<<<<<<< HEAD
 * igb_setup_tctl - configure the transmit control registers
 * @adapter: Board private structure
=======
 *  igb_setup_tctl - configure the transmit control registers
 *  @adapter: Board private structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
void igb_setup_tctl(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 tctl;

	/* disable queue 0 which is enabled by default on 82575 and 82576 */
	wr32(E1000_TXDCTL(0), 0);

	/* Program the Transmit Control Register */
	tctl = rd32(E1000_TCTL);
	tctl &= ~E1000_TCTL_CT;
	tctl |= E1000_TCTL_PSP | E1000_TCTL_RTLC |
		(E1000_COLLISION_THRESHOLD << E1000_CT_SHIFT);

	igb_config_collision_dist(hw);

	/* Enable transmits */
	tctl |= E1000_TCTL_EN;

	wr32(E1000_TCTL, tctl);
}

/**
<<<<<<< HEAD
 * igb_configure_tx_ring - Configure transmit ring after Reset
 * @adapter: board private structure
 * @ring: tx ring to configure
 *
 * Configure a transmit ring after a reset.
 **/
void igb_configure_tx_ring(struct igb_adapter *adapter,
                           struct igb_ring *ring)
=======
 *  igb_configure_tx_ring - Configure transmit ring after Reset
 *  @adapter: board private structure
 *  @ring: tx ring to configure
 *
 *  Configure a transmit ring after a reset.
 **/
void igb_configure_tx_ring(struct igb_adapter *adapter,
			   struct igb_ring *ring)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 txdctl = 0;
	u64 tdba = ring->dma;
	int reg_idx = ring->reg_idx;

<<<<<<< HEAD
	/* disable the queue */
	wr32(E1000_TXDCTL(reg_idx), 0);
	wrfl();
	mdelay(10);

	wr32(E1000_TDLEN(reg_idx),
	                ring->count * sizeof(union e1000_adv_tx_desc));
	wr32(E1000_TDBAL(reg_idx),
	                tdba & 0x00000000ffffffffULL);
	wr32(E1000_TDBAH(reg_idx), tdba >> 32);

	ring->tail = hw->hw_addr + E1000_TDT(reg_idx);
=======
	wr32(E1000_TDLEN(reg_idx),
	     ring->count * sizeof(union e1000_adv_tx_desc));
	wr32(E1000_TDBAL(reg_idx),
	     tdba & 0x00000000ffffffffULL);
	wr32(E1000_TDBAH(reg_idx), tdba >> 32);

	ring->tail = adapter->io_addr + E1000_TDT(reg_idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wr32(E1000_TDH(reg_idx), 0);
	writel(0, ring->tail);

	txdctl |= IGB_TX_PTHRESH;
	txdctl |= IGB_TX_HTHRESH << 8;
	txdctl |= IGB_TX_WTHRESH << 16;

<<<<<<< HEAD
=======
	/* reinitialize tx_buffer_info */
	memset(ring->tx_buffer_info, 0,
	       sizeof(struct igb_tx_buffer) * ring->count);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	txdctl |= E1000_TXDCTL_QUEUE_ENABLE;
	wr32(E1000_TXDCTL(reg_idx), txdctl);
}

/**
<<<<<<< HEAD
 * igb_configure_tx - Configure transmit Unit after Reset
 * @adapter: board private structure
 *
 * Configure the Tx unit of the MAC after a reset.
 **/
static void igb_configure_tx(struct igb_adapter *adapter)
{
	int i;

=======
 *  igb_configure_tx - Configure transmit Unit after Reset
 *  @adapter: board private structure
 *
 *  Configure the Tx unit of the MAC after a reset.
 **/
static void igb_configure_tx(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	int i;

	/* disable the queues */
	for (i = 0; i < adapter->num_tx_queues; i++)
		wr32(E1000_TXDCTL(adapter->tx_ring[i]->reg_idx), 0);

	wrfl();
	usleep_range(10000, 20000);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < adapter->num_tx_queues; i++)
		igb_configure_tx_ring(adapter, adapter->tx_ring[i]);
}

/**
<<<<<<< HEAD
 * igb_setup_rx_resources - allocate Rx resources (Descriptors)
 * @rx_ring:    rx descriptor ring (for a specific queue) to setup
 *
 * Returns 0 on success, negative on failure
 **/
int igb_setup_rx_resources(struct igb_ring *rx_ring)
{
	struct device *dev = rx_ring->dev;
	int orig_node = dev_to_node(dev);
	int size, desc_len;

	size = sizeof(struct igb_rx_buffer) * rx_ring->count;
	rx_ring->rx_buffer_info = vzalloc_node(size, rx_ring->numa_node);
	if (!rx_ring->rx_buffer_info)
		rx_ring->rx_buffer_info = vzalloc(size);
	if (!rx_ring->rx_buffer_info)
		goto err;

	desc_len = sizeof(union e1000_adv_rx_desc);

	/* Round up to nearest 4K */
	rx_ring->size = rx_ring->count * desc_len;
	rx_ring->size = ALIGN(rx_ring->size, 4096);

	set_dev_node(dev, rx_ring->numa_node);
	rx_ring->desc = dma_alloc_coherent(dev,
					   rx_ring->size,
					   &rx_ring->dma,
					   GFP_KERNEL);
	set_dev_node(dev, orig_node);
	if (!rx_ring->desc)
		rx_ring->desc = dma_alloc_coherent(dev,
						   rx_ring->size,
						   &rx_ring->dma,
						   GFP_KERNEL);

	if (!rx_ring->desc)
		goto err;

	rx_ring->next_to_clean = 0;
	rx_ring->next_to_use = 0;

	return 0;

err:
	vfree(rx_ring->rx_buffer_info);
	rx_ring->rx_buffer_info = NULL;
	dev_err(dev, "Unable to allocate memory for the receive descriptor"
		" ring\n");
=======
 *  igb_setup_rx_resources - allocate Rx resources (Descriptors)
 *  @rx_ring: Rx descriptor ring (for a specific queue) to setup
 *
 *  Returns 0 on success, negative on failure
 **/
int igb_setup_rx_resources(struct igb_ring *rx_ring)
{
	struct igb_adapter *adapter = netdev_priv(rx_ring->netdev);
	struct device *dev = rx_ring->dev;
	int size, res;

	/* XDP RX-queue info */
	if (xdp_rxq_info_is_reg(&rx_ring->xdp_rxq))
		xdp_rxq_info_unreg(&rx_ring->xdp_rxq);
	res = xdp_rxq_info_reg(&rx_ring->xdp_rxq, rx_ring->netdev,
			       rx_ring->queue_index, 0);
	if (res < 0) {
		dev_err(dev, "Failed to register xdp_rxq index %u\n",
			rx_ring->queue_index);
		return res;
	}

	size = sizeof(struct igb_rx_buffer) * rx_ring->count;

	rx_ring->rx_buffer_info = vmalloc(size);
	if (!rx_ring->rx_buffer_info)
		goto err;

	/* Round up to nearest 4K */
	rx_ring->size = rx_ring->count * sizeof(union e1000_adv_rx_desc);
	rx_ring->size = ALIGN(rx_ring->size, 4096);

	rx_ring->desc = dma_alloc_coherent(dev, rx_ring->size,
					   &rx_ring->dma, GFP_KERNEL);
	if (!rx_ring->desc)
		goto err;

	rx_ring->next_to_alloc = 0;
	rx_ring->next_to_clean = 0;
	rx_ring->next_to_use = 0;

	rx_ring->xdp_prog = adapter->xdp_prog;

	return 0;

err:
	xdp_rxq_info_unreg(&rx_ring->xdp_rxq);
	vfree(rx_ring->rx_buffer_info);
	rx_ring->rx_buffer_info = NULL;
	dev_err(dev, "Unable to allocate memory for the Rx descriptor ring\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -ENOMEM;
}

/**
<<<<<<< HEAD
 * igb_setup_all_rx_resources - wrapper to allocate Rx resources
 *				  (Descriptors) for all queues
 * @adapter: board private structure
 *
 * Return 0 on success, negative on failure
=======
 *  igb_setup_all_rx_resources - wrapper to allocate Rx resources
 *				 (Descriptors) for all queues
 *  @adapter: board private structure
 *
 *  Return 0 on success, negative on failure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static int igb_setup_all_rx_resources(struct igb_adapter *adapter)
{
	struct pci_dev *pdev = adapter->pdev;
	int i, err = 0;

	for (i = 0; i < adapter->num_rx_queues; i++) {
		err = igb_setup_rx_resources(adapter->rx_ring[i]);
		if (err) {
			dev_err(&pdev->dev,
				"Allocation for Rx Queue %u failed\n", i);
			for (i--; i >= 0; i--)
				igb_free_rx_resources(adapter->rx_ring[i]);
			break;
		}
	}

	return err;
}

/**
<<<<<<< HEAD
 * igb_setup_mrqc - configure the multiple receive queue control registers
 * @adapter: Board private structure
=======
 *  igb_setup_mrqc - configure the multiple receive queue control registers
 *  @adapter: Board private structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_setup_mrqc(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 mrqc, rxcsum;
<<<<<<< HEAD
	u32 j, num_rx_queues, shift = 0, shift2 = 0;
	union e1000_reta {
		u32 dword;
		u8  bytes[4];
	} reta;
	static const u8 rsshash[40] = {
		0x6d, 0x5a, 0x56, 0xda, 0x25, 0x5b, 0x0e, 0xc2, 0x41, 0x67,
		0x25, 0x3d, 0x43, 0xa3, 0x8f, 0xb0, 0xd0, 0xca, 0x2b, 0xcb,
		0xae, 0x7b, 0x30, 0xb4,	0x77, 0xcb, 0x2d, 0xa3, 0x80, 0x30,
		0xf2, 0x0c, 0x6a, 0x42, 0xb7, 0x3b, 0xbe, 0xac, 0x01, 0xfa };

	/* Fill out hash function seeds */
	for (j = 0; j < 10; j++) {
		u32 rsskey = rsshash[(j * 4)];
		rsskey |= rsshash[(j * 4) + 1] << 8;
		rsskey |= rsshash[(j * 4) + 2] << 16;
		rsskey |= rsshash[(j * 4) + 3] << 24;
		array_wr32(E1000_RSSRK(0), j, rsskey);
	}

	num_rx_queues = adapter->rss_queues;

	if (adapter->vfs_allocated_count) {
		/* 82575 and 82576 supports 2 RSS queues for VMDq */
		switch (hw->mac.type) {
		case e1000_i350:
		case e1000_82580:
			num_rx_queues = 1;
			shift = 0;
			break;
		case e1000_82576:
			shift = 3;
			num_rx_queues = 2;
			break;
		case e1000_82575:
			shift = 2;
			shift2 = 6;
		default:
			break;
		}
	} else {
		if (hw->mac.type == e1000_82575)
			shift = 6;
	}

	for (j = 0; j < (32 * 4); j++) {
		reta.bytes[j & 3] = (j % num_rx_queues) << shift;
		if (shift2)
			reta.bytes[j & 3] |= num_rx_queues << shift2;
		if ((j & 3) == 3)
			wr32(E1000_RETA(j >> 2), reta.dword);
	}

	/*
	 * Disable raw packet checksumming so that RSS hash is placed in
=======
	u32 j, num_rx_queues;
	u32 rss_key[10];

	netdev_rss_key_fill(rss_key, sizeof(rss_key));
	for (j = 0; j < 10; j++)
		wr32(E1000_RSSRK(j), rss_key[j]);

	num_rx_queues = adapter->rss_queues;

	switch (hw->mac.type) {
	case e1000_82576:
		/* 82576 supports 2 RSS queues for SR-IOV */
		if (adapter->vfs_allocated_count)
			num_rx_queues = 2;
		break;
	default:
		break;
	}

	if (adapter->rss_indir_tbl_init != num_rx_queues) {
		for (j = 0; j < IGB_RETA_SIZE; j++)
			adapter->rss_indir_tbl[j] =
			(j * num_rx_queues) / IGB_RETA_SIZE;
		adapter->rss_indir_tbl_init = num_rx_queues;
	}
	igb_write_rss_indir_tbl(adapter);

	/* Disable raw packet checksumming so that RSS hash is placed in
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * descriptor on writeback.  No need to enable TCP/UDP/IP checksum
	 * offloads as they are enabled by default
	 */
	rxcsum = rd32(E1000_RXCSUM);
	rxcsum |= E1000_RXCSUM_PCSD;

	if (adapter->hw.mac.type >= e1000_82576)
		/* Enable Receive Checksum Offload for SCTP */
		rxcsum |= E1000_RXCSUM_CRCOFL;

	/* Don't need to set TUOFL or IPOFL, they default to 1 */
	wr32(E1000_RXCSUM, rxcsum);

<<<<<<< HEAD
	/* If VMDq is enabled then we set the appropriate mode for that, else
	 * we default to RSS so that an RSS hash is calculated per packet even
	 * if we are only using one queue */
=======
	/* Generate RSS hash based on packet types, TCP/UDP
	 * port numbers and/or IPv4/v6 src and dst addresses
	 */
	mrqc = E1000_MRQC_RSS_FIELD_IPV4 |
	       E1000_MRQC_RSS_FIELD_IPV4_TCP |
	       E1000_MRQC_RSS_FIELD_IPV6 |
	       E1000_MRQC_RSS_FIELD_IPV6_TCP |
	       E1000_MRQC_RSS_FIELD_IPV6_TCP_EX;

	if (adapter->flags & IGB_FLAG_RSS_FIELD_IPV4_UDP)
		mrqc |= E1000_MRQC_RSS_FIELD_IPV4_UDP;
	if (adapter->flags & IGB_FLAG_RSS_FIELD_IPV6_UDP)
		mrqc |= E1000_MRQC_RSS_FIELD_IPV6_UDP;

	/* If VMDq is enabled then we set the appropriate mode for that, else
	 * we default to RSS so that an RSS hash is calculated per packet even
	 * if we are only using one queue
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (adapter->vfs_allocated_count) {
		if (hw->mac.type > e1000_82575) {
			/* Set the default pool for the PF's first queue */
			u32 vtctl = rd32(E1000_VT_CTL);
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			vtctl &= ~(E1000_VT_CTL_DEFAULT_POOL_MASK |
				   E1000_VT_CTL_DISABLE_DEF_POOL);
			vtctl |= adapter->vfs_allocated_count <<
				E1000_VT_CTL_DEFAULT_POOL_SHIFT;
			wr32(E1000_VT_CTL, vtctl);
		}
		if (adapter->rss_queues > 1)
<<<<<<< HEAD
			mrqc = E1000_MRQC_ENABLE_VMDQ_RSS_2Q;
		else
			mrqc = E1000_MRQC_ENABLE_VMDQ;
	} else {
		mrqc = E1000_MRQC_ENABLE_RSS_4Q;
	}
	igb_vmm_control(adapter);

	/*
	 * Generate RSS hash based on TCP port numbers and/or
	 * IPv4/v6 src and dst addresses since UDP cannot be
	 * hashed reliably due to IP fragmentation
	 */
	mrqc |= E1000_MRQC_RSS_FIELD_IPV4 |
		E1000_MRQC_RSS_FIELD_IPV4_TCP |
		E1000_MRQC_RSS_FIELD_IPV6 |
		E1000_MRQC_RSS_FIELD_IPV6_TCP |
		E1000_MRQC_RSS_FIELD_IPV6_TCP_EX;

=======
			mrqc |= E1000_MRQC_ENABLE_VMDQ_RSS_MQ;
		else
			mrqc |= E1000_MRQC_ENABLE_VMDQ;
	} else {
		mrqc |= E1000_MRQC_ENABLE_RSS_MQ;
	}
	igb_vmm_control(adapter);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wr32(E1000_MRQC, mrqc);
}

/**
<<<<<<< HEAD
 * igb_setup_rctl - configure the receive control registers
 * @adapter: Board private structure
=======
 *  igb_setup_rctl - configure the receive control registers
 *  @adapter: Board private structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
void igb_setup_rctl(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 rctl;

	rctl = rd32(E1000_RCTL);

	rctl &= ~(3 << E1000_RCTL_MO_SHIFT);
	rctl &= ~(E1000_RCTL_LBM_TCVR | E1000_RCTL_LBM_MAC);

	rctl |= E1000_RCTL_EN | E1000_RCTL_BAM | E1000_RCTL_RDMTS_HALF |
		(hw->mac.mc_filter_type << E1000_RCTL_MO_SHIFT);

<<<<<<< HEAD
	/*
	 * enable stripping of CRC. It's unlikely this will break BMC
=======
	/* enable stripping of CRC. It's unlikely this will break BMC
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * redirection as it did with e1000. Newer features require
	 * that the HW strips the CRC.
	 */
	rctl |= E1000_RCTL_SECRC;

	/* disable store bad packets and clear size bits. */
	rctl &= ~(E1000_RCTL_SBP | E1000_RCTL_SZ_256);

<<<<<<< HEAD
	/* enable LPE to prevent packets larger than max_frame_size */
=======
	/* enable LPE to allow for reception of jumbo frames */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rctl |= E1000_RCTL_LPE;

	/* disable queue 0 to prevent tail write w/o re-config */
	wr32(E1000_RXDCTL(0), 0);

	/* Attention!!!  For SR-IOV PF driver operations you must enable
	 * queue drop for all VF and PF queues to prevent head of line blocking
	 * if an un-trusted VF does not provide descriptors to hardware.
	 */
	if (adapter->vfs_allocated_count) {
		/* set all queue drop enable bits */
		wr32(E1000_QDE, ALL_QUEUES);
	}

	/* This is useful for sniffing bad packets. */
	if (adapter->netdev->features & NETIF_F_RXALL) {
		/* UPE and MPE will be handled by normal PROMISC logic
<<<<<<< HEAD
		 * in e1000e_set_rx_mode */
=======
		 * in e1000e_set_rx_mode
		 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rctl |= (E1000_RCTL_SBP | /* Receive bad packets */
			 E1000_RCTL_BAM | /* RX All Bcast Pkts */
			 E1000_RCTL_PMCF); /* RX All MAC Ctrl Pkts */

<<<<<<< HEAD
		rctl &= ~(E1000_RCTL_VFE | /* Disable VLAN filter */
			  E1000_RCTL_DPF | /* Allow filtered pause */
=======
		rctl &= ~(E1000_RCTL_DPF | /* Allow filtered pause */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  E1000_RCTL_CFIEN); /* Dis VLAN CFIEN Filter */
		/* Do not mess with E1000_CTRL_VME, it affects transmit as well,
		 * and that breaks VLANs.
		 */
	}

	wr32(E1000_RCTL, rctl);
}

static inline int igb_set_vf_rlpml(struct igb_adapter *adapter, int size,
<<<<<<< HEAD
                                   int vfn)
=======
				   int vfn)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 vmolr;

<<<<<<< HEAD
	/* if it isn't the PF check to see if VFs are enabled and
	 * increase the size to support vlan tags */
	if (vfn < adapter->vfs_allocated_count &&
	    adapter->vf_data[vfn].vlans_enabled)
		size += VLAN_TAG_SIZE;
=======
	if (size > MAX_JUMBO_FRAME_SIZE)
		size = MAX_JUMBO_FRAME_SIZE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	vmolr = rd32(E1000_VMOLR(vfn));
	vmolr &= ~E1000_VMOLR_RLPML_MASK;
	vmolr |= size | E1000_VMOLR_LPE;
	wr32(E1000_VMOLR(vfn), vmolr);

	return 0;
}

<<<<<<< HEAD
/**
 * igb_rlpml_set - set maximum receive packet size
 * @adapter: board private structure
 *
 * Configure maximum receivable packet size.
 **/
static void igb_rlpml_set(struct igb_adapter *adapter)
{
	u32 max_frame_size = adapter->max_frame_size;
	struct e1000_hw *hw = &adapter->hw;
	u16 pf_id = adapter->vfs_allocated_count;

	if (pf_id) {
		igb_set_vf_rlpml(adapter, max_frame_size, pf_id);
		/*
		 * If we're in VMDQ or SR-IOV mode, then set global RLPML
		 * to our max jumbo frame size, in case we need to enable
		 * jumbo frames on one of the rings later.
		 * This will not pass over-length frames into the default
		 * queue because it's gated by the VMOLR.RLPML.
		 */
		max_frame_size = MAX_JUMBO_FRAME_SIZE;
	}

	wr32(E1000_RLPML, max_frame_size);
=======
static inline void igb_set_vf_vlan_strip(struct igb_adapter *adapter,
					 int vfn, bool enable)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 val, reg;

	if (hw->mac.type < e1000_82576)
		return;

	if (hw->mac.type == e1000_i350)
		reg = E1000_DVMOLR(vfn);
	else
		reg = E1000_VMOLR(vfn);

	val = rd32(reg);
	if (enable)
		val |= E1000_VMOLR_STRVLAN;
	else
		val &= ~(E1000_VMOLR_STRVLAN);
	wr32(reg, val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void igb_set_vmolr(struct igb_adapter *adapter,
				 int vfn, bool aupe)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 vmolr;

<<<<<<< HEAD
	/*
	 * This register exists only on 82576 and newer so if we are older then
=======
	/* This register exists only on 82576 and newer so if we are older then
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * we should exit and do nothing
	 */
	if (hw->mac.type < e1000_82576)
		return;

	vmolr = rd32(E1000_VMOLR(vfn));
<<<<<<< HEAD
	vmolr |= E1000_VMOLR_STRVLAN;      /* Strip vlan tags */
	if (aupe)
		vmolr |= E1000_VMOLR_AUPE;        /* Accept untagged packets */
=======
	if (aupe)
		vmolr |= E1000_VMOLR_AUPE; /* Accept untagged packets */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		vmolr &= ~(E1000_VMOLR_AUPE); /* Tagged packets ONLY */

	/* clear all bits that might not be set */
	vmolr &= ~(E1000_VMOLR_BAM | E1000_VMOLR_RSSE);

	if (adapter->rss_queues > 1 && vfn == adapter->vfs_allocated_count)
		vmolr |= E1000_VMOLR_RSSE; /* enable RSS */
<<<<<<< HEAD
	/*
	 * for VMDq only allow the VFs and pool 0 to accept broadcast and
	 * multicast packets
	 */
	if (vfn <= adapter->vfs_allocated_count)
		vmolr |= E1000_VMOLR_BAM;	   /* Accept broadcast */
=======
	/* for VMDq only allow the VFs and pool 0 to accept broadcast and
	 * multicast packets
	 */
	if (vfn <= adapter->vfs_allocated_count)
		vmolr |= E1000_VMOLR_BAM; /* Accept broadcast */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	wr32(E1000_VMOLR(vfn), vmolr);
}

/**
<<<<<<< HEAD
 * igb_configure_rx_ring - Configure a receive ring after Reset
 * @adapter: board private structure
 * @ring: receive ring to be configured
 *
 * Configure the Rx unit of the MAC after a reset.
 **/
void igb_configure_rx_ring(struct igb_adapter *adapter,
                           struct igb_ring *ring)
{
	struct e1000_hw *hw = &adapter->hw;
	u64 rdba = ring->dma;
	int reg_idx = ring->reg_idx;
	u32 srrctl = 0, rxdctl = 0;
=======
 *  igb_setup_srrctl - configure the split and replication receive control
 *                     registers
 *  @adapter: Board private structure
 *  @ring: receive ring to be configured
 **/
void igb_setup_srrctl(struct igb_adapter *adapter, struct igb_ring *ring)
{
	struct e1000_hw *hw = &adapter->hw;
	int reg_idx = ring->reg_idx;
	u32 srrctl = 0;

	srrctl = IGB_RX_HDR_LEN << E1000_SRRCTL_BSIZEHDRSIZE_SHIFT;
	if (ring_uses_large_buffer(ring))
		srrctl |= IGB_RXBUFFER_3072 >> E1000_SRRCTL_BSIZEPKT_SHIFT;
	else
		srrctl |= IGB_RXBUFFER_2048 >> E1000_SRRCTL_BSIZEPKT_SHIFT;
	srrctl |= E1000_SRRCTL_DESCTYPE_ADV_ONEBUF;
	if (hw->mac.type >= e1000_82580)
		srrctl |= E1000_SRRCTL_TIMESTAMP;
	/* Only set Drop Enable if VFs allocated, or we are supporting multiple
	 * queues and rx flow control is disabled
	 */
	if (adapter->vfs_allocated_count ||
	    (!(hw->fc.current_mode & e1000_fc_rx_pause) &&
	     adapter->num_rx_queues > 1))
		srrctl |= E1000_SRRCTL_DROP_EN;

	wr32(E1000_SRRCTL(reg_idx), srrctl);
}

/**
 *  igb_configure_rx_ring - Configure a receive ring after Reset
 *  @adapter: board private structure
 *  @ring: receive ring to be configured
 *
 *  Configure the Rx unit of the MAC after a reset.
 **/
void igb_configure_rx_ring(struct igb_adapter *adapter,
			   struct igb_ring *ring)
{
	struct e1000_hw *hw = &adapter->hw;
	union e1000_adv_rx_desc *rx_desc;
	u64 rdba = ring->dma;
	int reg_idx = ring->reg_idx;
	u32 rxdctl = 0;

	xdp_rxq_info_unreg_mem_model(&ring->xdp_rxq);
	WARN_ON(xdp_rxq_info_reg_mem_model(&ring->xdp_rxq,
					   MEM_TYPE_PAGE_SHARED, NULL));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* disable the queue */
	wr32(E1000_RXDCTL(reg_idx), 0);

	/* Set DMA base address registers */
	wr32(E1000_RDBAL(reg_idx),
	     rdba & 0x00000000ffffffffULL);
	wr32(E1000_RDBAH(reg_idx), rdba >> 32);
	wr32(E1000_RDLEN(reg_idx),
<<<<<<< HEAD
	               ring->count * sizeof(union e1000_adv_rx_desc));

	/* initialize head and tail */
	ring->tail = hw->hw_addr + E1000_RDT(reg_idx);
=======
	     ring->count * sizeof(union e1000_adv_rx_desc));

	/* initialize head and tail */
	ring->tail = adapter->io_addr + E1000_RDT(reg_idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wr32(E1000_RDH(reg_idx), 0);
	writel(0, ring->tail);

	/* set descriptor configuration */
<<<<<<< HEAD
	srrctl = IGB_RX_HDR_LEN << E1000_SRRCTL_BSIZEHDRSIZE_SHIFT;
#if (PAGE_SIZE / 2) > IGB_RXBUFFER_16384
	srrctl |= IGB_RXBUFFER_16384 >> E1000_SRRCTL_BSIZEPKT_SHIFT;
#else
	srrctl |= (PAGE_SIZE / 2) >> E1000_SRRCTL_BSIZEPKT_SHIFT;
#endif
	srrctl |= E1000_SRRCTL_DESCTYPE_HDR_SPLIT_ALWAYS;
	if (hw->mac.type >= e1000_82580)
		srrctl |= E1000_SRRCTL_TIMESTAMP;
	/* Only set Drop Enable if we are supporting multiple queues */
	if (adapter->vfs_allocated_count || adapter->num_rx_queues > 1)
		srrctl |= E1000_SRRCTL_DROP_EN;

	wr32(E1000_SRRCTL(reg_idx), srrctl);
=======
	igb_setup_srrctl(adapter, ring);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* set filtering for VMDQ pools */
	igb_set_vmolr(adapter, reg_idx & 0x7, true);

	rxdctl |= IGB_RX_PTHRESH;
	rxdctl |= IGB_RX_HTHRESH << 8;
	rxdctl |= IGB_RX_WTHRESH << 16;

<<<<<<< HEAD
=======
	/* initialize rx_buffer_info */
	memset(ring->rx_buffer_info, 0,
	       sizeof(struct igb_rx_buffer) * ring->count);

	/* initialize Rx descriptor 0 */
	rx_desc = IGB_RX_DESC(ring, 0);
	rx_desc->wb.upper.length = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* enable receive descriptor fetching */
	rxdctl |= E1000_RXDCTL_QUEUE_ENABLE;
	wr32(E1000_RXDCTL(reg_idx), rxdctl);
}

<<<<<<< HEAD
/**
 * igb_configure_rx - Configure receive Unit after Reset
 * @adapter: board private structure
 *
 * Configure the Rx unit of the MAC after a reset.
=======
static void igb_set_rx_buffer_len(struct igb_adapter *adapter,
				  struct igb_ring *rx_ring)
{
#if (PAGE_SIZE < 8192)
	struct e1000_hw *hw = &adapter->hw;
#endif

	/* set build_skb and buffer size flags */
	clear_ring_build_skb_enabled(rx_ring);
	clear_ring_uses_large_buffer(rx_ring);

	if (adapter->flags & IGB_FLAG_RX_LEGACY)
		return;

	set_ring_build_skb_enabled(rx_ring);

#if (PAGE_SIZE < 8192)
	if (adapter->max_frame_size > IGB_MAX_FRAME_BUILD_SKB ||
	    rd32(E1000_RCTL) & E1000_RCTL_SBP)
		set_ring_uses_large_buffer(rx_ring);
#endif
}

/**
 *  igb_configure_rx - Configure receive Unit after Reset
 *  @adapter: board private structure
 *
 *  Configure the Rx unit of the MAC after a reset.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_configure_rx(struct igb_adapter *adapter)
{
	int i;

<<<<<<< HEAD
	/* set UTA to appropriate mode */
	igb_set_uta(adapter);

	/* set the correct pool for the PF default MAC address in entry 0 */
	igb_rar_set_qsel(adapter, adapter->hw.mac.addr, 0,
	                 adapter->vfs_allocated_count);

	/* Setup the HW Rx Head and Tail Descriptor Pointers and
	 * the Base and Length of the Rx Descriptor Ring */
	for (i = 0; i < adapter->num_rx_queues; i++)
		igb_configure_rx_ring(adapter, adapter->rx_ring[i]);
}

/**
 * igb_free_tx_resources - Free Tx Resources per Queue
 * @tx_ring: Tx descriptor ring for a specific queue
 *
 * Free all transmit software resources
=======
	/* set the correct pool for the PF default MAC address in entry 0 */
	igb_set_default_mac_filter(adapter);

	/* Setup the HW Rx Head and Tail Descriptor Pointers and
	 * the Base and Length of the Rx Descriptor Ring
	 */
	for (i = 0; i < adapter->num_rx_queues; i++) {
		struct igb_ring *rx_ring = adapter->rx_ring[i];

		igb_set_rx_buffer_len(adapter, rx_ring);
		igb_configure_rx_ring(adapter, rx_ring);
	}
}

/**
 *  igb_free_tx_resources - Free Tx Resources per Queue
 *  @tx_ring: Tx descriptor ring for a specific queue
 *
 *  Free all transmit software resources
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
void igb_free_tx_resources(struct igb_ring *tx_ring)
{
	igb_clean_tx_ring(tx_ring);

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
<<<<<<< HEAD
 * igb_free_all_tx_resources - Free Tx Resources for All Queues
 * @adapter: board private structure
 *
 * Free all transmit software resources
=======
 *  igb_free_all_tx_resources - Free Tx Resources for All Queues
 *  @adapter: board private structure
 *
 *  Free all transmit software resources
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_free_all_tx_resources(struct igb_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_tx_queues; i++)
<<<<<<< HEAD
		igb_free_tx_resources(adapter->tx_ring[i]);
}

void igb_unmap_and_free_tx_resource(struct igb_ring *ring,
				    struct igb_tx_buffer *tx_buffer)
{
	if (tx_buffer->skb) {
		dev_kfree_skb_any(tx_buffer->skb);
		if (tx_buffer->dma)
			dma_unmap_single(ring->dev,
					 tx_buffer->dma,
					 tx_buffer->length,
					 DMA_TO_DEVICE);
	} else if (tx_buffer->dma) {
		dma_unmap_page(ring->dev,
			       tx_buffer->dma,
			       tx_buffer->length,
			       DMA_TO_DEVICE);
	}
	tx_buffer->next_to_watch = NULL;
	tx_buffer->skb = NULL;
	tx_buffer->dma = 0;
	/* buffer_info must be completely set up in the transmit path */
}

/**
 * igb_clean_tx_ring - Free Tx Buffers
 * @tx_ring: ring to be cleaned
 **/
static void igb_clean_tx_ring(struct igb_ring *tx_ring)
{
	struct igb_tx_buffer *buffer_info;
	unsigned long size;
	u16 i;

	if (!tx_ring->tx_buffer_info)
		return;
	/* Free all the Tx ring sk_buffs */

	for (i = 0; i < tx_ring->count; i++) {
		buffer_info = &tx_ring->tx_buffer_info[i];
		igb_unmap_and_free_tx_resource(tx_ring, buffer_info);
	}

	netdev_tx_reset_queue(txring_txq(tx_ring));

	size = sizeof(struct igb_tx_buffer) * tx_ring->count;
	memset(tx_ring->tx_buffer_info, 0, size);

	/* Zero out the descriptor ring */
	memset(tx_ring->desc, 0, tx_ring->size);

=======
		if (adapter->tx_ring[i])
			igb_free_tx_resources(adapter->tx_ring[i]);
}

/**
 *  igb_clean_tx_ring - Free Tx Buffers
 *  @tx_ring: ring to be cleaned
 **/
static void igb_clean_tx_ring(struct igb_ring *tx_ring)
{
	u16 i = tx_ring->next_to_clean;
	struct igb_tx_buffer *tx_buffer = &tx_ring->tx_buffer_info[i];

	while (i != tx_ring->next_to_use) {
		union e1000_adv_tx_desc *eop_desc, *tx_desc;

		/* Free all the Tx ring sk_buffs or xdp frames */
		if (tx_buffer->type == IGB_TYPE_SKB)
			dev_kfree_skb_any(tx_buffer->skb);
		else
			xdp_return_frame(tx_buffer->xdpf);

		/* unmap skb header data */
		dma_unmap_single(tx_ring->dev,
				 dma_unmap_addr(tx_buffer, dma),
				 dma_unmap_len(tx_buffer, len),
				 DMA_TO_DEVICE);

		/* check for eop_desc to determine the end of the packet */
		eop_desc = tx_buffer->next_to_watch;
		tx_desc = IGB_TX_DESC(tx_ring, i);

		/* unmap remaining buffers */
		while (tx_desc != eop_desc) {
			tx_buffer++;
			tx_desc++;
			i++;
			if (unlikely(i == tx_ring->count)) {
				i = 0;
				tx_buffer = tx_ring->tx_buffer_info;
				tx_desc = IGB_TX_DESC(tx_ring, 0);
			}

			/* unmap any remaining paged data */
			if (dma_unmap_len(tx_buffer, len))
				dma_unmap_page(tx_ring->dev,
					       dma_unmap_addr(tx_buffer, dma),
					       dma_unmap_len(tx_buffer, len),
					       DMA_TO_DEVICE);
		}

		tx_buffer->next_to_watch = NULL;

		/* move us one more past the eop_desc for start of next pkt */
		tx_buffer++;
		i++;
		if (unlikely(i == tx_ring->count)) {
			i = 0;
			tx_buffer = tx_ring->tx_buffer_info;
		}
	}

	/* reset BQL for queue */
	netdev_tx_reset_queue(txring_txq(tx_ring));

	/* reset next_to_use and next_to_clean */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tx_ring->next_to_use = 0;
	tx_ring->next_to_clean = 0;
}

/**
<<<<<<< HEAD
 * igb_clean_all_tx_rings - Free Tx Buffers for all queues
 * @adapter: board private structure
=======
 *  igb_clean_all_tx_rings - Free Tx Buffers for all queues
 *  @adapter: board private structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_clean_all_tx_rings(struct igb_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_tx_queues; i++)
<<<<<<< HEAD
		igb_clean_tx_ring(adapter->tx_ring[i]);
}

/**
 * igb_free_rx_resources - Free Rx Resources
 * @rx_ring: ring to clean the resources from
 *
 * Free all receive software resources
=======
		if (adapter->tx_ring[i])
			igb_clean_tx_ring(adapter->tx_ring[i]);
}

/**
 *  igb_free_rx_resources - Free Rx Resources
 *  @rx_ring: ring to clean the resources from
 *
 *  Free all receive software resources
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
void igb_free_rx_resources(struct igb_ring *rx_ring)
{
	igb_clean_rx_ring(rx_ring);

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
<<<<<<< HEAD
 * igb_free_all_rx_resources - Free Rx Resources for All Queues
 * @adapter: board private structure
 *
 * Free all receive software resources
=======
 *  igb_free_all_rx_resources - Free Rx Resources for All Queues
 *  @adapter: board private structure
 *
 *  Free all receive software resources
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_free_all_rx_resources(struct igb_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_rx_queues; i++)
<<<<<<< HEAD
		igb_free_rx_resources(adapter->rx_ring[i]);
}

/**
 * igb_clean_rx_ring - Free Rx Buffers per Queue
 * @rx_ring: ring to free buffers from
 **/
static void igb_clean_rx_ring(struct igb_ring *rx_ring)
{
	unsigned long size;
	u16 i;

	if (!rx_ring->rx_buffer_info)
		return;

	/* Free all the Rx ring sk_buffs */
	for (i = 0; i < rx_ring->count; i++) {
		struct igb_rx_buffer *buffer_info = &rx_ring->rx_buffer_info[i];
		if (buffer_info->dma) {
			dma_unmap_single(rx_ring->dev,
			                 buffer_info->dma,
					 IGB_RX_HDR_LEN,
					 DMA_FROM_DEVICE);
			buffer_info->dma = 0;
		}

		if (buffer_info->skb) {
			dev_kfree_skb(buffer_info->skb);
			buffer_info->skb = NULL;
		}
		if (buffer_info->page_dma) {
			dma_unmap_page(rx_ring->dev,
			               buffer_info->page_dma,
				       PAGE_SIZE / 2,
				       DMA_FROM_DEVICE);
			buffer_info->page_dma = 0;
		}
		if (buffer_info->page) {
			put_page(buffer_info->page);
			buffer_info->page = NULL;
			buffer_info->page_offset = 0;
		}
	}

	size = sizeof(struct igb_rx_buffer) * rx_ring->count;
	memset(rx_ring->rx_buffer_info, 0, size);

	/* Zero out the descriptor ring */
	memset(rx_ring->desc, 0, rx_ring->size);

=======
		if (adapter->rx_ring[i])
			igb_free_rx_resources(adapter->rx_ring[i]);
}

/**
 *  igb_clean_rx_ring - Free Rx Buffers per Queue
 *  @rx_ring: ring to free buffers from
 **/
static void igb_clean_rx_ring(struct igb_ring *rx_ring)
{
	u16 i = rx_ring->next_to_clean;

	dev_kfree_skb(rx_ring->skb);
	rx_ring->skb = NULL;

	/* Free all the Rx ring sk_buffs */
	while (i != rx_ring->next_to_alloc) {
		struct igb_rx_buffer *buffer_info = &rx_ring->rx_buffer_info[i];

		/* Invalidate cache lines that may have been written to by
		 * device so that we avoid corrupting memory.
		 */
		dma_sync_single_range_for_cpu(rx_ring->dev,
					      buffer_info->dma,
					      buffer_info->page_offset,
					      igb_rx_bufsz(rx_ring),
					      DMA_FROM_DEVICE);

		/* free resources associated with mapping */
		dma_unmap_page_attrs(rx_ring->dev,
				     buffer_info->dma,
				     igb_rx_pg_size(rx_ring),
				     DMA_FROM_DEVICE,
				     IGB_RX_DMA_ATTR);
		__page_frag_cache_drain(buffer_info->page,
					buffer_info->pagecnt_bias);

		i++;
		if (i == rx_ring->count)
			i = 0;
	}

	rx_ring->next_to_alloc = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rx_ring->next_to_clean = 0;
	rx_ring->next_to_use = 0;
}

/**
<<<<<<< HEAD
 * igb_clean_all_rx_rings - Free Rx Buffers for all queues
 * @adapter: board private structure
=======
 *  igb_clean_all_rx_rings - Free Rx Buffers for all queues
 *  @adapter: board private structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_clean_all_rx_rings(struct igb_adapter *adapter)
{
	int i;

	for (i = 0; i < adapter->num_rx_queues; i++)
<<<<<<< HEAD
		igb_clean_rx_ring(adapter->rx_ring[i]);
}

/**
 * igb_set_mac - Change the Ethernet Address of the NIC
 * @netdev: network interface device structure
 * @p: pointer to an address structure
 *
 * Returns 0 on success, negative on failure
=======
		if (adapter->rx_ring[i])
			igb_clean_rx_ring(adapter->rx_ring[i]);
}

/**
 *  igb_set_mac - Change the Ethernet Address of the NIC
 *  @netdev: network interface device structure
 *  @p: pointer to an address structure
 *
 *  Returns 0 on success, negative on failure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static int igb_set_mac(struct net_device *netdev, void *p)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	struct sockaddr *addr = p;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

<<<<<<< HEAD
	memcpy(netdev->dev_addr, addr->sa_data, netdev->addr_len);
	memcpy(hw->mac.addr, addr->sa_data, netdev->addr_len);

	/* set the correct pool for the new PF MAC address in entry 0 */
	igb_rar_set_qsel(adapter, hw->mac.addr, 0,
	                 adapter->vfs_allocated_count);
=======
	eth_hw_addr_set(netdev, addr->sa_data);
	memcpy(hw->mac.addr, addr->sa_data, netdev->addr_len);

	/* set the correct pool for the new PF MAC address in entry 0 */
	igb_set_default_mac_filter(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/**
<<<<<<< HEAD
 * igb_write_mc_addr_list - write multicast addresses to MTA
 * @netdev: network interface device structure
 *
 * Writes multicast address list to the MTA hash table.
 * Returns: -ENOMEM on failure
 *                0 on no addresses written
 *                X on writing X addresses to MTA
=======
 *  igb_write_mc_addr_list - write multicast addresses to MTA
 *  @netdev: network interface device structure
 *
 *  Writes multicast address list to the MTA hash table.
 *  Returns: -ENOMEM on failure
 *           0 on no addresses written
 *           X on writing X addresses to MTA
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static int igb_write_mc_addr_list(struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	struct netdev_hw_addr *ha;
	u8  *mta_list;
	int i;

	if (netdev_mc_empty(netdev)) {
		/* nothing to program, so clear mc list */
		igb_update_mc_addr_list(hw, NULL, 0);
		igb_restore_vf_multicasts(adapter);
		return 0;
	}

<<<<<<< HEAD
	mta_list = kzalloc(netdev_mc_count(netdev) * 6, GFP_ATOMIC);
=======
	mta_list = kcalloc(netdev_mc_count(netdev), 6, GFP_ATOMIC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!mta_list)
		return -ENOMEM;

	/* The shared function expects a packed array of only addresses. */
	i = 0;
	netdev_for_each_mc_addr(ha, netdev)
		memcpy(mta_list + (i++ * ETH_ALEN), ha->addr, ETH_ALEN);

	igb_update_mc_addr_list(hw, mta_list, i);
	kfree(mta_list);

	return netdev_mc_count(netdev);
}

<<<<<<< HEAD
/**
 * igb_write_uc_addr_list - write unicast addresses to RAR table
 * @netdev: network interface device structure
 *
 * Writes unicast address list to the RAR table.
 * Returns: -ENOMEM on failure/insufficient address space
 *                0 on no addresses written
 *                X on writing X addresses to the RAR table
 **/
static int igb_write_uc_addr_list(struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	unsigned int vfn = adapter->vfs_allocated_count;
	unsigned int rar_entries = hw->mac.rar_entry_count - (vfn + 1);
	int count = 0;

	/* return ENOMEM indicating insufficient memory for addresses */
	if (netdev_uc_count(netdev) > rar_entries)
		return -ENOMEM;

	if (!netdev_uc_empty(netdev) && rar_entries) {
		struct netdev_hw_addr *ha;

		netdev_for_each_uc_addr(ha, netdev) {
			if (!rar_entries)
				break;
			igb_rar_set_qsel(adapter, ha->addr,
			                 rar_entries--,
			                 vfn);
			count++;
		}
	}
	/* write the addresses in reverse order to avoid write combining */
	for (; rar_entries > 0 ; rar_entries--) {
		wr32(E1000_RAH(rar_entries), 0);
		wr32(E1000_RAL(rar_entries), 0);
	}
	wrfl();

	return count;
}

/**
 * igb_set_rx_mode - Secondary Unicast, Multicast and Promiscuous mode set
 * @netdev: network interface device structure
 *
 * The set_rx_mode entry point is called whenever the unicast or multicast
 * address lists or the network interface flags are updated.  This routine is
 * responsible for configuring the hardware for proper unicast, multicast,
 * promiscuous mode, and all-multi behavior.
=======
static int igb_vlan_promisc_enable(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 i, pf_id;

	switch (hw->mac.type) {
	case e1000_i210:
	case e1000_i211:
	case e1000_i350:
		/* VLAN filtering needed for VLAN prio filter */
		if (adapter->netdev->features & NETIF_F_NTUPLE)
			break;
		fallthrough;
	case e1000_82576:
	case e1000_82580:
	case e1000_i354:
		/* VLAN filtering needed for pool filtering */
		if (adapter->vfs_allocated_count)
			break;
		fallthrough;
	default:
		return 1;
	}

	/* We are already in VLAN promisc, nothing to do */
	if (adapter->flags & IGB_FLAG_VLAN_PROMISC)
		return 0;

	if (!adapter->vfs_allocated_count)
		goto set_vfta;

	/* Add PF to all active pools */
	pf_id = adapter->vfs_allocated_count + E1000_VLVF_POOLSEL_SHIFT;

	for (i = E1000_VLVF_ARRAY_SIZE; --i;) {
		u32 vlvf = rd32(E1000_VLVF(i));

		vlvf |= BIT(pf_id);
		wr32(E1000_VLVF(i), vlvf);
	}

set_vfta:
	/* Set all bits in the VLAN filter table array */
	for (i = E1000_VLAN_FILTER_TBL_SIZE; i--;)
		hw->mac.ops.write_vfta(hw, i, ~0U);

	/* Set flag so we don't redo unnecessary work */
	adapter->flags |= IGB_FLAG_VLAN_PROMISC;

	return 0;
}

#define VFTA_BLOCK_SIZE 8
static void igb_scrub_vfta(struct igb_adapter *adapter, u32 vfta_offset)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 vfta[VFTA_BLOCK_SIZE] = { 0 };
	u32 vid_start = vfta_offset * 32;
	u32 vid_end = vid_start + (VFTA_BLOCK_SIZE * 32);
	u32 i, vid, word, bits, pf_id;

	/* guarantee that we don't scrub out management VLAN */
	vid = adapter->mng_vlan_id;
	if (vid >= vid_start && vid < vid_end)
		vfta[(vid - vid_start) / 32] |= BIT(vid % 32);

	if (!adapter->vfs_allocated_count)
		goto set_vfta;

	pf_id = adapter->vfs_allocated_count + E1000_VLVF_POOLSEL_SHIFT;

	for (i = E1000_VLVF_ARRAY_SIZE; --i;) {
		u32 vlvf = rd32(E1000_VLVF(i));

		/* pull VLAN ID from VLVF */
		vid = vlvf & VLAN_VID_MASK;

		/* only concern ourselves with a certain range */
		if (vid < vid_start || vid >= vid_end)
			continue;

		if (vlvf & E1000_VLVF_VLANID_ENABLE) {
			/* record VLAN ID in VFTA */
			vfta[(vid - vid_start) / 32] |= BIT(vid % 32);

			/* if PF is part of this then continue */
			if (test_bit(vid, adapter->active_vlans))
				continue;
		}

		/* remove PF from the pool */
		bits = ~BIT(pf_id);
		bits &= rd32(E1000_VLVF(i));
		wr32(E1000_VLVF(i), bits);
	}

set_vfta:
	/* extract values from active_vlans and write back to VFTA */
	for (i = VFTA_BLOCK_SIZE; i--;) {
		vid = (vfta_offset + i) * 32;
		word = vid / BITS_PER_LONG;
		bits = vid % BITS_PER_LONG;

		vfta[i] |= adapter->active_vlans[word] >> bits;

		hw->mac.ops.write_vfta(hw, vfta_offset + i, vfta[i]);
	}
}

static void igb_vlan_promisc_disable(struct igb_adapter *adapter)
{
	u32 i;

	/* We are not in VLAN promisc, nothing to do */
	if (!(adapter->flags & IGB_FLAG_VLAN_PROMISC))
		return;

	/* Set flag so we don't redo unnecessary work */
	adapter->flags &= ~IGB_FLAG_VLAN_PROMISC;

	for (i = 0; i < E1000_VLAN_FILTER_TBL_SIZE; i += VFTA_BLOCK_SIZE)
		igb_scrub_vfta(adapter, i);
}

/**
 *  igb_set_rx_mode - Secondary Unicast, Multicast and Promiscuous mode set
 *  @netdev: network interface device structure
 *
 *  The set_rx_mode entry point is called whenever the unicast or multicast
 *  address lists or the network interface flags are updated.  This routine is
 *  responsible for configuring the hardware for proper unicast, multicast,
 *  promiscuous mode, and all-multi behavior.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_set_rx_mode(struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	unsigned int vfn = adapter->vfs_allocated_count;
<<<<<<< HEAD
	u32 rctl, vmolr = 0;
	int count;

	/* Check for Promiscuous and All Multicast modes */
	rctl = rd32(E1000_RCTL);

	/* clear the effected bits */
	rctl &= ~(E1000_RCTL_UPE | E1000_RCTL_MPE | E1000_RCTL_VFE);

	if (netdev->flags & IFF_PROMISC) {
		rctl |= (E1000_RCTL_UPE | E1000_RCTL_MPE);
		vmolr |= (E1000_VMOLR_ROPE | E1000_VMOLR_MPME);
=======
	u32 rctl = 0, vmolr = 0, rlpml = MAX_JUMBO_FRAME_SIZE;
	int count;

	/* Check for Promiscuous and All Multicast modes */
	if (netdev->flags & IFF_PROMISC) {
		rctl |= E1000_RCTL_UPE | E1000_RCTL_MPE;
		vmolr |= E1000_VMOLR_MPME;

		/* enable use of UTA filter to force packets to default pool */
		if (hw->mac.type == e1000_82576)
			vmolr |= E1000_VMOLR_ROPE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		if (netdev->flags & IFF_ALLMULTI) {
			rctl |= E1000_RCTL_MPE;
			vmolr |= E1000_VMOLR_MPME;
		} else {
<<<<<<< HEAD
			/*
			 * Write addresses to the MTA, if the attempt fails
=======
			/* Write addresses to the MTA, if the attempt fails
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 * then we should just turn on promiscuous mode so
			 * that we can at least receive multicast traffic
			 */
			count = igb_write_mc_addr_list(netdev);
			if (count < 0) {
				rctl |= E1000_RCTL_MPE;
				vmolr |= E1000_VMOLR_MPME;
			} else if (count) {
				vmolr |= E1000_VMOLR_ROMPE;
			}
		}
<<<<<<< HEAD
		/*
		 * Write addresses to available RAR registers, if there is not
		 * sufficient space to store all the addresses then enable
		 * unicast promiscuous mode
		 */
		count = igb_write_uc_addr_list(netdev);
		if (count < 0) {
			rctl |= E1000_RCTL_UPE;
			vmolr |= E1000_VMOLR_ROPE;
		}
		rctl |= E1000_RCTL_VFE;
	}
	wr32(E1000_RCTL, rctl);

	/*
	 * In order to support SR-IOV and eventually VMDq it is necessary to set
=======
	}

	/* Write addresses to available RAR registers, if there is not
	 * sufficient space to store all the addresses then enable
	 * unicast promiscuous mode
	 */
	if (__dev_uc_sync(netdev, igb_uc_sync, igb_uc_unsync)) {
		rctl |= E1000_RCTL_UPE;
		vmolr |= E1000_VMOLR_ROPE;
	}

	/* enable VLAN filtering by default */
	rctl |= E1000_RCTL_VFE;

	/* disable VLAN filtering for modes that require it */
	if ((netdev->flags & IFF_PROMISC) ||
	    (netdev->features & NETIF_F_RXALL)) {
		/* if we fail to set all rules then just clear VFE */
		if (igb_vlan_promisc_enable(adapter))
			rctl &= ~E1000_RCTL_VFE;
	} else {
		igb_vlan_promisc_disable(adapter);
	}

	/* update state of unicast, multicast, and VLAN filtering modes */
	rctl |= rd32(E1000_RCTL) & ~(E1000_RCTL_UPE | E1000_RCTL_MPE |
				     E1000_RCTL_VFE);
	wr32(E1000_RCTL, rctl);

#if (PAGE_SIZE < 8192)
	if (!adapter->vfs_allocated_count) {
		if (adapter->max_frame_size <= IGB_MAX_FRAME_BUILD_SKB)
			rlpml = IGB_MAX_FRAME_BUILD_SKB;
	}
#endif
	wr32(E1000_RLPML, rlpml);

	/* In order to support SR-IOV and eventually VMDq it is necessary to set
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * the VMOLR to enable the appropriate modes.  Without this workaround
	 * we will have issues with VLAN tag stripping not being done for frames
	 * that are only arriving because we are the default pool
	 */
<<<<<<< HEAD
	if (hw->mac.type < e1000_82576)
		return;

	vmolr |= rd32(E1000_VMOLR(vfn)) &
	         ~(E1000_VMOLR_ROPE | E1000_VMOLR_MPME | E1000_VMOLR_ROMPE);
	wr32(E1000_VMOLR(vfn), vmolr);
=======
	if ((hw->mac.type < e1000_82576) || (hw->mac.type > e1000_i350))
		return;

	/* set UTA to appropriate mode */
	igb_set_uta(adapter, !!(vmolr & E1000_VMOLR_ROPE));

	vmolr |= rd32(E1000_VMOLR(vfn)) &
		 ~(E1000_VMOLR_ROPE | E1000_VMOLR_MPME | E1000_VMOLR_ROMPE);

	/* enable Rx jumbo frames, restrict as needed to support build_skb */
	vmolr &= ~E1000_VMOLR_RLPML_MASK;
#if (PAGE_SIZE < 8192)
	if (adapter->max_frame_size <= IGB_MAX_FRAME_BUILD_SKB)
		vmolr |= IGB_MAX_FRAME_BUILD_SKB;
	else
#endif
		vmolr |= MAX_JUMBO_FRAME_SIZE;
	vmolr |= E1000_VMOLR_LPE;

	wr32(E1000_VMOLR(vfn), vmolr);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	igb_restore_vf_multicasts(adapter);
}

static void igb_check_wvbr(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 wvbr = 0;

	switch (hw->mac.type) {
	case e1000_82576:
	case e1000_i350:
<<<<<<< HEAD
		if (!(wvbr = rd32(E1000_WVBR)))
=======
		wvbr = rd32(E1000_WVBR);
		if (!wvbr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return;
		break;
	default:
		break;
	}

	adapter->wvbr |= wvbr;
}

#define IGB_STAGGERED_QUEUE_OFFSET 8

static void igb_spoof_check(struct igb_adapter *adapter)
{
	int j;

	if (!adapter->wvbr)
		return;

<<<<<<< HEAD
	for(j = 0; j < adapter->vfs_allocated_count; j++) {
		if (adapter->wvbr & (1 << j) ||
		    adapter->wvbr & (1 << (j + IGB_STAGGERED_QUEUE_OFFSET))) {
			dev_warn(&adapter->pdev->dev,
				"Spoof event(s) detected on VF %d\n", j);
			adapter->wvbr &=
				~((1 << j) |
				  (1 << (j + IGB_STAGGERED_QUEUE_OFFSET)));
=======
	for (j = 0; j < adapter->vfs_allocated_count; j++) {
		if (adapter->wvbr & BIT(j) ||
		    adapter->wvbr & BIT(j + IGB_STAGGERED_QUEUE_OFFSET)) {
			dev_warn(&adapter->pdev->dev,
				"Spoof event(s) detected on VF %d\n", j);
			adapter->wvbr &=
				~(BIT(j) |
				  BIT(j + IGB_STAGGERED_QUEUE_OFFSET));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

/* Need to wait a few seconds after link up to get diagnostic information from
<<<<<<< HEAD
 * the phy */
static void igb_update_phy_info(unsigned long data)
{
	struct igb_adapter *adapter = (struct igb_adapter *) data;
=======
 * the phy
 */
static void igb_update_phy_info(struct timer_list *t)
{
	struct igb_adapter *adapter = from_timer(adapter, t, phy_info_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	igb_get_phy_info(&adapter->hw);
}

/**
<<<<<<< HEAD
 * igb_has_link - check shared code for link and determine up/down
 * @adapter: pointer to driver private info
=======
 *  igb_has_link - check shared code for link and determine up/down
 *  @adapter: pointer to driver private info
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
bool igb_has_link(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	bool link_active = false;
<<<<<<< HEAD
	s32 ret_val = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* get_link_status is set on LSC (link status) interrupt or
	 * rx sequence error interrupt.  get_link_status will stay
	 * false until the e1000_check_for_link establishes link
	 * for copper adapters ONLY
	 */
	switch (hw->phy.media_type) {
	case e1000_media_type_copper:
<<<<<<< HEAD
		if (hw->mac.get_link_status) {
			ret_val = hw->mac.ops.check_for_link(hw);
			link_active = !hw->mac.get_link_status;
		} else {
			link_active = true;
		}
		break;
	case e1000_media_type_internal_serdes:
		ret_val = hw->mac.ops.check_for_link(hw);
		link_active = hw->mac.serdes_has_link;
=======
		if (!hw->mac.get_link_status)
			return true;
		fallthrough;
	case e1000_media_type_internal_serdes:
		hw->mac.ops.check_for_link(hw);
		link_active = !hw->mac.get_link_status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
	case e1000_media_type_unknown:
		break;
	}

<<<<<<< HEAD
=======
	if (((hw->mac.type == e1000_i210) ||
	     (hw->mac.type == e1000_i211)) &&
	     (hw->phy.id == I210_I_PHY_ID)) {
		if (!netif_carrier_ok(adapter->netdev)) {
			adapter->flags &= ~IGB_FLAG_NEED_LINK_UPDATE;
		} else if (!(adapter->flags & IGB_FLAG_NEED_LINK_UPDATE)) {
			adapter->flags |= IGB_FLAG_NEED_LINK_UPDATE;
			adapter->link_check_timeout = jiffies;
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return link_active;
}

static bool igb_thermal_sensor_event(struct e1000_hw *hw, u32 event)
{
	bool ret = false;
	u32 ctrl_ext, thstat;

<<<<<<< HEAD
	/* check for thermal sensor event on i350, copper only */
=======
	/* check for thermal sensor event on i350 copper only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (hw->mac.type == e1000_i350) {
		thstat = rd32(E1000_THSTAT);
		ctrl_ext = rd32(E1000_CTRL_EXT);

		if ((hw->phy.media_type == e1000_media_type_copper) &&
<<<<<<< HEAD
		    !(ctrl_ext & E1000_CTRL_EXT_LINK_MODE_SGMII)) {
			ret = !!(thstat & event);
		}
=======
		    !(ctrl_ext & E1000_CTRL_EXT_LINK_MODE_SGMII))
			ret = !!(thstat & event);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return ret;
}

/**
<<<<<<< HEAD
 * igb_watchdog - Timer Call-back
 * @data: pointer to adapter cast into an unsigned long
 **/
static void igb_watchdog(unsigned long data)
{
	struct igb_adapter *adapter = (struct igb_adapter *)data;
=======
 *  igb_check_lvmmc - check for malformed packets received
 *  and indicated in LVMMC register
 *  @adapter: pointer to adapter
 **/
static void igb_check_lvmmc(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 lvmmc;

	lvmmc = rd32(E1000_LVMMC);
	if (lvmmc) {
		if (unlikely(net_ratelimit())) {
			netdev_warn(adapter->netdev,
				    "malformed Tx packet detected and dropped, LVMMC:0x%08x\n",
				    lvmmc);
		}
	}
}

/**
 *  igb_watchdog - Timer Call-back
 *  @t: pointer to timer_list containing our private info pointer
 **/
static void igb_watchdog(struct timer_list *t)
{
	struct igb_adapter *adapter = from_timer(adapter, t, watchdog_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Do the rest outside of interrupt context */
	schedule_work(&adapter->watchdog_task);
}

static void igb_watchdog_task(struct work_struct *work)
{
	struct igb_adapter *adapter = container_of(work,
<<<<<<< HEAD
	                                           struct igb_adapter,
                                                   watchdog_task);
	struct e1000_hw *hw = &adapter->hw;
	struct net_device *netdev = adapter->netdev;
	u32 link;
	int i;

	link = igb_has_link(adapter);
	if (link) {
=======
						   struct igb_adapter,
						   watchdog_task);
	struct e1000_hw *hw = &adapter->hw;
	struct e1000_phy_info *phy = &hw->phy;
	struct net_device *netdev = adapter->netdev;
	u32 link;
	int i;
	u32 connsw;
	u16 phy_data, retry_count = 20;

	link = igb_has_link(adapter);

	if (adapter->flags & IGB_FLAG_NEED_LINK_UPDATE) {
		if (time_after(jiffies, (adapter->link_check_timeout + HZ)))
			adapter->flags &= ~IGB_FLAG_NEED_LINK_UPDATE;
		else
			link = false;
	}

	/* Force link down if we have fiber to swap to */
	if (adapter->flags & IGB_FLAG_MAS_ENABLE) {
		if (hw->phy.media_type == e1000_media_type_copper) {
			connsw = rd32(E1000_CONNSW);
			if (!(connsw & E1000_CONNSW_AUTOSENSE_EN))
				link = 0;
		}
	}
	if (link) {
		/* Perform a reset if the media type changed. */
		if (hw->dev_spec._82575.media_changed) {
			hw->dev_spec._82575.media_changed = false;
			adapter->flags |= IGB_FLAG_MEDIA_RESET;
			igb_reset(adapter);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Cancel scheduled suspend requests. */
		pm_runtime_resume(netdev->dev.parent);

		if (!netif_carrier_ok(netdev)) {
			u32 ctrl;
<<<<<<< HEAD
			hw->mac.ops.get_speed_and_duplex(hw,
			                                 &adapter->link_speed,
			                                 &adapter->link_duplex);

			ctrl = rd32(E1000_CTRL);
			/* Links status message must follow this format */
			printk(KERN_INFO "igb: %s NIC Link is Up %d Mbps %s "
			       "Duplex, Flow Control: %s\n",
=======

			hw->mac.ops.get_speed_and_duplex(hw,
							 &adapter->link_speed,
							 &adapter->link_duplex);

			ctrl = rd32(E1000_CTRL);
			/* Links status message must follow this format */
			netdev_info(netdev,
			       "igb: %s NIC Link is Up %d Mbps %s Duplex, Flow Control: %s\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       netdev->name,
			       adapter->link_speed,
			       adapter->link_duplex == FULL_DUPLEX ?
			       "Full" : "Half",
			       (ctrl & E1000_CTRL_TFCE) &&
			       (ctrl & E1000_CTRL_RFCE) ? "RX/TX" :
			       (ctrl & E1000_CTRL_RFCE) ?  "RX" :
			       (ctrl & E1000_CTRL_TFCE) ?  "TX" : "None");

<<<<<<< HEAD
			/* check for thermal sensor event */
			if (igb_thermal_sensor_event(hw,
			    E1000_THSTAT_LINK_THROTTLE)) {
				netdev_info(netdev, "The network adapter link "
					    "speed was downshifted because it "
					    "overheated\n");
			}
=======
			/* disable EEE if enabled */
			if ((adapter->flags & IGB_FLAG_EEE) &&
				(adapter->link_duplex == HALF_DUPLEX)) {
				dev_info(&adapter->pdev->dev,
				"EEE Disabled: unsupported at half duplex. Re-enable using ethtool when at full duplex.\n");
				adapter->hw.dev_spec._82575.eee_disable = true;
				adapter->flags &= ~IGB_FLAG_EEE;
			}

			/* check if SmartSpeed worked */
			igb_check_downshift(hw);
			if (phy->speed_downgraded)
				netdev_warn(netdev, "Link Speed was downgraded by SmartSpeed\n");

			/* check for thermal sensor event */
			if (igb_thermal_sensor_event(hw,
			    E1000_THSTAT_LINK_THROTTLE))
				netdev_info(netdev, "The network adapter link speed was downshifted because it overheated\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			/* adjust timeout factor according to speed/duplex */
			adapter->tx_timeout_factor = 1;
			switch (adapter->link_speed) {
			case SPEED_10:
				adapter->tx_timeout_factor = 14;
				break;
			case SPEED_100:
				/* maybe add some timeout factor ? */
				break;
			}

<<<<<<< HEAD
=======
			if (adapter->link_speed != SPEED_1000 ||
			    !hw->phy.ops.read_reg)
				goto no_wait;

			/* wait for Remote receiver status OK */
retry_read_status:
			if (!igb_read_phy_reg(hw, PHY_1000T_STATUS,
					      &phy_data)) {
				if (!(phy_data & SR_1000T_REMOTE_RX_STATUS) &&
				    retry_count) {
					msleep(100);
					retry_count--;
					goto retry_read_status;
				} else if (!retry_count) {
					dev_err(&adapter->pdev->dev, "exceed max 2 second\n");
				}
			} else {
				dev_err(&adapter->pdev->dev, "read 1000Base-T Status Reg\n");
			}
no_wait:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			netif_carrier_on(netdev);

			igb_ping_all_vfs(adapter);
			igb_check_vf_rate_limit(adapter);

			/* link state has changed, schedule phy info update */
			if (!test_bit(__IGB_DOWN, &adapter->state))
				mod_timer(&adapter->phy_info_timer,
					  round_jiffies(jiffies + 2 * HZ));
		}
	} else {
		if (netif_carrier_ok(netdev)) {
			adapter->link_speed = 0;
			adapter->link_duplex = 0;

			/* check for thermal sensor event */
			if (igb_thermal_sensor_event(hw,
			    E1000_THSTAT_PWR_DOWN)) {
<<<<<<< HEAD
				netdev_err(netdev, "The network adapter was "
					   "stopped because it overheated\n");
			}

			/* Links status message must follow this format */
			printk(KERN_INFO "igb: %s NIC Link is Down\n",
=======
				netdev_err(netdev, "The network adapter was stopped because it overheated\n");
			}

			/* Links status message must follow this format */
			netdev_info(netdev, "igb: %s NIC Link is Down\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       netdev->name);
			netif_carrier_off(netdev);

			igb_ping_all_vfs(adapter);

			/* link state has changed, schedule phy info update */
			if (!test_bit(__IGB_DOWN, &adapter->state))
				mod_timer(&adapter->phy_info_timer,
					  round_jiffies(jiffies + 2 * HZ));

<<<<<<< HEAD
			pm_schedule_suspend(netdev->dev.parent,
					    MSEC_PER_SEC * 5);
=======
			/* link is down, time to check for alternate media */
			if (adapter->flags & IGB_FLAG_MAS_ENABLE) {
				igb_check_swap_media(adapter);
				if (adapter->flags & IGB_FLAG_MEDIA_RESET) {
					schedule_work(&adapter->reset_task);
					/* return immediately */
					return;
				}
			}
			pm_schedule_suspend(netdev->dev.parent,
					    MSEC_PER_SEC * 5);

		/* also check for alternate media here */
		} else if (!netif_carrier_ok(netdev) &&
			   (adapter->flags & IGB_FLAG_MAS_ENABLE)) {
			igb_check_swap_media(adapter);
			if (adapter->flags & IGB_FLAG_MEDIA_RESET) {
				schedule_work(&adapter->reset_task);
				/* return immediately */
				return;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	spin_lock(&adapter->stats64_lock);
<<<<<<< HEAD
	igb_update_stats(adapter, &adapter->stats64);
=======
	igb_update_stats(adapter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&adapter->stats64_lock);

	for (i = 0; i < adapter->num_tx_queues; i++) {
		struct igb_ring *tx_ring = adapter->tx_ring[i];
		if (!netif_carrier_ok(netdev)) {
			/* We've lost link, so the controller stops DMA,
			 * but we've got queued Tx work that's never going
			 * to get done, so reset controller to flush Tx.
<<<<<<< HEAD
			 * (Do the reset outside of interrupt context). */
=======
			 * (Do the reset outside of interrupt context).
			 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (igb_desc_unused(tx_ring) + 1 < tx_ring->count) {
				adapter->tx_timeout_count++;
				schedule_work(&adapter->reset_task);
				/* return immediately since reset is imminent */
				return;
			}
		}

		/* Force detection of hung controller every watchdog period */
		set_bit(IGB_RING_FLAG_TX_DETECT_HANG, &tx_ring->flags);
	}

<<<<<<< HEAD
	/* Cause software interrupt to ensure rx ring is cleaned */
	if (adapter->msix_entries) {
		u32 eics = 0;
=======
	/* Cause software interrupt to ensure Rx ring is cleaned */
	if (adapter->flags & IGB_FLAG_HAS_MSIX) {
		u32 eics = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		for (i = 0; i < adapter->num_q_vectors; i++)
			eics |= adapter->q_vector[i]->eims_value;
		wr32(E1000_EICS, eics);
	} else {
		wr32(E1000_ICS, E1000_ICS_RXDMT0);
	}

	igb_spoof_check(adapter);
<<<<<<< HEAD

	/* Reset the timer */
	if (!test_bit(__IGB_DOWN, &adapter->state))
		mod_timer(&adapter->watchdog_timer,
			  round_jiffies(jiffies + 2 * HZ));
=======
	igb_ptp_rx_hang(adapter);
	igb_ptp_tx_hang(adapter);

	/* Check LVMMC register on i350/i354 only */
	if ((adapter->hw.mac.type == e1000_i350) ||
	    (adapter->hw.mac.type == e1000_i354))
		igb_check_lvmmc(adapter);

	/* Reset the timer */
	if (!test_bit(__IGB_DOWN, &adapter->state)) {
		if (adapter->flags & IGB_FLAG_NEED_LINK_UPDATE)
			mod_timer(&adapter->watchdog_timer,
				  round_jiffies(jiffies +  HZ));
		else
			mod_timer(&adapter->watchdog_timer,
				  round_jiffies(jiffies + 2 * HZ));
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

enum latency_range {
	lowest_latency = 0,
	low_latency = 1,
	bulk_latency = 2,
	latency_invalid = 255
};

/**
<<<<<<< HEAD
 * igb_update_ring_itr - update the dynamic ITR value based on packet size
 *
 *      Stores a new ITR value based on strictly on packet size.  This
 *      algorithm is less sophisticated than that used in igb_update_itr,
 *      due to the difficulty of synchronizing statistics across multiple
 *      receive rings.  The divisors and thresholds used by this function
 *      were determined based on theoretical maximum wire speed and testing
 *      data, in order to minimize response time while increasing bulk
 *      throughput.
 *      This functionality is controlled by the InterruptThrottleRate module
 *      parameter (see igb_param.c)
 *      NOTE:  This function is called only when operating in a multiqueue
 *             receive environment.
 * @q_vector: pointer to q_vector
=======
 *  igb_update_ring_itr - update the dynamic ITR value based on packet size
 *  @q_vector: pointer to q_vector
 *
 *  Stores a new ITR value based on strictly on packet size.  This
 *  algorithm is less sophisticated than that used in igb_update_itr,
 *  due to the difficulty of synchronizing statistics across multiple
 *  receive rings.  The divisors and thresholds used by this function
 *  were determined based on theoretical maximum wire speed and testing
 *  data, in order to minimize response time while increasing bulk
 *  throughput.
 *  This functionality is controlled by ethtool's coalescing settings.
 *  NOTE:  This function is called only when operating in a multiqueue
 *         receive environment.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_update_ring_itr(struct igb_q_vector *q_vector)
{
	int new_val = q_vector->itr_val;
	int avg_wire_size = 0;
	struct igb_adapter *adapter = q_vector->adapter;
	unsigned int packets;

	/* For non-gigabit speeds, just fix the interrupt rate at 4000
	 * ints/sec - ITR timer value of 120 ticks.
	 */
	if (adapter->link_speed != SPEED_1000) {
		new_val = IGB_4K_ITR;
		goto set_itr_val;
	}

	packets = q_vector->rx.total_packets;
	if (packets)
		avg_wire_size = q_vector->rx.total_bytes / packets;

	packets = q_vector->tx.total_packets;
	if (packets)
		avg_wire_size = max_t(u32, avg_wire_size,
				      q_vector->tx.total_bytes / packets);

	/* if avg_wire_size isn't set no work was done */
	if (!avg_wire_size)
		goto clear_counts;

	/* Add 24 bytes to size to account for CRC, preamble, and gap */
	avg_wire_size += 24;

	/* Don't starve jumbo frames */
	avg_wire_size = min(avg_wire_size, 3000);

	/* Give a little boost to mid-size frames */
	if ((avg_wire_size > 300) && (avg_wire_size < 1200))
		new_val = avg_wire_size / 3;
	else
		new_val = avg_wire_size / 2;

	/* conservative mode (itr 3) eliminates the lowest_latency setting */
	if (new_val < IGB_20K_ITR &&
	    ((q_vector->rx.ring && adapter->rx_itr_setting == 3) ||
	     (!q_vector->rx.ring && adapter->tx_itr_setting == 3)))
		new_val = IGB_20K_ITR;

set_itr_val:
	if (new_val != q_vector->itr_val) {
		q_vector->itr_val = new_val;
		q_vector->set_itr = 1;
	}
clear_counts:
	q_vector->rx.total_bytes = 0;
	q_vector->rx.total_packets = 0;
	q_vector->tx.total_bytes = 0;
	q_vector->tx.total_packets = 0;
}

/**
<<<<<<< HEAD
 * igb_update_itr - update the dynamic ITR value based on statistics
 *      Stores a new ITR value based on packets and byte
 *      counts during the last interrupt.  The advantage of per interrupt
 *      computation is faster updates and more accurate ITR for the current
 *      traffic pattern.  Constants in this function were computed
 *      based on theoretical maximum wire speed and thresholds were set based
 *      on testing data as well as attempting to minimize response time
 *      while increasing bulk throughput.
 *      this functionality is controlled by the InterruptThrottleRate module
 *      parameter (see igb_param.c)
 *      NOTE:  These calculations are only valid when operating in a single-
 *             queue environment.
 * @q_vector: pointer to q_vector
 * @ring_container: ring info to update the itr for
=======
 *  igb_update_itr - update the dynamic ITR value based on statistics
 *  @q_vector: pointer to q_vector
 *  @ring_container: ring info to update the itr for
 *
 *  Stores a new ITR value based on packets and byte
 *  counts during the last interrupt.  The advantage of per interrupt
 *  computation is faster updates and more accurate ITR for the current
 *  traffic pattern.  Constants in this function were computed
 *  based on theoretical maximum wire speed and thresholds were set based
 *  on testing data as well as attempting to minimize response time
 *  while increasing bulk throughput.
 *  This functionality is controlled by ethtool's coalescing settings.
 *  NOTE:  These calculations are only valid when operating in a single-
 *         queue environment.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static void igb_update_itr(struct igb_q_vector *q_vector,
			   struct igb_ring_container *ring_container)
{
	unsigned int packets = ring_container->total_packets;
	unsigned int bytes = ring_container->total_bytes;
	u8 itrval = ring_container->itr;

	/* no packets, exit with status unchanged */
	if (packets == 0)
		return;

	switch (itrval) {
	case lowest_latency:
		/* handle TSO and jumbo frames */
		if (bytes/packets > 8000)
			itrval = bulk_latency;
		else if ((packets < 5) && (bytes > 512))
			itrval = low_latency;
		break;
	case low_latency:  /* 50 usec aka 20000 ints/s */
		if (bytes > 10000) {
			/* this if handles the TSO accounting */
<<<<<<< HEAD
			if (bytes/packets > 8000) {
				itrval = bulk_latency;
			} else if ((packets < 10) || ((bytes/packets) > 1200)) {
				itrval = bulk_latency;
			} else if ((packets > 35)) {
				itrval = lowest_latency;
			}
=======
			if (bytes/packets > 8000)
				itrval = bulk_latency;
			else if ((packets < 10) || ((bytes/packets) > 1200))
				itrval = bulk_latency;
			else if ((packets > 35))
				itrval = lowest_latency;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else if (bytes/packets > 2000) {
			itrval = bulk_latency;
		} else if (packets <= 2 && bytes < 512) {
			itrval = lowest_latency;
		}
		break;
	case bulk_latency: /* 250 usec aka 4000 ints/s */
		if (bytes > 25000) {
			if (packets > 35)
				itrval = low_latency;
		} else if (bytes < 1500) {
			itrval = low_latency;
		}
		break;
	}

	/* clear work counters since we have the values we need */
	ring_container->total_bytes = 0;
	ring_container->total_packets = 0;

	/* write updated itr to ring container */
	ring_container->itr = itrval;
}

static void igb_set_itr(struct igb_q_vector *q_vector)
{
	struct igb_adapter *adapter = q_vector->adapter;
	u32 new_itr = q_vector->itr_val;
	u8 current_itr = 0;

	/* for non-gigabit speeds, just fix the interrupt rate at 4000 */
	if (adapter->link_speed != SPEED_1000) {
		current_itr = 0;
		new_itr = IGB_4K_ITR;
		goto set_itr_now;
	}

	igb_update_itr(q_vector, &q_vector->tx);
	igb_update_itr(q_vector, &q_vector->rx);

	current_itr = max(q_vector->rx.itr, q_vector->tx.itr);

	/* conservative mode (itr 3) eliminates the lowest_latency setting */
	if (current_itr == lowest_latency &&
	    ((q_vector->rx.ring && adapter->rx_itr_setting == 3) ||
	     (!q_vector->rx.ring && adapter->tx_itr_setting == 3)))
		current_itr = low_latency;

	switch (current_itr) {
	/* counts and packets in update_itr are dependent on these numbers */
	case lowest_latency:
		new_itr = IGB_70K_ITR; /* 70,000 ints/sec */
		break;
	case low_latency:
		new_itr = IGB_20K_ITR; /* 20,000 ints/sec */
		break;
	case bulk_latency:
		new_itr = IGB_4K_ITR;  /* 4,000 ints/sec */
		break;
	default:
		break;
	}

set_itr_now:
	if (new_itr != q_vector->itr_val) {
		/* this attempts to bias the interrupt rate towards Bulk
		 * by adding intermediate steps when interrupt rate is
<<<<<<< HEAD
		 * increasing */
		new_itr = new_itr > q_vector->itr_val ?
		             max((new_itr * q_vector->itr_val) /
		                 (new_itr + (q_vector->itr_val >> 2)),
				 new_itr) :
			     new_itr;
=======
		 * increasing
		 */
		new_itr = new_itr > q_vector->itr_val ?
			  max((new_itr * q_vector->itr_val) /
			  (new_itr + (q_vector->itr_val >> 2)),
			  new_itr) : new_itr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Don't write the value here; it resets the adapter's
		 * internal timer, and causes us to delay far longer than
		 * we should between interrupts.  Instead, we write the ITR
		 * value at the beginning of the next interrupt so the timing
		 * ends up being correct.
		 */
		q_vector->itr_val = new_itr;
		q_vector->set_itr = 1;
	}
}

<<<<<<< HEAD
static void igb_tx_ctxtdesc(struct igb_ring *tx_ring, u32 vlan_macip_lens,
			    u32 type_tucmd, u32 mss_l4len_idx)
{
	struct e1000_adv_tx_context_desc *context_desc;
	u16 i = tx_ring->next_to_use;
=======
static void igb_tx_ctxtdesc(struct igb_ring *tx_ring,
			    struct igb_tx_buffer *first,
			    u32 vlan_macip_lens, u32 type_tucmd,
			    u32 mss_l4len_idx)
{
	struct e1000_adv_tx_context_desc *context_desc;
	u16 i = tx_ring->next_to_use;
	struct timespec64 ts;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	context_desc = IGB_TX_CTXTDESC(tx_ring, i);

	i++;
	tx_ring->next_to_use = (i < tx_ring->count) ? i : 0;

	/* set bits to identify this as an advanced context descriptor */
	type_tucmd |= E1000_TXD_CMD_DEXT | E1000_ADVTXD_DTYP_CTXT;

	/* For 82575, context index must be unique per ring. */
	if (test_bit(IGB_RING_FLAG_TX_CTX_IDX, &tx_ring->flags))
		mss_l4len_idx |= tx_ring->reg_idx << 4;

	context_desc->vlan_macip_lens	= cpu_to_le32(vlan_macip_lens);
<<<<<<< HEAD
	context_desc->seqnum_seed	= 0;
	context_desc->type_tucmd_mlhl	= cpu_to_le32(type_tucmd);
	context_desc->mss_l4len_idx	= cpu_to_le32(mss_l4len_idx);
=======
	context_desc->type_tucmd_mlhl	= cpu_to_le32(type_tucmd);
	context_desc->mss_l4len_idx	= cpu_to_le32(mss_l4len_idx);

	/* We assume there is always a valid tx time available. Invalid times
	 * should have been handled by the upper layers.
	 */
	if (tx_ring->launchtime_enable) {
		ts = ktime_to_timespec64(first->skb->tstamp);
		skb_txtime_consumed(first->skb);
		context_desc->seqnum_seed = cpu_to_le32(ts.tv_nsec / 32);
	} else {
		context_desc->seqnum_seed = 0;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int igb_tso(struct igb_ring *tx_ring,
		   struct igb_tx_buffer *first,
		   u8 *hdr_len)
{
<<<<<<< HEAD
	struct sk_buff *skb = first->skb;
	u32 vlan_macip_lens, type_tucmd;
	u32 mss_l4len_idx, l4len;
=======
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
	type_tucmd = E1000_ADVTXD_TUCMD_L4T_TCP;

	if (first->protocol == __constant_htons(ETH_P_IP)) {
		struct iphdr *iph = ip_hdr(skb);
		iph->tot_len = 0;
		iph->check = 0;
		tcp_hdr(skb)->check = ~csum_tcpudp_magic(iph->saddr,
							 iph->daddr, 0,
							 IPPROTO_TCP,
							 0);
		type_tucmd |= E1000_ADVTXD_TUCMD_IPV4;
		first->tx_flags |= IGB_TX_FLAGS_TSO |
				   IGB_TX_FLAGS_CSUM |
				   IGB_TX_FLAGS_IPV4;
	} else if (skb_is_gso_v6(skb)) {
		ipv6_hdr(skb)->payload_len = 0;
		tcp_hdr(skb)->check = ~csum_ipv6_magic(&ipv6_hdr(skb)->saddr,
						       &ipv6_hdr(skb)->daddr,
						       0, IPPROTO_TCP, 0);
=======
	err = skb_cow_head(skb, 0);
	if (err < 0)
		return err;

	ip.hdr = skb_network_header(skb);
	l4.hdr = skb_checksum_start(skb);

	/* ADV DTYP TUCMD MKRLOC/ISCSIHEDLEN */
	type_tucmd = (skb_shinfo(skb)->gso_type & SKB_GSO_UDP_L4) ?
		      E1000_ADVTXD_TUCMD_L4T_UDP : E1000_ADVTXD_TUCMD_L4T_TCP;

	/* initialize outer IP header fields */
	if (ip.v4->version == 4) {
		unsigned char *csum_start = skb_checksum_start(skb);
		unsigned char *trans_start = ip.hdr + (ip.v4->ihl * 4);

		/* IP header will have to cancel out any data that
		 * is not a part of the outer IP header
		 */
		ip.v4->check = csum_fold(csum_partial(trans_start,
						      csum_start - trans_start,
						      0));
		type_tucmd |= E1000_ADVTXD_TUCMD_IPV4;

		ip.v4->tot_len = 0;
		first->tx_flags |= IGB_TX_FLAGS_TSO |
				   IGB_TX_FLAGS_CSUM |
				   IGB_TX_FLAGS_IPV4;
	} else {
		ip.v6->payload_len = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		first->tx_flags |= IGB_TX_FLAGS_TSO |
				   IGB_TX_FLAGS_CSUM;
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
	if (type_tucmd & E1000_ADVTXD_TUCMD_L4T_TCP) {
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

	/* MSS L4LEN IDX */
<<<<<<< HEAD
	mss_l4len_idx = l4len << E1000_ADVTXD_L4LEN_SHIFT;
	mss_l4len_idx |= skb_shinfo(skb)->gso_size << E1000_ADVTXD_MSS_SHIFT;

	/* VLAN MACLEN IPLEN */
	vlan_macip_lens = skb_network_header_len(skb);
	vlan_macip_lens |= skb_network_offset(skb) << E1000_ADVTXD_MACLEN_SHIFT;
	vlan_macip_lens |= first->tx_flags & IGB_TX_FLAGS_VLAN_MASK;

	igb_tx_ctxtdesc(tx_ring, vlan_macip_lens, type_tucmd, mss_l4len_idx);
=======
	mss_l4len_idx = (*hdr_len - l4_offset) << E1000_ADVTXD_L4LEN_SHIFT;
	mss_l4len_idx |= skb_shinfo(skb)->gso_size << E1000_ADVTXD_MSS_SHIFT;

	/* VLAN MACLEN IPLEN */
	vlan_macip_lens = l4.hdr - ip.hdr;
	vlan_macip_lens |= (ip.hdr - skb->data) << E1000_ADVTXD_MACLEN_SHIFT;
	vlan_macip_lens |= first->tx_flags & IGB_TX_FLAGS_VLAN_MASK;

	igb_tx_ctxtdesc(tx_ring, first, vlan_macip_lens,
			type_tucmd, mss_l4len_idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 1;
}

static void igb_tx_csum(struct igb_ring *tx_ring, struct igb_tx_buffer *first)
{
	struct sk_buff *skb = first->skb;
	u32 vlan_macip_lens = 0;
<<<<<<< HEAD
	u32 mss_l4len_idx = 0;
	u32 type_tucmd = 0;

	if (skb->ip_summed != CHECKSUM_PARTIAL) {
		if (!(first->tx_flags & IGB_TX_FLAGS_VLAN))
			return;
	} else {
		u8 l4_hdr = 0;
		switch (first->protocol) {
		case __constant_htons(ETH_P_IP):
			vlan_macip_lens |= skb_network_header_len(skb);
			type_tucmd |= E1000_ADVTXD_TUCMD_IPV4;
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
			type_tucmd |= E1000_ADVTXD_TUCMD_L4T_TCP;
			mss_l4len_idx = tcp_hdrlen(skb) <<
					E1000_ADVTXD_L4LEN_SHIFT;
			break;
		case IPPROTO_SCTP:
			type_tucmd |= E1000_ADVTXD_TUCMD_L4T_SCTP;
			mss_l4len_idx = sizeof(struct sctphdr) <<
					E1000_ADVTXD_L4LEN_SHIFT;
			break;
		case IPPROTO_UDP:
			mss_l4len_idx = sizeof(struct udphdr) <<
					E1000_ADVTXD_L4LEN_SHIFT;
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
		first->tx_flags |= IGB_TX_FLAGS_CSUM;
	}

	vlan_macip_lens |= skb_network_offset(skb) << E1000_ADVTXD_MACLEN_SHIFT;
	vlan_macip_lens |= first->tx_flags & IGB_TX_FLAGS_VLAN_MASK;

	igb_tx_ctxtdesc(tx_ring, vlan_macip_lens, type_tucmd, mss_l4len_idx);
}

static __le32 igb_tx_cmd_type(u32 tx_flags)
{
	/* set type for advanced descriptor with frame checksum insertion */
	__le32 cmd_type = cpu_to_le32(E1000_ADVTXD_DTYP_DATA |
				      E1000_ADVTXD_DCMD_IFCS |
				      E1000_ADVTXD_DCMD_DEXT);

	/* set HW vlan bit if vlan is present */
	if (tx_flags & IGB_TX_FLAGS_VLAN)
		cmd_type |= cpu_to_le32(E1000_ADVTXD_DCMD_VLE);

	/* set timestamp bit if present */
	if (tx_flags & IGB_TX_FLAGS_TSTAMP)
		cmd_type |= cpu_to_le32(E1000_ADVTXD_MAC_TSTAMP);

	/* set segmentation bits for TSO */
	if (tx_flags & IGB_TX_FLAGS_TSO)
		cmd_type |= cpu_to_le32(E1000_ADVTXD_DCMD_TSE);
=======
	u32 type_tucmd = 0;

	if (skb->ip_summed != CHECKSUM_PARTIAL) {
csum_failed:
		if (!(first->tx_flags & IGB_TX_FLAGS_VLAN) &&
		    !tx_ring->launchtime_enable)
			return;
		goto no_csum;
	}

	switch (skb->csum_offset) {
	case offsetof(struct tcphdr, check):
		type_tucmd = E1000_ADVTXD_TUCMD_L4T_TCP;
		fallthrough;
	case offsetof(struct udphdr, check):
		break;
	case offsetof(struct sctphdr, checksum):
		/* validate that this is actually an SCTP request */
		if (skb_csum_is_sctp(skb)) {
			type_tucmd = E1000_ADVTXD_TUCMD_L4T_SCTP;
			break;
		}
		fallthrough;
	default:
		skb_checksum_help(skb);
		goto csum_failed;
	}

	/* update TX checksum flag */
	first->tx_flags |= IGB_TX_FLAGS_CSUM;
	vlan_macip_lens = skb_checksum_start_offset(skb) -
			  skb_network_offset(skb);
no_csum:
	vlan_macip_lens |= skb_network_offset(skb) << E1000_ADVTXD_MACLEN_SHIFT;
	vlan_macip_lens |= first->tx_flags & IGB_TX_FLAGS_VLAN_MASK;

	igb_tx_ctxtdesc(tx_ring, first, vlan_macip_lens, type_tucmd, 0);
}

#define IGB_SET_FLAG(_input, _flag, _result) \
	((_flag <= _result) ? \
	 ((u32)(_input & _flag) * (_result / _flag)) : \
	 ((u32)(_input & _flag) / (_flag / _result)))

static u32 igb_tx_cmd_type(struct sk_buff *skb, u32 tx_flags)
{
	/* set type for advanced descriptor with frame checksum insertion */
	u32 cmd_type = E1000_ADVTXD_DTYP_DATA |
		       E1000_ADVTXD_DCMD_DEXT |
		       E1000_ADVTXD_DCMD_IFCS;

	/* set HW vlan bit if vlan is present */
	cmd_type |= IGB_SET_FLAG(tx_flags, IGB_TX_FLAGS_VLAN,
				 (E1000_ADVTXD_DCMD_VLE));

	/* set segmentation bits for TSO */
	cmd_type |= IGB_SET_FLAG(tx_flags, IGB_TX_FLAGS_TSO,
				 (E1000_ADVTXD_DCMD_TSE));

	/* set timestamp bit if present */
	cmd_type |= IGB_SET_FLAG(tx_flags, IGB_TX_FLAGS_TSTAMP,
				 (E1000_ADVTXD_MAC_TSTAMP));

	/* insert frame checksum */
	cmd_type ^= IGB_SET_FLAG(skb->no_fcs, 1, E1000_ADVTXD_DCMD_IFCS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return cmd_type;
}

static void igb_tx_olinfo_status(struct igb_ring *tx_ring,
				 union e1000_adv_tx_desc *tx_desc,
				 u32 tx_flags, unsigned int paylen)
{
	u32 olinfo_status = paylen << E1000_ADVTXD_PAYLEN_SHIFT;

<<<<<<< HEAD
	/* 82575 requires a unique index per ring if any offload is enabled */
	if ((tx_flags & (IGB_TX_FLAGS_CSUM | IGB_TX_FLAGS_VLAN)) &&
	    test_bit(IGB_RING_FLAG_TX_CTX_IDX, &tx_ring->flags))
		olinfo_status |= tx_ring->reg_idx << 4;

	/* insert L4 checksum */
	if (tx_flags & IGB_TX_FLAGS_CSUM) {
		olinfo_status |= E1000_TXD_POPTS_TXSM << 8;

		/* insert IPv4 checksum */
		if (tx_flags & IGB_TX_FLAGS_IPV4)
			olinfo_status |= E1000_TXD_POPTS_IXSM << 8;
	}
=======
	/* 82575 requires a unique index per ring */
	if (test_bit(IGB_RING_FLAG_TX_CTX_IDX, &tx_ring->flags))
		olinfo_status |= tx_ring->reg_idx << 4;

	/* insert L4 checksum */
	olinfo_status |= IGB_SET_FLAG(tx_flags,
				      IGB_TX_FLAGS_CSUM,
				      (E1000_TXD_POPTS_TXSM << 8));

	/* insert IPv4 checksum */
	olinfo_status |= IGB_SET_FLAG(tx_flags,
				      IGB_TX_FLAGS_IPV4,
				      (E1000_TXD_POPTS_IXSM << 8));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	tx_desc->read.olinfo_status = cpu_to_le32(olinfo_status);
}

<<<<<<< HEAD
/*
 * The largest size we can write to the descriptor is 65535.  In order to
 * maintain a power of two alignment we have to limit ourselves to 32K.
 */
#define IGB_MAX_TXD_PWR	15
#define IGB_MAX_DATA_PER_TXD	(1<<IGB_MAX_TXD_PWR)

static void igb_tx_map(struct igb_ring *tx_ring,
		       struct igb_tx_buffer *first,
		       const u8 hdr_len)
{
	struct sk_buff *skb = first->skb;
	struct igb_tx_buffer *tx_buffer_info;
	union e1000_adv_tx_desc *tx_desc;
	dma_addr_t dma;
	struct skb_frag_struct *frag = &skb_shinfo(skb)->frags[0];
	unsigned int data_len = skb->data_len;
	unsigned int size = skb_headlen(skb);
	unsigned int paylen = skb->len - hdr_len;
	__le32 cmd_type;
	u32 tx_flags = first->tx_flags;
	u16 i = tx_ring->next_to_use;

	tx_desc = IGB_TX_DESC(tx_ring, i);

	igb_tx_olinfo_status(tx_ring, tx_desc, tx_flags, paylen);
	cmd_type = igb_tx_cmd_type(tx_flags);

	dma = dma_map_single(tx_ring->dev, skb->data, size, DMA_TO_DEVICE);
	if (dma_mapping_error(tx_ring->dev, dma))
		goto dma_error;

	/* record length, and DMA address */
	first->length = size;
	first->dma = dma;
	tx_desc->read.buffer_addr = cpu_to_le64(dma);

	for (;;) {
		while (unlikely(size > IGB_MAX_DATA_PER_TXD)) {
			tx_desc->read.cmd_type_len =
				cmd_type | cpu_to_le32(IGB_MAX_DATA_PER_TXD);

			i++;
			tx_desc++;
			if (i == tx_ring->count) {
				tx_desc = IGB_TX_DESC(tx_ring, 0);
				i = 0;
			}

			dma += IGB_MAX_DATA_PER_TXD;
			size -= IGB_MAX_DATA_PER_TXD;

			tx_desc->read.olinfo_status = 0;
			tx_desc->read.buffer_addr = cpu_to_le64(dma);
		}

		if (likely(!data_len))
			break;

		tx_desc->read.cmd_type_len = cmd_type | cpu_to_le32(size);

		i++;
		tx_desc++;
		if (i == tx_ring->count) {
			tx_desc = IGB_TX_DESC(tx_ring, 0);
			i = 0;
		}

		size = skb_frag_size(frag);
		data_len -= size;

		dma = skb_frag_dma_map(tx_ring->dev, frag, 0,
				   size, DMA_TO_DEVICE);
		if (dma_mapping_error(tx_ring->dev, dma))
			goto dma_error;

		tx_buffer_info = &tx_ring->tx_buffer_info[i];
		tx_buffer_info->length = size;
		tx_buffer_info->dma = dma;

		tx_desc->read.olinfo_status = 0;
		tx_desc->read.buffer_addr = cpu_to_le64(dma);

		frag++;
	}

	netdev_tx_sent_queue(txring_txq(tx_ring), first->bytecount);

	/* write last descriptor with RS and EOP bits */
	cmd_type |= cpu_to_le32(size) | cpu_to_le32(IGB_TXD_DCMD);
	if (unlikely(skb->no_fcs))
		cmd_type &= ~(cpu_to_le32(E1000_ADVTXD_DCMD_IFCS));
	tx_desc->read.cmd_type_len = cmd_type;

	/* set the timestamp */
	first->time_stamp = jiffies;

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

	writel(i, tx_ring->tail);

	/* we need this if more than one processor can write to our tail
	 * at a time, it syncronizes IO on IA64/Altix systems */
	mmiowb();

	return;

dma_error:
	dev_err(tx_ring->dev, "TX DMA map failed\n");

	/* clear dma mappings for failed tx_buffer_info map */
	for (;;) {
		tx_buffer_info = &tx_ring->tx_buffer_info[i];
		igb_unmap_and_free_tx_resource(tx_ring, tx_buffer_info);
		if (tx_buffer_info == first)
			break;
		if (i == 0)
			i = tx_ring->count;
		i--;
	}

	tx_ring->next_to_use = i;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __igb_maybe_stop_tx(struct igb_ring *tx_ring, const u16 size)
{
	struct net_device *netdev = tx_ring->netdev;

	netif_stop_subqueue(netdev, tx_ring->queue_index);

	/* Herbert's original patch had:
	 *  smp_mb__after_netif_stop_queue();
<<<<<<< HEAD
	 * but since that doesn't exist yet, just open code it. */
	smp_mb();

	/* We need to check again in a case another CPU has just
	 * made room available. */
=======
	 * but since that doesn't exist yet, just open code it.
	 */
	smp_mb();

	/* We need to check again in a case another CPU has just
	 * made room available.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (igb_desc_unused(tx_ring) < size)
		return -EBUSY;

	/* A reprieve! */
	netif_wake_subqueue(netdev, tx_ring->queue_index);

	u64_stats_update_begin(&tx_ring->tx_syncp2);
	tx_ring->tx_stats.restart_queue2++;
	u64_stats_update_end(&tx_ring->tx_syncp2);

	return 0;
}

static inline int igb_maybe_stop_tx(struct igb_ring *tx_ring, const u16 size)
{
	if (igb_desc_unused(tx_ring) >= size)
		return 0;
	return __igb_maybe_stop_tx(tx_ring, size);
}

<<<<<<< HEAD
=======
static int igb_tx_map(struct igb_ring *tx_ring,
		      struct igb_tx_buffer *first,
		      const u8 hdr_len)
{
	struct sk_buff *skb = first->skb;
	struct igb_tx_buffer *tx_buffer;
	union e1000_adv_tx_desc *tx_desc;
	skb_frag_t *frag;
	dma_addr_t dma;
	unsigned int data_len, size;
	u32 tx_flags = first->tx_flags;
	u32 cmd_type = igb_tx_cmd_type(skb, tx_flags);
	u16 i = tx_ring->next_to_use;

	tx_desc = IGB_TX_DESC(tx_ring, i);

	igb_tx_olinfo_status(tx_ring, tx_desc, tx_flags, skb->len - hdr_len);

	size = skb_headlen(skb);
	data_len = skb->data_len;

	dma = dma_map_single(tx_ring->dev, skb->data, size, DMA_TO_DEVICE);

	tx_buffer = first;

	for (frag = &skb_shinfo(skb)->frags[0];; frag++) {
		if (dma_mapping_error(tx_ring->dev, dma))
			goto dma_error;

		/* record length, and DMA address */
		dma_unmap_len_set(tx_buffer, len, size);
		dma_unmap_addr_set(tx_buffer, dma, dma);

		tx_desc->read.buffer_addr = cpu_to_le64(dma);

		while (unlikely(size > IGB_MAX_DATA_PER_TXD)) {
			tx_desc->read.cmd_type_len =
				cpu_to_le32(cmd_type ^ IGB_MAX_DATA_PER_TXD);

			i++;
			tx_desc++;
			if (i == tx_ring->count) {
				tx_desc = IGB_TX_DESC(tx_ring, 0);
				i = 0;
			}
			tx_desc->read.olinfo_status = 0;

			dma += IGB_MAX_DATA_PER_TXD;
			size -= IGB_MAX_DATA_PER_TXD;

			tx_desc->read.buffer_addr = cpu_to_le64(dma);
		}

		if (likely(!data_len))
			break;

		tx_desc->read.cmd_type_len = cpu_to_le32(cmd_type ^ size);

		i++;
		tx_desc++;
		if (i == tx_ring->count) {
			tx_desc = IGB_TX_DESC(tx_ring, 0);
			i = 0;
		}
		tx_desc->read.olinfo_status = 0;

		size = skb_frag_size(frag);
		data_len -= size;

		dma = skb_frag_dma_map(tx_ring->dev, frag, 0,
				       size, DMA_TO_DEVICE);

		tx_buffer = &tx_ring->tx_buffer_info[i];
	}

	/* write last descriptor with RS and EOP bits */
	cmd_type |= size | IGB_TXD_DCMD;
	tx_desc->read.cmd_type_len = cpu_to_le32(cmd_type);

	netdev_tx_sent_queue(txring_txq(tx_ring), first->bytecount);

	/* set the timestamp */
	first->time_stamp = jiffies;

	skb_tx_timestamp(skb);

	/* Force memory writes to complete before letting h/w know there
	 * are new descriptors to fetch.  (Only applicable for weak-ordered
	 * memory model archs, such as IA-64).
	 *
	 * We also need this memory barrier to make certain all of the
	 * status bits have been updated before next_to_watch is written.
	 */
	dma_wmb();

	/* set next_to_watch value indicating a packet is present */
	first->next_to_watch = tx_desc;

	i++;
	if (i == tx_ring->count)
		i = 0;

	tx_ring->next_to_use = i;

	/* Make sure there is space in the ring for the next send. */
	igb_maybe_stop_tx(tx_ring, DESC_NEEDED);

	if (netif_xmit_stopped(txring_txq(tx_ring)) || !netdev_xmit_more()) {
		writel(i, tx_ring->tail);
	}
	return 0;

dma_error:
	dev_err(tx_ring->dev, "TX DMA map failed\n");
	tx_buffer = &tx_ring->tx_buffer_info[i];

	/* clear dma mappings for failed tx_buffer_info map */
	while (tx_buffer != first) {
		if (dma_unmap_len(tx_buffer, len))
			dma_unmap_page(tx_ring->dev,
				       dma_unmap_addr(tx_buffer, dma),
				       dma_unmap_len(tx_buffer, len),
				       DMA_TO_DEVICE);
		dma_unmap_len_set(tx_buffer, len, 0);

		if (i-- == 0)
			i += tx_ring->count;
		tx_buffer = &tx_ring->tx_buffer_info[i];
	}

	if (dma_unmap_len(tx_buffer, len))
		dma_unmap_single(tx_ring->dev,
				 dma_unmap_addr(tx_buffer, dma),
				 dma_unmap_len(tx_buffer, len),
				 DMA_TO_DEVICE);
	dma_unmap_len_set(tx_buffer, len, 0);

	dev_kfree_skb_any(tx_buffer->skb);
	tx_buffer->skb = NULL;

	tx_ring->next_to_use = i;

	return -1;
}

int igb_xmit_xdp_ring(struct igb_adapter *adapter,
		      struct igb_ring *tx_ring,
		      struct xdp_frame *xdpf)
{
	struct skb_shared_info *sinfo = xdp_get_shared_info_from_frame(xdpf);
	u8 nr_frags = unlikely(xdp_frame_has_frags(xdpf)) ? sinfo->nr_frags : 0;
	u16 count, i, index = tx_ring->next_to_use;
	struct igb_tx_buffer *tx_head = &tx_ring->tx_buffer_info[index];
	struct igb_tx_buffer *tx_buffer = tx_head;
	union e1000_adv_tx_desc *tx_desc = IGB_TX_DESC(tx_ring, index);
	u32 len = xdpf->len, cmd_type, olinfo_status;
	void *data = xdpf->data;

	count = TXD_USE_COUNT(len);
	for (i = 0; i < nr_frags; i++)
		count += TXD_USE_COUNT(skb_frag_size(&sinfo->frags[i]));

	if (igb_maybe_stop_tx(tx_ring, count + 3))
		return IGB_XDP_CONSUMED;

	i = 0;
	/* record the location of the first descriptor for this packet */
	tx_head->bytecount = xdp_get_frame_len(xdpf);
	tx_head->type = IGB_TYPE_XDP;
	tx_head->gso_segs = 1;
	tx_head->xdpf = xdpf;

	olinfo_status = tx_head->bytecount << E1000_ADVTXD_PAYLEN_SHIFT;
	/* 82575 requires a unique index per ring */
	if (test_bit(IGB_RING_FLAG_TX_CTX_IDX, &tx_ring->flags))
		olinfo_status |= tx_ring->reg_idx << 4;
	tx_desc->read.olinfo_status = cpu_to_le32(olinfo_status);

	for (;;) {
		dma_addr_t dma;

		dma = dma_map_single(tx_ring->dev, data, len, DMA_TO_DEVICE);
		if (dma_mapping_error(tx_ring->dev, dma))
			goto unmap;

		/* record length, and DMA address */
		dma_unmap_len_set(tx_buffer, len, len);
		dma_unmap_addr_set(tx_buffer, dma, dma);

		/* put descriptor type bits */
		cmd_type = E1000_ADVTXD_DTYP_DATA | E1000_ADVTXD_DCMD_DEXT |
			   E1000_ADVTXD_DCMD_IFCS | len;

		tx_desc->read.cmd_type_len = cpu_to_le32(cmd_type);
		tx_desc->read.buffer_addr = cpu_to_le64(dma);

		tx_buffer->protocol = 0;

		if (++index == tx_ring->count)
			index = 0;

		if (i == nr_frags)
			break;

		tx_buffer = &tx_ring->tx_buffer_info[index];
		tx_desc = IGB_TX_DESC(tx_ring, index);
		tx_desc->read.olinfo_status = 0;

		data = skb_frag_address(&sinfo->frags[i]);
		len = skb_frag_size(&sinfo->frags[i]);
		i++;
	}
	tx_desc->read.cmd_type_len |= cpu_to_le32(IGB_TXD_DCMD);

	netdev_tx_sent_queue(txring_txq(tx_ring), tx_head->bytecount);
	/* set the timestamp */
	tx_head->time_stamp = jiffies;

	/* Avoid any potential race with xdp_xmit and cleanup */
	smp_wmb();

	/* set next_to_watch value indicating a packet is present */
	tx_head->next_to_watch = tx_desc;
	tx_ring->next_to_use = index;

	/* Make sure there is space in the ring for the next send. */
	igb_maybe_stop_tx(tx_ring, DESC_NEEDED);

	if (netif_xmit_stopped(txring_txq(tx_ring)) || !netdev_xmit_more())
		writel(index, tx_ring->tail);

	return IGB_XDP_TX;

unmap:
	for (;;) {
		tx_buffer = &tx_ring->tx_buffer_info[index];
		if (dma_unmap_len(tx_buffer, len))
			dma_unmap_page(tx_ring->dev,
				       dma_unmap_addr(tx_buffer, dma),
				       dma_unmap_len(tx_buffer, len),
				       DMA_TO_DEVICE);
		dma_unmap_len_set(tx_buffer, len, 0);
		if (tx_buffer == tx_head)
			break;

		if (!index)
			index += tx_ring->count;
		index--;
	}

	return IGB_XDP_CONSUMED;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
netdev_tx_t igb_xmit_frame_ring(struct sk_buff *skb,
				struct igb_ring *tx_ring)
{
	struct igb_tx_buffer *first;
	int tso;
	u32 tx_flags = 0;
<<<<<<< HEAD
	__be16 protocol = vlan_get_protocol(skb);
	u8 hdr_len = 0;

	/* need: 1 descriptor per page,
	 *       + 2 desc gap to keep tail from touching head,
	 *       + 1 desc for skb->data,
	 *       + 1 desc for context descriptor,
	 * otherwise try next time */
	if (igb_maybe_stop_tx(tx_ring, skb_shinfo(skb)->nr_frags + 4)) {
=======
	unsigned short f;
	u16 count = TXD_USE_COUNT(skb_headlen(skb));
	__be16 protocol = vlan_get_protocol(skb);
	u8 hdr_len = 0;

	/* need: 1 descriptor per page * PAGE_SIZE/IGB_MAX_DATA_PER_TXD,
	 *       + 1 desc for skb_headlen/IGB_MAX_DATA_PER_TXD,
	 *       + 2 desc gap to keep tail from touching head,
	 *       + 1 desc for context descriptor,
	 * otherwise try next time
	 */
	for (f = 0; f < skb_shinfo(skb)->nr_frags; f++)
		count += TXD_USE_COUNT(skb_frag_size(
						&skb_shinfo(skb)->frags[f]));

	if (igb_maybe_stop_tx(tx_ring, count + 3)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* this is a hard error */
		return NETDEV_TX_BUSY;
	}

	/* record the location of the first descriptor for this packet */
	first = &tx_ring->tx_buffer_info[tx_ring->next_to_use];
<<<<<<< HEAD
=======
	first->type = IGB_TYPE_SKB;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	first->skb = skb;
	first->bytecount = skb->len;
	first->gso_segs = 1;

	if (unlikely(skb_shinfo(skb)->tx_flags & SKBTX_HW_TSTAMP)) {
<<<<<<< HEAD
		skb_shinfo(skb)->tx_flags |= SKBTX_IN_PROGRESS;
		tx_flags |= IGB_TX_FLAGS_TSTAMP;
	}

	if (vlan_tx_tag_present(skb)) {
		tx_flags |= IGB_TX_FLAGS_VLAN;
		tx_flags |= (vlan_tx_tag_get(skb) << IGB_TX_FLAGS_VLAN_SHIFT);
=======
		struct igb_adapter *adapter = netdev_priv(tx_ring->netdev);

		if (adapter->tstamp_config.tx_type == HWTSTAMP_TX_ON &&
		    !test_and_set_bit_lock(__IGB_PTP_TX_IN_PROGRESS,
					   &adapter->state)) {
			skb_shinfo(skb)->tx_flags |= SKBTX_IN_PROGRESS;
			tx_flags |= IGB_TX_FLAGS_TSTAMP;

			adapter->ptp_tx_skb = skb_get(skb);
			adapter->ptp_tx_start = jiffies;
			if (adapter->hw.mac.type == e1000_82576)
				schedule_work(&adapter->ptp_tx_work);
		} else {
			adapter->tx_hwtstamp_skipped++;
		}
	}

	if (skb_vlan_tag_present(skb)) {
		tx_flags |= IGB_TX_FLAGS_VLAN;
		tx_flags |= (skb_vlan_tag_get(skb) << IGB_TX_FLAGS_VLAN_SHIFT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* record initial flags and protocol */
	first->tx_flags = tx_flags;
	first->protocol = protocol;

	tso = igb_tso(tx_ring, first, &hdr_len);
	if (tso < 0)
		goto out_drop;
	else if (!tso)
		igb_tx_csum(tx_ring, first);

<<<<<<< HEAD
	igb_tx_map(tx_ring, first, hdr_len);

	/* Make sure there is space in the ring for the next send. */
	igb_maybe_stop_tx(tx_ring, MAX_SKB_FRAGS + 4);
=======
	if (igb_tx_map(tx_ring, first, hdr_len))
		goto cleanup_tx_tstamp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NETDEV_TX_OK;

out_drop:
<<<<<<< HEAD
	igb_unmap_and_free_tx_resource(tx_ring, first);
=======
	dev_kfree_skb_any(first->skb);
	first->skb = NULL;
cleanup_tx_tstamp:
	if (unlikely(tx_flags & IGB_TX_FLAGS_TSTAMP)) {
		struct igb_adapter *adapter = netdev_priv(tx_ring->netdev);

		dev_kfree_skb_any(adapter->ptp_tx_skb);
		adapter->ptp_tx_skb = NULL;
		if (adapter->hw.mac.type == e1000_82576)
			cancel_work_sync(&adapter->ptp_tx_work);
		clear_bit_unlock(__IGB_PTP_TX_IN_PROGRESS, &adapter->state);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NETDEV_TX_OK;
}

static inline struct igb_ring *igb_tx_queue_mapping(struct igb_adapter *adapter,
						    struct sk_buff *skb)
{
	unsigned int r_idx = skb->queue_mapping;

	if (r_idx >= adapter->num_tx_queues)
		r_idx = r_idx % adapter->num_tx_queues;

	return adapter->tx_ring[r_idx];
}

static netdev_tx_t igb_xmit_frame(struct sk_buff *skb,
				  struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);

<<<<<<< HEAD
	if (test_bit(__IGB_DOWN, &adapter->state)) {
		dev_kfree_skb_any(skb);
		return NETDEV_TX_OK;
	}

	if (skb->len <= 0) {
		dev_kfree_skb_any(skb);
		return NETDEV_TX_OK;
	}

	/*
	 * The minimum packet size with TCTL.PSP set is 17 so pad the skb
	 * in order to meet this minimum size requirement.
	 */
	if (skb->len < 17) {
		if (skb_padto(skb, 17))
			return NETDEV_TX_OK;
		skb->len = 17;
	}
=======
	/* The minimum packet size with TCTL.PSP set is 17 so pad the skb
	 * in order to meet this minimum size requirement.
	 */
	if (skb_put_padto(skb, 17))
		return NETDEV_TX_OK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return igb_xmit_frame_ring(skb, igb_tx_queue_mapping(adapter, skb));
}

/**
<<<<<<< HEAD
 * igb_tx_timeout - Respond to a Tx Hang
 * @netdev: network interface device structure
 **/
static void igb_tx_timeout(struct net_device *netdev)
=======
 *  igb_tx_timeout - Respond to a Tx Hang
 *  @netdev: network interface device structure
 *  @txqueue: number of the Tx queue that hung (unused)
 **/
static void igb_tx_timeout(struct net_device *netdev, unsigned int __always_unused txqueue)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;

	/* Do the reset outside of interrupt context */
	adapter->tx_timeout_count++;

	if (hw->mac.type >= e1000_82580)
		hw->dev_spec._82575.global_device_reset = true;

	schedule_work(&adapter->reset_task);
	wr32(E1000_EICS,
	     (adapter->eims_enable_mask & ~adapter->eims_other));
}

static void igb_reset_task(struct work_struct *work)
{
	struct igb_adapter *adapter;
	adapter = container_of(work, struct igb_adapter, reset_task);

<<<<<<< HEAD
	igb_dump(adapter);
	netdev_err(adapter->netdev, "Reset adapter\n");
	igb_reinit_locked(adapter);
}

/**
 * igb_get_stats64 - Get System Network Statistics
 * @netdev: network interface device structure
 * @stats: rtnl_link_stats64 pointer
 *
 **/
static struct rtnl_link_stats64 *igb_get_stats64(struct net_device *netdev,
						 struct rtnl_link_stats64 *stats)
=======
	rtnl_lock();
	/* If we're already down or resetting, just bail */
	if (test_bit(__IGB_DOWN, &adapter->state) ||
	    test_bit(__IGB_RESETTING, &adapter->state)) {
		rtnl_unlock();
		return;
	}

	igb_dump(adapter);
	netdev_err(adapter->netdev, "Reset adapter\n");
	igb_reinit_locked(adapter);
	rtnl_unlock();
}

/**
 *  igb_get_stats64 - Get System Network Statistics
 *  @netdev: network interface device structure
 *  @stats: rtnl_link_stats64 pointer
 **/
static void igb_get_stats64(struct net_device *netdev,
			    struct rtnl_link_stats64 *stats)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct igb_adapter *adapter = netdev_priv(netdev);

	spin_lock(&adapter->stats64_lock);
<<<<<<< HEAD
	igb_update_stats(adapter, &adapter->stats64);
	memcpy(stats, &adapter->stats64, sizeof(*stats));
	spin_unlock(&adapter->stats64_lock);

	return stats;
}

/**
 * igb_change_mtu - Change the Maximum Transfer Unit
 * @netdev: network interface device structure
 * @new_mtu: new value for maximum frame size
 *
 * Returns 0 on success, negative on failure
=======
	igb_update_stats(adapter);
	memcpy(stats, &adapter->stats64, sizeof(*stats));
	spin_unlock(&adapter->stats64_lock);
}

/**
 *  igb_change_mtu - Change the Maximum Transfer Unit
 *  @netdev: network interface device structure
 *  @new_mtu: new value for maximum frame size
 *
 *  Returns 0 on success, negative on failure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static int igb_change_mtu(struct net_device *netdev, int new_mtu)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
<<<<<<< HEAD
	struct pci_dev *pdev = adapter->pdev;
	int max_frame = new_mtu + ETH_HLEN + ETH_FCS_LEN + VLAN_HLEN;

	if ((new_mtu < 68) || (max_frame > MAX_JUMBO_FRAME_SIZE)) {
		dev_err(&pdev->dev, "Invalid MTU setting\n");
		return -EINVAL;
	}

#define MAX_STD_JUMBO_FRAME_SIZE 9238
	if (max_frame > MAX_STD_JUMBO_FRAME_SIZE) {
		dev_err(&pdev->dev, "MTU > 9216 not supported.\n");
		return -EINVAL;
	}

	while (test_and_set_bit(__IGB_RESETTING, &adapter->state))
		msleep(1);
=======
	int max_frame = new_mtu + IGB_ETH_PKT_HDR_PAD;

	if (adapter->xdp_prog) {
		int i;

		for (i = 0; i < adapter->num_rx_queues; i++) {
			struct igb_ring *ring = adapter->rx_ring[i];

			if (max_frame > igb_rx_bufsz(ring)) {
				netdev_warn(adapter->netdev,
					    "Requested MTU size is not supported with XDP. Max frame size is %d\n",
					    max_frame);
				return -EINVAL;
			}
		}
	}

	/* adjust max frame to be at least the size of a standard frame */
	if (max_frame < (ETH_FRAME_LEN + ETH_FCS_LEN))
		max_frame = ETH_FRAME_LEN + ETH_FCS_LEN;

	while (test_and_set_bit(__IGB_RESETTING, &adapter->state))
		usleep_range(1000, 2000);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* igb_down has a dependency on max_frame_size */
	adapter->max_frame_size = max_frame;

	if (netif_running(netdev))
		igb_down(adapter);

<<<<<<< HEAD
	dev_info(&pdev->dev, "changing MTU from %d to %d\n",
		 netdev->mtu, new_mtu);
=======
	netdev_dbg(netdev, "changing MTU from %d to %d\n",
		   netdev->mtu, new_mtu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	netdev->mtu = new_mtu;

	if (netif_running(netdev))
		igb_up(adapter);
	else
		igb_reset(adapter);

	clear_bit(__IGB_RESETTING, &adapter->state);

	return 0;
}

/**
<<<<<<< HEAD
 * igb_update_stats - Update the board statistics counters
 * @adapter: board private structure
 **/

void igb_update_stats(struct igb_adapter *adapter,
		      struct rtnl_link_stats64 *net_stats)
{
	struct e1000_hw *hw = &adapter->hw;
	struct pci_dev *pdev = adapter->pdev;
	u32 reg, mpc;
	u16 phy_tmp;
=======
 *  igb_update_stats - Update the board statistics counters
 *  @adapter: board private structure
 **/
void igb_update_stats(struct igb_adapter *adapter)
{
	struct rtnl_link_stats64 *net_stats = &adapter->stats64;
	struct e1000_hw *hw = &adapter->hw;
	struct pci_dev *pdev = adapter->pdev;
	u32 reg, mpc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i;
	u64 bytes, packets;
	unsigned int start;
	u64 _bytes, _packets;

<<<<<<< HEAD
#define PHY_IDLE_ERROR_COUNT_MASK 0x00FF

	/*
	 * Prevent stats update while adapter is being reset, or if the pci
=======
	/* Prevent stats update while adapter is being reset, or if the pci
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * connection is down.
	 */
	if (adapter->link_speed == 0)
		return;
	if (pci_channel_offline(pdev))
		return;

	bytes = 0;
	packets = 0;
<<<<<<< HEAD
	for (i = 0; i < adapter->num_rx_queues; i++) {
		u32 rqdpc = rd32(E1000_RQDPC(i));
		struct igb_ring *ring = adapter->rx_ring[i];
=======

	rcu_read_lock();
	for (i = 0; i < adapter->num_rx_queues; i++) {
		struct igb_ring *ring = adapter->rx_ring[i];
		u32 rqdpc = rd32(E1000_RQDPC(i));
		if (hw->mac.type >= e1000_i210)
			wr32(E1000_RQDPC(i), 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (rqdpc) {
			ring->rx_stats.drops += rqdpc;
			net_stats->rx_fifo_errors += rqdpc;
		}

		do {
<<<<<<< HEAD
			start = u64_stats_fetch_begin_irq(&ring->rx_syncp);
			_bytes = ring->rx_stats.bytes;
			_packets = ring->rx_stats.packets;
		} while (u64_stats_fetch_retry_irq(&ring->rx_syncp, start));
=======
			start = u64_stats_fetch_begin(&ring->rx_syncp);
			_bytes = ring->rx_stats.bytes;
			_packets = ring->rx_stats.packets;
		} while (u64_stats_fetch_retry(&ring->rx_syncp, start));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bytes += _bytes;
		packets += _packets;
	}

	net_stats->rx_bytes = bytes;
	net_stats->rx_packets = packets;

	bytes = 0;
	packets = 0;
	for (i = 0; i < adapter->num_tx_queues; i++) {
		struct igb_ring *ring = adapter->tx_ring[i];
		do {
<<<<<<< HEAD
			start = u64_stats_fetch_begin_irq(&ring->tx_syncp);
			_bytes = ring->tx_stats.bytes;
			_packets = ring->tx_stats.packets;
		} while (u64_stats_fetch_retry_irq(&ring->tx_syncp, start));
=======
			start = u64_stats_fetch_begin(&ring->tx_syncp);
			_bytes = ring->tx_stats.bytes;
			_packets = ring->tx_stats.packets;
		} while (u64_stats_fetch_retry(&ring->tx_syncp, start));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bytes += _bytes;
		packets += _packets;
	}
	net_stats->tx_bytes = bytes;
	net_stats->tx_packets = packets;
<<<<<<< HEAD
=======
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* read stats registers */
	adapter->stats.crcerrs += rd32(E1000_CRCERRS);
	adapter->stats.gprc += rd32(E1000_GPRC);
	adapter->stats.gorc += rd32(E1000_GORCL);
	rd32(E1000_GORCH); /* clear GORCL */
	adapter->stats.bprc += rd32(E1000_BPRC);
	adapter->stats.mprc += rd32(E1000_MPRC);
	adapter->stats.roc += rd32(E1000_ROC);

	adapter->stats.prc64 += rd32(E1000_PRC64);
	adapter->stats.prc127 += rd32(E1000_PRC127);
	adapter->stats.prc255 += rd32(E1000_PRC255);
	adapter->stats.prc511 += rd32(E1000_PRC511);
	adapter->stats.prc1023 += rd32(E1000_PRC1023);
	adapter->stats.prc1522 += rd32(E1000_PRC1522);
	adapter->stats.symerrs += rd32(E1000_SYMERRS);
	adapter->stats.sec += rd32(E1000_SEC);

	mpc = rd32(E1000_MPC);
	adapter->stats.mpc += mpc;
	net_stats->rx_fifo_errors += mpc;
	adapter->stats.scc += rd32(E1000_SCC);
	adapter->stats.ecol += rd32(E1000_ECOL);
	adapter->stats.mcc += rd32(E1000_MCC);
	adapter->stats.latecol += rd32(E1000_LATECOL);
	adapter->stats.dc += rd32(E1000_DC);
	adapter->stats.rlec += rd32(E1000_RLEC);
	adapter->stats.xonrxc += rd32(E1000_XONRXC);
	adapter->stats.xontxc += rd32(E1000_XONTXC);
	adapter->stats.xoffrxc += rd32(E1000_XOFFRXC);
	adapter->stats.xofftxc += rd32(E1000_XOFFTXC);
	adapter->stats.fcruc += rd32(E1000_FCRUC);
	adapter->stats.gptc += rd32(E1000_GPTC);
	adapter->stats.gotc += rd32(E1000_GOTCL);
	rd32(E1000_GOTCH); /* clear GOTCL */
	adapter->stats.rnbc += rd32(E1000_RNBC);
	adapter->stats.ruc += rd32(E1000_RUC);
	adapter->stats.rfc += rd32(E1000_RFC);
	adapter->stats.rjc += rd32(E1000_RJC);
	adapter->stats.tor += rd32(E1000_TORH);
	adapter->stats.tot += rd32(E1000_TOTH);
	adapter->stats.tpr += rd32(E1000_TPR);

	adapter->stats.ptc64 += rd32(E1000_PTC64);
	adapter->stats.ptc127 += rd32(E1000_PTC127);
	adapter->stats.ptc255 += rd32(E1000_PTC255);
	adapter->stats.ptc511 += rd32(E1000_PTC511);
	adapter->stats.ptc1023 += rd32(E1000_PTC1023);
	adapter->stats.ptc1522 += rd32(E1000_PTC1522);

	adapter->stats.mptc += rd32(E1000_MPTC);
	adapter->stats.bptc += rd32(E1000_BPTC);

	adapter->stats.tpt += rd32(E1000_TPT);
	adapter->stats.colc += rd32(E1000_COLC);

	adapter->stats.algnerrc += rd32(E1000_ALGNERRC);
	/* read internal phy specific stats */
	reg = rd32(E1000_CTRL_EXT);
	if (!(reg & E1000_CTRL_EXT_LINK_MODE_MASK)) {
		adapter->stats.rxerrc += rd32(E1000_RXERRC);
<<<<<<< HEAD
		adapter->stats.tncrs += rd32(E1000_TNCRS);
=======

		/* this stat has invalid values on i210/i211 */
		if ((hw->mac.type != e1000_i210) &&
		    (hw->mac.type != e1000_i211))
			adapter->stats.tncrs += rd32(E1000_TNCRS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	adapter->stats.tsctc += rd32(E1000_TSCTC);
	adapter->stats.tsctfc += rd32(E1000_TSCTFC);

	adapter->stats.iac += rd32(E1000_IAC);
	adapter->stats.icrxoc += rd32(E1000_ICRXOC);
	adapter->stats.icrxptc += rd32(E1000_ICRXPTC);
	adapter->stats.icrxatc += rd32(E1000_ICRXATC);
	adapter->stats.ictxptc += rd32(E1000_ICTXPTC);
	adapter->stats.ictxatc += rd32(E1000_ICTXATC);
	adapter->stats.ictxqec += rd32(E1000_ICTXQEC);
	adapter->stats.ictxqmtc += rd32(E1000_ICTXQMTC);
	adapter->stats.icrxdmtc += rd32(E1000_ICRXDMTC);

	/* Fill out the OS statistics structure */
	net_stats->multicast = adapter->stats.mprc;
	net_stats->collisions = adapter->stats.colc;

	/* Rx Errors */

	/* RLEC on some newer hardware can be incorrect so build
<<<<<<< HEAD
	 * our own version based on RUC and ROC */
=======
	 * our own version based on RUC and ROC
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	net_stats->rx_errors = adapter->stats.rxerrc +
		adapter->stats.crcerrs + adapter->stats.algnerrc +
		adapter->stats.ruc + adapter->stats.roc +
		adapter->stats.cexterr;
	net_stats->rx_length_errors = adapter->stats.ruc +
				      adapter->stats.roc;
	net_stats->rx_crc_errors = adapter->stats.crcerrs;
	net_stats->rx_frame_errors = adapter->stats.algnerrc;
	net_stats->rx_missed_errors = adapter->stats.mpc;

	/* Tx Errors */
	net_stats->tx_errors = adapter->stats.ecol +
			       adapter->stats.latecol;
	net_stats->tx_aborted_errors = adapter->stats.ecol;
	net_stats->tx_window_errors = adapter->stats.latecol;
	net_stats->tx_carrier_errors = adapter->stats.tncrs;

	/* Tx Dropped needs to be maintained elsewhere */

<<<<<<< HEAD
	/* Phy Stats */
	if (hw->phy.media_type == e1000_media_type_copper) {
		if ((adapter->link_speed == SPEED_1000) &&
		   (!igb_read_phy_reg(hw, PHY_1000T_STATUS, &phy_tmp))) {
			phy_tmp &= PHY_IDLE_ERROR_COUNT_MASK;
			adapter->phy_stats.idle_errors += phy_tmp;
		}
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Management Stats */
	adapter->stats.mgptc += rd32(E1000_MGTPTC);
	adapter->stats.mgprc += rd32(E1000_MGTPRC);
	adapter->stats.mgpdc += rd32(E1000_MGTPDC);

	/* OS2BMC Stats */
	reg = rd32(E1000_MANC);
	if (reg & E1000_MANC_EN_BMC2OS) {
		adapter->stats.o2bgptc += rd32(E1000_O2BGPTC);
		adapter->stats.o2bspc += rd32(E1000_O2BSPC);
		adapter->stats.b2ospc += rd32(E1000_B2OSPC);
		adapter->stats.b2ogprc += rd32(E1000_B2OGPRC);
	}
}

<<<<<<< HEAD
=======
static void igb_perout(struct igb_adapter *adapter, int tsintr_tt)
{
	int pin = ptp_find_pin(adapter->ptp_clock, PTP_PF_PEROUT, tsintr_tt);
	struct e1000_hw *hw = &adapter->hw;
	struct timespec64 ts;
	u32 tsauxc;

	if (pin < 0 || pin >= IGB_N_SDP)
		return;

	spin_lock(&adapter->tmreg_lock);

	if (hw->mac.type == e1000_82580 ||
	    hw->mac.type == e1000_i354 ||
	    hw->mac.type == e1000_i350) {
		s64 ns = timespec64_to_ns(&adapter->perout[tsintr_tt].period);
		u32 systiml, systimh, level_mask, level, rem;
		u64 systim, now;

		/* read systim registers in sequence */
		rd32(E1000_SYSTIMR);
		systiml = rd32(E1000_SYSTIML);
		systimh = rd32(E1000_SYSTIMH);
		systim = (((u64)(systimh & 0xFF)) << 32) | ((u64)systiml);
		now = timecounter_cyc2time(&adapter->tc, systim);

		if (pin < 2) {
			level_mask = (tsintr_tt == 1) ? 0x80000 : 0x40000;
			level = (rd32(E1000_CTRL) & level_mask) ? 1 : 0;
		} else {
			level_mask = (tsintr_tt == 1) ? 0x80 : 0x40;
			level = (rd32(E1000_CTRL_EXT) & level_mask) ? 1 : 0;
		}

		div_u64_rem(now, ns, &rem);
		systim = systim + (ns - rem);

		/* synchronize pin level with rising/falling edges */
		div_u64_rem(now, ns << 1, &rem);
		if (rem < ns) {
			/* first half of period */
			if (level == 0) {
				/* output is already low, skip this period */
				systim += ns;
				pr_notice("igb: periodic output on %s missed falling edge\n",
					  adapter->sdp_config[pin].name);
			}
		} else {
			/* second half of period */
			if (level == 1) {
				/* output is already high, skip this period */
				systim += ns;
				pr_notice("igb: periodic output on %s missed rising edge\n",
					  adapter->sdp_config[pin].name);
			}
		}

		/* for this chip family tv_sec is the upper part of the binary value,
		 * so not seconds
		 */
		ts.tv_nsec = (u32)systim;
		ts.tv_sec  = ((u32)(systim >> 32)) & 0xFF;
	} else {
		ts = timespec64_add(adapter->perout[tsintr_tt].start,
				    adapter->perout[tsintr_tt].period);
	}

	/* u32 conversion of tv_sec is safe until y2106 */
	wr32((tsintr_tt == 1) ? E1000_TRGTTIML1 : E1000_TRGTTIML0, ts.tv_nsec);
	wr32((tsintr_tt == 1) ? E1000_TRGTTIMH1 : E1000_TRGTTIMH0, (u32)ts.tv_sec);
	tsauxc = rd32(E1000_TSAUXC);
	tsauxc |= TSAUXC_EN_TT0;
	wr32(E1000_TSAUXC, tsauxc);
	adapter->perout[tsintr_tt].start = ts;

	spin_unlock(&adapter->tmreg_lock);
}

static void igb_extts(struct igb_adapter *adapter, int tsintr_tt)
{
	int pin = ptp_find_pin(adapter->ptp_clock, PTP_PF_EXTTS, tsintr_tt);
	int auxstmpl = (tsintr_tt == 1) ? E1000_AUXSTMPL1 : E1000_AUXSTMPL0;
	int auxstmph = (tsintr_tt == 1) ? E1000_AUXSTMPH1 : E1000_AUXSTMPH0;
	struct e1000_hw *hw = &adapter->hw;
	struct ptp_clock_event event;
	struct timespec64 ts;
	unsigned long flags;

	if (pin < 0 || pin >= IGB_N_SDP)
		return;

	if (hw->mac.type == e1000_82580 ||
	    hw->mac.type == e1000_i354 ||
	    hw->mac.type == e1000_i350) {
		u64 ns = rd32(auxstmpl);

		ns += ((u64)(rd32(auxstmph) & 0xFF)) << 32;
		spin_lock_irqsave(&adapter->tmreg_lock, flags);
		ns = timecounter_cyc2time(&adapter->tc, ns);
		spin_unlock_irqrestore(&adapter->tmreg_lock, flags);
		ts = ns_to_timespec64(ns);
	} else {
		ts.tv_nsec = rd32(auxstmpl);
		ts.tv_sec  = rd32(auxstmph);
	}

	event.type = PTP_CLOCK_EXTTS;
	event.index = tsintr_tt;
	event.timestamp = ts.tv_sec * 1000000000ULL + ts.tv_nsec;
	ptp_clock_event(adapter->ptp_clock, &event);
}

static void igb_tsync_interrupt(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 tsicr = rd32(E1000_TSICR);
	struct ptp_clock_event event;

	if (tsicr & TSINTR_SYS_WRAP) {
		event.type = PTP_CLOCK_PPS;
		if (adapter->ptp_caps.pps)
			ptp_clock_event(adapter->ptp_clock, &event);
	}

	if (tsicr & E1000_TSICR_TXTS) {
		/* retrieve hardware timestamp */
		schedule_work(&adapter->ptp_tx_work);
	}

	if (tsicr & TSINTR_TT0)
		igb_perout(adapter, 0);

	if (tsicr & TSINTR_TT1)
		igb_perout(adapter, 1);

	if (tsicr & TSINTR_AUTT0)
		igb_extts(adapter, 0);

	if (tsicr & TSINTR_AUTT1)
		igb_extts(adapter, 1);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static irqreturn_t igb_msix_other(int irq, void *data)
{
	struct igb_adapter *adapter = data;
	struct e1000_hw *hw = &adapter->hw;
	u32 icr = rd32(E1000_ICR);
	/* reading ICR causes bit 31 of EICR to be cleared */

	if (icr & E1000_ICR_DRSTA)
		schedule_work(&adapter->reset_task);

	if (icr & E1000_ICR_DOUTSYNC) {
		/* HW is reporting DMA is out of sync */
		adapter->stats.doosync++;
		/* The DMA Out of Sync is also indication of a spoof event
		 * in IOV mode. Check the Wrong VM Behavior register to
<<<<<<< HEAD
		 * see if it is really a spoof event. */
=======
		 * see if it is really a spoof event.
		 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		igb_check_wvbr(adapter);
	}

	/* Check for a mailbox event */
	if (icr & E1000_ICR_VMMB)
		igb_msg_task(adapter);

	if (icr & E1000_ICR_LSC) {
		hw->mac.get_link_status = 1;
		/* guard against interrupt when we're going down */
		if (!test_bit(__IGB_DOWN, &adapter->state))
			mod_timer(&adapter->watchdog_timer, jiffies + 1);
	}

<<<<<<< HEAD
=======
	if (icr & E1000_ICR_TS)
		igb_tsync_interrupt(adapter);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wr32(E1000_EIMS, adapter->eims_other);

	return IRQ_HANDLED;
}

static void igb_write_itr(struct igb_q_vector *q_vector)
{
	struct igb_adapter *adapter = q_vector->adapter;
	u32 itr_val = q_vector->itr_val & 0x7FFC;

	if (!q_vector->set_itr)
		return;

	if (!itr_val)
		itr_val = 0x4;

	if (adapter->hw.mac.type == e1000_82575)
		itr_val |= itr_val << 16;
	else
		itr_val |= E1000_EITR_CNT_IGNR;

	writel(itr_val, q_vector->itr_register);
	q_vector->set_itr = 0;
}

static irqreturn_t igb_msix_ring(int irq, void *data)
{
	struct igb_q_vector *q_vector = data;

	/* Write the ITR value calculated from the previous interrupt. */
	igb_write_itr(q_vector);

	napi_schedule(&q_vector->napi);

	return IRQ_HANDLED;
}

#ifdef CONFIG_IGB_DCA
<<<<<<< HEAD
static void igb_update_dca(struct igb_q_vector *q_vector)
{
	struct igb_adapter *adapter = q_vector->adapter;
	struct e1000_hw *hw = &adapter->hw;
=======
static void igb_update_tx_dca(struct igb_adapter *adapter,
			      struct igb_ring *tx_ring,
			      int cpu)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 txctrl = dca3_get_tag(tx_ring->dev, cpu);

	if (hw->mac.type != e1000_82575)
		txctrl <<= E1000_DCA_TXCTRL_CPUID_SHIFT;

	/* We can enable relaxed ordering for reads, but not writes when
	 * DCA is enabled.  This is due to a known issue in some chipsets
	 * which will cause the DCA tag to be cleared.
	 */
	txctrl |= E1000_DCA_TXCTRL_DESC_RRO_EN |
		  E1000_DCA_TXCTRL_DATA_RRO_EN |
		  E1000_DCA_TXCTRL_DESC_DCA_EN;

	wr32(E1000_DCA_TXCTRL(tx_ring->reg_idx), txctrl);
}

static void igb_update_rx_dca(struct igb_adapter *adapter,
			      struct igb_ring *rx_ring,
			      int cpu)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 rxctrl = dca3_get_tag(&adapter->pdev->dev, cpu);

	if (hw->mac.type != e1000_82575)
		rxctrl <<= E1000_DCA_RXCTRL_CPUID_SHIFT;

	/* We can enable relaxed ordering for reads, but not writes when
	 * DCA is enabled.  This is due to a known issue in some chipsets
	 * which will cause the DCA tag to be cleared.
	 */
	rxctrl |= E1000_DCA_RXCTRL_DESC_RRO_EN |
		  E1000_DCA_RXCTRL_DESC_DCA_EN;

	wr32(E1000_DCA_RXCTRL(rx_ring->reg_idx), rxctrl);
}

static void igb_update_dca(struct igb_q_vector *q_vector)
{
	struct igb_adapter *adapter = q_vector->adapter;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int cpu = get_cpu();

	if (q_vector->cpu == cpu)
		goto out_no_update;

<<<<<<< HEAD
	if (q_vector->tx.ring) {
		int q = q_vector->tx.ring->reg_idx;
		u32 dca_txctrl = rd32(E1000_DCA_TXCTRL(q));
		if (hw->mac.type == e1000_82575) {
			dca_txctrl &= ~E1000_DCA_TXCTRL_CPUID_MASK;
			dca_txctrl |= dca3_get_tag(&adapter->pdev->dev, cpu);
		} else {
			dca_txctrl &= ~E1000_DCA_TXCTRL_CPUID_MASK_82576;
			dca_txctrl |= dca3_get_tag(&adapter->pdev->dev, cpu) <<
			              E1000_DCA_TXCTRL_CPUID_SHIFT;
		}
		dca_txctrl |= E1000_DCA_TXCTRL_DESC_DCA_EN;
		wr32(E1000_DCA_TXCTRL(q), dca_txctrl);
	}
	if (q_vector->rx.ring) {
		int q = q_vector->rx.ring->reg_idx;
		u32 dca_rxctrl = rd32(E1000_DCA_RXCTRL(q));
		if (hw->mac.type == e1000_82575) {
			dca_rxctrl &= ~E1000_DCA_RXCTRL_CPUID_MASK;
			dca_rxctrl |= dca3_get_tag(&adapter->pdev->dev, cpu);
		} else {
			dca_rxctrl &= ~E1000_DCA_RXCTRL_CPUID_MASK_82576;
			dca_rxctrl |= dca3_get_tag(&adapter->pdev->dev, cpu) <<
			              E1000_DCA_RXCTRL_CPUID_SHIFT;
		}
		dca_rxctrl |= E1000_DCA_RXCTRL_DESC_DCA_EN;
		dca_rxctrl |= E1000_DCA_RXCTRL_HEAD_DCA_EN;
		dca_rxctrl |= E1000_DCA_RXCTRL_DATA_DCA_EN;
		wr32(E1000_DCA_RXCTRL(q), dca_rxctrl);
	}
=======
	if (q_vector->tx.ring)
		igb_update_tx_dca(adapter, q_vector->tx.ring, cpu);

	if (q_vector->rx.ring)
		igb_update_rx_dca(adapter, q_vector->rx.ring, cpu);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	q_vector->cpu = cpu;
out_no_update:
	put_cpu();
}

static void igb_setup_dca(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	int i;

	if (!(adapter->flags & IGB_FLAG_DCA_ENABLED))
		return;

	/* Always use CB2 mode, difference is masked in the CB driver. */
	wr32(E1000_DCA_CTRL, E1000_DCA_CTRL_DCA_MODE_CB2);

	for (i = 0; i < adapter->num_q_vectors; i++) {
		adapter->q_vector[i]->cpu = -1;
		igb_update_dca(adapter->q_vector[i]);
	}
}

static int __igb_notify_dca(struct device *dev, void *data)
{
	struct net_device *netdev = dev_get_drvdata(dev);
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct pci_dev *pdev = adapter->pdev;
	struct e1000_hw *hw = &adapter->hw;
	unsigned long event = *(unsigned long *)data;

	switch (event) {
	case DCA_PROVIDER_ADD:
		/* if already enabled, don't do it again */
		if (adapter->flags & IGB_FLAG_DCA_ENABLED)
			break;
		if (dca_add_requester(dev) == 0) {
			adapter->flags |= IGB_FLAG_DCA_ENABLED;
			dev_info(&pdev->dev, "DCA enabled\n");
			igb_setup_dca(adapter);
			break;
		}
<<<<<<< HEAD
		/* Fall Through since DCA is disabled. */
	case DCA_PROVIDER_REMOVE:
		if (adapter->flags & IGB_FLAG_DCA_ENABLED) {
			/* without this a class_device is left
			 * hanging around in the sysfs model */
=======
		fallthrough; /* since DCA is disabled. */
	case DCA_PROVIDER_REMOVE:
		if (adapter->flags & IGB_FLAG_DCA_ENABLED) {
			/* without this a class_device is left
			 * hanging around in the sysfs model
			 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			dca_remove_requester(dev);
			dev_info(&pdev->dev, "DCA disabled\n");
			adapter->flags &= ~IGB_FLAG_DCA_ENABLED;
			wr32(E1000_DCA_CTRL, E1000_DCA_CTRL_DCA_MODE_DISABLE);
		}
		break;
	}

	return 0;
}

static int igb_notify_dca(struct notifier_block *nb, unsigned long event,
<<<<<<< HEAD
                          void *p)
=======
			  void *p)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret_val;

	ret_val = driver_for_each_device(&igb_driver.driver, NULL, &event,
<<<<<<< HEAD
	                                 __igb_notify_dca);
=======
					 __igb_notify_dca);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret_val ? NOTIFY_BAD : NOTIFY_DONE;
}
#endif /* CONFIG_IGB_DCA */

#ifdef CONFIG_PCI_IOV
static int igb_vf_configure(struct igb_adapter *adapter, int vf)
{
	unsigned char mac_addr[ETH_ALEN];
<<<<<<< HEAD
	struct pci_dev *pdev = adapter->pdev;
	struct e1000_hw *hw = &adapter->hw;
	struct pci_dev *pvfdev;
	unsigned int device_id;
	u16 thisvf_devfn;

	random_ether_addr(mac_addr);
	igb_set_vf_mac(adapter, vf, mac_addr);

	switch (adapter->hw.mac.type) {
	case e1000_82576:
		device_id = IGB_82576_VF_DEV_ID;
		/* VF Stride for 82576 is 2 */
		thisvf_devfn = (pdev->devfn + 0x80 + (vf << 1)) |
			(pdev->devfn & 1);
		break;
	case e1000_i350:
		device_id = IGB_I350_VF_DEV_ID;
		/* VF Stride for I350 is 4 */
		thisvf_devfn = (pdev->devfn + 0x80 + (vf << 2)) |
				(pdev->devfn & 3);
		break;
	default:
		device_id = 0;
		thisvf_devfn = 0;
		break;
	}

	pvfdev = pci_get_device(hw->vendor_id, device_id, NULL);
	while (pvfdev) {
		if (pvfdev->devfn == thisvf_devfn)
			break;
		pvfdev = pci_get_device(hw->vendor_id,
					device_id, pvfdev);
	}

	if (pvfdev)
		adapter->vf_data[vf].vfdev = pvfdev;
	else
		dev_err(&pdev->dev,
			"Couldn't find pci dev ptr for VF %4.4x\n",
			thisvf_devfn);
	return pvfdev != NULL;
}

static int igb_find_enabled_vfs(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	struct pci_dev *pdev = adapter->pdev;
	struct pci_dev *pvfdev;
	u16 vf_devfn = 0;
	u16 vf_stride;
	unsigned int device_id;
	int vfs_found = 0;

	switch (adapter->hw.mac.type) {
	case e1000_82576:
		device_id = IGB_82576_VF_DEV_ID;
		/* VF Stride for 82576 is 2 */
		vf_stride = 2;
		break;
	case e1000_i350:
		device_id = IGB_I350_VF_DEV_ID;
		/* VF Stride for I350 is 4 */
		vf_stride = 4;
		break;
	default:
		device_id = 0;
		vf_stride = 0;
		break;
	}

	vf_devfn = pdev->devfn + 0x80;
	pvfdev = pci_get_device(hw->vendor_id, device_id, NULL);
	while (pvfdev) {
		if (pvfdev->devfn == vf_devfn &&
		    (pvfdev->bus->number >= pdev->bus->number))
			vfs_found++;
		vf_devfn += vf_stride;
		pvfdev = pci_get_device(hw->vendor_id,
					device_id, pvfdev);
	}

	return vfs_found;
}

static int igb_check_vf_assignment(struct igb_adapter *adapter)
{
	int i;
	for (i = 0; i < adapter->vfs_allocated_count; i++) {
		if (adapter->vf_data[i].vfdev) {
			if (adapter->vf_data[i].vfdev->dev_flags &
			    PCI_DEV_FLAGS_ASSIGNED)
				return true;
		}
	}
	return false;
=======

	eth_zero_addr(mac_addr);
	igb_set_vf_mac(adapter, vf, mac_addr);

	/* By default spoof check is enabled for all VFs */
	adapter->vf_data[vf].spoofchk_enabled = true;

	/* By default VFs are not trusted */
	adapter->vf_data[vf].trusted = false;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif
static void igb_ping_all_vfs(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 ping;
	int i;

	for (i = 0 ; i < adapter->vfs_allocated_count; i++) {
		ping = E1000_PF_CONTROL_MSG;
		if (adapter->vf_data[i].flags & IGB_VF_FLAG_CTS)
			ping |= E1000_VT_MSGTYPE_CTS;
		igb_write_mbx(hw, &ping, 1, i);
	}
}

static int igb_set_vf_promisc(struct igb_adapter *adapter, u32 *msgbuf, u32 vf)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 vmolr = rd32(E1000_VMOLR(vf));
	struct vf_data_storage *vf_data = &adapter->vf_data[vf];

	vf_data->flags &= ~(IGB_VF_FLAG_UNI_PROMISC |
<<<<<<< HEAD
	                    IGB_VF_FLAG_MULTI_PROMISC);
=======
			    IGB_VF_FLAG_MULTI_PROMISC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vmolr &= ~(E1000_VMOLR_ROPE | E1000_VMOLR_ROMPE | E1000_VMOLR_MPME);

	if (*msgbuf & E1000_VF_SET_PROMISC_MULTICAST) {
		vmolr |= E1000_VMOLR_MPME;
		vf_data->flags |= IGB_VF_FLAG_MULTI_PROMISC;
		*msgbuf &= ~E1000_VF_SET_PROMISC_MULTICAST;
	} else {
<<<<<<< HEAD
		/*
		 * if we have hashes and we are clearing a multicast promisc
=======
		/* if we have hashes and we are clearing a multicast promisc
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * flag we need to write the hashes to the MTA as this step
		 * was previously skipped
		 */
		if (vf_data->num_vf_mc_hashes > 30) {
			vmolr |= E1000_VMOLR_MPME;
		} else if (vf_data->num_vf_mc_hashes) {
			int j;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			vmolr |= E1000_VMOLR_ROMPE;
			for (j = 0; j < vf_data->num_vf_mc_hashes; j++)
				igb_mta_set(hw, vf_data->vf_mc_hashes[j]);
		}
	}

	wr32(E1000_VMOLR(vf), vmolr);

	/* there are flags left unprocessed, likely not supported */
	if (*msgbuf & E1000_VT_MSGINFO_MASK)
		return -EINVAL;

	return 0;
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int igb_set_vf_multicasts(struct igb_adapter *adapter,
				  u32 *msgbuf, u32 vf)
{
<<<<<<< HEAD
	int n = (msgbuf[0] & E1000_VT_MSGINFO_MASK) >> E1000_VT_MSGINFO_SHIFT;
=======
	int n = FIELD_GET(E1000_VT_MSGINFO_MASK, msgbuf[0]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u16 *hash_list = (u16 *)&msgbuf[1];
	struct vf_data_storage *vf_data = &adapter->vf_data[vf];
	int i;

	/* salt away the number of multicast addresses assigned
	 * to this VF for later use to restore when the PF multi cast
	 * list changes
	 */
	vf_data->num_vf_mc_hashes = n;

	/* only up to 30 hash values supported */
	if (n > 30)
		n = 30;

	/* store the hashes for later use */
	for (i = 0; i < n; i++)
		vf_data->vf_mc_hashes[i] = hash_list[i];

	/* Flush and reset the mta with the new values */
	igb_set_rx_mode(adapter->netdev);

	return 0;
}

static void igb_restore_vf_multicasts(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	struct vf_data_storage *vf_data;
	int i, j;

	for (i = 0; i < adapter->vfs_allocated_count; i++) {
		u32 vmolr = rd32(E1000_VMOLR(i));
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		vmolr &= ~(E1000_VMOLR_ROMPE | E1000_VMOLR_MPME);

		vf_data = &adapter->vf_data[i];

		if ((vf_data->num_vf_mc_hashes > 30) ||
		    (vf_data->flags & IGB_VF_FLAG_MULTI_PROMISC)) {
			vmolr |= E1000_VMOLR_MPME;
		} else if (vf_data->num_vf_mc_hashes) {
			vmolr |= E1000_VMOLR_ROMPE;
			for (j = 0; j < vf_data->num_vf_mc_hashes; j++)
				igb_mta_set(hw, vf_data->vf_mc_hashes[j]);
		}
		wr32(E1000_VMOLR(i), vmolr);
	}
}

static void igb_clear_vf_vfta(struct igb_adapter *adapter, u32 vf)
{
	struct e1000_hw *hw = &adapter->hw;
<<<<<<< HEAD
	u32 pool_mask, reg, vid;
	int i;

	pool_mask = 1 << (E1000_VLVF_POOLSEL_SHIFT + vf);

	/* Find the vlan filter for this id */
	for (i = 0; i < E1000_VLVF_ARRAY_SIZE; i++) {
		reg = rd32(E1000_VLVF(i));

		/* remove the vf from the pool */
		reg &= ~pool_mask;

		/* if pool is empty then remove entry from vfta */
		if (!(reg & E1000_VLVF_POOLSEL_MASK) &&
		    (reg & E1000_VLVF_VLANID_ENABLE)) {
			reg = 0;
			vid = reg & E1000_VLVF_VLANID_MASK;
			igb_vfta_set(hw, vid, false);
		}

		wr32(E1000_VLVF(i), reg);
	}

	adapter->vf_data[vf].vlans_enabled = 0;
}

static s32 igb_vlvf_set(struct igb_adapter *adapter, u32 vid, bool add, u32 vf)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 reg, i;

	/* The vlvf table only exists on 82576 hardware and newer */
	if (hw->mac.type < e1000_82576)
		return -1;

	/* we only need to do this if VMDq is enabled */
	if (!adapter->vfs_allocated_count)
		return -1;

	/* Find the vlan filter for this id */
	for (i = 0; i < E1000_VLVF_ARRAY_SIZE; i++) {
		reg = rd32(E1000_VLVF(i));
		if ((reg & E1000_VLVF_VLANID_ENABLE) &&
		    vid == (reg & E1000_VLVF_VLANID_MASK))
			break;
	}

	if (add) {
		if (i == E1000_VLVF_ARRAY_SIZE) {
			/* Did not find a matching VLAN ID entry that was
			 * enabled.  Search for a free filter entry, i.e.
			 * one without the enable bit set
			 */
			for (i = 0; i < E1000_VLVF_ARRAY_SIZE; i++) {
				reg = rd32(E1000_VLVF(i));
				if (!(reg & E1000_VLVF_VLANID_ENABLE))
					break;
			}
		}
		if (i < E1000_VLVF_ARRAY_SIZE) {
			/* Found an enabled/available entry */
			reg |= 1 << (E1000_VLVF_POOLSEL_SHIFT + vf);

			/* if !enabled we need to set this up in vfta */
			if (!(reg & E1000_VLVF_VLANID_ENABLE)) {
				/* add VID to filter table */
				igb_vfta_set(hw, vid, true);
				reg |= E1000_VLVF_VLANID_ENABLE;
			}
			reg &= ~E1000_VLVF_VLANID_MASK;
			reg |= vid;
			wr32(E1000_VLVF(i), reg);

			/* do not modify RLPML for PF devices */
			if (vf >= adapter->vfs_allocated_count)
				return 0;

			if (!adapter->vf_data[vf].vlans_enabled) {
				u32 size;
				reg = rd32(E1000_VMOLR(vf));
				size = reg & E1000_VMOLR_RLPML_MASK;
				size += 4;
				reg &= ~E1000_VMOLR_RLPML_MASK;
				reg |= size;
				wr32(E1000_VMOLR(vf), reg);
			}

			adapter->vf_data[vf].vlans_enabled++;
		}
	} else {
		if (i < E1000_VLVF_ARRAY_SIZE) {
			/* remove vf from the pool */
			reg &= ~(1 << (E1000_VLVF_POOLSEL_SHIFT + vf));
			/* if pool is empty then remove entry from vfta */
			if (!(reg & E1000_VLVF_POOLSEL_MASK)) {
				reg = 0;
				igb_vfta_set(hw, vid, false);
			}
			wr32(E1000_VLVF(i), reg);

			/* do not modify RLPML for PF devices */
			if (vf >= adapter->vfs_allocated_count)
				return 0;

			adapter->vf_data[vf].vlans_enabled--;
			if (!adapter->vf_data[vf].vlans_enabled) {
				u32 size;
				reg = rd32(E1000_VMOLR(vf));
				size = reg & E1000_VMOLR_RLPML_MASK;
				size -= 4;
				reg &= ~E1000_VMOLR_RLPML_MASK;
				reg |= size;
				wr32(E1000_VMOLR(vf), reg);
			}
		}
	}
	return 0;
=======
	u32 pool_mask, vlvf_mask, i;

	/* create mask for VF and other pools */
	pool_mask = E1000_VLVF_POOLSEL_MASK;
	vlvf_mask = BIT(E1000_VLVF_POOLSEL_SHIFT + vf);

	/* drop PF from pool bits */
	pool_mask &= ~BIT(E1000_VLVF_POOLSEL_SHIFT +
			     adapter->vfs_allocated_count);

	/* Find the vlan filter for this id */
	for (i = E1000_VLVF_ARRAY_SIZE; i--;) {
		u32 vlvf = rd32(E1000_VLVF(i));
		u32 vfta_mask, vid, vfta;

		/* remove the vf from the pool */
		if (!(vlvf & vlvf_mask))
			continue;

		/* clear out bit from VLVF */
		vlvf ^= vlvf_mask;

		/* if other pools are present, just remove ourselves */
		if (vlvf & pool_mask)
			goto update_vlvfb;

		/* if PF is present, leave VFTA */
		if (vlvf & E1000_VLVF_POOLSEL_MASK)
			goto update_vlvf;

		vid = vlvf & E1000_VLVF_VLANID_MASK;
		vfta_mask = BIT(vid % 32);

		/* clear bit from VFTA */
		vfta = adapter->shadow_vfta[vid / 32];
		if (vfta & vfta_mask)
			hw->mac.ops.write_vfta(hw, vid / 32, vfta ^ vfta_mask);
update_vlvf:
		/* clear pool selection enable */
		if (adapter->flags & IGB_FLAG_VLAN_PROMISC)
			vlvf &= E1000_VLVF_POOLSEL_MASK;
		else
			vlvf = 0;
update_vlvfb:
		/* clear pool bits */
		wr32(E1000_VLVF(i), vlvf);
	}
}

static int igb_find_vlvf_entry(struct e1000_hw *hw, u32 vlan)
{
	u32 vlvf;
	int idx;

	/* short cut the special case */
	if (vlan == 0)
		return 0;

	/* Search for the VLAN id in the VLVF entries */
	for (idx = E1000_VLVF_ARRAY_SIZE; --idx;) {
		vlvf = rd32(E1000_VLVF(idx));
		if ((vlvf & VLAN_VID_MASK) == vlan)
			break;
	}

	return idx;
}

static void igb_update_pf_vlvf(struct igb_adapter *adapter, u32 vid)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 bits, pf_id;
	int idx;

	idx = igb_find_vlvf_entry(hw, vid);
	if (!idx)
		return;

	/* See if any other pools are set for this VLAN filter
	 * entry other than the PF.
	 */
	pf_id = adapter->vfs_allocated_count + E1000_VLVF_POOLSEL_SHIFT;
	bits = ~BIT(pf_id) & E1000_VLVF_POOLSEL_MASK;
	bits &= rd32(E1000_VLVF(idx));

	/* Disable the filter so this falls into the default pool. */
	if (!bits) {
		if (adapter->flags & IGB_FLAG_VLAN_PROMISC)
			wr32(E1000_VLVF(idx), BIT(pf_id));
		else
			wr32(E1000_VLVF(idx), 0);
	}
}

static s32 igb_set_vf_vlan(struct igb_adapter *adapter, u32 vid,
			   bool add, u32 vf)
{
	int pf_id = adapter->vfs_allocated_count;
	struct e1000_hw *hw = &adapter->hw;
	int err;

	/* If VLAN overlaps with one the PF is currently monitoring make
	 * sure that we are able to allocate a VLVF entry.  This may be
	 * redundant but it guarantees PF will maintain visibility to
	 * the VLAN.
	 */
	if (add && test_bit(vid, adapter->active_vlans)) {
		err = igb_vfta_set(hw, vid, pf_id, true, false);
		if (err)
			return err;
	}

	err = igb_vfta_set(hw, vid, vf, add, false);

	if (add && !err)
		return err;

	/* If we failed to add the VF VLAN or we are removing the VF VLAN
	 * we may need to drop the PF pool bit in order to allow us to free
	 * up the VLVF resources.
	 */
	if (test_bit(vid, adapter->active_vlans) ||
	    (adapter->flags & IGB_FLAG_VLAN_PROMISC))
		igb_update_pf_vlvf(adapter, vid);

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void igb_set_vmvir(struct igb_adapter *adapter, u32 vid, u32 vf)
{
	struct e1000_hw *hw = &adapter->hw;

	if (vid)
		wr32(E1000_VMVIR(vf), (vid | E1000_VMVIR_VLANA_DEFAULT));
	else
		wr32(E1000_VMVIR(vf), 0);
}

<<<<<<< HEAD
static int igb_ndo_set_vf_vlan(struct net_device *netdev,
			       int vf, u16 vlan, u8 qos)
{
	int err = 0;
=======
static int igb_enable_port_vlan(struct igb_adapter *adapter, int vf,
				u16 vlan, u8 qos)
{
	int err;

	err = igb_set_vf_vlan(adapter, vlan, true, vf);
	if (err)
		return err;

	igb_set_vmvir(adapter, vlan | (qos << VLAN_PRIO_SHIFT), vf);
	igb_set_vmolr(adapter, vf, !vlan);

	/* revoke access to previous VLAN */
	if (vlan != adapter->vf_data[vf].pf_vlan)
		igb_set_vf_vlan(adapter, adapter->vf_data[vf].pf_vlan,
				false, vf);

	adapter->vf_data[vf].pf_vlan = vlan;
	adapter->vf_data[vf].pf_qos = qos;
	igb_set_vf_vlan_strip(adapter, vf, true);
	dev_info(&adapter->pdev->dev,
		 "Setting VLAN %d, QOS 0x%x on VF %d\n", vlan, qos, vf);
	if (test_bit(__IGB_DOWN, &adapter->state)) {
		dev_warn(&adapter->pdev->dev,
			 "The VF VLAN has been set, but the PF device is not up.\n");
		dev_warn(&adapter->pdev->dev,
			 "Bring the PF device up before attempting to use the VF device.\n");
	}

	return err;
}

static int igb_disable_port_vlan(struct igb_adapter *adapter, int vf)
{
	/* Restore tagless access via VLAN 0 */
	igb_set_vf_vlan(adapter, 0, true, vf);

	igb_set_vmvir(adapter, 0, vf);
	igb_set_vmolr(adapter, vf, true);

	/* Remove any PF assigned VLAN */
	if (adapter->vf_data[vf].pf_vlan)
		igb_set_vf_vlan(adapter, adapter->vf_data[vf].pf_vlan,
				false, vf);

	adapter->vf_data[vf].pf_vlan = 0;
	adapter->vf_data[vf].pf_qos = 0;
	igb_set_vf_vlan_strip(adapter, vf, false);

	return 0;
}

static int igb_ndo_set_vf_vlan(struct net_device *netdev, int vf,
			       u16 vlan, u8 qos, __be16 vlan_proto)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct igb_adapter *adapter = netdev_priv(netdev);

	if ((vf >= adapter->vfs_allocated_count) || (vlan > 4095) || (qos > 7))
		return -EINVAL;
<<<<<<< HEAD
	if (vlan || qos) {
		err = igb_vlvf_set(adapter, vlan, !!vlan, vf);
		if (err)
			goto out;
		igb_set_vmvir(adapter, vlan | (qos << VLAN_PRIO_SHIFT), vf);
		igb_set_vmolr(adapter, vf, !vlan);
		adapter->vf_data[vf].pf_vlan = vlan;
		adapter->vf_data[vf].pf_qos = qos;
		dev_info(&adapter->pdev->dev,
			 "Setting VLAN %d, QOS 0x%x on VF %d\n", vlan, qos, vf);
		if (test_bit(__IGB_DOWN, &adapter->state)) {
			dev_warn(&adapter->pdev->dev,
				 "The VF VLAN has been set,"
				 " but the PF device is not up.\n");
			dev_warn(&adapter->pdev->dev,
				 "Bring the PF device up before"
				 " attempting to use the VF device.\n");
		}
	} else {
		igb_vlvf_set(adapter, adapter->vf_data[vf].pf_vlan,
				   false, vf);
		igb_set_vmvir(adapter, vlan, vf);
		igb_set_vmolr(adapter, vf, true);
		adapter->vf_data[vf].pf_vlan = 0;
		adapter->vf_data[vf].pf_qos = 0;
       }
out:
       return err;
}

static int igb_set_vf_vlan(struct igb_adapter *adapter, u32 *msgbuf, u32 vf)
{
	int add = (msgbuf[0] & E1000_VT_MSGINFO_MASK) >> E1000_VT_MSGINFO_SHIFT;
	int vid = (msgbuf[1] & E1000_VLVF_VLANID_MASK);

	return igb_vlvf_set(adapter, vid, add, vf);
=======

	if (vlan_proto != htons(ETH_P_8021Q))
		return -EPROTONOSUPPORT;

	return (vlan || qos) ? igb_enable_port_vlan(adapter, vf, vlan, qos) :
			       igb_disable_port_vlan(adapter, vf);
}

static int igb_set_vf_vlan_msg(struct igb_adapter *adapter, u32 *msgbuf, u32 vf)
{
	int add = FIELD_GET(E1000_VT_MSGINFO_MASK, msgbuf[0]);
	int vid = (msgbuf[1] & E1000_VLVF_VLANID_MASK);
	int ret;

	if (adapter->vf_data[vf].pf_vlan)
		return -1;

	/* VLAN 0 is a special case, don't allow it to be removed */
	if (!vid && !add)
		return 0;

	ret = igb_set_vf_vlan(adapter, vid, !!add, vf);
	if (!ret)
		igb_set_vf_vlan_strip(adapter, vf, !!vid);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void igb_vf_reset(struct igb_adapter *adapter, u32 vf)
{
<<<<<<< HEAD
	/* clear flags - except flag that indicates PF has set the MAC */
	adapter->vf_data[vf].flags &= IGB_VF_FLAG_PF_SET_MAC;
	adapter->vf_data[vf].last_nack = jiffies;

	/* reset offloads to defaults */
	igb_set_vmolr(adapter, vf, true);

	/* reset vlans for device */
	igb_clear_vf_vfta(adapter, vf);
	if (adapter->vf_data[vf].pf_vlan)
		igb_ndo_set_vf_vlan(adapter->netdev, vf,
				    adapter->vf_data[vf].pf_vlan,
				    adapter->vf_data[vf].pf_qos);
	else
		igb_clear_vf_vfta(adapter, vf);
=======
	struct vf_data_storage *vf_data = &adapter->vf_data[vf];

	/* clear flags - except flag that indicates PF has set the MAC */
	vf_data->flags &= IGB_VF_FLAG_PF_SET_MAC;
	vf_data->last_nack = jiffies;

	/* reset vlans for device */
	igb_clear_vf_vfta(adapter, vf);
	igb_set_vf_vlan(adapter, vf_data->pf_vlan, true, vf);
	igb_set_vmvir(adapter, vf_data->pf_vlan |
			       (vf_data->pf_qos << VLAN_PRIO_SHIFT), vf);
	igb_set_vmolr(adapter, vf, !vf_data->pf_vlan);
	igb_set_vf_vlan_strip(adapter, vf, !!(vf_data->pf_vlan));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* reset multicast table array for vf */
	adapter->vf_data[vf].num_vf_mc_hashes = 0;

	/* Flush and reset the mta with the new values */
	igb_set_rx_mode(adapter->netdev);
}

static void igb_vf_reset_event(struct igb_adapter *adapter, u32 vf)
{
	unsigned char *vf_mac = adapter->vf_data[vf].vf_mac_addresses;

<<<<<<< HEAD
	/* generate a new mac address as we were hotplug removed/added */
	if (!(adapter->vf_data[vf].flags & IGB_VF_FLAG_PF_SET_MAC))
		random_ether_addr(vf_mac);
=======
	/* clear mac address as we were hotplug removed/added */
	if (!(adapter->vf_data[vf].flags & IGB_VF_FLAG_PF_SET_MAC))
		eth_zero_addr(vf_mac);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* process remaining reset events */
	igb_vf_reset(adapter, vf);
}

static void igb_vf_reset_msg(struct igb_adapter *adapter, u32 vf)
{
	struct e1000_hw *hw = &adapter->hw;
	unsigned char *vf_mac = adapter->vf_data[vf].vf_mac_addresses;
<<<<<<< HEAD
	int rar_entry = hw->mac.rar_entry_count - (vf + 1);
	u32 reg, msgbuf[3];
=======
	u32 reg, msgbuf[3] = {};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 *addr = (u8 *)(&msgbuf[1]);

	/* process all the same items cleared in a function level reset */
	igb_vf_reset(adapter, vf);

	/* set vf mac address */
<<<<<<< HEAD
	igb_rar_set_qsel(adapter, vf_mac, rar_entry, vf);

	/* enable transmit and receive for vf */
	reg = rd32(E1000_VFTE);
	wr32(E1000_VFTE, reg | (1 << vf));
	reg = rd32(E1000_VFRE);
	wr32(E1000_VFRE, reg | (1 << vf));
=======
	igb_set_vf_mac(adapter, vf, vf_mac);

	/* enable transmit and receive for vf */
	reg = rd32(E1000_VFTE);
	wr32(E1000_VFTE, reg | BIT(vf));
	reg = rd32(E1000_VFRE);
	wr32(E1000_VFRE, reg | BIT(vf));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	adapter->vf_data[vf].flags |= IGB_VF_FLAG_CTS;

	/* reply to reset with ack and vf mac address */
<<<<<<< HEAD
	msgbuf[0] = E1000_VF_RESET | E1000_VT_MSGTYPE_ACK;
	memcpy(addr, vf_mac, 6);
	igb_write_mbx(hw, msgbuf, 3, vf);
}

static int igb_set_vf_mac_addr(struct igb_adapter *adapter, u32 *msg, int vf)
{
	/*
	 * The VF MAC Address is stored in a packed array of bytes
	 * starting at the second 32 bit word of the msg array
	 */
	unsigned char *addr = (char *)&msg[1];
	int err = -1;

	if (is_valid_ether_addr(addr))
		err = igb_set_vf_mac(adapter, vf, addr);

	return err;
=======
	if (!is_zero_ether_addr(vf_mac)) {
		msgbuf[0] = E1000_VF_RESET | E1000_VT_MSGTYPE_ACK;
		memcpy(addr, vf_mac, ETH_ALEN);
	} else {
		msgbuf[0] = E1000_VF_RESET | E1000_VT_MSGTYPE_NACK;
	}
	igb_write_mbx(hw, msgbuf, 3, vf);
}

static void igb_flush_mac_table(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	int i;

	for (i = 0; i < hw->mac.rar_entry_count; i++) {
		adapter->mac_table[i].state &= ~IGB_MAC_STATE_IN_USE;
		eth_zero_addr(adapter->mac_table[i].addr);
		adapter->mac_table[i].queue = 0;
		igb_rar_set_index(adapter, i);
	}
}

static int igb_available_rars(struct igb_adapter *adapter, u8 queue)
{
	struct e1000_hw *hw = &adapter->hw;
	/* do not count rar entries reserved for VFs MAC addresses */
	int rar_entries = hw->mac.rar_entry_count -
			  adapter->vfs_allocated_count;
	int i, count = 0;

	for (i = 0; i < rar_entries; i++) {
		/* do not count default entries */
		if (adapter->mac_table[i].state & IGB_MAC_STATE_DEFAULT)
			continue;

		/* do not count "in use" entries for different queues */
		if ((adapter->mac_table[i].state & IGB_MAC_STATE_IN_USE) &&
		    (adapter->mac_table[i].queue != queue))
			continue;

		count++;
	}

	return count;
}

/* Set default MAC address for the PF in the first RAR entry */
static void igb_set_default_mac_filter(struct igb_adapter *adapter)
{
	struct igb_mac_addr *mac_table = &adapter->mac_table[0];

	ether_addr_copy(mac_table->addr, adapter->hw.mac.addr);
	mac_table->queue = adapter->vfs_allocated_count;
	mac_table->state = IGB_MAC_STATE_DEFAULT | IGB_MAC_STATE_IN_USE;

	igb_rar_set_index(adapter, 0);
}

/* If the filter to be added and an already existing filter express
 * the same address and address type, it should be possible to only
 * override the other configurations, for example the queue to steer
 * traffic.
 */
static bool igb_mac_entry_can_be_used(const struct igb_mac_addr *entry,
				      const u8 *addr, const u8 flags)
{
	if (!(entry->state & IGB_MAC_STATE_IN_USE))
		return true;

	if ((entry->state & IGB_MAC_STATE_SRC_ADDR) !=
	    (flags & IGB_MAC_STATE_SRC_ADDR))
		return false;

	if (!ether_addr_equal(addr, entry->addr))
		return false;

	return true;
}

/* Add a MAC filter for 'addr' directing matching traffic to 'queue',
 * 'flags' is used to indicate what kind of match is made, match is by
 * default for the destination address, if matching by source address
 * is desired the flag IGB_MAC_STATE_SRC_ADDR can be used.
 */
static int igb_add_mac_filter_flags(struct igb_adapter *adapter,
				    const u8 *addr, const u8 queue,
				    const u8 flags)
{
	struct e1000_hw *hw = &adapter->hw;
	int rar_entries = hw->mac.rar_entry_count -
			  adapter->vfs_allocated_count;
	int i;

	if (is_zero_ether_addr(addr))
		return -EINVAL;

	/* Search for the first empty entry in the MAC table.
	 * Do not touch entries at the end of the table reserved for the VF MAC
	 * addresses.
	 */
	for (i = 0; i < rar_entries; i++) {
		if (!igb_mac_entry_can_be_used(&adapter->mac_table[i],
					       addr, flags))
			continue;

		ether_addr_copy(adapter->mac_table[i].addr, addr);
		adapter->mac_table[i].queue = queue;
		adapter->mac_table[i].state |= IGB_MAC_STATE_IN_USE | flags;

		igb_rar_set_index(adapter, i);
		return i;
	}

	return -ENOSPC;
}

static int igb_add_mac_filter(struct igb_adapter *adapter, const u8 *addr,
			      const u8 queue)
{
	return igb_add_mac_filter_flags(adapter, addr, queue, 0);
}

/* Remove a MAC filter for 'addr' directing matching traffic to
 * 'queue', 'flags' is used to indicate what kind of match need to be
 * removed, match is by default for the destination address, if
 * matching by source address is to be removed the flag
 * IGB_MAC_STATE_SRC_ADDR can be used.
 */
static int igb_del_mac_filter_flags(struct igb_adapter *adapter,
				    const u8 *addr, const u8 queue,
				    const u8 flags)
{
	struct e1000_hw *hw = &adapter->hw;
	int rar_entries = hw->mac.rar_entry_count -
			  adapter->vfs_allocated_count;
	int i;

	if (is_zero_ether_addr(addr))
		return -EINVAL;

	/* Search for matching entry in the MAC table based on given address
	 * and queue. Do not touch entries at the end of the table reserved
	 * for the VF MAC addresses.
	 */
	for (i = 0; i < rar_entries; i++) {
		if (!(adapter->mac_table[i].state & IGB_MAC_STATE_IN_USE))
			continue;
		if ((adapter->mac_table[i].state & flags) != flags)
			continue;
		if (adapter->mac_table[i].queue != queue)
			continue;
		if (!ether_addr_equal(adapter->mac_table[i].addr, addr))
			continue;

		/* When a filter for the default address is "deleted",
		 * we return it to its initial configuration
		 */
		if (adapter->mac_table[i].state & IGB_MAC_STATE_DEFAULT) {
			adapter->mac_table[i].state =
				IGB_MAC_STATE_DEFAULT | IGB_MAC_STATE_IN_USE;
			adapter->mac_table[i].queue =
				adapter->vfs_allocated_count;
		} else {
			adapter->mac_table[i].state = 0;
			adapter->mac_table[i].queue = 0;
			eth_zero_addr(adapter->mac_table[i].addr);
		}

		igb_rar_set_index(adapter, i);
		return 0;
	}

	return -ENOENT;
}

static int igb_del_mac_filter(struct igb_adapter *adapter, const u8 *addr,
			      const u8 queue)
{
	return igb_del_mac_filter_flags(adapter, addr, queue, 0);
}

int igb_add_mac_steering_filter(struct igb_adapter *adapter,
				const u8 *addr, u8 queue, u8 flags)
{
	struct e1000_hw *hw = &adapter->hw;

	/* In theory, this should be supported on 82575 as well, but
	 * that part wasn't easily accessible during development.
	 */
	if (hw->mac.type != e1000_i210)
		return -EOPNOTSUPP;

	return igb_add_mac_filter_flags(adapter, addr, queue,
					IGB_MAC_STATE_QUEUE_STEERING | flags);
}

int igb_del_mac_steering_filter(struct igb_adapter *adapter,
				const u8 *addr, u8 queue, u8 flags)
{
	return igb_del_mac_filter_flags(adapter, addr, queue,
					IGB_MAC_STATE_QUEUE_STEERING | flags);
}

static int igb_uc_sync(struct net_device *netdev, const unsigned char *addr)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	int ret;

	ret = igb_add_mac_filter(adapter, addr, adapter->vfs_allocated_count);

	return min_t(int, ret, 0);
}

static int igb_uc_unsync(struct net_device *netdev, const unsigned char *addr)
{
	struct igb_adapter *adapter = netdev_priv(netdev);

	igb_del_mac_filter(adapter, addr, adapter->vfs_allocated_count);

	return 0;
}

static int igb_set_vf_mac_filter(struct igb_adapter *adapter, const int vf,
				 const u32 info, const u8 *addr)
{
	struct pci_dev *pdev = adapter->pdev;
	struct vf_data_storage *vf_data = &adapter->vf_data[vf];
	struct vf_mac_filter *entry;
	bool found = false;
	int ret = 0;

	if ((vf_data->flags & IGB_VF_FLAG_PF_SET_MAC) &&
	    !vf_data->trusted) {
		dev_warn(&pdev->dev,
			 "VF %d requested MAC filter but is administratively denied\n",
			  vf);
		return -EINVAL;
	}
	if (!is_valid_ether_addr(addr)) {
		dev_warn(&pdev->dev,
			 "VF %d attempted to set invalid MAC filter\n",
			  vf);
		return -EINVAL;
	}

	switch (info) {
	case E1000_VF_MAC_FILTER_CLR:
		/* remove all unicast MAC filters related to the current VF */
		list_for_each_entry(entry, &adapter->vf_macs.l, l) {
			if (entry->vf == vf) {
				entry->vf = -1;
				entry->free = true;
				igb_del_mac_filter(adapter, entry->vf_mac, vf);
			}
		}
		break;
	case E1000_VF_MAC_FILTER_ADD:
		/* try to find empty slot in the list */
		list_for_each_entry(entry, &adapter->vf_macs.l, l) {
			if (entry->free) {
				found = true;
				break;
			}
		}

		if (found) {
			entry->free = false;
			entry->vf = vf;
			ether_addr_copy(entry->vf_mac, addr);

			ret = igb_add_mac_filter(adapter, addr, vf);
			ret = min_t(int, ret, 0);
		} else {
			ret = -ENOSPC;
		}

		if (ret == -ENOSPC)
			dev_warn(&pdev->dev,
				 "VF %d has requested MAC filter but there is no space for it\n",
				 vf);
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static int igb_set_vf_mac_addr(struct igb_adapter *adapter, u32 *msg, int vf)
{
	struct pci_dev *pdev = adapter->pdev;
	struct vf_data_storage *vf_data = &adapter->vf_data[vf];
	u32 info = msg[0] & E1000_VT_MSGINFO_MASK;

	/* The VF MAC Address is stored in a packed array of bytes
	 * starting at the second 32 bit word of the msg array
	 */
	unsigned char *addr = (unsigned char *)&msg[1];
	int ret = 0;

	if (!info) {
		if ((vf_data->flags & IGB_VF_FLAG_PF_SET_MAC) &&
		    !vf_data->trusted) {
			dev_warn(&pdev->dev,
				 "VF %d attempted to override administratively set MAC address\nReload the VF driver to resume operations\n",
				 vf);
			return -EINVAL;
		}

		if (!is_valid_ether_addr(addr)) {
			dev_warn(&pdev->dev,
				 "VF %d attempted to set invalid MAC\n",
				 vf);
			return -EINVAL;
		}

		ret = igb_set_vf_mac(adapter, vf, addr);
	} else {
		ret = igb_set_vf_mac_filter(adapter, vf, info, addr);
	}

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void igb_rcv_ack_from_vf(struct igb_adapter *adapter, u32 vf)
{
	struct e1000_hw *hw = &adapter->hw;
	struct vf_data_storage *vf_data = &adapter->vf_data[vf];
	u32 msg = E1000_VT_MSGTYPE_NACK;

	/* if device isn't clear to send it shouldn't be reading either */
	if (!(vf_data->flags & IGB_VF_FLAG_CTS) &&
	    time_after(jiffies, vf_data->last_nack + (2 * HZ))) {
		igb_write_mbx(hw, &msg, 1, vf);
		vf_data->last_nack = jiffies;
	}
}

static void igb_rcv_msg_from_vf(struct igb_adapter *adapter, u32 vf)
{
	struct pci_dev *pdev = adapter->pdev;
	u32 msgbuf[E1000_VFMAILBOX_SIZE];
	struct e1000_hw *hw = &adapter->hw;
	struct vf_data_storage *vf_data = &adapter->vf_data[vf];
	s32 retval;

<<<<<<< HEAD
	retval = igb_read_mbx(hw, msgbuf, E1000_VFMAILBOX_SIZE, vf);
=======
	retval = igb_read_mbx(hw, msgbuf, E1000_VFMAILBOX_SIZE, vf, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (retval) {
		/* if receive failed revoke VF CTS stats and restart init */
		dev_err(&pdev->dev, "Error receiving message from VF\n");
		vf_data->flags &= ~IGB_VF_FLAG_CTS;
		if (!time_after(jiffies, vf_data->last_nack + (2 * HZ)))
<<<<<<< HEAD
			return;
=======
			goto unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	/* this is a message we already processed, do nothing */
	if (msgbuf[0] & (E1000_VT_MSGTYPE_ACK | E1000_VT_MSGTYPE_NACK))
<<<<<<< HEAD
		return;

	/*
	 * until the vf completes a reset it should not be
	 * allowed to start any configuration.
	 */

	if (msgbuf[0] == E1000_VF_RESET) {
=======
		goto unlock;

	/* until the vf completes a reset it should not be
	 * allowed to start any configuration.
	 */
	if (msgbuf[0] == E1000_VF_RESET) {
		/* unlocks mailbox */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		igb_vf_reset_msg(adapter, vf);
		return;
	}

	if (!(vf_data->flags & IGB_VF_FLAG_CTS)) {
		if (!time_after(jiffies, vf_data->last_nack + (2 * HZ)))
<<<<<<< HEAD
			return;
=======
			goto unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		retval = -1;
		goto out;
	}

	switch ((msgbuf[0] & 0xFFFF)) {
	case E1000_VF_SET_MAC_ADDR:
<<<<<<< HEAD
		retval = -EINVAL;
		if (!(vf_data->flags & IGB_VF_FLAG_PF_SET_MAC))
			retval = igb_set_vf_mac_addr(adapter, msgbuf, vf);
		else
			dev_warn(&pdev->dev,
				 "VF %d attempted to override administratively "
				 "set MAC address\nReload the VF driver to "
				 "resume operations\n", vf);
=======
		retval = igb_set_vf_mac_addr(adapter, msgbuf, vf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case E1000_VF_SET_PROMISC:
		retval = igb_set_vf_promisc(adapter, msgbuf, vf);
		break;
	case E1000_VF_SET_MULTICAST:
		retval = igb_set_vf_multicasts(adapter, msgbuf, vf);
		break;
	case E1000_VF_SET_LPE:
		retval = igb_set_vf_rlpml(adapter, msgbuf[1], vf);
		break;
	case E1000_VF_SET_VLAN:
		retval = -1;
		if (vf_data->pf_vlan)
			dev_warn(&pdev->dev,
<<<<<<< HEAD
				 "VF %d attempted to override administratively "
				 "set VLAN tag\nReload the VF driver to "
				 "resume operations\n", vf);
		else
			retval = igb_set_vf_vlan(adapter, msgbuf, vf);
=======
				 "VF %d attempted to override administratively set VLAN tag\nReload the VF driver to resume operations\n",
				 vf);
		else
			retval = igb_set_vf_vlan_msg(adapter, msgbuf, vf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		dev_err(&pdev->dev, "Unhandled Msg %08x\n", msgbuf[0]);
		retval = -1;
		break;
	}

	msgbuf[0] |= E1000_VT_MSGTYPE_CTS;
out:
	/* notify the VF of the results of what it sent us */
	if (retval)
		msgbuf[0] |= E1000_VT_MSGTYPE_NACK;
	else
		msgbuf[0] |= E1000_VT_MSGTYPE_ACK;

<<<<<<< HEAD
	igb_write_mbx(hw, msgbuf, 1, vf);
=======
	/* unlocks mailbox */
	igb_write_mbx(hw, msgbuf, 1, vf);
	return;

unlock:
	igb_unlock_mbx(hw, vf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void igb_msg_task(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
<<<<<<< HEAD
	u32 vf;

=======
	unsigned long flags;
	u32 vf;

	spin_lock_irqsave(&adapter->vfs_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (vf = 0; vf < adapter->vfs_allocated_count; vf++) {
		/* process any reset requests */
		if (!igb_check_for_rst(hw, vf))
			igb_vf_reset_event(adapter, vf);

		/* process any messages pending */
		if (!igb_check_for_msg(hw, vf))
			igb_rcv_msg_from_vf(adapter, vf);

		/* process any acks */
		if (!igb_check_for_ack(hw, vf))
			igb_rcv_ack_from_vf(adapter, vf);
	}
<<<<<<< HEAD
=======
	spin_unlock_irqrestore(&adapter->vfs_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 *  igb_set_uta - Set unicast filter table address
 *  @adapter: board private structure
<<<<<<< HEAD
=======
 *  @set: boolean indicating if we are setting or clearing bits
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *  The unicast table address is a register array of 32-bit registers.
 *  The table is meant to be used in a way similar to how the MTA is used
 *  however due to certain limitations in the hardware it is necessary to
 *  set all the hash bits to 1 and use the VMOLR ROPE bit as a promiscuous
 *  enable bit to allow vlan tag stripping when promiscuous mode is enabled
 **/
<<<<<<< HEAD
static void igb_set_uta(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	int i;

	/* The UTA table only exists on 82576 hardware and newer */
	if (hw->mac.type < e1000_82576)
		return;

=======
static void igb_set_uta(struct igb_adapter *adapter, bool set)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 uta = set ? ~0 : 0;
	int i;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* we only need to do this if VMDq is enabled */
	if (!adapter->vfs_allocated_count)
		return;

<<<<<<< HEAD
	for (i = 0; i < hw->mac.uta_reg_count; i++)
		array_wr32(E1000_UTA, i, ~0);
}

/**
 * igb_intr_msi - Interrupt Handler
 * @irq: interrupt number
 * @data: pointer to a network interface device structure
=======
	for (i = hw->mac.uta_reg_count; i--;)
		array_wr32(E1000_UTA, i, uta);
}

/**
 *  igb_intr_msi - Interrupt Handler
 *  @irq: interrupt number
 *  @data: pointer to a network interface device structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static irqreturn_t igb_intr_msi(int irq, void *data)
{
	struct igb_adapter *adapter = data;
	struct igb_q_vector *q_vector = adapter->q_vector[0];
	struct e1000_hw *hw = &adapter->hw;
	/* read ICR disables interrupts using IAM */
	u32 icr = rd32(E1000_ICR);

	igb_write_itr(q_vector);

	if (icr & E1000_ICR_DRSTA)
		schedule_work(&adapter->reset_task);

	if (icr & E1000_ICR_DOUTSYNC) {
		/* HW is reporting DMA is out of sync */
		adapter->stats.doosync++;
	}

	if (icr & (E1000_ICR_RXSEQ | E1000_ICR_LSC)) {
		hw->mac.get_link_status = 1;
		if (!test_bit(__IGB_DOWN, &adapter->state))
			mod_timer(&adapter->watchdog_timer, jiffies + 1);
	}

<<<<<<< HEAD
=======
	if (icr & E1000_ICR_TS)
		igb_tsync_interrupt(adapter);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	napi_schedule(&q_vector->napi);

	return IRQ_HANDLED;
}

/**
<<<<<<< HEAD
 * igb_intr - Legacy Interrupt Handler
 * @irq: interrupt number
 * @data: pointer to a network interface device structure
=======
 *  igb_intr - Legacy Interrupt Handler
 *  @irq: interrupt number
 *  @data: pointer to a network interface device structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static irqreturn_t igb_intr(int irq, void *data)
{
	struct igb_adapter *adapter = data;
	struct igb_q_vector *q_vector = adapter->q_vector[0];
	struct e1000_hw *hw = &adapter->hw;
	/* Interrupt Auto-Mask...upon reading ICR, interrupts are masked.  No
<<<<<<< HEAD
	 * need for the IMC write */
	u32 icr = rd32(E1000_ICR);

	/* IMS will not auto-mask if INT_ASSERTED is not set, and if it is
	 * not set, then the adapter didn't send an interrupt */
=======
	 * need for the IMC write
	 */
	u32 icr = rd32(E1000_ICR);

	/* IMS will not auto-mask if INT_ASSERTED is not set, and if it is
	 * not set, then the adapter didn't send an interrupt
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!(icr & E1000_ICR_INT_ASSERTED))
		return IRQ_NONE;

	igb_write_itr(q_vector);

	if (icr & E1000_ICR_DRSTA)
		schedule_work(&adapter->reset_task);

	if (icr & E1000_ICR_DOUTSYNC) {
		/* HW is reporting DMA is out of sync */
		adapter->stats.doosync++;
	}

	if (icr & (E1000_ICR_RXSEQ | E1000_ICR_LSC)) {
		hw->mac.get_link_status = 1;
		/* guard against interrupt when we're going down */
		if (!test_bit(__IGB_DOWN, &adapter->state))
			mod_timer(&adapter->watchdog_timer, jiffies + 1);
	}

<<<<<<< HEAD
=======
	if (icr & E1000_ICR_TS)
		igb_tsync_interrupt(adapter);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	napi_schedule(&q_vector->napi);

	return IRQ_HANDLED;
}

static void igb_ring_irq_enable(struct igb_q_vector *q_vector)
{
	struct igb_adapter *adapter = q_vector->adapter;
	struct e1000_hw *hw = &adapter->hw;

	if ((q_vector->rx.ring && (adapter->rx_itr_setting & 3)) ||
	    (!q_vector->rx.ring && (adapter->tx_itr_setting & 3))) {
		if ((adapter->num_q_vectors == 1) && !adapter->vf_data)
			igb_set_itr(q_vector);
		else
			igb_update_ring_itr(q_vector);
	}

	if (!test_bit(__IGB_DOWN, &adapter->state)) {
<<<<<<< HEAD
		if (adapter->msix_entries)
=======
		if (adapter->flags & IGB_FLAG_HAS_MSIX)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			wr32(E1000_EIMS, q_vector->eims_value);
		else
			igb_irq_enable(adapter);
	}
}

/**
<<<<<<< HEAD
 * igb_poll - NAPI Rx polling callback
 * @napi: napi polling structure
 * @budget: count of how many packets we should handle
=======
 *  igb_poll - NAPI Rx polling callback
 *  @napi: napi polling structure
 *  @budget: count of how many packets we should handle
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static int igb_poll(struct napi_struct *napi, int budget)
{
	struct igb_q_vector *q_vector = container_of(napi,
<<<<<<< HEAD
	                                             struct igb_q_vector,
	                                             napi);
	bool clean_complete = true;
=======
						     struct igb_q_vector,
						     napi);
	bool clean_complete = true;
	int work_done = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_IGB_DCA
	if (q_vector->adapter->flags & IGB_FLAG_DCA_ENABLED)
		igb_update_dca(q_vector);
#endif
	if (q_vector->tx.ring)
<<<<<<< HEAD
		clean_complete = igb_clean_tx_irq(q_vector);

	if (q_vector->rx.ring)
		clean_complete &= igb_clean_rx_irq(q_vector, budget);
=======
		clean_complete = igb_clean_tx_irq(q_vector, budget);

	if (q_vector->rx.ring) {
		int cleaned = igb_clean_rx_irq(q_vector, budget);

		work_done += cleaned;
		if (cleaned >= budget)
			clean_complete = false;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If all work not completed, return budget and keep polling */
	if (!clean_complete)
		return budget;

<<<<<<< HEAD
	/* If not enough Rx work done, exit the polling mode */
	napi_complete(napi);
	igb_ring_irq_enable(q_vector);

	return 0;
}

/**
 * igb_systim_to_hwtstamp - convert system time value to hw timestamp
 * @adapter: board private structure
 * @shhwtstamps: timestamp structure to update
 * @regval: unsigned 64bit system time value.
 *
 * We need to convert the system time value stored in the RX/TXSTMP registers
 * into a hwtstamp which can be used by the upper level timestamping functions
 */
static void igb_systim_to_hwtstamp(struct igb_adapter *adapter,
                                   struct skb_shared_hwtstamps *shhwtstamps,
                                   u64 regval)
{
	u64 ns;

	/*
	 * The 82580 starts with 1ns at bit 0 in RX/TXSTMPL, shift this up to
	 * 24 to match clock shift we setup earlier.
	 */
	if (adapter->hw.mac.type >= e1000_82580)
		regval <<= IGB_82580_TSYNC_SHIFT;

	ns = timecounter_cyc2time(&adapter->clock, regval);
	timecompare_update(&adapter->compare, ns);
	memset(shhwtstamps, 0, sizeof(struct skb_shared_hwtstamps));
	shhwtstamps->hwtstamp = ns_to_ktime(ns);
	shhwtstamps->syststamp = timecompare_transform(&adapter->compare, ns);
}

/**
 * igb_tx_hwtstamp - utility function which checks for TX time stamp
 * @q_vector: pointer to q_vector containing needed info
 * @buffer: pointer to igb_tx_buffer structure
 *
 * If we were asked to do hardware stamping and such a time stamp is
 * available, then it must have been for this skb here because we only
 * allow only one such packet into the queue.
 */
static void igb_tx_hwtstamp(struct igb_q_vector *q_vector,
			    struct igb_tx_buffer *buffer_info)
{
	struct igb_adapter *adapter = q_vector->adapter;
	struct e1000_hw *hw = &adapter->hw;
	struct skb_shared_hwtstamps shhwtstamps;
	u64 regval;

	/* if skb does not support hw timestamp or TX stamp not valid exit */
	if (likely(!(buffer_info->tx_flags & IGB_TX_FLAGS_TSTAMP)) ||
	    !(rd32(E1000_TSYNCTXCTL) & E1000_TSYNCTXCTL_VALID))
		return;

	regval = rd32(E1000_TXSTMPL);
	regval |= (u64)rd32(E1000_TXSTMPH) << 32;

	igb_systim_to_hwtstamp(adapter, &shhwtstamps, regval);
	skb_tstamp_tx(buffer_info->skb, &shhwtstamps);
}

/**
 * igb_clean_tx_irq - Reclaim resources after transmit completes
 * @q_vector: pointer to q_vector containing needed info
 * returns true if ring is completely cleaned
 **/
static bool igb_clean_tx_irq(struct igb_q_vector *q_vector)
=======
	/* Exit the polling mode, but don't re-enable interrupts if stack might
	 * poll us due to busy-polling
	 */
	if (likely(napi_complete_done(napi, work_done)))
		igb_ring_irq_enable(q_vector);

	return work_done;
}

/**
 *  igb_clean_tx_irq - Reclaim resources after transmit completes
 *  @q_vector: pointer to q_vector containing needed info
 *  @napi_budget: Used to determine if we are in netpoll
 *
 *  returns true if ring is completely cleaned
 **/
static bool igb_clean_tx_irq(struct igb_q_vector *q_vector, int napi_budget)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct igb_adapter *adapter = q_vector->adapter;
	struct igb_ring *tx_ring = q_vector->tx.ring;
	struct igb_tx_buffer *tx_buffer;
<<<<<<< HEAD
	union e1000_adv_tx_desc *tx_desc, *eop_desc;
=======
	union e1000_adv_tx_desc *tx_desc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int total_bytes = 0, total_packets = 0;
	unsigned int budget = q_vector->tx.work_limit;
	unsigned int i = tx_ring->next_to_clean;

	if (test_bit(__IGB_DOWN, &adapter->state))
		return true;

	tx_buffer = &tx_ring->tx_buffer_info[i];
	tx_desc = IGB_TX_DESC(tx_ring, i);
	i -= tx_ring->count;

<<<<<<< HEAD
	for (; budget; budget--) {
		eop_desc = tx_buffer->next_to_watch;

		/* prevent any other reads prior to eop_desc */
		rmb();
=======
	do {
		union e1000_adv_tx_desc *eop_desc = tx_buffer->next_to_watch;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* if next_to_watch is not set then there is no work pending */
		if (!eop_desc)
			break;

<<<<<<< HEAD
=======
		/* prevent any other reads prior to eop_desc */
		smp_rmb();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* if DD is not set pending work has not been completed */
		if (!(eop_desc->wb.status & cpu_to_le32(E1000_TXD_STAT_DD)))
			break;

		/* clear next_to_watch to prevent false hangs */
		tx_buffer->next_to_watch = NULL;

		/* update the statistics for this packet */
		total_bytes += tx_buffer->bytecount;
		total_packets += tx_buffer->gso_segs;

<<<<<<< HEAD
		/* retrieve hardware timestamp */
		igb_tx_hwtstamp(q_vector, tx_buffer);

		/* free the skb */
		dev_kfree_skb_any(tx_buffer->skb);
		tx_buffer->skb = NULL;

		/* unmap skb header data */
		dma_unmap_single(tx_ring->dev,
				 tx_buffer->dma,
				 tx_buffer->length,
				 DMA_TO_DEVICE);

		/* clear last DMA location and unmap remaining buffers */
		while (tx_desc != eop_desc) {
			tx_buffer->dma = 0;

=======
		/* free the skb */
		if (tx_buffer->type == IGB_TYPE_SKB)
			napi_consume_skb(tx_buffer->skb, napi_budget);
		else
			xdp_return_frame(tx_buffer->xdpf);

		/* unmap skb header data */
		dma_unmap_single(tx_ring->dev,
				 dma_unmap_addr(tx_buffer, dma),
				 dma_unmap_len(tx_buffer, len),
				 DMA_TO_DEVICE);

		/* clear tx_buffer data */
		dma_unmap_len_set(tx_buffer, len, 0);

		/* clear last DMA location and unmap remaining buffers */
		while (tx_desc != eop_desc) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			tx_buffer++;
			tx_desc++;
			i++;
			if (unlikely(!i)) {
				i -= tx_ring->count;
				tx_buffer = tx_ring->tx_buffer_info;
				tx_desc = IGB_TX_DESC(tx_ring, 0);
			}

			/* unmap any remaining paged data */
<<<<<<< HEAD
			if (tx_buffer->dma) {
				dma_unmap_page(tx_ring->dev,
					       tx_buffer->dma,
					       tx_buffer->length,
					       DMA_TO_DEVICE);
			}
		}

		/* clear last DMA location */
		tx_buffer->dma = 0;

=======
			if (dma_unmap_len(tx_buffer, len)) {
				dma_unmap_page(tx_ring->dev,
					       dma_unmap_addr(tx_buffer, dma),
					       dma_unmap_len(tx_buffer, len),
					       DMA_TO_DEVICE);
				dma_unmap_len_set(tx_buffer, len, 0);
			}
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* move us one more past the eop_desc for start of next pkt */
		tx_buffer++;
		tx_desc++;
		i++;
		if (unlikely(!i)) {
			i -= tx_ring->count;
			tx_buffer = tx_ring->tx_buffer_info;
			tx_desc = IGB_TX_DESC(tx_ring, 0);
		}
<<<<<<< HEAD
	}
=======

		/* issue prefetch for next Tx descriptor */
		prefetch(tx_desc);

		/* update budget accounting */
		budget--;
	} while (likely(budget));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	netdev_tx_completed_queue(txring_txq(tx_ring),
				  total_packets, total_bytes);
	i += tx_ring->count;
	tx_ring->next_to_clean = i;
	u64_stats_update_begin(&tx_ring->tx_syncp);
	tx_ring->tx_stats.bytes += total_bytes;
	tx_ring->tx_stats.packets += total_packets;
	u64_stats_update_end(&tx_ring->tx_syncp);
	q_vector->tx.total_bytes += total_bytes;
	q_vector->tx.total_packets += total_packets;

	if (test_bit(IGB_RING_FLAG_TX_DETECT_HANG, &tx_ring->flags)) {
		struct e1000_hw *hw = &adapter->hw;

<<<<<<< HEAD
		eop_desc = tx_buffer->next_to_watch;

		/* Detect a transmit hang in hardware, this serializes the
		 * check with the clearing of time_stamp and movement of i */
		clear_bit(IGB_RING_FLAG_TX_DETECT_HANG, &tx_ring->flags);
		if (eop_desc &&
=======
		/* Detect a transmit hang in hardware, this serializes the
		 * check with the clearing of time_stamp and movement of i
		 */
		clear_bit(IGB_RING_FLAG_TX_DETECT_HANG, &tx_ring->flags);
		if (tx_buffer->next_to_watch &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    time_after(jiffies, tx_buffer->time_stamp +
			       (adapter->tx_timeout_factor * HZ)) &&
		    !(rd32(E1000_STATUS) & E1000_STATUS_TXOFF)) {

			/* detected Tx unit hang */
			dev_err(tx_ring->dev,
				"Detected Tx Unit Hang\n"
				"  Tx Queue             <%d>\n"
				"  TDH                  <%x>\n"
				"  TDT                  <%x>\n"
				"  next_to_use          <%x>\n"
				"  next_to_clean        <%x>\n"
				"buffer_info[next_to_clean]\n"
				"  time_stamp           <%lx>\n"
				"  next_to_watch        <%p>\n"
				"  jiffies              <%lx>\n"
				"  desc.status          <%x>\n",
				tx_ring->queue_index,
				rd32(E1000_TDH(tx_ring->reg_idx)),
				readl(tx_ring->tail),
				tx_ring->next_to_use,
				tx_ring->next_to_clean,
				tx_buffer->time_stamp,
<<<<<<< HEAD
				eop_desc,
				jiffies,
				eop_desc->wb.status);
=======
				tx_buffer->next_to_watch,
				jiffies,
				tx_buffer->next_to_watch->wb.status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			netif_stop_subqueue(tx_ring->netdev,
					    tx_ring->queue_index);

			/* we are about to reset, no point in enabling stuff */
			return true;
		}
	}

<<<<<<< HEAD
	if (unlikely(total_packets &&
		     netif_carrier_ok(tx_ring->netdev) &&
		     igb_desc_unused(tx_ring) >= IGB_TX_QUEUE_WAKE)) {
=======
#define TX_WAKE_THRESHOLD (DESC_NEEDED * 2)
	if (unlikely(total_packets &&
	    netif_carrier_ok(tx_ring->netdev) &&
	    igb_desc_unused(tx_ring) >= TX_WAKE_THRESHOLD)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Make sure that anybody stopping the queue after this
		 * sees the new next_to_clean.
		 */
		smp_mb();
		if (__netif_subqueue_stopped(tx_ring->netdev,
					     tx_ring->queue_index) &&
		    !(test_bit(__IGB_DOWN, &adapter->state))) {
			netif_wake_subqueue(tx_ring->netdev,
					    tx_ring->queue_index);

			u64_stats_update_begin(&tx_ring->tx_syncp);
			tx_ring->tx_stats.restart_queue++;
			u64_stats_update_end(&tx_ring->tx_syncp);
		}
	}

	return !!budget;
}

<<<<<<< HEAD
=======
/**
 *  igb_reuse_rx_page - page flip buffer and store it back on the ring
 *  @rx_ring: rx descriptor ring to store buffers on
 *  @old_buff: donor buffer to have page reused
 *
 *  Synchronizes page for reuse by the adapter
 **/
static void igb_reuse_rx_page(struct igb_ring *rx_ring,
			      struct igb_rx_buffer *old_buff)
{
	struct igb_rx_buffer *new_buff;
	u16 nta = rx_ring->next_to_alloc;

	new_buff = &rx_ring->rx_buffer_info[nta];

	/* update, and store next to alloc */
	nta++;
	rx_ring->next_to_alloc = (nta < rx_ring->count) ? nta : 0;

	/* Transfer page from old buffer to new buffer.
	 * Move each member individually to avoid possible store
	 * forwarding stalls.
	 */
	new_buff->dma		= old_buff->dma;
	new_buff->page		= old_buff->page;
	new_buff->page_offset	= old_buff->page_offset;
	new_buff->pagecnt_bias	= old_buff->pagecnt_bias;
}

static bool igb_can_reuse_rx_page(struct igb_rx_buffer *rx_buffer,
				  int rx_buf_pgcnt)
{
	unsigned int pagecnt_bias = rx_buffer->pagecnt_bias;
	struct page *page = rx_buffer->page;

	/* avoid re-using remote and pfmemalloc pages */
	if (!dev_page_is_reusable(page))
		return false;

#if (PAGE_SIZE < 8192)
	/* if we are only owner of page we can reuse it */
	if (unlikely((rx_buf_pgcnt - pagecnt_bias) > 1))
		return false;
#else
#define IGB_LAST_OFFSET \
	(SKB_WITH_OVERHEAD(PAGE_SIZE) - IGB_RXBUFFER_2048)

	if (rx_buffer->page_offset > IGB_LAST_OFFSET)
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
}

/**
 *  igb_add_rx_frag - Add contents of Rx buffer to sk_buff
 *  @rx_ring: rx descriptor ring to transact packets on
 *  @rx_buffer: buffer containing page to add
 *  @skb: sk_buff to place the data into
 *  @size: size of buffer to be added
 *
 *  This function will add the data contained in rx_buffer->page to the skb.
 **/
static void igb_add_rx_frag(struct igb_ring *rx_ring,
			    struct igb_rx_buffer *rx_buffer,
			    struct sk_buff *skb,
			    unsigned int size)
{
#if (PAGE_SIZE < 8192)
	unsigned int truesize = igb_rx_pg_size(rx_ring) / 2;
#else
	unsigned int truesize = ring_uses_build_skb(rx_ring) ?
				SKB_DATA_ALIGN(IGB_SKB_PAD + size) :
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

static struct sk_buff *igb_construct_skb(struct igb_ring *rx_ring,
					 struct igb_rx_buffer *rx_buffer,
					 struct xdp_buff *xdp,
					 ktime_t timestamp)
{
#if (PAGE_SIZE < 8192)
	unsigned int truesize = igb_rx_pg_size(rx_ring) / 2;
#else
	unsigned int truesize = SKB_DATA_ALIGN(xdp->data_end -
					       xdp->data_hard_start);
#endif
	unsigned int size = xdp->data_end - xdp->data;
	unsigned int headlen;
	struct sk_buff *skb;

	/* prefetch first cache line of first page */
	net_prefetch(xdp->data);

	/* allocate a skb to store the frags */
	skb = napi_alloc_skb(&rx_ring->q_vector->napi, IGB_RX_HDR_LEN);
	if (unlikely(!skb))
		return NULL;

	if (timestamp)
		skb_hwtstamps(skb)->hwtstamp = timestamp;

	/* Determine available headroom for copy */
	headlen = size;
	if (headlen > IGB_RX_HDR_LEN)
		headlen = eth_get_headlen(skb->dev, xdp->data, IGB_RX_HDR_LEN);

	/* align pull length to size of long to optimize memcpy performance */
	memcpy(__skb_put(skb, headlen), xdp->data, ALIGN(headlen, sizeof(long)));

	/* update all of the pointers */
	size -= headlen;
	if (size) {
		skb_add_rx_frag(skb, 0, rx_buffer->page,
				(xdp->data + headlen) - page_address(rx_buffer->page),
				size, truesize);
#if (PAGE_SIZE < 8192)
		rx_buffer->page_offset ^= truesize;
#else
		rx_buffer->page_offset += truesize;
#endif
	} else {
		rx_buffer->pagecnt_bias++;
	}

	return skb;
}

static struct sk_buff *igb_build_skb(struct igb_ring *rx_ring,
				     struct igb_rx_buffer *rx_buffer,
				     struct xdp_buff *xdp,
				     ktime_t timestamp)
{
#if (PAGE_SIZE < 8192)
	unsigned int truesize = igb_rx_pg_size(rx_ring) / 2;
#else
	unsigned int truesize = SKB_DATA_ALIGN(sizeof(struct skb_shared_info)) +
				SKB_DATA_ALIGN(xdp->data_end -
					       xdp->data_hard_start);
#endif
	unsigned int metasize = xdp->data - xdp->data_meta;
	struct sk_buff *skb;

	/* prefetch first cache line of first page */
	net_prefetch(xdp->data_meta);

	/* build an skb around the page buffer */
	skb = napi_build_skb(xdp->data_hard_start, truesize);
	if (unlikely(!skb))
		return NULL;

	/* update pointers within the skb to store the data */
	skb_reserve(skb, xdp->data - xdp->data_hard_start);
	__skb_put(skb, xdp->data_end - xdp->data);

	if (metasize)
		skb_metadata_set(skb, metasize);

	if (timestamp)
		skb_hwtstamps(skb)->hwtstamp = timestamp;

	/* update buffer offset */
#if (PAGE_SIZE < 8192)
	rx_buffer->page_offset ^= truesize;
#else
	rx_buffer->page_offset += truesize;
#endif

	return skb;
}

static struct sk_buff *igb_run_xdp(struct igb_adapter *adapter,
				   struct igb_ring *rx_ring,
				   struct xdp_buff *xdp)
{
	int err, result = IGB_XDP_PASS;
	struct bpf_prog *xdp_prog;
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
		result = igb_xdp_xmit_back(adapter, xdp);
		if (result == IGB_XDP_CONSUMED)
			goto out_failure;
		break;
	case XDP_REDIRECT:
		err = xdp_do_redirect(adapter->netdev, xdp, xdp_prog);
		if (err)
			goto out_failure;
		result = IGB_XDP_REDIR;
		break;
	default:
		bpf_warn_invalid_xdp_action(adapter->netdev, xdp_prog, act);
		fallthrough;
	case XDP_ABORTED:
out_failure:
		trace_xdp_exception(rx_ring->netdev, xdp_prog, act);
		fallthrough;
	case XDP_DROP:
		result = IGB_XDP_CONSUMED;
		break;
	}
xdp_out:
	return ERR_PTR(-result);
}

static unsigned int igb_rx_frame_truesize(struct igb_ring *rx_ring,
					  unsigned int size)
{
	unsigned int truesize;

#if (PAGE_SIZE < 8192)
	truesize = igb_rx_pg_size(rx_ring) / 2; /* Must be power-of-2 */
#else
	truesize = ring_uses_build_skb(rx_ring) ?
		SKB_DATA_ALIGN(IGB_SKB_PAD + size) +
		SKB_DATA_ALIGN(sizeof(struct skb_shared_info)) :
		SKB_DATA_ALIGN(size);
#endif
	return truesize;
}

static void igb_rx_buffer_flip(struct igb_ring *rx_ring,
			       struct igb_rx_buffer *rx_buffer,
			       unsigned int size)
{
	unsigned int truesize = igb_rx_frame_truesize(rx_ring, size);
#if (PAGE_SIZE < 8192)
	rx_buffer->page_offset ^= truesize;
#else
	rx_buffer->page_offset += truesize;
#endif
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void igb_rx_checksum(struct igb_ring *ring,
				   union e1000_adv_rx_desc *rx_desc,
				   struct sk_buff *skb)
{
	skb_checksum_none_assert(skb);

	/* Ignore Checksum bit is set */
	if (igb_test_staterr(rx_desc, E1000_RXD_STAT_IXSM))
		return;

	/* Rx checksum disabled via ethtool */
	if (!(ring->netdev->features & NETIF_F_RXCSUM))
		return;

	/* TCP/UDP checksum error bit is set */
	if (igb_test_staterr(rx_desc,
			     E1000_RXDEXT_STATERR_TCPE |
			     E1000_RXDEXT_STATERR_IPE)) {
<<<<<<< HEAD
		/*
		 * work around errata with sctp packets where the TCPE aka
=======
		/* work around errata with sctp packets where the TCPE aka
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * L4E bit is set incorrectly on 64 byte (60 byte w/o crc)
		 * packets, (aka let the stack check the crc32c)
		 */
		if (!((skb->len == 60) &&
		      test_bit(IGB_RING_FLAG_RX_SCTP_CSUM, &ring->flags))) {
			u64_stats_update_begin(&ring->rx_syncp);
			ring->rx_stats.csum_err++;
			u64_stats_update_end(&ring->rx_syncp);
		}
		/* let the stack verify checksum errors */
		return;
	}
	/* It must be a TCP or UDP packet with a valid checksum */
	if (igb_test_staterr(rx_desc, E1000_RXD_STAT_TCPCS |
				      E1000_RXD_STAT_UDPCS))
		skb->ip_summed = CHECKSUM_UNNECESSARY;

	dev_dbg(ring->dev, "cksum success: bits %08X\n",
		le32_to_cpu(rx_desc->wb.upper.status_error));
}

static inline void igb_rx_hash(struct igb_ring *ring,
			       union e1000_adv_rx_desc *rx_desc,
			       struct sk_buff *skb)
{
	if (ring->netdev->features & NETIF_F_RXHASH)
<<<<<<< HEAD
		skb->rxhash = le32_to_cpu(rx_desc->wb.lower.hi_dword.rss);
}

static void igb_rx_hwtstamp(struct igb_q_vector *q_vector,
			    union e1000_adv_rx_desc *rx_desc,
			    struct sk_buff *skb)
{
	struct igb_adapter *adapter = q_vector->adapter;
	struct e1000_hw *hw = &adapter->hw;
	u64 regval;

	if (!igb_test_staterr(rx_desc, E1000_RXDADV_STAT_TSIP |
				       E1000_RXDADV_STAT_TS))
		return;

	/*
	 * If this bit is set, then the RX registers contain the time stamp. No
	 * other packet will be time stamped until we read these registers, so
	 * read the registers to make them available again. Because only one
	 * packet can be time stamped at a time, we know that the register
	 * values must belong to this one here and therefore we don't need to
	 * compare any of the additional attributes stored for it.
	 *
	 * If nothing went wrong, then it should have a shared tx_flags that we
	 * can turn into a skb_shared_hwtstamps.
	 */
	if (igb_test_staterr(rx_desc, E1000_RXDADV_STAT_TSIP)) {
		u32 *stamp = (u32 *)skb->data;
		regval = le32_to_cpu(*(stamp + 2));
		regval |= (u64)le32_to_cpu(*(stamp + 3)) << 32;
		skb_pull(skb, IGB_TS_HDR_LEN);
	} else {
		if(!(rd32(E1000_TSYNCRXCTL) & E1000_TSYNCRXCTL_VALID))
			return;

		regval = rd32(E1000_RXSTMPL);
		regval |= (u64)rd32(E1000_RXSTMPH) << 32;
	}

	igb_systim_to_hwtstamp(adapter, skb_hwtstamps(skb), regval);
}

static void igb_rx_vlan(struct igb_ring *ring,
			union e1000_adv_rx_desc *rx_desc,
			struct sk_buff *skb)
{
	if (igb_test_staterr(rx_desc, E1000_RXD_STAT_VP)) {
		u16 vid;
		if (igb_test_staterr(rx_desc, E1000_RXDEXT_STATERR_LB) &&
		    test_bit(IGB_RING_FLAG_RX_LB_VLAN_BSWAP, &ring->flags))
			vid = be16_to_cpu(rx_desc->wb.upper.vlan);
		else
			vid = le16_to_cpu(rx_desc->wb.upper.vlan);

		__vlan_hwaccel_put_tag(skb, vid);
	}
}

static inline u16 igb_get_hlen(union e1000_adv_rx_desc *rx_desc)
{
	/* HW will not DMA in data larger than the given buffer, even if it
	 * parses the (NFS, of course) header to be larger.  In that case, it
	 * fills the header buffer and spills the rest into the page.
	 */
	u16 hlen = (le16_to_cpu(rx_desc->wb.lower.lo_dword.hdr_info) &
	           E1000_RXDADV_HDRBUFLEN_MASK) >> E1000_RXDADV_HDRBUFLEN_SHIFT;
	if (hlen > IGB_RX_HDR_LEN)
		hlen = IGB_RX_HDR_LEN;
	return hlen;
}

static bool igb_clean_rx_irq(struct igb_q_vector *q_vector, int budget)
{
	struct igb_ring *rx_ring = q_vector->rx.ring;
	union e1000_adv_rx_desc *rx_desc;
	const int current_node = numa_node_id();
	unsigned int total_bytes = 0, total_packets = 0;
	u16 cleaned_count = igb_desc_unused(rx_ring);
	u16 i = rx_ring->next_to_clean;

	rx_desc = IGB_RX_DESC(rx_ring, i);

	while (igb_test_staterr(rx_desc, E1000_RXD_STAT_DD)) {
		struct igb_rx_buffer *buffer_info = &rx_ring->rx_buffer_info[i];
		struct sk_buff *skb = buffer_info->skb;
		union e1000_adv_rx_desc *next_rxd;

		buffer_info->skb = NULL;
		prefetch(skb->data);

		i++;
		if (i == rx_ring->count)
			i = 0;

		next_rxd = IGB_RX_DESC(rx_ring, i);
		prefetch(next_rxd);

		/*
		 * This memory barrier is needed to keep us from reading
		 * any other fields out of the rx_desc until we know the
		 * RXD_STAT_DD bit is set
		 */
		rmb();

		if (!skb_is_nonlinear(skb)) {
			__skb_put(skb, igb_get_hlen(rx_desc));
			dma_unmap_single(rx_ring->dev, buffer_info->dma,
					 IGB_RX_HDR_LEN,
					 DMA_FROM_DEVICE);
			buffer_info->dma = 0;
		}

		if (rx_desc->wb.upper.length) {
			u16 length = le16_to_cpu(rx_desc->wb.upper.length);

			skb_fill_page_desc(skb, skb_shinfo(skb)->nr_frags,
						buffer_info->page,
						buffer_info->page_offset,
						length);

			skb->len += length;
			skb->data_len += length;
			skb->truesize += PAGE_SIZE / 2;

			if ((page_count(buffer_info->page) != 1) ||
			    (page_to_nid(buffer_info->page) != current_node))
				buffer_info->page = NULL;
			else
				get_page(buffer_info->page);

			dma_unmap_page(rx_ring->dev, buffer_info->page_dma,
				       PAGE_SIZE / 2, DMA_FROM_DEVICE);
			buffer_info->page_dma = 0;
		}

		if (!igb_test_staterr(rx_desc, E1000_RXD_STAT_EOP)) {
			struct igb_rx_buffer *next_buffer;
			next_buffer = &rx_ring->rx_buffer_info[i];
			buffer_info->skb = next_buffer->skb;
			buffer_info->dma = next_buffer->dma;
			next_buffer->skb = skb;
			next_buffer->dma = 0;
			goto next_desc;
		}

		if (unlikely((igb_test_staterr(rx_desc,
					       E1000_RXDEXT_ERR_FRAME_ERR_MASK))
			     && !(rx_ring->netdev->features & NETIF_F_RXALL))) {
			dev_kfree_skb_any(skb);
			goto next_desc;
		}

		igb_rx_hwtstamp(q_vector, rx_desc, skb);
		igb_rx_hash(rx_ring, rx_desc, skb);
		igb_rx_checksum(rx_ring, rx_desc, skb);
		igb_rx_vlan(rx_ring, rx_desc, skb);

		total_bytes += skb->len;
		total_packets++;

		skb->protocol = eth_type_trans(skb, rx_ring->netdev);

		napi_gro_receive(&q_vector->napi, skb);

		budget--;
next_desc:
		if (!budget)
			break;

		cleaned_count++;
=======
		skb_set_hash(skb,
			     le32_to_cpu(rx_desc->wb.lower.hi_dword.rss),
			     PKT_HASH_TYPE_L3);
}

/**
 *  igb_is_non_eop - process handling of non-EOP buffers
 *  @rx_ring: Rx ring being processed
 *  @rx_desc: Rx descriptor for current buffer
 *
 *  This function updates next to clean.  If the buffer is an EOP buffer
 *  this function exits returning false, otherwise it will place the
 *  sk_buff in the next buffer to be chained and return true indicating
 *  that this is in fact a non-EOP buffer.
 **/
static bool igb_is_non_eop(struct igb_ring *rx_ring,
			   union e1000_adv_rx_desc *rx_desc)
{
	u32 ntc = rx_ring->next_to_clean + 1;

	/* fetch, update, and store next to clean */
	ntc = (ntc < rx_ring->count) ? ntc : 0;
	rx_ring->next_to_clean = ntc;

	prefetch(IGB_RX_DESC(rx_ring, ntc));

	if (likely(igb_test_staterr(rx_desc, E1000_RXD_STAT_EOP)))
		return false;

	return true;
}

/**
 *  igb_cleanup_headers - Correct corrupted or empty headers
 *  @rx_ring: rx descriptor ring packet is being transacted on
 *  @rx_desc: pointer to the EOP Rx descriptor
 *  @skb: pointer to current skb being fixed
 *
 *  Address the case where we are pulling data in on pages only
 *  and as such no data is present in the skb header.
 *
 *  In addition if skb is not at least 60 bytes we need to pad it so that
 *  it is large enough to qualify as a valid Ethernet frame.
 *
 *  Returns true if an error was encountered and skb was freed.
 **/
static bool igb_cleanup_headers(struct igb_ring *rx_ring,
				union e1000_adv_rx_desc *rx_desc,
				struct sk_buff *skb)
{
	/* XDP packets use error pointer so abort at this point */
	if (IS_ERR(skb))
		return true;

	if (unlikely((igb_test_staterr(rx_desc,
				       E1000_RXDEXT_ERR_FRAME_ERR_MASK)))) {
		struct net_device *netdev = rx_ring->netdev;
		if (!(netdev->features & NETIF_F_RXALL)) {
			dev_kfree_skb_any(skb);
			return true;
		}
	}

	/* if eth_skb_pad returns an error the skb was freed */
	if (eth_skb_pad(skb))
		return true;

	return false;
}

/**
 *  igb_process_skb_fields - Populate skb header fields from Rx descriptor
 *  @rx_ring: rx descriptor ring packet is being transacted on
 *  @rx_desc: pointer to the EOP Rx descriptor
 *  @skb: pointer to current skb being populated
 *
 *  This function checks the ring, descriptor, and packet information in
 *  order to populate the hash, checksum, VLAN, timestamp, protocol, and
 *  other fields within the skb.
 **/
static void igb_process_skb_fields(struct igb_ring *rx_ring,
				   union e1000_adv_rx_desc *rx_desc,
				   struct sk_buff *skb)
{
	struct net_device *dev = rx_ring->netdev;

	igb_rx_hash(rx_ring, rx_desc, skb);

	igb_rx_checksum(rx_ring, rx_desc, skb);

	if (igb_test_staterr(rx_desc, E1000_RXDADV_STAT_TS) &&
	    !igb_test_staterr(rx_desc, E1000_RXDADV_STAT_TSIP))
		igb_ptp_rx_rgtstamp(rx_ring->q_vector, skb);

	if ((dev->features & NETIF_F_HW_VLAN_CTAG_RX) &&
	    igb_test_staterr(rx_desc, E1000_RXD_STAT_VP)) {
		u16 vid;

		if (igb_test_staterr(rx_desc, E1000_RXDEXT_STATERR_LB) &&
		    test_bit(IGB_RING_FLAG_RX_LB_VLAN_BSWAP, &rx_ring->flags))
			vid = be16_to_cpu((__force __be16)rx_desc->wb.upper.vlan);
		else
			vid = le16_to_cpu(rx_desc->wb.upper.vlan);

		__vlan_hwaccel_put_tag(skb, htons(ETH_P_8021Q), vid);
	}

	skb_record_rx_queue(skb, rx_ring->queue_index);

	skb->protocol = eth_type_trans(skb, rx_ring->netdev);
}

static unsigned int igb_rx_offset(struct igb_ring *rx_ring)
{
	return ring_uses_build_skb(rx_ring) ? IGB_SKB_PAD : 0;
}

static struct igb_rx_buffer *igb_get_rx_buffer(struct igb_ring *rx_ring,
					       const unsigned int size, int *rx_buf_pgcnt)
{
	struct igb_rx_buffer *rx_buffer;

	rx_buffer = &rx_ring->rx_buffer_info[rx_ring->next_to_clean];
	*rx_buf_pgcnt =
#if (PAGE_SIZE < 8192)
		page_count(rx_buffer->page);
#else
		0;
#endif
	prefetchw(rx_buffer->page);

	/* we are reusing so sync this buffer for CPU use */
	dma_sync_single_range_for_cpu(rx_ring->dev,
				      rx_buffer->dma,
				      rx_buffer->page_offset,
				      size,
				      DMA_FROM_DEVICE);

	rx_buffer->pagecnt_bias--;

	return rx_buffer;
}

static void igb_put_rx_buffer(struct igb_ring *rx_ring,
			      struct igb_rx_buffer *rx_buffer, int rx_buf_pgcnt)
{
	if (igb_can_reuse_rx_page(rx_buffer, rx_buf_pgcnt)) {
		/* hand second half of page back to the ring */
		igb_reuse_rx_page(rx_ring, rx_buffer);
	} else {
		/* We are not reusing the buffer so unmap it and free
		 * any references we are holding to it
		 */
		dma_unmap_page_attrs(rx_ring->dev, rx_buffer->dma,
				     igb_rx_pg_size(rx_ring), DMA_FROM_DEVICE,
				     IGB_RX_DMA_ATTR);
		__page_frag_cache_drain(rx_buffer->page,
					rx_buffer->pagecnt_bias);
	}

	/* clear contents of rx_buffer */
	rx_buffer->page = NULL;
}

static int igb_clean_rx_irq(struct igb_q_vector *q_vector, const int budget)
{
	struct igb_adapter *adapter = q_vector->adapter;
	struct igb_ring *rx_ring = q_vector->rx.ring;
	struct sk_buff *skb = rx_ring->skb;
	unsigned int total_bytes = 0, total_packets = 0;
	u16 cleaned_count = igb_desc_unused(rx_ring);
	unsigned int xdp_xmit = 0;
	struct xdp_buff xdp;
	u32 frame_sz = 0;
	int rx_buf_pgcnt;

	/* Frame size depend on rx_ring setup when PAGE_SIZE=4K */
#if (PAGE_SIZE < 8192)
	frame_sz = igb_rx_frame_truesize(rx_ring, 0);
#endif
	xdp_init_buff(&xdp, frame_sz, &rx_ring->xdp_rxq);

	while (likely(total_packets < budget)) {
		union e1000_adv_rx_desc *rx_desc;
		struct igb_rx_buffer *rx_buffer;
		ktime_t timestamp = 0;
		int pkt_offset = 0;
		unsigned int size;
		void *pktbuf;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* return some buffers to hardware, one at a time is too slow */
		if (cleaned_count >= IGB_RX_BUFFER_WRITE) {
			igb_alloc_rx_buffers(rx_ring, cleaned_count);
			cleaned_count = 0;
		}

<<<<<<< HEAD
		/* use prefetched values */
		rx_desc = next_rxd;
	}

	rx_ring->next_to_clean = i;
=======
		rx_desc = IGB_RX_DESC(rx_ring, rx_ring->next_to_clean);
		size = le16_to_cpu(rx_desc->wb.upper.length);
		if (!size)
			break;

		/* This memory barrier is needed to keep us from reading
		 * any other fields out of the rx_desc until we know the
		 * descriptor has been written back
		 */
		dma_rmb();

		rx_buffer = igb_get_rx_buffer(rx_ring, size, &rx_buf_pgcnt);
		pktbuf = page_address(rx_buffer->page) + rx_buffer->page_offset;

		/* pull rx packet timestamp if available and valid */
		if (igb_test_staterr(rx_desc, E1000_RXDADV_STAT_TSIP)) {
			int ts_hdr_len;

			ts_hdr_len = igb_ptp_rx_pktstamp(rx_ring->q_vector,
							 pktbuf, &timestamp);

			pkt_offset += ts_hdr_len;
			size -= ts_hdr_len;
		}

		/* retrieve a buffer from the ring */
		if (!skb) {
			unsigned char *hard_start = pktbuf - igb_rx_offset(rx_ring);
			unsigned int offset = pkt_offset + igb_rx_offset(rx_ring);

			xdp_prepare_buff(&xdp, hard_start, offset, size, true);
			xdp_buff_clear_frags_flag(&xdp);
#if (PAGE_SIZE > 4096)
			/* At larger PAGE_SIZE, frame_sz depend on len size */
			xdp.frame_sz = igb_rx_frame_truesize(rx_ring, size);
#endif
			skb = igb_run_xdp(adapter, rx_ring, &xdp);
		}

		if (IS_ERR(skb)) {
			unsigned int xdp_res = -PTR_ERR(skb);

			if (xdp_res & (IGB_XDP_TX | IGB_XDP_REDIR)) {
				xdp_xmit |= xdp_res;
				igb_rx_buffer_flip(rx_ring, rx_buffer, size);
			} else {
				rx_buffer->pagecnt_bias++;
			}
			total_packets++;
			total_bytes += size;
		} else if (skb)
			igb_add_rx_frag(rx_ring, rx_buffer, skb, size);
		else if (ring_uses_build_skb(rx_ring))
			skb = igb_build_skb(rx_ring, rx_buffer, &xdp,
					    timestamp);
		else
			skb = igb_construct_skb(rx_ring, rx_buffer,
						&xdp, timestamp);

		/* exit if we failed to retrieve a buffer */
		if (!skb) {
			rx_ring->rx_stats.alloc_failed++;
			rx_buffer->pagecnt_bias++;
			break;
		}

		igb_put_rx_buffer(rx_ring, rx_buffer, rx_buf_pgcnt);
		cleaned_count++;

		/* fetch next buffer in frame if non-eop */
		if (igb_is_non_eop(rx_ring, rx_desc))
			continue;

		/* verify the packet layout is correct */
		if (igb_cleanup_headers(rx_ring, rx_desc, skb)) {
			skb = NULL;
			continue;
		}

		/* probably a little skewed due to removing CRC */
		total_bytes += skb->len;

		/* populate checksum, timestamp, VLAN, and protocol */
		igb_process_skb_fields(rx_ring, rx_desc, skb);

		napi_gro_receive(&q_vector->napi, skb);

		/* reset skb pointer */
		skb = NULL;

		/* update budget accounting */
		total_packets++;
	}

	/* place incomplete frames back on ring for completion */
	rx_ring->skb = skb;

	if (xdp_xmit & IGB_XDP_REDIR)
		xdp_do_flush();

	if (xdp_xmit & IGB_XDP_TX) {
		struct igb_ring *tx_ring = igb_xdp_tx_queue_mapping(adapter);

		igb_xdp_ring_update_tail(tx_ring);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u64_stats_update_begin(&rx_ring->rx_syncp);
	rx_ring->rx_stats.packets += total_packets;
	rx_ring->rx_stats.bytes += total_bytes;
	u64_stats_update_end(&rx_ring->rx_syncp);
	q_vector->rx.total_packets += total_packets;
	q_vector->rx.total_bytes += total_bytes;

	if (cleaned_count)
		igb_alloc_rx_buffers(rx_ring, cleaned_count);

<<<<<<< HEAD
	return !!budget;
}

static bool igb_alloc_mapped_skb(struct igb_ring *rx_ring,
				 struct igb_rx_buffer *bi)
{
	struct sk_buff *skb = bi->skb;
	dma_addr_t dma = bi->dma;

	if (dma)
		return true;

	if (likely(!skb)) {
		skb = netdev_alloc_skb_ip_align(rx_ring->netdev,
						IGB_RX_HDR_LEN);
		bi->skb = skb;
		if (!skb) {
			rx_ring->rx_stats.alloc_failed++;
			return false;
		}

		/* initialize skb for ring */
		skb_record_rx_queue(skb, rx_ring->queue_index);
	}

	dma = dma_map_single(rx_ring->dev, skb->data,
			     IGB_RX_HDR_LEN, DMA_FROM_DEVICE);

	if (dma_mapping_error(rx_ring->dev, dma)) {
		rx_ring->rx_stats.alloc_failed++;
		return false;
	}

	bi->dma = dma;
	return true;
=======
	return total_packets;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static bool igb_alloc_mapped_page(struct igb_ring *rx_ring,
				  struct igb_rx_buffer *bi)
{
	struct page *page = bi->page;
<<<<<<< HEAD
	dma_addr_t page_dma = bi->page_dma;
	unsigned int page_offset = bi->page_offset ^ (PAGE_SIZE / 2);

	if (page_dma)
		return true;

	if (!page) {
		page = alloc_page(GFP_ATOMIC | __GFP_COLD);
		bi->page = page;
		if (unlikely(!page)) {
			rx_ring->rx_stats.alloc_failed++;
			return false;
		}
	}

	page_dma = dma_map_page(rx_ring->dev, page,
				page_offset, PAGE_SIZE / 2,
				DMA_FROM_DEVICE);

	if (dma_mapping_error(rx_ring->dev, page_dma)) {
=======
	dma_addr_t dma;

	/* since we are recycling buffers we should seldom need to alloc */
	if (likely(page))
		return true;

	/* alloc new page for storage */
	page = dev_alloc_pages(igb_rx_pg_order(rx_ring));
	if (unlikely(!page)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rx_ring->rx_stats.alloc_failed++;
		return false;
	}

<<<<<<< HEAD
	bi->page_dma = page_dma;
	bi->page_offset = page_offset;
=======
	/* map page for use */
	dma = dma_map_page_attrs(rx_ring->dev, page, 0,
				 igb_rx_pg_size(rx_ring),
				 DMA_FROM_DEVICE,
				 IGB_RX_DMA_ATTR);

	/* if mapping failed free memory back to system since
	 * there isn't much point in holding memory we can't use
	 */
	if (dma_mapping_error(rx_ring->dev, dma)) {
		__free_pages(page, igb_rx_pg_order(rx_ring));

		rx_ring->rx_stats.alloc_failed++;
		return false;
	}

	bi->dma = dma;
	bi->page = page;
	bi->page_offset = igb_rx_offset(rx_ring);
	page_ref_add(page, USHRT_MAX - 1);
	bi->pagecnt_bias = USHRT_MAX;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return true;
}

/**
<<<<<<< HEAD
 * igb_alloc_rx_buffers - Replace used receive buffers; packet split
 * @adapter: address of board private structure
=======
 *  igb_alloc_rx_buffers - Replace used receive buffers
 *  @rx_ring: rx descriptor ring to allocate new receive buffers
 *  @cleaned_count: count of buffers to allocate
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
void igb_alloc_rx_buffers(struct igb_ring *rx_ring, u16 cleaned_count)
{
	union e1000_adv_rx_desc *rx_desc;
	struct igb_rx_buffer *bi;
	u16 i = rx_ring->next_to_use;
<<<<<<< HEAD
=======
	u16 bufsz;

	/* nothing to do */
	if (!cleaned_count)
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rx_desc = IGB_RX_DESC(rx_ring, i);
	bi = &rx_ring->rx_buffer_info[i];
	i -= rx_ring->count;

<<<<<<< HEAD
	while (cleaned_count--) {
		if (!igb_alloc_mapped_skb(rx_ring, bi))
			break;

		/* Refresh the desc even if buffer_addrs didn't change
		 * because each write-back erases this info. */
		rx_desc->read.hdr_addr = cpu_to_le64(bi->dma);

		if (!igb_alloc_mapped_page(rx_ring, bi))
			break;

		rx_desc->read.pkt_addr = cpu_to_le64(bi->page_dma);
=======
	bufsz = igb_rx_bufsz(rx_ring);

	do {
		if (!igb_alloc_mapped_page(rx_ring, bi))
			break;

		/* sync the buffer for use by the device */
		dma_sync_single_range_for_device(rx_ring->dev, bi->dma,
						 bi->page_offset, bufsz,
						 DMA_FROM_DEVICE);

		/* Refresh the desc even if buffer_addrs didn't change
		 * because each write-back erases this info.
		 */
		rx_desc->read.pkt_addr = cpu_to_le64(bi->dma + bi->page_offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		rx_desc++;
		bi++;
		i++;
		if (unlikely(!i)) {
			rx_desc = IGB_RX_DESC(rx_ring, 0);
			bi = rx_ring->rx_buffer_info;
			i -= rx_ring->count;
		}

<<<<<<< HEAD
		/* clear the hdr_addr for the next_to_use descriptor */
		rx_desc->read.hdr_addr = 0;
	}
=======
		/* clear the length for the next_to_use descriptor */
		rx_desc->wb.upper.length = 0;

		cleaned_count--;
	} while (cleaned_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	i += rx_ring->count;

	if (rx_ring->next_to_use != i) {
<<<<<<< HEAD
		rx_ring->next_to_use = i;

		/* Force memory writes to complete before letting h/w
		 * know there are new descriptors to fetch.  (Only
		 * applicable for weak-ordered memory model archs,
		 * such as IA-64). */
		wmb();
=======
		/* record the next descriptor to use */
		rx_ring->next_to_use = i;

		/* update next to alloc since we have filled the ring */
		rx_ring->next_to_alloc = i;

		/* Force memory writes to complete before letting h/w
		 * know there are new descriptors to fetch.  (Only
		 * applicable for weak-ordered memory model archs,
		 * such as IA-64).
		 */
		dma_wmb();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		writel(i, rx_ring->tail);
	}
}

/**
 * igb_mii_ioctl -
<<<<<<< HEAD
 * @netdev:
 * @ifreq:
 * @cmd:
=======
 * @netdev: pointer to netdev struct
 * @ifr: interface structure
 * @cmd: ioctl command to execute
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static int igb_mii_ioctl(struct net_device *netdev, struct ifreq *ifr, int cmd)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct mii_ioctl_data *data = if_mii(ifr);

	if (adapter->hw.phy.media_type != e1000_media_type_copper)
		return -EOPNOTSUPP;

	switch (cmd) {
	case SIOCGMIIPHY:
		data->phy_id = adapter->hw.phy.addr;
		break;
	case SIOCGMIIREG:
		if (igb_read_phy_reg(&adapter->hw, data->reg_num & 0x1F,
<<<<<<< HEAD
		                     &data->val_out))
=======
				     &data->val_out))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EIO;
		break;
	case SIOCSMIIREG:
	default:
		return -EOPNOTSUPP;
	}
	return 0;
}

/**
<<<<<<< HEAD
 * igb_hwtstamp_ioctl - control hardware time stamping
 * @netdev:
 * @ifreq:
 * @cmd:
 *
 * Outgoing time stamping can be enabled and disabled. Play nice and
 * disable it when requested, although it shouldn't case any overhead
 * when no packet needs it. At most one packet in the queue may be
 * marked for time stamping, otherwise it would be impossible to tell
 * for sure to which packet the hardware time stamp belongs.
 *
 * Incoming time stamping has to be configured via the hardware
 * filters. Not all combinations are supported, in particular event
 * type has to be specified. Matching the kind of event packet is
 * not supported, with the exception of "all V2 events regardless of
 * level 2 or 4".
 *
 **/
static int igb_hwtstamp_ioctl(struct net_device *netdev,
			      struct ifreq *ifr, int cmd)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	struct hwtstamp_config config;
	u32 tsync_tx_ctl = E1000_TSYNCTXCTL_ENABLED;
	u32 tsync_rx_ctl = E1000_TSYNCRXCTL_ENABLED;
	u32 tsync_rx_cfg = 0;
	bool is_l4 = false;
	bool is_l2 = false;
	u32 regval;

	if (copy_from_user(&config, ifr->ifr_data, sizeof(config)))
		return -EFAULT;

	/* reserved for future extensions */
	if (config.flags)
		return -EINVAL;

	switch (config.tx_type) {
	case HWTSTAMP_TX_OFF:
		tsync_tx_ctl = 0;
	case HWTSTAMP_TX_ON:
		break;
	default:
		return -ERANGE;
	}

	switch (config.rx_filter) {
	case HWTSTAMP_FILTER_NONE:
		tsync_rx_ctl = 0;
		break;
	case HWTSTAMP_FILTER_PTP_V1_L4_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_L4_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_L2_EVENT:
	case HWTSTAMP_FILTER_ALL:
		/*
		 * register TSYNCRXCFG must be set, therefore it is not
		 * possible to time stamp both Sync and Delay_Req messages
		 * => fall back to time stamping all packets
		 */
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_ALL;
		config.rx_filter = HWTSTAMP_FILTER_ALL;
		break;
	case HWTSTAMP_FILTER_PTP_V1_L4_SYNC:
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_L4_V1;
		tsync_rx_cfg = E1000_TSYNCRXCFG_PTP_V1_SYNC_MESSAGE;
		is_l4 = true;
		break;
	case HWTSTAMP_FILTER_PTP_V1_L4_DELAY_REQ:
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_L4_V1;
		tsync_rx_cfg = E1000_TSYNCRXCFG_PTP_V1_DELAY_REQ_MESSAGE;
		is_l4 = true;
		break;
	case HWTSTAMP_FILTER_PTP_V2_L2_SYNC:
	case HWTSTAMP_FILTER_PTP_V2_L4_SYNC:
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_L2_L4_V2;
		tsync_rx_cfg = E1000_TSYNCRXCFG_PTP_V2_SYNC_MESSAGE;
		is_l2 = true;
		is_l4 = true;
		config.rx_filter = HWTSTAMP_FILTER_SOME;
		break;
	case HWTSTAMP_FILTER_PTP_V2_L2_DELAY_REQ:
	case HWTSTAMP_FILTER_PTP_V2_L4_DELAY_REQ:
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_L2_L4_V2;
		tsync_rx_cfg = E1000_TSYNCRXCFG_PTP_V2_DELAY_REQ_MESSAGE;
		is_l2 = true;
		is_l4 = true;
		config.rx_filter = HWTSTAMP_FILTER_SOME;
		break;
	case HWTSTAMP_FILTER_PTP_V2_EVENT:
	case HWTSTAMP_FILTER_PTP_V2_SYNC:
	case HWTSTAMP_FILTER_PTP_V2_DELAY_REQ:
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_EVENT_V2;
		config.rx_filter = HWTSTAMP_FILTER_PTP_V2_EVENT;
		is_l2 = true;
		is_l4 = true;
		break;
	default:
		return -ERANGE;
	}

	if (hw->mac.type == e1000_82575) {
		if (tsync_rx_ctl | tsync_tx_ctl)
			return -EINVAL;
		return 0;
	}

	/*
	 * Per-packet timestamping only works if all packets are
	 * timestamped, so enable timestamping in all packets as
	 * long as one rx filter was configured.
	 */
	if ((hw->mac.type >= e1000_82580) && tsync_rx_ctl) {
		tsync_rx_ctl = E1000_TSYNCRXCTL_ENABLED;
		tsync_rx_ctl |= E1000_TSYNCRXCTL_TYPE_ALL;
	}

	/* enable/disable TX */
	regval = rd32(E1000_TSYNCTXCTL);
	regval &= ~E1000_TSYNCTXCTL_ENABLED;
	regval |= tsync_tx_ctl;
	wr32(E1000_TSYNCTXCTL, regval);

	/* enable/disable RX */
	regval = rd32(E1000_TSYNCRXCTL);
	regval &= ~(E1000_TSYNCRXCTL_ENABLED | E1000_TSYNCRXCTL_TYPE_MASK);
	regval |= tsync_rx_ctl;
	wr32(E1000_TSYNCRXCTL, regval);

	/* define which PTP packets are time stamped */
	wr32(E1000_TSYNCRXCFG, tsync_rx_cfg);

	/* define ethertype filter for timestamped packets */
	if (is_l2)
		wr32(E1000_ETQF(3),
		                (E1000_ETQF_FILTER_ENABLE | /* enable filter */
		                 E1000_ETQF_1588 | /* enable timestamping */
		                 ETH_P_1588));     /* 1588 eth protocol type */
	else
		wr32(E1000_ETQF(3), 0);

#define PTP_PORT 319
	/* L4 Queue Filter[3]: filter by destination port and protocol */
	if (is_l4) {
		u32 ftqf = (IPPROTO_UDP /* UDP */
			| E1000_FTQF_VF_BP /* VF not compared */
			| E1000_FTQF_1588_TIME_STAMP /* Enable Timestamping */
			| E1000_FTQF_MASK); /* mask all inputs */
		ftqf &= ~E1000_FTQF_MASK_PROTO_BP; /* enable protocol check */

		wr32(E1000_IMIR(3), htons(PTP_PORT));
		wr32(E1000_IMIREXT(3),
		     (E1000_IMIREXT_SIZE_BP | E1000_IMIREXT_CTRL_BP));
		if (hw->mac.type == e1000_82576) {
			/* enable source port check */
			wr32(E1000_SPQF(3), htons(PTP_PORT));
			ftqf &= ~E1000_FTQF_MASK_SOURCE_PORT_BP;
		}
		wr32(E1000_FTQF(3), ftqf);
	} else {
		wr32(E1000_FTQF(3), E1000_FTQF_MASK);
	}
	wrfl();

	adapter->hwtstamp_config = config;

	/* clear TX/RX time stamp registers, just to be sure */
	regval = rd32(E1000_TXSTMPH);
	regval = rd32(E1000_RXSTMPH);

	return copy_to_user(ifr->ifr_data, &config, sizeof(config)) ?
		-EFAULT : 0;
}

/**
 * igb_ioctl -
 * @netdev:
 * @ifreq:
 * @cmd:
=======
 * igb_ioctl -
 * @netdev: pointer to netdev struct
 * @ifr: interface structure
 * @cmd: ioctl command to execute
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 **/
static int igb_ioctl(struct net_device *netdev, struct ifreq *ifr, int cmd)
{
	switch (cmd) {
	case SIOCGMIIPHY:
	case SIOCGMIIREG:
	case SIOCSMIIREG:
		return igb_mii_ioctl(netdev, ifr, cmd);
<<<<<<< HEAD
	case SIOCSHWTSTAMP:
		return igb_hwtstamp_ioctl(netdev, ifr, cmd);
=======
	case SIOCGHWTSTAMP:
		return igb_ptp_get_ts_config(netdev, ifr);
	case SIOCSHWTSTAMP:
		return igb_ptp_set_ts_config(netdev, ifr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return -EOPNOTSUPP;
	}
}

<<<<<<< HEAD
s32 igb_read_pcie_cap_reg(struct e1000_hw *hw, u32 reg, u16 *value)
{
	struct igb_adapter *adapter = hw->back;
	u16 cap_offset;

	cap_offset = adapter->pdev->pcie_cap;
	if (!cap_offset)
		return -E1000_ERR_CONFIG;

	pci_read_config_word(adapter->pdev, cap_offset + reg, value);

=======
void igb_read_pci_cfg(struct e1000_hw *hw, u32 reg, u16 *value)
{
	struct igb_adapter *adapter = hw->back;

	pci_read_config_word(adapter->pdev, reg, value);
}

void igb_write_pci_cfg(struct e1000_hw *hw, u32 reg, u16 *value)
{
	struct igb_adapter *adapter = hw->back;

	pci_write_config_word(adapter->pdev, reg, *value);
}

s32 igb_read_pcie_cap_reg(struct e1000_hw *hw, u32 reg, u16 *value)
{
	struct igb_adapter *adapter = hw->back;

	if (pcie_capability_read_word(adapter->pdev, reg, value))
		return -E1000_ERR_CONFIG;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

s32 igb_write_pcie_cap_reg(struct e1000_hw *hw, u32 reg, u16 *value)
{
	struct igb_adapter *adapter = hw->back;
<<<<<<< HEAD
	u16 cap_offset;

	cap_offset = adapter->pdev->pcie_cap;
	if (!cap_offset)
		return -E1000_ERR_CONFIG;

	pci_write_config_word(adapter->pdev, cap_offset + reg, *value);

=======

	if (pcie_capability_write_word(adapter->pdev, reg, *value))
		return -E1000_ERR_CONFIG;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static void igb_vlan_mode(struct net_device *netdev, netdev_features_t features)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	u32 ctrl, rctl;
<<<<<<< HEAD
	bool enable = !!(features & NETIF_F_HW_VLAN_RX);
=======
	bool enable = !!(features & NETIF_F_HW_VLAN_CTAG_RX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (enable) {
		/* enable VLAN tag insert/strip */
		ctrl = rd32(E1000_CTRL);
		ctrl |= E1000_CTRL_VME;
		wr32(E1000_CTRL, ctrl);

		/* Disable CFI check */
		rctl = rd32(E1000_RCTL);
		rctl &= ~E1000_RCTL_CFIEN;
		wr32(E1000_RCTL, rctl);
	} else {
		/* disable VLAN tag insert/strip */
		ctrl = rd32(E1000_CTRL);
		ctrl &= ~E1000_CTRL_VME;
		wr32(E1000_CTRL, ctrl);
	}

<<<<<<< HEAD
	igb_rlpml_set(adapter);
}

static int igb_vlan_rx_add_vid(struct net_device *netdev, u16 vid)
=======
	igb_set_vf_vlan_strip(adapter, adapter->vfs_allocated_count, enable);
}

static int igb_vlan_rx_add_vid(struct net_device *netdev,
			       __be16 proto, u16 vid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	int pf_id = adapter->vfs_allocated_count;

<<<<<<< HEAD
	/* attempt to add filter to vlvf array */
	igb_vlvf_set(adapter, vid, true, pf_id);

	/* add the filter since PF can receive vlans w/o entry in vlvf */
	igb_vfta_set(hw, vid, true);
=======
	/* add the filter since PF can receive vlans w/o entry in vlvf */
	if (!vid || !(adapter->flags & IGB_FLAG_VLAN_PROMISC))
		igb_vfta_set(hw, vid, pf_id, true, !!vid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	set_bit(vid, adapter->active_vlans);

	return 0;
}

<<<<<<< HEAD
static int igb_vlan_rx_kill_vid(struct net_device *netdev, u16 vid)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	int pf_id = adapter->vfs_allocated_count;
	s32 err;

	/* remove vlan from VLVF table array */
	err = igb_vlvf_set(adapter, vid, false, pf_id);

	/* if vid was not present in VLVF just remove it from table */
	if (err)
		igb_vfta_set(hw, vid, false);
=======
static int igb_vlan_rx_kill_vid(struct net_device *netdev,
				__be16 proto, u16 vid)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	int pf_id = adapter->vfs_allocated_count;
	struct e1000_hw *hw = &adapter->hw;

	/* remove VID from filter table */
	if (vid && !(adapter->flags & IGB_FLAG_VLAN_PROMISC))
		igb_vfta_set(hw, vid, pf_id, false, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	clear_bit(vid, adapter->active_vlans);

	return 0;
}

static void igb_restore_vlan(struct igb_adapter *adapter)
{
<<<<<<< HEAD
	u16 vid;

	igb_vlan_mode(adapter->netdev, adapter->netdev->features);

	for_each_set_bit(vid, adapter->active_vlans, VLAN_N_VID)
		igb_vlan_rx_add_vid(adapter->netdev, vid);
=======
	u16 vid = 1;

	igb_vlan_mode(adapter->netdev, adapter->netdev->features);
	igb_vlan_rx_add_vid(adapter->netdev, htons(ETH_P_8021Q), 0);

	for_each_set_bit_from(vid, adapter->active_vlans, VLAN_N_VID)
		igb_vlan_rx_add_vid(adapter->netdev, htons(ETH_P_8021Q), vid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int igb_set_spd_dplx(struct igb_adapter *adapter, u32 spd, u8 dplx)
{
	struct pci_dev *pdev = adapter->pdev;
	struct e1000_mac_info *mac = &adapter->hw.mac;

	mac->autoneg = 0;

	/* Make sure dplx is at most 1 bit and lsb of speed is not set
<<<<<<< HEAD
	 * for the switch() below to work */
	if ((spd & 1) || (dplx & ~1))
		goto err_inval;

	/* Fiber NIC's only allow 1000 Gbps Full duplex */
	if ((adapter->hw.phy.media_type == e1000_media_type_internal_serdes) &&
	    spd != SPEED_1000 &&
	    dplx != DUPLEX_FULL)
		goto err_inval;
=======
	 * for the switch() below to work
	 */
	if ((spd & 1) || (dplx & ~1))
		goto err_inval;

	/* Fiber NIC's only allow 1000 gbps Full duplex
	 * and 100Mbps Full duplex for 100baseFx sfp
	 */
	if (adapter->hw.phy.media_type == e1000_media_type_internal_serdes) {
		switch (spd + dplx) {
		case SPEED_10 + DUPLEX_HALF:
		case SPEED_10 + DUPLEX_FULL:
		case SPEED_100 + DUPLEX_HALF:
			goto err_inval;
		default:
			break;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (spd + dplx) {
	case SPEED_10 + DUPLEX_HALF:
		mac->forced_speed_duplex = ADVERTISE_10_HALF;
		break;
	case SPEED_10 + DUPLEX_FULL:
		mac->forced_speed_duplex = ADVERTISE_10_FULL;
		break;
	case SPEED_100 + DUPLEX_HALF:
		mac->forced_speed_duplex = ADVERTISE_100_HALF;
		break;
	case SPEED_100 + DUPLEX_FULL:
		mac->forced_speed_duplex = ADVERTISE_100_FULL;
		break;
	case SPEED_1000 + DUPLEX_FULL:
		mac->autoneg = 1;
		adapter->hw.phy.autoneg_advertised = ADVERTISE_1000_FULL;
		break;
	case SPEED_1000 + DUPLEX_HALF: /* not supported */
	default:
		goto err_inval;
	}
<<<<<<< HEAD
=======

	/* clear MDI, MDI(-X) override is only allowed when autoneg enabled */
	adapter->hw.phy.mdix = AUTO_ALL_MODES;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

err_inval:
	dev_err(&pdev->dev, "Unsupported Speed/Duplex configuration\n");
	return -EINVAL;
}

static int __igb_shutdown(struct pci_dev *pdev, bool *enable_wake,
			  bool runtime)
{
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	u32 ctrl, rctl, status;
	u32 wufc = runtime ? E1000_WUFC_LNKC : adapter->wol;
<<<<<<< HEAD
#ifdef CONFIG_PM
	int retval = 0;
#endif

=======
	bool wake;

	rtnl_lock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	netif_device_detach(netdev);

	if (netif_running(netdev))
		__igb_close(netdev, true);

<<<<<<< HEAD
	igb_clear_interrupt_scheme(adapter);

#ifdef CONFIG_PM
	retval = pci_save_state(pdev);
	if (retval)
		return retval;
#endif
=======
	igb_ptp_suspend(adapter);

	igb_clear_interrupt_scheme(adapter);
	rtnl_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	status = rd32(E1000_STATUS);
	if (status & E1000_STATUS_LU)
		wufc &= ~E1000_WUFC_LNKC;

	if (wufc) {
		igb_setup_rctl(adapter);
		igb_set_rx_mode(netdev);

		/* turn on all-multi mode if wake on multicast is enabled */
		if (wufc & E1000_WUFC_MC) {
			rctl = rd32(E1000_RCTL);
			rctl |= E1000_RCTL_MPE;
			wr32(E1000_RCTL, rctl);
		}

		ctrl = rd32(E1000_CTRL);
<<<<<<< HEAD
		/* advertise wake from D3Cold */
		#define E1000_CTRL_ADVD3WUC 0x00100000
		/* phy power management enable */
		#define E1000_CTRL_EN_PHY_PWR_MGMT 0x00200000
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ctrl |= E1000_CTRL_ADVD3WUC;
		wr32(E1000_CTRL, ctrl);

		/* Allow time for pending master requests to run */
		igb_disable_pcie_master(hw);

		wr32(E1000_WUC, E1000_WUC_PME_EN);
		wr32(E1000_WUFC, wufc);
	} else {
		wr32(E1000_WUC, 0);
		wr32(E1000_WUFC, 0);
	}

<<<<<<< HEAD
	*enable_wake = wufc || adapter->en_mng_pt;
	if (!*enable_wake)
=======
	wake = wufc || adapter->en_mng_pt;
	if (!wake)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		igb_power_down_link(adapter);
	else
		igb_power_up_link(adapter);

<<<<<<< HEAD
	/* Release control of h/w to f/w.  If f/w is AMT enabled, this
	 * would have already happened in close and is redundant. */
=======
	if (enable_wake)
		*enable_wake = wake;

	/* Release control of h/w to f/w.  If f/w is AMT enabled, this
	 * would have already happened in close and is redundant.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	igb_release_hw_control(adapter);

	pci_disable_device(pdev);

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM
#ifdef CONFIG_PM_SLEEP
static int igb_suspend(struct device *dev)
{
	int retval;
	bool wake;
	struct pci_dev *pdev = to_pci_dev(dev);

	retval = __igb_shutdown(pdev, &wake, 0);
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
#endif /* CONFIG_PM_SLEEP */

static int igb_resume(struct device *dev)
=======
static void igb_deliver_wake_packet(struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	struct sk_buff *skb;
	u32 wupl;

	wupl = rd32(E1000_WUPL) & E1000_WUPL_MASK;

	/* WUPM stores only the first 128 bytes of the wake packet.
	 * Read the packet only if we have the whole thing.
	 */
	if ((wupl == 0) || (wupl > E1000_WUPM_BYTES))
		return;

	skb = netdev_alloc_skb_ip_align(netdev, E1000_WUPM_BYTES);
	if (!skb)
		return;

	skb_put(skb, wupl);

	/* Ensure reads are 32-bit aligned */
	wupl = roundup(wupl, 4);

	memcpy_fromio(skb->data, hw->hw_addr + E1000_WUPM_REG(0), wupl);

	skb->protocol = eth_type_trans(skb, netdev);
	netif_rx(skb);
}

static int __maybe_unused igb_suspend(struct device *dev)
{
	return __igb_shutdown(to_pci_dev(dev), NULL, 0);
}

static int __maybe_unused __igb_resume(struct device *dev, bool rpm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
<<<<<<< HEAD
	u32 err;
=======
	u32 err, val;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pci_set_power_state(pdev, PCI_D0);
	pci_restore_state(pdev);
	pci_save_state(pdev);

<<<<<<< HEAD
=======
	if (!pci_device_is_present(pdev))
		return -ENODEV;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = pci_enable_device_mem(pdev);
	if (err) {
		dev_err(&pdev->dev,
			"igb: Cannot enable PCI device from suspend\n");
		return err;
	}
	pci_set_master(pdev);

	pci_enable_wake(pdev, PCI_D3hot, 0);
	pci_enable_wake(pdev, PCI_D3cold, 0);

<<<<<<< HEAD
	if (igb_init_interrupt_scheme(adapter)) {
=======
	if (igb_init_interrupt_scheme(adapter, true)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev_err(&pdev->dev, "Unable to allocate memory for queues\n");
		return -ENOMEM;
	}

	igb_reset(adapter);

	/* let the f/w know that the h/w is now under the control of the
<<<<<<< HEAD
	 * driver. */
	igb_get_hw_control(adapter);

	wr32(E1000_WUS, ~0);

	if (netdev->flags & IFF_UP) {
		err = __igb_open(netdev, true);
		if (err)
			return err;
	}

	netif_device_attach(netdev);
	return 0;
}

#ifdef CONFIG_PM_RUNTIME
static int igb_runtime_idle(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	struct net_device *netdev = pci_get_drvdata(pdev);
=======
	 * driver.
	 */
	igb_get_hw_control(adapter);

	val = rd32(E1000_WUS);
	if (val & WAKE_PKT_WUS)
		igb_deliver_wake_packet(netdev);

	wr32(E1000_WUS, ~0);

	if (!rpm)
		rtnl_lock();
	if (!err && netif_running(netdev))
		err = __igb_open(netdev, true);

	if (!err)
		netif_device_attach(netdev);
	if (!rpm)
		rtnl_unlock();

	return err;
}

static int __maybe_unused igb_resume(struct device *dev)
{
	return __igb_resume(dev, false);
}

static int __maybe_unused igb_runtime_idle(struct device *dev)
{
	struct net_device *netdev = dev_get_drvdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct igb_adapter *adapter = netdev_priv(netdev);

	if (!igb_has_link(adapter))
		pm_schedule_suspend(dev, MSEC_PER_SEC * 5);

	return -EBUSY;
}

<<<<<<< HEAD
static int igb_runtime_suspend(struct device *dev)
{
	struct pci_dev *pdev = to_pci_dev(dev);
	int retval;
	bool wake;

	retval = __igb_shutdown(pdev, &wake, 1);
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

static int igb_runtime_resume(struct device *dev)
{
	return igb_resume(dev);
}
#endif /* CONFIG_PM_RUNTIME */
#endif
=======
static int __maybe_unused igb_runtime_suspend(struct device *dev)
{
	return __igb_shutdown(to_pci_dev(dev), NULL, 1);
}

static int __maybe_unused igb_runtime_resume(struct device *dev)
{
	return __igb_resume(dev, true);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void igb_shutdown(struct pci_dev *pdev)
{
	bool wake;

	__igb_shutdown(pdev, &wake, 0);

	if (system_state == SYSTEM_POWER_OFF) {
		pci_wake_from_d3(pdev, wake);
		pci_set_power_state(pdev, PCI_D3hot);
	}
}

<<<<<<< HEAD
#ifdef CONFIG_NET_POLL_CONTROLLER
/*
 * Polling 'interrupt' - used by things like netconsole to send skbs
 * without having to re-enable interrupts. It's not called while
 * the interrupt routine is executing.
 */
static void igb_netpoll(struct net_device *netdev)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	struct igb_q_vector *q_vector;
	int i;

	for (i = 0; i < adapter->num_q_vectors; i++) {
		q_vector = adapter->q_vector[i];
		if (adapter->msix_entries)
			wr32(E1000_EIMC, q_vector->eims_value);
		else
			igb_irq_disable(adapter);
		napi_schedule(&q_vector->napi);
	}
}
#endif /* CONFIG_NET_POLL_CONTROLLER */

/**
 * igb_io_error_detected - called when PCI error is detected
 * @pdev: Pointer to PCI device
 * @state: The current pci connection state
 *
 * This function is called after a PCI bus error affecting
 * this device has been detected.
 */
=======
static int igb_pci_sriov_configure(struct pci_dev *dev, int num_vfs)
{
#ifdef CONFIG_PCI_IOV
	int err;

	if (num_vfs == 0) {
		return igb_disable_sriov(dev, true);
	} else {
		err = igb_enable_sriov(dev, num_vfs, true);
		return err ? err : num_vfs;
	}
#endif
	return 0;
}

/**
 *  igb_io_error_detected - called when PCI error is detected
 *  @pdev: Pointer to PCI device
 *  @state: The current pci connection state
 *
 *  This function is called after a PCI bus error affecting
 *  this device has been detected.
 **/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static pci_ers_result_t igb_io_error_detected(struct pci_dev *pdev,
					      pci_channel_state_t state)
{
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);

<<<<<<< HEAD
=======
	if (state == pci_channel_io_normal) {
		dev_warn(&pdev->dev, "Non-correctable non-fatal error reported.\n");
		return PCI_ERS_RESULT_CAN_RECOVER;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	netif_device_detach(netdev);

	if (state == pci_channel_io_perm_failure)
		return PCI_ERS_RESULT_DISCONNECT;

	if (netif_running(netdev))
		igb_down(adapter);
	pci_disable_device(pdev);

<<<<<<< HEAD
	/* Request a slot slot reset. */
=======
	/* Request a slot reset. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return PCI_ERS_RESULT_NEED_RESET;
}

/**
<<<<<<< HEAD
 * igb_io_slot_reset - called after the pci bus has been reset.
 * @pdev: Pointer to PCI device
 *
 * Restart the card from scratch, as if from a cold-boot. Implementation
 * resembles the first-half of the igb_resume routine.
 */
=======
 *  igb_io_slot_reset - called after the pci bus has been reset.
 *  @pdev: Pointer to PCI device
 *
 *  Restart the card from scratch, as if from a cold-boot. Implementation
 *  resembles the first-half of the __igb_resume routine.
 **/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static pci_ers_result_t igb_io_slot_reset(struct pci_dev *pdev)
{
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	pci_ers_result_t result;
<<<<<<< HEAD
	int err;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (pci_enable_device_mem(pdev)) {
		dev_err(&pdev->dev,
			"Cannot re-enable PCI device after reset.\n");
		result = PCI_ERS_RESULT_DISCONNECT;
	} else {
		pci_set_master(pdev);
		pci_restore_state(pdev);
		pci_save_state(pdev);

		pci_enable_wake(pdev, PCI_D3hot, 0);
		pci_enable_wake(pdev, PCI_D3cold, 0);

<<<<<<< HEAD
=======
		/* In case of PCI error, adapter lose its HW address
		 * so we should re-assign it here.
		 */
		hw->hw_addr = adapter->io_addr;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		igb_reset(adapter);
		wr32(E1000_WUS, ~0);
		result = PCI_ERS_RESULT_RECOVERED;
	}

<<<<<<< HEAD
	err = pci_cleanup_aer_uncorrect_error_status(pdev);
	if (err) {
		dev_err(&pdev->dev, "pci_cleanup_aer_uncorrect_error_status "
		        "failed 0x%0x\n", err);
		/* non-fatal, continue */
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return result;
}

/**
<<<<<<< HEAD
 * igb_io_resume - called when traffic can start flowing again.
 * @pdev: Pointer to PCI device
 *
 * This callback is called when the error recovery driver tells us that
 * its OK to resume normal operation. Implementation resembles the
 * second-half of the igb_resume routine.
=======
 *  igb_io_resume - called when traffic can start flowing again.
 *  @pdev: Pointer to PCI device
 *
 *  This callback is called when the error recovery driver tells us that
 *  its OK to resume normal operation. Implementation resembles the
 *  second-half of the __igb_resume routine.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static void igb_io_resume(struct pci_dev *pdev)
{
	struct net_device *netdev = pci_get_drvdata(pdev);
	struct igb_adapter *adapter = netdev_priv(netdev);

	if (netif_running(netdev)) {
		if (igb_up(adapter)) {
			dev_err(&pdev->dev, "igb_up failed after reset\n");
			return;
		}
	}

	netif_device_attach(netdev);

	/* let the f/w know that the h/w is now under the control of the
<<<<<<< HEAD
	 * driver. */
	igb_get_hw_control(adapter);
}

static void igb_rar_set_qsel(struct igb_adapter *adapter, u8 *addr, u32 index,
                             u8 qsel)
{
	u32 rar_low, rar_high;
	struct e1000_hw *hw = &adapter->hw;

	/* HW expects these in little endian so we reverse the byte order
	 * from network order (big endian) to little endian
	 */
	rar_low = ((u32) addr[0] | ((u32) addr[1] << 8) |
	          ((u32) addr[2] << 16) | ((u32) addr[3] << 24));
	rar_high = ((u32) addr[4] | ((u32) addr[5] << 8));

	/* Indicate to hardware the Address is Valid. */
	rar_high |= E1000_RAH_AV;

	if (hw->mac.type == e1000_82575)
		rar_high |= E1000_RAH_POOL_1 * qsel;
	else
		rar_high |= E1000_RAH_POOL_1 << qsel;
=======
	 * driver.
	 */
	igb_get_hw_control(adapter);
}

/**
 *  igb_rar_set_index - Sync RAL[index] and RAH[index] registers with MAC table
 *  @adapter: Pointer to adapter structure
 *  @index: Index of the RAR entry which need to be synced with MAC table
 **/
static void igb_rar_set_index(struct igb_adapter *adapter, u32 index)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 rar_low, rar_high;
	u8 *addr = adapter->mac_table[index].addr;

	/* HW expects these to be in network order when they are plugged
	 * into the registers which are little endian.  In order to guarantee
	 * that ordering we need to do an leXX_to_cpup here in order to be
	 * ready for the byteswap that occurs with writel
	 */
	rar_low = le32_to_cpup((__le32 *)(addr));
	rar_high = le16_to_cpup((__le16 *)(addr + 4));

	/* Indicate to hardware the Address is Valid. */
	if (adapter->mac_table[index].state & IGB_MAC_STATE_IN_USE) {
		if (is_valid_ether_addr(addr))
			rar_high |= E1000_RAH_AV;

		if (adapter->mac_table[index].state & IGB_MAC_STATE_SRC_ADDR)
			rar_high |= E1000_RAH_ASEL_SRC_ADDR;

		switch (hw->mac.type) {
		case e1000_82575:
		case e1000_i210:
			if (adapter->mac_table[index].state &
			    IGB_MAC_STATE_QUEUE_STEERING)
				rar_high |= E1000_RAH_QSEL_ENABLE;

			rar_high |= E1000_RAH_POOL_1 *
				    adapter->mac_table[index].queue;
			break;
		default:
			rar_high |= E1000_RAH_POOL_1 <<
				    adapter->mac_table[index].queue;
			break;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	wr32(E1000_RAL(index), rar_low);
	wrfl();
	wr32(E1000_RAH(index), rar_high);
	wrfl();
}

static int igb_set_vf_mac(struct igb_adapter *adapter,
<<<<<<< HEAD
                          int vf, unsigned char *mac_addr)
{
	struct e1000_hw *hw = &adapter->hw;
	/* VF MAC addresses start at end of receive addresses and moves
	 * torwards the first, as a result a collision should not be possible */
	int rar_entry = hw->mac.rar_entry_count - (vf + 1);

	memcpy(adapter->vf_data[vf].vf_mac_addresses, mac_addr, ETH_ALEN);

	igb_rar_set_qsel(adapter, mac_addr, rar_entry, vf);
=======
			  int vf, unsigned char *mac_addr)
{
	struct e1000_hw *hw = &adapter->hw;
	/* VF MAC addresses start at end of receive addresses and moves
	 * towards the first, as a result a collision should not be possible
	 */
	int rar_entry = hw->mac.rar_entry_count - (vf + 1);
	unsigned char *vf_mac_addr = adapter->vf_data[vf].vf_mac_addresses;

	ether_addr_copy(vf_mac_addr, mac_addr);
	ether_addr_copy(adapter->mac_table[rar_entry].addr, mac_addr);
	adapter->mac_table[rar_entry].queue = vf;
	adapter->mac_table[rar_entry].state |= IGB_MAC_STATE_IN_USE;
	igb_rar_set_index(adapter, rar_entry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int igb_ndo_set_vf_mac(struct net_device *netdev, int vf, u8 *mac)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
<<<<<<< HEAD
	if (!is_valid_ether_addr(mac) || (vf >= adapter->vfs_allocated_count))
		return -EINVAL;
	adapter->vf_data[vf].flags |= IGB_VF_FLAG_PF_SET_MAC;
	dev_info(&adapter->pdev->dev, "setting MAC %pM on VF %d\n", mac, vf);
	dev_info(&adapter->pdev->dev, "Reload the VF driver to make this"
				      " change effective.");
	if (test_bit(__IGB_DOWN, &adapter->state)) {
		dev_warn(&adapter->pdev->dev, "The VF MAC address has been set,"
			 " but the PF device is not up.\n");
		dev_warn(&adapter->pdev->dev, "Bring the PF device up before"
			 " attempting to use the VF device.\n");
=======

	if (vf >= adapter->vfs_allocated_count)
		return -EINVAL;

	/* Setting the VF MAC to 0 reverts the IGB_VF_FLAG_PF_SET_MAC
	 * flag and allows to overwrite the MAC via VF netdev.  This
	 * is necessary to allow libvirt a way to restore the original
	 * MAC after unbinding vfio-pci and reloading igbvf after shutting
	 * down a VM.
	 */
	if (is_zero_ether_addr(mac)) {
		adapter->vf_data[vf].flags &= ~IGB_VF_FLAG_PF_SET_MAC;
		dev_info(&adapter->pdev->dev,
			 "remove administratively set MAC on VF %d\n",
			 vf);
	} else if (is_valid_ether_addr(mac)) {
		adapter->vf_data[vf].flags |= IGB_VF_FLAG_PF_SET_MAC;
		dev_info(&adapter->pdev->dev, "setting MAC %pM on VF %d\n",
			 mac, vf);
		dev_info(&adapter->pdev->dev,
			 "Reload the VF driver to make this change effective.");
		/* Generate additional warning if PF is down */
		if (test_bit(__IGB_DOWN, &adapter->state)) {
			dev_warn(&adapter->pdev->dev,
				 "The VF MAC address has been set, but the PF device is not up.\n");
			dev_warn(&adapter->pdev->dev,
				 "Bring the PF device up before attempting to use the VF device.\n");
		}
	} else {
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return igb_set_vf_mac(adapter, vf, mac);
}

static int igb_link_mbps(int internal_link_speed)
{
	switch (internal_link_speed) {
	case SPEED_100:
		return 100;
	case SPEED_1000:
		return 1000;
	default:
		return 0;
	}
}

static void igb_set_vf_rate_limit(struct e1000_hw *hw, int vf, int tx_rate,
				  int link_speed)
{
	int rf_dec, rf_int;
	u32 bcnrc_val;

	if (tx_rate != 0) {
		/* Calculate the rate factor values to set */
		rf_int = link_speed / tx_rate;
		rf_dec = (link_speed - (rf_int * tx_rate));
<<<<<<< HEAD
		rf_dec = (rf_dec * (1<<E1000_RTTBCNRC_RF_INT_SHIFT)) / tx_rate;

		bcnrc_val = E1000_RTTBCNRC_RS_ENA;
		bcnrc_val |= ((rf_int<<E1000_RTTBCNRC_RF_INT_SHIFT) &
		               E1000_RTTBCNRC_RF_INT_MASK);
=======
		rf_dec = (rf_dec * BIT(E1000_RTTBCNRC_RF_INT_SHIFT)) /
			 tx_rate;

		bcnrc_val = E1000_RTTBCNRC_RS_ENA;
		bcnrc_val |= FIELD_PREP(E1000_RTTBCNRC_RF_INT_MASK, rf_int);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bcnrc_val |= (rf_dec & E1000_RTTBCNRC_RF_DEC_MASK);
	} else {
		bcnrc_val = 0;
	}

	wr32(E1000_RTTDQSEL, vf); /* vf X uses queue X */
<<<<<<< HEAD
=======
	/* Set global transmit compensation time to the MMW_SIZE in RTTBCNRM
	 * register. MMW_SIZE=0x014 if 9728-byte jumbo is supported.
	 */
	wr32(E1000_RTTBCNRM, 0x14);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wr32(E1000_RTTBCNRC, bcnrc_val);
}

static void igb_check_vf_rate_limit(struct igb_adapter *adapter)
{
	int actual_link_speed, i;
	bool reset_rate = false;

	/* VF TX rate limit was not set or not supported */
	if ((adapter->vf_rate_link_speed == 0) ||
	    (adapter->hw.mac.type != e1000_82576))
		return;

	actual_link_speed = igb_link_mbps(adapter->link_speed);
	if (actual_link_speed != adapter->vf_rate_link_speed) {
		reset_rate = true;
		adapter->vf_rate_link_speed = 0;
		dev_info(&adapter->pdev->dev,
<<<<<<< HEAD
		         "Link speed has been changed. VF Transmit "
		         "rate is disabled\n");
=======
			 "Link speed has been changed. VF Transmit rate is disabled\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	for (i = 0; i < adapter->vfs_allocated_count; i++) {
		if (reset_rate)
			adapter->vf_data[i].tx_rate = 0;

		igb_set_vf_rate_limit(&adapter->hw, i,
<<<<<<< HEAD
		                      adapter->vf_data[i].tx_rate,
		                      actual_link_speed);
	}
}

static int igb_ndo_set_vf_bw(struct net_device *netdev, int vf, int tx_rate)
=======
				      adapter->vf_data[i].tx_rate,
				      actual_link_speed);
	}
}

static int igb_ndo_set_vf_bw(struct net_device *netdev, int vf,
			     int min_tx_rate, int max_tx_rate)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	int actual_link_speed;

	if (hw->mac.type != e1000_82576)
		return -EOPNOTSUPP;

<<<<<<< HEAD
	actual_link_speed = igb_link_mbps(adapter->link_speed);
	if ((vf >= adapter->vfs_allocated_count) ||
	    (!(rd32(E1000_STATUS) & E1000_STATUS_LU)) ||
	    (tx_rate < 0) || (tx_rate > actual_link_speed))
		return -EINVAL;

	adapter->vf_rate_link_speed = actual_link_speed;
	adapter->vf_data[vf].tx_rate = (u16)tx_rate;
	igb_set_vf_rate_limit(hw, vf, tx_rate, actual_link_speed);
=======
	if (min_tx_rate)
		return -EINVAL;

	actual_link_speed = igb_link_mbps(adapter->link_speed);
	if ((vf >= adapter->vfs_allocated_count) ||
	    (!(rd32(E1000_STATUS) & E1000_STATUS_LU)) ||
	    (max_tx_rate < 0) ||
	    (max_tx_rate > actual_link_speed))
		return -EINVAL;

	adapter->vf_rate_link_speed = actual_link_speed;
	adapter->vf_data[vf].tx_rate = (u16)max_tx_rate;
	igb_set_vf_rate_limit(hw, vf, max_tx_rate, actual_link_speed);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
=======
static int igb_ndo_set_vf_spoofchk(struct net_device *netdev, int vf,
				   bool setting)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	struct e1000_hw *hw = &adapter->hw;
	u32 reg_val, reg_offset;

	if (!adapter->vfs_allocated_count)
		return -EOPNOTSUPP;

	if (vf >= adapter->vfs_allocated_count)
		return -EINVAL;

	reg_offset = (hw->mac.type == e1000_82576) ? E1000_DTXSWC : E1000_TXSWC;
	reg_val = rd32(reg_offset);
	if (setting)
		reg_val |= (BIT(vf) |
			    BIT(vf + E1000_DTXSWC_VLAN_SPOOF_SHIFT));
	else
		reg_val &= ~(BIT(vf) |
			     BIT(vf + E1000_DTXSWC_VLAN_SPOOF_SHIFT));
	wr32(reg_offset, reg_val);

	adapter->vf_data[vf].spoofchk_enabled = setting;
	return 0;
}

static int igb_ndo_set_vf_trust(struct net_device *netdev, int vf, bool setting)
{
	struct igb_adapter *adapter = netdev_priv(netdev);

	if (vf >= adapter->vfs_allocated_count)
		return -EINVAL;
	if (adapter->vf_data[vf].trusted == setting)
		return 0;

	adapter->vf_data[vf].trusted = setting;

	dev_info(&adapter->pdev->dev, "VF %u is %strusted\n",
		 vf, setting ? "" : "not ");
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int igb_ndo_get_vf_config(struct net_device *netdev,
				 int vf, struct ifla_vf_info *ivi)
{
	struct igb_adapter *adapter = netdev_priv(netdev);
	if (vf >= adapter->vfs_allocated_count)
		return -EINVAL;
	ivi->vf = vf;
	memcpy(&ivi->mac, adapter->vf_data[vf].vf_mac_addresses, ETH_ALEN);
<<<<<<< HEAD
	ivi->tx_rate = adapter->vf_data[vf].tx_rate;
	ivi->vlan = adapter->vf_data[vf].pf_vlan;
	ivi->qos = adapter->vf_data[vf].pf_qos;
=======
	ivi->max_tx_rate = adapter->vf_data[vf].tx_rate;
	ivi->min_tx_rate = 0;
	ivi->vlan = adapter->vf_data[vf].pf_vlan;
	ivi->qos = adapter->vf_data[vf].pf_qos;
	ivi->spoofchk = adapter->vf_data[vf].spoofchk_enabled;
	ivi->trusted = adapter->vf_data[vf].trusted;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static void igb_vmm_control(struct igb_adapter *adapter)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 reg;

	switch (hw->mac.type) {
	case e1000_82575:
<<<<<<< HEAD
=======
	case e1000_i210:
	case e1000_i211:
	case e1000_i354:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		/* replication is not supported for 82575 */
		return;
	case e1000_82576:
		/* notify HW that the MAC is adding vlan tags */
		reg = rd32(E1000_DTXCTL);
		reg |= E1000_DTXCTL_VLAN_ADDED;
		wr32(E1000_DTXCTL, reg);
<<<<<<< HEAD
=======
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case e1000_82580:
		/* enable replication vlan tag stripping */
		reg = rd32(E1000_RPLOLR);
		reg |= E1000_RPLOLR_STRVLAN;
		wr32(E1000_RPLOLR, reg);
<<<<<<< HEAD
=======
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case e1000_i350:
		/* none of the above registers are supported by i350 */
		break;
	}

	if (adapter->vfs_allocated_count) {
		igb_vmdq_set_loopback_pf(hw, true);
		igb_vmdq_set_replication_pf(hw, true);
		igb_vmdq_set_anti_spoofing_pf(hw, true,
<<<<<<< HEAD
						adapter->vfs_allocated_count);
=======
					      adapter->vfs_allocated_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		igb_vmdq_set_loopback_pf(hw, false);
		igb_vmdq_set_replication_pf(hw, false);
	}
}

static void igb_init_dmac(struct igb_adapter *adapter, u32 pba)
{
	struct e1000_hw *hw = &adapter->hw;
	u32 dmac_thr;
	u16 hwm;
<<<<<<< HEAD

	if (hw->mac.type > e1000_82580) {
		if (adapter->flags & IGB_FLAG_DMAC) {
			u32 reg;

			/* force threshold to 0. */
			wr32(E1000_DMCTXTH, 0);

			/*
			 * DMA Coalescing high water mark needs to be greater
			 * than the Rx threshold. Set hwm to PBA - max frame
			 * size in 16B units, capping it at PBA - 6KB.
			 */
			hwm = 64 * pba - adapter->max_frame_size / 16;
			if (hwm < 64 * (pba - 6))
				hwm = 64 * (pba - 6);
			reg = rd32(E1000_FCRTC);
			reg &= ~E1000_FCRTC_RTH_COAL_MASK;
			reg |= ((hwm << E1000_FCRTC_RTH_COAL_SHIFT)
				& E1000_FCRTC_RTH_COAL_MASK);
			wr32(E1000_FCRTC, reg);

			/*
			 * Set the DMA Coalescing Rx threshold to PBA - 2 * max
			 * frame size, capping it at PBA - 10KB.
			 */
			dmac_thr = pba - adapter->max_frame_size / 512;
			if (dmac_thr < pba - 10)
				dmac_thr = pba - 10;
			reg = rd32(E1000_DMACR);
			reg &= ~E1000_DMACR_DMACTHR_MASK;
			reg |= ((dmac_thr << E1000_DMACR_DMACTHR_SHIFT)
				& E1000_DMACR_DMACTHR_MASK);
=======
	u32 reg;

	if (hw->mac.type > e1000_82580) {
		if (adapter->flags & IGB_FLAG_DMAC) {
			/* force threshold to 0. */
			wr32(E1000_DMCTXTH, 0);

			/* DMA Coalescing high water mark needs to be greater
			 * than the Rx threshold. Set hwm to PBA - max frame
			 * size in 16B units, capping it at PBA - 6KB.
			 */
			hwm = 64 * (pba - 6);
			reg = rd32(E1000_FCRTC);
			reg &= ~E1000_FCRTC_RTH_COAL_MASK;
			reg |= FIELD_PREP(E1000_FCRTC_RTH_COAL_MASK, hwm);
			wr32(E1000_FCRTC, reg);

			/* Set the DMA Coalescing Rx threshold to PBA - 2 * max
			 * frame size, capping it at PBA - 10KB.
			 */
			dmac_thr = pba - 10;
			reg = rd32(E1000_DMACR);
			reg &= ~E1000_DMACR_DMACTHR_MASK;
			reg |= FIELD_PREP(E1000_DMACR_DMACTHR_MASK, dmac_thr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			/* transition to L0x or L1 if available..*/
			reg |= (E1000_DMACR_DMAC_EN | E1000_DMACR_DMAC_LX_MASK);

			/* watchdog timer= +-1000 usec in 32usec intervals */
			reg |= (1000 >> 5);
<<<<<<< HEAD
			wr32(E1000_DMACR, reg);

			/*
			 * no lower threshold to disable
=======

			/* Disable BMC-to-OS Watchdog Enable */
			if (hw->mac.type != e1000_i354)
				reg &= ~E1000_DMACR_DC_BMC2OSW_EN;
			wr32(E1000_DMACR, reg);

			/* no lower threshold to disable
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 * coalescing(smart fifb)-UTRESH=0
			 */
			wr32(E1000_DMCRTRH, 0);

			reg = (IGB_DMCTLX_DCFLUSH_DIS | 0x4);

			wr32(E1000_DMCTLX, reg);

<<<<<<< HEAD
			/*
			 * free space in tx packet buffer to wake from
=======
			/* free space in tx packet buffer to wake from
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 * DMA coal
			 */
			wr32(E1000_DMCTXTH, (IGB_MIN_TXPBSIZE -
			     (IGB_TX_BUF_4096 + adapter->max_frame_size)) >> 6);
<<<<<<< HEAD

			/*
			 * make low power state decision controlled
			 * by DMA coal
			 */
			reg = rd32(E1000_PCIEMISC);
			reg &= ~E1000_PCIEMISC_LX_DECISION;
=======
		}

		if (hw->mac.type >= e1000_i210 ||
		    (adapter->flags & IGB_FLAG_DMAC)) {
			reg = rd32(E1000_PCIEMISC);
			reg |= E1000_PCIEMISC_LX_DECISION;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			wr32(E1000_PCIEMISC, reg);
		} /* endif adapter->dmac is not disabled */
	} else if (hw->mac.type == e1000_82580) {
		u32 reg = rd32(E1000_PCIEMISC);
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		wr32(E1000_PCIEMISC, reg & ~E1000_PCIEMISC_LX_DECISION);
		wr32(E1000_DMACR, 0);
	}
}

<<<<<<< HEAD
=======
/**
 *  igb_read_i2c_byte - Reads 8 bit word over I2C
 *  @hw: pointer to hardware structure
 *  @byte_offset: byte offset to read
 *  @dev_addr: device address
 *  @data: value read
 *
 *  Performs byte read operation over I2C interface at
 *  a specified device address.
 **/
s32 igb_read_i2c_byte(struct e1000_hw *hw, u8 byte_offset,
		      u8 dev_addr, u8 *data)
{
	struct igb_adapter *adapter = container_of(hw, struct igb_adapter, hw);
	struct i2c_client *this_client = adapter->i2c_client;
	s32 status;
	u16 swfw_mask = 0;

	if (!this_client)
		return E1000_ERR_I2C;

	swfw_mask = E1000_SWFW_PHY0_SM;

	if (hw->mac.ops.acquire_swfw_sync(hw, swfw_mask))
		return E1000_ERR_SWFW_SYNC;

	status = i2c_smbus_read_byte_data(this_client, byte_offset);
	hw->mac.ops.release_swfw_sync(hw, swfw_mask);

	if (status < 0)
		return E1000_ERR_I2C;
	else {
		*data = status;
		return 0;
	}
}

/**
 *  igb_write_i2c_byte - Writes 8 bit word over I2C
 *  @hw: pointer to hardware structure
 *  @byte_offset: byte offset to write
 *  @dev_addr: device address
 *  @data: value to write
 *
 *  Performs byte write operation over I2C interface at
 *  a specified device address.
 **/
s32 igb_write_i2c_byte(struct e1000_hw *hw, u8 byte_offset,
		       u8 dev_addr, u8 data)
{
	struct igb_adapter *adapter = container_of(hw, struct igb_adapter, hw);
	struct i2c_client *this_client = adapter->i2c_client;
	s32 status;
	u16 swfw_mask = E1000_SWFW_PHY0_SM;

	if (!this_client)
		return E1000_ERR_I2C;

	if (hw->mac.ops.acquire_swfw_sync(hw, swfw_mask))
		return E1000_ERR_SWFW_SYNC;
	status = i2c_smbus_write_byte_data(this_client, byte_offset, data);
	hw->mac.ops.release_swfw_sync(hw, swfw_mask);

	if (status)
		return E1000_ERR_I2C;
	else
		return 0;

}

int igb_reinit_queues(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct pci_dev *pdev = adapter->pdev;
	int err = 0;

	if (netif_running(netdev))
		igb_close(netdev);

	igb_reset_interrupt_capability(adapter);

	if (igb_init_interrupt_scheme(adapter, true)) {
		dev_err(&pdev->dev, "Unable to allocate memory for queues\n");
		return -ENOMEM;
	}

	if (netif_running(netdev))
		err = igb_open(netdev);

	return err;
}

static void igb_nfc_filter_exit(struct igb_adapter *adapter)
{
	struct igb_nfc_filter *rule;

	spin_lock(&adapter->nfc_lock);

	hlist_for_each_entry(rule, &adapter->nfc_filter_list, nfc_node)
		igb_erase_filter(adapter, rule);

	hlist_for_each_entry(rule, &adapter->cls_flower_list, nfc_node)
		igb_erase_filter(adapter, rule);

	spin_unlock(&adapter->nfc_lock);
}

static void igb_nfc_filter_restore(struct igb_adapter *adapter)
{
	struct igb_nfc_filter *rule;

	spin_lock(&adapter->nfc_lock);

	hlist_for_each_entry(rule, &adapter->nfc_filter_list, nfc_node)
		igb_add_filter(adapter, rule);

	spin_unlock(&adapter->nfc_lock);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* igb_main.c */
