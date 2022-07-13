<<<<<<< HEAD
/****************************************************************************
 * Driver for Solarflare Solarstorm network controllers and boards
 * Copyright 2005-2006 Fen Systems Ltd.
 * Copyright 2006-2011 Solarflare Communications Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/****************************************************************************
 * Driver for Solarflare network controllers and boards
 * Copyright 2005-2006 Fen Systems Ltd.
 * Copyright 2006-2013 Solarflare Communications Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef EFX_NIC_H
#define EFX_NIC_H

<<<<<<< HEAD
#include <linux/i2c-algo-bit.h>
#include "net_driver.h"
#include "efx.h"
#include "mcdi.h"
#include "spi.h"

/*
 * Falcon hardware control
 */

enum {
	EFX_REV_FALCON_A0 = 0,
	EFX_REV_FALCON_A1 = 1,
	EFX_REV_FALCON_B0 = 2,
	EFX_REV_SIENA_A0 = 3,
};

static inline int efx_nic_rev(struct efx_nic *efx)
{
	return efx->type->revision;
}

extern u32 efx_nic_fpga_ver(struct efx_nic *efx);

/* NIC has two interlinked PCI functions for the same port. */
static inline bool efx_nic_is_dual_func(struct efx_nic *efx)
{
	return efx_nic_rev(efx) < EFX_REV_FALCON_B0;
}
=======
#include "nic_common.h"
#include "efx.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum {
	PHY_TYPE_NONE = 0,
	PHY_TYPE_TXC43128 = 1,
	PHY_TYPE_88E1111 = 2,
	PHY_TYPE_SFX7101 = 3,
	PHY_TYPE_QT2022C2 = 4,
	PHY_TYPE_PM8358 = 6,
	PHY_TYPE_SFT9001A = 8,
	PHY_TYPE_QT2025C = 9,
	PHY_TYPE_SFT9001B = 10,
};

<<<<<<< HEAD
#define FALCON_XMAC_LOOPBACKS			\
	((1 << LOOPBACK_XGMII) |		\
	 (1 << LOOPBACK_XGXS) |			\
	 (1 << LOOPBACK_XAUI))

#define FALCON_GMAC_LOOPBACKS			\
	(1 << LOOPBACK_GMAC)

/* Alignment of PCIe DMA boundaries (4KB) */
#define EFX_PAGE_SIZE	4096
/* Size and alignment of buffer table entries (same) */
#define EFX_BUF_SIZE	EFX_PAGE_SIZE

/**
 * struct falcon_board_type - board operations and type information
 * @id: Board type id, as found in NVRAM
 * @init: Allocate resources and initialise peripheral hardware
 * @init_phy: Do board-specific PHY initialisation
 * @fini: Shut down hardware and free resources
 * @set_id_led: Set state of identifying LED or revert to automatic function
 * @monitor: Board-specific health check function
 */
struct falcon_board_type {
	u8 id;
	int (*init) (struct efx_nic *nic);
	void (*init_phy) (struct efx_nic *efx);
	void (*fini) (struct efx_nic *nic);
	void (*set_id_led) (struct efx_nic *efx, enum efx_led_mode mode);
	int (*monitor) (struct efx_nic *nic);
};

/**
 * struct falcon_board - board information
 * @type: Type of board
 * @major: Major rev. ('A', 'B' ...)
 * @minor: Minor rev. (0, 1, ...)
 * @i2c_adap: I2C adapter for on-board peripherals
 * @i2c_data: Data for bit-banging algorithm
 * @hwmon_client: I2C client for hardware monitor
 * @ioexp_client: I2C client for power/port control
 */
struct falcon_board {
	const struct falcon_board_type *type;
	int major;
	int minor;
	struct i2c_adapter i2c_adap;
	struct i2c_algo_bit_data i2c_data;
	struct i2c_client *hwmon_client, *ioexp_client;
};

/**
 * struct falcon_nic_data - Falcon NIC state
 * @pci_dev2: Secondary function of Falcon A
 * @board: Board state and functions
 * @stats_disable_count: Nest count for disabling statistics fetches
 * @stats_pending: Is there a pending DMA of MAC statistics.
 * @stats_timer: A timer for regularly fetching MAC statistics.
 * @stats_dma_done: Pointer to the flag which indicates DMA completion.
 * @spi_flash: SPI flash device
 * @spi_eeprom: SPI EEPROM device
 * @spi_lock: SPI bus lock
 * @mdio_lock: MDIO bus lock
 * @xmac_poll_required: XMAC link state needs polling
 */
struct falcon_nic_data {
	struct pci_dev *pci_dev2;
	struct falcon_board board;
	unsigned int stats_disable_count;
	bool stats_pending;
	struct timer_list stats_timer;
	u32 *stats_dma_done;
	struct efx_spi_device spi_flash;
	struct efx_spi_device spi_eeprom;
	struct mutex spi_lock;
	struct mutex mdio_lock;
	bool xmac_poll_required;
};

static inline struct falcon_board *falcon_board(struct efx_nic *efx)
{
	struct falcon_nic_data *data = efx->nic_data;
	return &data->board;
}

/**
 * struct siena_nic_data - Siena NIC state
 * @mcdi: Management-Controller-to-Driver Interface
 * @wol_filter_id: Wake-on-LAN packet filter id
 * @hwmon: Hardware monitor state
 */
struct siena_nic_data {
	struct efx_mcdi_iface mcdi;
	int wol_filter_id;
#ifdef CONFIG_SFC_MCDI_MON
	struct efx_mcdi_mon hwmon;
#endif
};

#ifdef CONFIG_SFC_MCDI_MON
static inline struct efx_mcdi_mon *efx_mcdi_mon(struct efx_nic *efx)
{
	struct siena_nic_data *nic_data;
	EFX_BUG_ON_PARANOID(efx_nic_rev(efx) < EFX_REV_SIENA_A0);
	nic_data = efx->nic_data;
	return &nic_data->hwmon;
}
#endif

/*
 * On the SFC9000 family each port is associated with 1 PCI physical
 * function (PF) handled by sfc and a configurable number of virtual
 * functions (VFs) that may be handled by some other driver, often in
 * a VM guest.  The queue pointer registers are mapped in both PF and
 * VF BARs such that an 8K region provides access to a single RX, TX
 * and event queue (collectively a Virtual Interface, VI or VNIC).
 *
 * The PF has access to all 1024 VIs while VFs are mapped to VIs
 * according to VI_BASE and VI_SCALE: VF i has access to VIs numbered
 * in range [VI_BASE + i << VI_SCALE, VI_BASE + i + 1 << VI_SCALE).
 * The number of VIs and the VI_SCALE value are configurable but must
 * be established at boot time by firmware.
 */

/* Maximum VI_SCALE parameter supported by Siena */
#define EFX_VI_SCALE_MAX 6
/* Base VI to use for SR-IOV. Must be aligned to (1 << EFX_VI_SCALE_MAX),
 * so this is the smallest allowed value. */
#define EFX_VI_BASE 128U
/* Maximum number of VFs allowed */
#define EFX_VF_COUNT_MAX 127
/* Limit EVQs on VFs to be only 8k to reduce buffer table reservation */
#define EFX_MAX_VF_EVQ_SIZE 8192UL
/* The number of buffer table entries reserved for each VI on a VF */
#define EFX_VF_BUFTBL_PER_VI					\
	((EFX_MAX_VF_EVQ_SIZE + 2 * EFX_MAX_DMAQ_SIZE) *	\
	 sizeof(efx_qword_t) / EFX_BUF_SIZE)

#ifdef CONFIG_SFC_SRIOV

static inline bool efx_sriov_wanted(struct efx_nic *efx)
{
	return efx->vf_count != 0;
}
static inline bool efx_sriov_enabled(struct efx_nic *efx)
{
	return efx->vf_init_count != 0;
}
static inline unsigned int efx_vf_size(struct efx_nic *efx)
{
	return 1 << efx->vi_scale;
}

extern int efx_init_sriov(void);
extern void efx_sriov_probe(struct efx_nic *efx);
extern int efx_sriov_init(struct efx_nic *efx);
extern void efx_sriov_mac_address_changed(struct efx_nic *efx);
extern void efx_sriov_tx_flush_done(struct efx_nic *efx, efx_qword_t *event);
extern void efx_sriov_rx_flush_done(struct efx_nic *efx, efx_qword_t *event);
extern void efx_sriov_event(struct efx_channel *channel, efx_qword_t *event);
extern void efx_sriov_desc_fetch_err(struct efx_nic *efx, unsigned dmaq);
extern void efx_sriov_flr(struct efx_nic *efx, unsigned flr);
extern void efx_sriov_reset(struct efx_nic *efx);
extern void efx_sriov_fini(struct efx_nic *efx);
extern void efx_fini_sriov(void);

#else

static inline bool efx_sriov_wanted(struct efx_nic *efx) { return false; }
static inline bool efx_sriov_enabled(struct efx_nic *efx) { return false; }
static inline unsigned int efx_vf_size(struct efx_nic *efx) { return 0; }

static inline int efx_init_sriov(void) { return 0; }
static inline void efx_sriov_probe(struct efx_nic *efx) {}
static inline int efx_sriov_init(struct efx_nic *efx) { return -EOPNOTSUPP; }
static inline void efx_sriov_mac_address_changed(struct efx_nic *efx) {}
static inline void efx_sriov_tx_flush_done(struct efx_nic *efx,
					   efx_qword_t *event) {}
static inline void efx_sriov_rx_flush_done(struct efx_nic *efx,
					   efx_qword_t *event) {}
static inline void efx_sriov_event(struct efx_channel *channel,
				   efx_qword_t *event) {}
static inline void efx_sriov_desc_fetch_err(struct efx_nic *efx, unsigned dmaq) {}
static inline void efx_sriov_flr(struct efx_nic *efx, unsigned flr) {}
static inline void efx_sriov_reset(struct efx_nic *efx) {}
static inline void efx_sriov_fini(struct efx_nic *efx) {}
static inline void efx_fini_sriov(void) {}

#endif

extern int efx_sriov_set_vf_mac(struct net_device *dev, int vf, u8 *mac);
extern int efx_sriov_set_vf_vlan(struct net_device *dev, int vf,
				 u16 vlan, u8 qos);
extern int efx_sriov_get_vf_config(struct net_device *dev, int vf,
				   struct ifla_vf_info *ivf);
extern int efx_sriov_set_vf_spoofchk(struct net_device *net_dev, int vf,
				     bool spoofchk);

extern const struct efx_nic_type falcon_a1_nic_type;
extern const struct efx_nic_type falcon_b0_nic_type;
extern const struct efx_nic_type siena_a0_nic_type;

/**************************************************************************
 *
 * Externs
 *
 **************************************************************************
 */

extern int falcon_probe_board(struct efx_nic *efx, u16 revision_info);

/* TX data path */
extern int efx_nic_probe_tx(struct efx_tx_queue *tx_queue);
extern void efx_nic_init_tx(struct efx_tx_queue *tx_queue);
extern void efx_nic_fini_tx(struct efx_tx_queue *tx_queue);
extern void efx_nic_remove_tx(struct efx_tx_queue *tx_queue);
extern void efx_nic_push_buffers(struct efx_tx_queue *tx_queue);

/* RX data path */
extern int efx_nic_probe_rx(struct efx_rx_queue *rx_queue);
extern void efx_nic_init_rx(struct efx_rx_queue *rx_queue);
extern void efx_nic_fini_rx(struct efx_rx_queue *rx_queue);
extern void efx_nic_remove_rx(struct efx_rx_queue *rx_queue);
extern void efx_nic_notify_rx_desc(struct efx_rx_queue *rx_queue);
extern void efx_nic_generate_fill_event(struct efx_rx_queue *rx_queue);

/* Event data path */
extern int efx_nic_probe_eventq(struct efx_channel *channel);
extern void efx_nic_init_eventq(struct efx_channel *channel);
extern void efx_nic_fini_eventq(struct efx_channel *channel);
extern void efx_nic_remove_eventq(struct efx_channel *channel);
extern int efx_nic_process_eventq(struct efx_channel *channel, int rx_quota);
extern void efx_nic_eventq_read_ack(struct efx_channel *channel);
extern bool efx_nic_event_present(struct efx_channel *channel);

/* MAC/PHY */
extern void falcon_drain_tx_fifo(struct efx_nic *efx);
extern void falcon_reconfigure_mac_wrapper(struct efx_nic *efx);
extern bool falcon_xmac_check_fault(struct efx_nic *efx);
extern int falcon_reconfigure_xmac(struct efx_nic *efx);
extern void falcon_update_stats_xmac(struct efx_nic *efx);

/* Interrupts and test events */
extern int efx_nic_init_interrupt(struct efx_nic *efx);
extern void efx_nic_enable_interrupts(struct efx_nic *efx);
extern void efx_nic_event_test_start(struct efx_channel *channel);
extern void efx_nic_irq_test_start(struct efx_nic *efx);
extern void efx_nic_disable_interrupts(struct efx_nic *efx);
extern void efx_nic_fini_interrupt(struct efx_nic *efx);
extern irqreturn_t efx_nic_fatal_interrupt(struct efx_nic *efx);
extern irqreturn_t falcon_legacy_interrupt_a1(int irq, void *dev_id);
extern void falcon_irq_ack_a1(struct efx_nic *efx);

static inline int efx_nic_event_test_irq_cpu(struct efx_channel *channel)
{
	return ACCESS_ONCE(channel->event_test_cpu);
}
static inline int efx_nic_irq_test_irq_cpu(struct efx_nic *efx)
{
	return ACCESS_ONCE(efx->last_irq_cpu);
}

/* Global Resources */
extern int efx_nic_flush_queues(struct efx_nic *efx);
extern void siena_prepare_flush(struct efx_nic *efx);
extern void siena_finish_flush(struct efx_nic *efx);
extern void falcon_start_nic_stats(struct efx_nic *efx);
extern void falcon_stop_nic_stats(struct efx_nic *efx);
extern void falcon_setup_xaui(struct efx_nic *efx);
extern int falcon_reset_xaui(struct efx_nic *efx);
extern void
efx_nic_dimension_resources(struct efx_nic *efx, unsigned sram_lim_qw);
extern void efx_nic_init_common(struct efx_nic *efx);
extern void efx_nic_push_rx_indir_table(struct efx_nic *efx);

int efx_nic_alloc_buffer(struct efx_nic *efx, struct efx_buffer *buffer,
			 unsigned int len);
void efx_nic_free_buffer(struct efx_nic *efx, struct efx_buffer *buffer);

/* Tests */
struct efx_nic_register_test {
	unsigned address;
	efx_oword_t mask;
};
extern int efx_nic_test_registers(struct efx_nic *efx,
				  const struct efx_nic_register_test *regs,
				  size_t n_regs);

extern size_t efx_nic_get_regs_len(struct efx_nic *efx);
extern void efx_nic_get_regs(struct efx_nic *efx, void *buf);

/**************************************************************************
 *
 * Falcon MAC stats
 *
 **************************************************************************
 */

#define FALCON_STAT_OFFSET(falcon_stat) EFX_VAL(falcon_stat, offset)
#define FALCON_STAT_WIDTH(falcon_stat) EFX_VAL(falcon_stat, WIDTH)

/* Retrieve statistic from statistics block */
#define FALCON_STAT(efx, falcon_stat, efx_stat) do {		\
	if (FALCON_STAT_WIDTH(falcon_stat) == 16)		\
		(efx)->mac_stats.efx_stat += le16_to_cpu(	\
			*((__force __le16 *)				\
			  (efx->stats_buffer.addr +		\
			   FALCON_STAT_OFFSET(falcon_stat))));	\
	else if (FALCON_STAT_WIDTH(falcon_stat) == 32)		\
		(efx)->mac_stats.efx_stat += le32_to_cpu(	\
			*((__force __le32 *)				\
			  (efx->stats_buffer.addr +		\
			   FALCON_STAT_OFFSET(falcon_stat))));	\
	else							\
		(efx)->mac_stats.efx_stat += le64_to_cpu(	\
			*((__force __le64 *)				\
			  (efx->stats_buffer.addr +		\
			   FALCON_STAT_OFFSET(falcon_stat))));	\
	} while (0)

#define FALCON_MAC_STATS_SIZE 0x100

#define MAC_DATA_LBN 0
#define MAC_DATA_WIDTH 32

extern void efx_generate_event(struct efx_nic *efx, unsigned int evq,
			       efx_qword_t *event);

extern void falcon_poll_xmac(struct efx_nic *efx);
=======
enum {
	EF10_STAT_port_tx_bytes = GENERIC_STAT_COUNT,
	EF10_STAT_port_tx_packets,
	EF10_STAT_port_tx_pause,
	EF10_STAT_port_tx_control,
	EF10_STAT_port_tx_unicast,
	EF10_STAT_port_tx_multicast,
	EF10_STAT_port_tx_broadcast,
	EF10_STAT_port_tx_lt64,
	EF10_STAT_port_tx_64,
	EF10_STAT_port_tx_65_to_127,
	EF10_STAT_port_tx_128_to_255,
	EF10_STAT_port_tx_256_to_511,
	EF10_STAT_port_tx_512_to_1023,
	EF10_STAT_port_tx_1024_to_15xx,
	EF10_STAT_port_tx_15xx_to_jumbo,
	EF10_STAT_port_rx_bytes,
	EF10_STAT_port_rx_bytes_minus_good_bytes,
	EF10_STAT_port_rx_good_bytes,
	EF10_STAT_port_rx_bad_bytes,
	EF10_STAT_port_rx_packets,
	EF10_STAT_port_rx_good,
	EF10_STAT_port_rx_bad,
	EF10_STAT_port_rx_pause,
	EF10_STAT_port_rx_control,
	EF10_STAT_port_rx_unicast,
	EF10_STAT_port_rx_multicast,
	EF10_STAT_port_rx_broadcast,
	EF10_STAT_port_rx_lt64,
	EF10_STAT_port_rx_64,
	EF10_STAT_port_rx_65_to_127,
	EF10_STAT_port_rx_128_to_255,
	EF10_STAT_port_rx_256_to_511,
	EF10_STAT_port_rx_512_to_1023,
	EF10_STAT_port_rx_1024_to_15xx,
	EF10_STAT_port_rx_15xx_to_jumbo,
	EF10_STAT_port_rx_gtjumbo,
	EF10_STAT_port_rx_bad_gtjumbo,
	EF10_STAT_port_rx_overflow,
	EF10_STAT_port_rx_align_error,
	EF10_STAT_port_rx_length_error,
	EF10_STAT_port_rx_nodesc_drops,
	EF10_STAT_port_rx_pm_trunc_bb_overflow,
	EF10_STAT_port_rx_pm_discard_bb_overflow,
	EF10_STAT_port_rx_pm_trunc_vfifo_full,
	EF10_STAT_port_rx_pm_discard_vfifo_full,
	EF10_STAT_port_rx_pm_trunc_qbb,
	EF10_STAT_port_rx_pm_discard_qbb,
	EF10_STAT_port_rx_pm_discard_mapping,
	EF10_STAT_port_rx_dp_q_disabled_packets,
	EF10_STAT_port_rx_dp_di_dropped_packets,
	EF10_STAT_port_rx_dp_streaming_packets,
	EF10_STAT_port_rx_dp_hlb_fetch,
	EF10_STAT_port_rx_dp_hlb_wait,
	EF10_STAT_rx_unicast,
	EF10_STAT_rx_unicast_bytes,
	EF10_STAT_rx_multicast,
	EF10_STAT_rx_multicast_bytes,
	EF10_STAT_rx_broadcast,
	EF10_STAT_rx_broadcast_bytes,
	EF10_STAT_rx_bad,
	EF10_STAT_rx_bad_bytes,
	EF10_STAT_rx_overflow,
	EF10_STAT_tx_unicast,
	EF10_STAT_tx_unicast_bytes,
	EF10_STAT_tx_multicast,
	EF10_STAT_tx_multicast_bytes,
	EF10_STAT_tx_broadcast,
	EF10_STAT_tx_broadcast_bytes,
	EF10_STAT_tx_bad,
	EF10_STAT_tx_bad_bytes,
	EF10_STAT_tx_overflow,
	EF10_STAT_V1_COUNT,
	EF10_STAT_fec_uncorrected_errors = EF10_STAT_V1_COUNT,
	EF10_STAT_fec_corrected_errors,
	EF10_STAT_fec_corrected_symbols_lane0,
	EF10_STAT_fec_corrected_symbols_lane1,
	EF10_STAT_fec_corrected_symbols_lane2,
	EF10_STAT_fec_corrected_symbols_lane3,
	EF10_STAT_ctpio_vi_busy_fallback,
	EF10_STAT_ctpio_long_write_success,
	EF10_STAT_ctpio_missing_dbell_fail,
	EF10_STAT_ctpio_overflow_fail,
	EF10_STAT_ctpio_underflow_fail,
	EF10_STAT_ctpio_timeout_fail,
	EF10_STAT_ctpio_noncontig_wr_fail,
	EF10_STAT_ctpio_frm_clobber_fail,
	EF10_STAT_ctpio_invalid_wr_fail,
	EF10_STAT_ctpio_vi_clobber_fallback,
	EF10_STAT_ctpio_unqualified_fallback,
	EF10_STAT_ctpio_runt_fallback,
	EF10_STAT_ctpio_success,
	EF10_STAT_ctpio_fallback,
	EF10_STAT_ctpio_poison,
	EF10_STAT_ctpio_erase,
	EF10_STAT_COUNT
};

/* Maximum number of TX PIO buffers we may allocate to a function.
 * This matches the total number of buffers on each SFC9100-family
 * controller.
 */
#define EF10_TX_PIOBUF_COUNT 16

/**
 * struct efx_ef10_nic_data - EF10 architecture NIC state
 * @mcdi_buf: DMA buffer for MCDI
 * @warm_boot_count: Last seen MC warm boot count
 * @vi_base: Absolute index of first VI in this function
 * @n_allocated_vis: Number of VIs allocated to this function
 * @n_piobufs: Number of PIO buffers allocated to this function
 * @wc_membase: Base address of write-combining mapping of the memory BAR
 * @pio_write_base: Base address for writing PIO buffers
 * @pio_write_vi_base: Relative VI number for @pio_write_base
 * @piobuf_handle: Handle of each PIO buffer allocated
 * @piobuf_size: size of a single PIO buffer
 * @must_restore_piobufs: Flag: PIO buffers have yet to be restored after MC
 *	reboot
 * @mc_stats: Scratch buffer for converting statistics to the kernel's format
 * @stats: Hardware statistics
 * @workaround_35388: Flag: firmware supports workaround for bug 35388
 * @workaround_26807: Flag: firmware supports workaround for bug 26807
 * @workaround_61265: Flag: firmware supports workaround for bug 61265
 * @must_check_datapath_caps: Flag: @datapath_caps needs to be revalidated
 *	after MC reboot
 * @datapath_caps: Capabilities of datapath firmware (FLAGS1 field of
 *	%MC_CMD_GET_CAPABILITIES response)
 * @datapath_caps2: Further Capabilities of datapath firmware (FLAGS2 field of
 * %MC_CMD_GET_CAPABILITIES response)
 * @rx_dpcpu_fw_id: Firmware ID of the RxDPCPU
 * @tx_dpcpu_fw_id: Firmware ID of the TxDPCPU
 * @must_probe_vswitching: Flag: vswitching has yet to be setup after MC reboot
 * @pf_index: The number for this PF, or the parent PF if this is a VF
#ifdef CONFIG_SFC_SRIOV
 * @vf: Pointer to VF data structure
#endif
 * @vport_mac: The MAC address on the vport, only for PFs; VFs will be zero
 * @vlan_list: List of VLANs added over the interface. Serialised by vlan_lock.
 * @vlan_lock: Lock to serialize access to vlan_list.
 * @udp_tunnels: UDP tunnel port numbers and types.
 * @udp_tunnels_dirty: flag indicating a reboot occurred while pushing
 *	@udp_tunnels to hardware and thus the push must be re-done.
 * @udp_tunnels_lock: Serialises writes to @udp_tunnels and @udp_tunnels_dirty.
 */
struct efx_ef10_nic_data {
	struct efx_buffer mcdi_buf;
	u16 warm_boot_count;
	unsigned int vi_base;
	unsigned int n_allocated_vis;
	unsigned int n_piobufs;
	void __iomem *wc_membase, *pio_write_base;
	unsigned int pio_write_vi_base;
	unsigned int piobuf_handle[EF10_TX_PIOBUF_COUNT];
	u16 piobuf_size;
	bool must_restore_piobufs;
	__le64 *mc_stats;
	u64 stats[EF10_STAT_COUNT];
	bool workaround_35388;
	bool workaround_26807;
	bool workaround_61265;
	bool must_check_datapath_caps;
	u32 datapath_caps;
	u32 datapath_caps2;
	unsigned int rx_dpcpu_fw_id;
	unsigned int tx_dpcpu_fw_id;
	bool must_probe_vswitching;
	unsigned int pf_index;
	u8 port_id[ETH_ALEN];
#ifdef CONFIG_SFC_SRIOV
	unsigned int vf_index;
	struct ef10_vf *vf;
#endif
	u8 vport_mac[ETH_ALEN];
	struct list_head vlan_list;
	struct mutex vlan_lock;
	struct efx_udp_tunnel udp_tunnels[16];
	bool udp_tunnels_dirty;
	struct mutex udp_tunnels_lock;
	u64 licensed_features;
};

/* TSOv2 */
int efx_ef10_tx_tso_desc(struct efx_tx_queue *tx_queue, struct sk_buff *skb,
			 bool *data_mapped);

extern const struct efx_nic_type efx_hunt_a0_nic_type;
extern const struct efx_nic_type efx_hunt_a0_vf_nic_type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* EFX_NIC_H */
