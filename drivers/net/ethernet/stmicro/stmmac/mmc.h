<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*******************************************************************************
  MMC Header file

  Copyright (C) 2011  STMicroelectronics Ltd

<<<<<<< HEAD
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
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

  Author: Giuseppe Cavallaro <peppe.cavallaro@st.com>
*******************************************************************************/

<<<<<<< HEAD
/* MMC control register */
/* When set, all counter are reset */
#define MMC_CNTRL_COUNTER_RESET		0x1
/* When set, do not roll over zero
 * after reaching the max value*/
=======
#ifndef __MMC_H__
#define __MMC_H__

/* MMC control register */
/* When set, all counter are reset */
#define MMC_CNTRL_COUNTER_RESET		0x1
/* When set, do not roll over zero after reaching the max value*/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define MMC_CNTRL_COUNTER_STOP_ROLLOVER	0x2
#define MMC_CNTRL_RESET_ON_READ		0x4	/* Reset after reading */
#define MMC_CNTRL_COUNTER_FREEZER	0x8	/* Freeze counter values to the
						 * current value.*/
#define MMC_CNTRL_PRESET		0x10
#define MMC_CNTRL_FULL_HALF_PRESET	0x20
<<<<<<< HEAD
=======

#define MMC_GMAC4_OFFSET		0x700
#define MMC_GMAC3_X_OFFSET		0x100
#define MMC_XGMAC_OFFSET		0x800

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct stmmac_counters {
	unsigned int mmc_tx_octetcount_gb;
	unsigned int mmc_tx_framecount_gb;
	unsigned int mmc_tx_broadcastframe_g;
	unsigned int mmc_tx_multicastframe_g;
	unsigned int mmc_tx_64_octets_gb;
	unsigned int mmc_tx_65_to_127_octets_gb;
	unsigned int mmc_tx_128_to_255_octets_gb;
	unsigned int mmc_tx_256_to_511_octets_gb;
	unsigned int mmc_tx_512_to_1023_octets_gb;
	unsigned int mmc_tx_1024_to_max_octets_gb;
	unsigned int mmc_tx_unicast_gb;
	unsigned int mmc_tx_multicast_gb;
	unsigned int mmc_tx_broadcast_gb;
	unsigned int mmc_tx_underflow_error;
	unsigned int mmc_tx_singlecol_g;
	unsigned int mmc_tx_multicol_g;
	unsigned int mmc_tx_deferred;
	unsigned int mmc_tx_latecol;
	unsigned int mmc_tx_exesscol;
	unsigned int mmc_tx_carrier_error;
	unsigned int mmc_tx_octetcount_g;
	unsigned int mmc_tx_framecount_g;
	unsigned int mmc_tx_excessdef;
	unsigned int mmc_tx_pause_frame;
	unsigned int mmc_tx_vlan_frame_g;
<<<<<<< HEAD
=======
	unsigned int mmc_tx_oversize_g;
	unsigned int mmc_tx_lpi_usec;
	unsigned int mmc_tx_lpi_tran;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* MMC RX counter registers */
	unsigned int mmc_rx_framecount_gb;
	unsigned int mmc_rx_octetcount_gb;
	unsigned int mmc_rx_octetcount_g;
	unsigned int mmc_rx_broadcastframe_g;
	unsigned int mmc_rx_multicastframe_g;
<<<<<<< HEAD
	unsigned int mmc_rx_crc_errror;
=======
	unsigned int mmc_rx_crc_error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int mmc_rx_align_error;
	unsigned int mmc_rx_run_error;
	unsigned int mmc_rx_jabber_error;
	unsigned int mmc_rx_undersize_g;
	unsigned int mmc_rx_oversize_g;
	unsigned int mmc_rx_64_octets_gb;
	unsigned int mmc_rx_65_to_127_octets_gb;
	unsigned int mmc_rx_128_to_255_octets_gb;
	unsigned int mmc_rx_256_to_511_octets_gb;
	unsigned int mmc_rx_512_to_1023_octets_gb;
	unsigned int mmc_rx_1024_to_max_octets_gb;
	unsigned int mmc_rx_unicast_g;
	unsigned int mmc_rx_length_error;
	unsigned int mmc_rx_autofrangetype;
	unsigned int mmc_rx_pause_frames;
	unsigned int mmc_rx_fifo_overflow;
	unsigned int mmc_rx_vlan_frames_gb;
	unsigned int mmc_rx_watchdog_error;
<<<<<<< HEAD
	/* IPC */
	unsigned int mmc_rx_ipc_intr_mask;
	unsigned int mmc_rx_ipc_intr;
=======
	unsigned int mmc_rx_error;
	unsigned int mmc_rx_lpi_usec;
	unsigned int mmc_rx_lpi_tran;
	unsigned int mmc_rx_discard_frames_gb;
	unsigned int mmc_rx_discard_octets_gb;
	unsigned int mmc_rx_align_err_frames;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* IPv4 */
	unsigned int mmc_rx_ipv4_gd;
	unsigned int mmc_rx_ipv4_hderr;
	unsigned int mmc_rx_ipv4_nopay;
	unsigned int mmc_rx_ipv4_frag;
	unsigned int mmc_rx_ipv4_udsbl;

	unsigned int mmc_rx_ipv4_gd_octets;
	unsigned int mmc_rx_ipv4_hderr_octets;
	unsigned int mmc_rx_ipv4_nopay_octets;
	unsigned int mmc_rx_ipv4_frag_octets;
	unsigned int mmc_rx_ipv4_udsbl_octets;

	/* IPV6 */
	unsigned int mmc_rx_ipv6_gd_octets;
	unsigned int mmc_rx_ipv6_hderr_octets;
	unsigned int mmc_rx_ipv6_nopay_octets;

	unsigned int mmc_rx_ipv6_gd;
	unsigned int mmc_rx_ipv6_hderr;
	unsigned int mmc_rx_ipv6_nopay;

	/* Protocols */
	unsigned int mmc_rx_udp_gd;
	unsigned int mmc_rx_udp_err;
	unsigned int mmc_rx_tcp_gd;
	unsigned int mmc_rx_tcp_err;
	unsigned int mmc_rx_icmp_gd;
	unsigned int mmc_rx_icmp_err;

	unsigned int mmc_rx_udp_gd_octets;
	unsigned int mmc_rx_udp_err_octets;
	unsigned int mmc_rx_tcp_gd_octets;
	unsigned int mmc_rx_tcp_err_octets;
	unsigned int mmc_rx_icmp_gd_octets;
	unsigned int mmc_rx_icmp_err_octets;
<<<<<<< HEAD
};

extern void dwmac_mmc_ctrl(void __iomem *ioaddr, unsigned int mode);
extern void dwmac_mmc_intr_all_mask(void __iomem *ioaddr);
extern void dwmac_mmc_read(void __iomem *ioaddr, struct stmmac_counters *mmc);
=======

	/* Stream-Gate Filter */
	unsigned int mmc_sgf_pass_fragment_cntr;
	unsigned int mmc_sgf_fail_fragment_cntr;

	/* FPE */
	unsigned int mmc_tx_fpe_fragment_cntr;
	unsigned int mmc_tx_hold_req_cntr;
	unsigned int mmc_tx_gate_overrun_cntr;
	unsigned int mmc_rx_packet_assembly_err_cntr;
	unsigned int mmc_rx_packet_smd_err_cntr;
	unsigned int mmc_rx_packet_assembly_ok_cntr;
	unsigned int mmc_rx_fpe_fragment_cntr;
};

#endif /* __MMC_H__ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
