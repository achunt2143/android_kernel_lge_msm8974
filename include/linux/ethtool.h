<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ethtool.h: Defines for Linux ethtool.
 *
 * Copyright (C) 1998 David S. Miller (davem@redhat.com)
 * Copyright 2001 Jeff Garzik <jgarzik@pobox.com>
 * Portions Copyright 2001 Sun Microsystems (thockin@sun.com)
 * Portions Copyright 2002 Intel (eli.kupermann@intel.com,
 *                                christopher.leech@intel.com,
 *                                scott.feldman@intel.com)
 * Portions Copyright (C) Sun Microsystems 2008
 */
<<<<<<< HEAD

#ifndef _LINUX_ETHTOOL_H
#define _LINUX_ETHTOOL_H

#ifdef __KERNEL__
#include <linux/compat.h>
#endif
#include <linux/types.h>
#include <linux/if_ether.h>

/* This should work for both 32 and 64 bit userland. */
struct ethtool_cmd {
	__u32	cmd;
	__u32	supported;	/* Features this interface supports */
	__u32	advertising;	/* Features this interface advertises */
	__u16	speed;	        /* The forced speed (lower bits) in
				 * Mbps. Please use
				 * ethtool_cmd_speed()/_set() to
				 * access it */
	__u8	duplex;		/* Duplex, half or full */
	__u8	port;		/* Which connector port */
	__u8	phy_address;	/* MDIO PHY address (PRTAD for clause 45).
				 * May be read-only or read-write
				 * depending on the driver.
				 */
	__u8	transceiver;	/* Which transceiver to use */
	__u8	autoneg;	/* Enable or disable autonegotiation */
	__u8	mdio_support;	/* MDIO protocols supported.  Read-only.
				 * Not set by all drivers.
				 */
	__u32	maxtxpkt;	/* Tx pkts before generating tx int */
	__u32	maxrxpkt;	/* Rx pkts before generating rx int */
	__u16	speed_hi;       /* The forced speed (upper
				 * bits) in Mbps. Please use
				 * ethtool_cmd_speed()/_set() to
				 * access it */
	__u8	eth_tp_mdix;
	__u8	reserved2;
	__u32	lp_advertising;	/* Features the link partner advertises */
	__u32	reserved[2];
};

static inline void ethtool_cmd_speed_set(struct ethtool_cmd *ep,
					 __u32 speed)
{

	ep->speed = (__u16)speed;
	ep->speed_hi = (__u16)(speed >> 16);
}

static inline __u32 ethtool_cmd_speed(const struct ethtool_cmd *ep)
{
	return (ep->speed_hi << 16) | ep->speed;
}

/* Device supports clause 22 register access to PHY or peripherals
 * using the interface defined in <linux/mii.h>.  This should not be
 * set if there are known to be no such peripherals present or if
 * the driver only emulates clause 22 registers for compatibility.
 */
#define ETH_MDIO_SUPPORTS_C22	1

/* Device supports clause 45 register access to PHY or peripherals
 * using the interface defined in <linux/mii.h> and <linux/mdio.h>.
 * This should not be set if there are known to be no such peripherals
 * present.
 */
#define ETH_MDIO_SUPPORTS_C45	2

#define ETHTOOL_FWVERS_LEN	32
#define ETHTOOL_BUSINFO_LEN	32
/* these strings are set to whatever the driver author decides... */
struct ethtool_drvinfo {
	__u32	cmd;
	char	driver[32];	/* driver short name, "tulip", "eepro100" */
	char	version[32];	/* driver version string */
	char	fw_version[ETHTOOL_FWVERS_LEN];	/* firmware version string */
	char	bus_info[ETHTOOL_BUSINFO_LEN];	/* Bus info for this IF. */
				/* For PCI devices, use pci_name(pci_dev). */
	char	reserved1[32];
	char	reserved2[12];
				/*
				 * Some struct members below are filled in
				 * using ops->get_sset_count().  Obtaining
				 * this info from ethtool_drvinfo is now
				 * deprecated; Use ETHTOOL_GSSET_INFO
				 * instead.
				 */
	__u32	n_priv_flags;	/* number of flags valid in ETHTOOL_GPFLAGS */
	__u32	n_stats;	/* number of u64's from ETHTOOL_GSTATS */
	__u32	testinfo_len;
	__u32	eedump_len;	/* Size of data from ETHTOOL_GEEPROM (bytes) */
	__u32	regdump_len;	/* Size of data from ETHTOOL_GREGS (bytes) */
};

#define SOPASS_MAX	6
/* wake-on-lan settings */
struct ethtool_wolinfo {
	__u32	cmd;
	__u32	supported;
	__u32	wolopts;
	__u8	sopass[SOPASS_MAX]; /* SecureOn(tm) password */
};

/* for passing single values */
struct ethtool_value {
	__u32	cmd;
	__u32	data;
};

/* for passing big chunks of data */
struct ethtool_regs {
	__u32	cmd;
	__u32	version; /* driver-specific, indicates different chips/revs */
	__u32	len; /* bytes */
	__u8	data[0];
};

/* for passing EEPROM chunks */
struct ethtool_eeprom {
	__u32	cmd;
	__u32	magic;
	__u32	offset; /* in bytes */
	__u32	len; /* in bytes */
	__u8	data[0];
};

/**
 * struct ethtool_coalesce - coalescing parameters for IRQs and stats updates
 * @cmd: ETHTOOL_{G,S}COALESCE
 * @rx_coalesce_usecs: How many usecs to delay an RX interrupt after
 *	a packet arrives.
 * @rx_max_coalesced_frames: Maximum number of packets to receive
 *	before an RX interrupt.
 * @rx_coalesce_usecs_irq: Same as @rx_coalesce_usecs, except that
 *	this value applies while an IRQ is being serviced by the host.
 * @rx_max_coalesced_frames_irq: Same as @rx_max_coalesced_frames,
 *	except that this value applies while an IRQ is being serviced
 *	by the host.
 * @tx_coalesce_usecs: How many usecs to delay a TX interrupt after
 *	a packet is sent.
 * @tx_max_coalesced_frames: Maximum number of packets to be sent
 *	before a TX interrupt.
 * @tx_coalesce_usecs_irq: Same as @tx_coalesce_usecs, except that
 *	this value applies while an IRQ is being serviced by the host.
 * @tx_max_coalesced_frames_irq: Same as @tx_max_coalesced_frames,
 *	except that this value applies while an IRQ is being serviced
 *	by the host.
 * @stats_block_coalesce_usecs: How many usecs to delay in-memory
 *	statistics block updates.  Some drivers do not have an
 *	in-memory statistic block, and in such cases this value is
 *	ignored.  This value must not be zero.
 * @use_adaptive_rx_coalesce: Enable adaptive RX coalescing.
 * @use_adaptive_tx_coalesce: Enable adaptive TX coalescing.
 * @pkt_rate_low: Threshold for low packet rate (packets per second).
 * @rx_coalesce_usecs_low: How many usecs to delay an RX interrupt after
 *	a packet arrives, when the packet rate is below @pkt_rate_low.
 * @rx_max_coalesced_frames_low: Maximum number of packets to be received
 *	before an RX interrupt, when the packet rate is below @pkt_rate_low.
 * @tx_coalesce_usecs_low: How many usecs to delay a TX interrupt after
 *	a packet is sent, when the packet rate is below @pkt_rate_low.
 * @tx_max_coalesced_frames_low: Maximum nuumber of packets to be sent before
 *	a TX interrupt, when the packet rate is below @pkt_rate_low.
 * @pkt_rate_high: Threshold for high packet rate (packets per second).
 * @rx_coalesce_usecs_high: How many usecs to delay an RX interrupt after
 *	a packet arrives, when the packet rate is above @pkt_rate_high.
 * @rx_max_coalesced_frames_high: Maximum number of packets to be received
 *	before an RX interrupt, when the packet rate is above @pkt_rate_high.
 * @tx_coalesce_usecs_high: How many usecs to delay a TX interrupt after
 *	a packet is sent, when the packet rate is above @pkt_rate_high.
 * @tx_max_coalesced_frames_high: Maximum number of packets to be sent before
 *	a TX interrupt, when the packet rate is above @pkt_rate_high.
 * @rate_sample_interval: How often to do adaptive coalescing packet rate
 *	sampling, measured in seconds.  Must not be zero.
 *
 * Each pair of (usecs, max_frames) fields specifies this exit
 * condition for interrupt coalescing:
 *	(usecs > 0 && time_since_first_completion >= usecs) ||
 *	(max_frames > 0 && completed_frames >= max_frames)
 * It is illegal to set both usecs and max_frames to zero as this
 * would cause interrupts to never be generated.  To disable
 * coalescing, set usecs = 0 and max_frames = 1.
 *
 * Some implementations ignore the value of max_frames and use the
 * condition:
 *	time_since_first_completion >= usecs
 * This is deprecated.  Drivers for hardware that does not support
 * counting completions should validate that max_frames == !rx_usecs.
 *
 * Adaptive RX/TX coalescing is an algorithm implemented by some
 * drivers to improve latency under low packet rates and improve
 * throughput under high packet rates.  Some drivers only implement
 * one of RX or TX adaptive coalescing.  Anything not implemented by
 * the driver causes these values to be silently ignored.
 *
 * When the packet rate is below @pkt_rate_high but above
 * @pkt_rate_low (both measured in packets per second) the
 * normal {rx,tx}_* coalescing parameters are used.
 */
struct ethtool_coalesce {
	__u32	cmd;
	__u32	rx_coalesce_usecs;
	__u32	rx_max_coalesced_frames;
	__u32	rx_coalesce_usecs_irq;
	__u32	rx_max_coalesced_frames_irq;
	__u32	tx_coalesce_usecs;
	__u32	tx_max_coalesced_frames;
	__u32	tx_coalesce_usecs_irq;
	__u32	tx_max_coalesced_frames_irq;
	__u32	stats_block_coalesce_usecs;
	__u32	use_adaptive_rx_coalesce;
	__u32	use_adaptive_tx_coalesce;
	__u32	pkt_rate_low;
	__u32	rx_coalesce_usecs_low;
	__u32	rx_max_coalesced_frames_low;
	__u32	tx_coalesce_usecs_low;
	__u32	tx_max_coalesced_frames_low;
	__u32	pkt_rate_high;
	__u32	rx_coalesce_usecs_high;
	__u32	rx_max_coalesced_frames_high;
	__u32	tx_coalesce_usecs_high;
	__u32	tx_max_coalesced_frames_high;
	__u32	rate_sample_interval;
};

/* for configuring RX/TX ring parameters */
struct ethtool_ringparam {
	__u32	cmd;	/* ETHTOOL_{G,S}RINGPARAM */

	/* Read only attributes.  These indicate the maximum number
	 * of pending RX/TX ring entries the driver will allow the
	 * user to set.
	 */
	__u32	rx_max_pending;
	__u32	rx_mini_max_pending;
	__u32	rx_jumbo_max_pending;
	__u32	tx_max_pending;

	/* Values changeable by the user.  The valid values are
	 * in the range 1 to the "*_max_pending" counterpart above.
	 */
	__u32	rx_pending;
	__u32	rx_mini_pending;
	__u32	rx_jumbo_pending;
	__u32	tx_pending;
};

/**
 * struct ethtool_channels - configuring number of network channel
 * @cmd: ETHTOOL_{G,S}CHANNELS
 * @max_rx: Read only. Maximum number of receive channel the driver support.
 * @max_tx: Read only. Maximum number of transmit channel the driver support.
 * @max_other: Read only. Maximum number of other channel the driver support.
 * @max_combined: Read only. Maximum number of combined channel the driver
 *	support. Set of queues RX, TX or other.
 * @rx_count: Valid values are in the range 1 to the max_rx.
 * @tx_count: Valid values are in the range 1 to the max_tx.
 * @other_count: Valid values are in the range 1 to the max_other.
 * @combined_count: Valid values are in the range 1 to the max_combined.
 *
 * This can be used to configure RX, TX and other channels.
 */

struct ethtool_channels {
	__u32	cmd;
	__u32	max_rx;
	__u32	max_tx;
	__u32	max_other;
	__u32	max_combined;
	__u32	rx_count;
	__u32	tx_count;
	__u32	other_count;
	__u32	combined_count;
};

/* for configuring link flow control parameters */
struct ethtool_pauseparam {
	__u32	cmd;	/* ETHTOOL_{G,S}PAUSEPARAM */

	/* If the link is being auto-negotiated (via ethtool_cmd.autoneg
	 * being true) the user may set 'autoneg' here non-zero to have the
	 * pause parameters be auto-negotiated too.  In such a case, the
	 * {rx,tx}_pause values below determine what capabilities are
	 * advertised.
	 *
	 * If 'autoneg' is zero or the link is not being auto-negotiated,
	 * then {rx,tx}_pause force the driver to use/not-use pause
	 * flow control.
	 */
	__u32	autoneg;
	__u32	rx_pause;
	__u32	tx_pause;
};

#define ETH_GSTRING_LEN		32
enum ethtool_stringset {
	ETH_SS_TEST		= 0,
	ETH_SS_STATS,
	ETH_SS_PRIV_FLAGS,
	ETH_SS_NTUPLE_FILTERS,	/* Do not use, GRXNTUPLE is now deprecated */
	ETH_SS_FEATURES,
};

/* for passing string sets for data tagging */
struct ethtool_gstrings {
	__u32	cmd;		/* ETHTOOL_GSTRINGS */
	__u32	string_set;	/* string set id e.c. ETH_SS_TEST, etc*/
	__u32	len;		/* number of strings in the string set */
	__u8	data[0];
};

struct ethtool_sset_info {
	__u32	cmd;		/* ETHTOOL_GSSET_INFO */
	__u32	reserved;
	__u64	sset_mask;	/* input: each bit selects an sset to query */
				/* output: each bit a returned sset */
	__u32	data[0];	/* ETH_SS_xxx count, in order, based on bits
				   in sset_mask.  One bit implies one
				   __u32, two bits implies two
				   __u32's, etc. */
};

/**
 * enum ethtool_test_flags - flags definition of ethtool_test
 * @ETH_TEST_FL_OFFLINE: if set perform online and offline tests, otherwise
 *	only online tests.
 * @ETH_TEST_FL_FAILED: Driver set this flag if test fails.
 * @ETH_TEST_FL_EXTERNAL_LB: Application request to perform external loopback
 *	test.
 * @ETH_TEST_FL_EXTERNAL_LB_DONE: Driver performed the external loopback test
 */

enum ethtool_test_flags {
	ETH_TEST_FL_OFFLINE	= (1 << 0),
	ETH_TEST_FL_FAILED	= (1 << 1),
	ETH_TEST_FL_EXTERNAL_LB	= (1 << 2),
	ETH_TEST_FL_EXTERNAL_LB_DONE	= (1 << 3),
};

/* for requesting NIC test and getting results*/
struct ethtool_test {
	__u32	cmd;		/* ETHTOOL_TEST */
	__u32	flags;		/* ETH_TEST_FL_xxx */
	__u32	reserved;
	__u32	len;		/* result length, in number of u64 elements */
	__u64	data[0];
};

/* for dumping NIC-specific statistics */
struct ethtool_stats {
	__u32	cmd;		/* ETHTOOL_GSTATS */
	__u32	n_stats;	/* number of u64's being returned */
	__u64	data[0];
};

struct ethtool_perm_addr {
	__u32	cmd;		/* ETHTOOL_GPERMADDR */
	__u32	size;
	__u8	data[0];
};

/* boolean flags controlling per-interface behavior characteristics.
 * When reading, the flag indicates whether or not a certain behavior
 * is enabled/present.  When writing, the flag indicates whether
 * or not the driver should turn on (set) or off (clear) a behavior.
 *
 * Some behaviors may read-only (unconditionally absent or present).
 * If such is the case, return EINVAL in the set-flags operation if the
 * flag differs from the read-only value.
 */
enum ethtool_flags {
	ETH_FLAG_TXVLAN		= (1 << 7),	/* TX VLAN offload enabled */
	ETH_FLAG_RXVLAN		= (1 << 8),	/* RX VLAN offload enabled */
	ETH_FLAG_LRO		= (1 << 15),	/* LRO is enabled */
	ETH_FLAG_NTUPLE		= (1 << 27),	/* N-tuple filters enabled */
	ETH_FLAG_RXHASH		= (1 << 28),
};

/* The following structures are for supporting RX network flow
 * classification and RX n-tuple configuration. Note, all multibyte
 * fields, e.g., ip4src, ip4dst, psrc, pdst, spi, etc. are expected to
 * be in network byte order.
 */

/**
 * struct ethtool_tcpip4_spec - flow specification for TCP/IPv4 etc.
 * @ip4src: Source host
 * @ip4dst: Destination host
 * @psrc: Source port
 * @pdst: Destination port
 * @tos: Type-of-service
 *
 * This can be used to specify a TCP/IPv4, UDP/IPv4 or SCTP/IPv4 flow.
 */
struct ethtool_tcpip4_spec {
	__be32	ip4src;
	__be32	ip4dst;
	__be16	psrc;
	__be16	pdst;
	__u8    tos;
};

/**
 * struct ethtool_ah_espip4_spec - flow specification for IPsec/IPv4
 * @ip4src: Source host
 * @ip4dst: Destination host
 * @spi: Security parameters index
 * @tos: Type-of-service
 *
 * This can be used to specify an IPsec transport or tunnel over IPv4.
 */
struct ethtool_ah_espip4_spec {
	__be32	ip4src;
	__be32	ip4dst;
	__be32	spi;
	__u8    tos;
};

#define	ETH_RX_NFC_IP4	1

/**
 * struct ethtool_usrip4_spec - general flow specification for IPv4
 * @ip4src: Source host
 * @ip4dst: Destination host
 * @l4_4_bytes: First 4 bytes of transport (layer 4) header
 * @tos: Type-of-service
 * @ip_ver: Value must be %ETH_RX_NFC_IP4; mask must be 0
 * @proto: Transport protocol number; mask must be 0
 */
struct ethtool_usrip4_spec {
	__be32	ip4src;
	__be32	ip4dst;
	__be32	l4_4_bytes;
	__u8    tos;
	__u8    ip_ver;
	__u8    proto;
};

union ethtool_flow_union {
	struct ethtool_tcpip4_spec		tcp_ip4_spec;
	struct ethtool_tcpip4_spec		udp_ip4_spec;
	struct ethtool_tcpip4_spec		sctp_ip4_spec;
	struct ethtool_ah_espip4_spec		ah_ip4_spec;
	struct ethtool_ah_espip4_spec		esp_ip4_spec;
	struct ethtool_usrip4_spec		usr_ip4_spec;
	struct ethhdr				ether_spec;
	__u8					hdata[60];
};

struct ethtool_flow_ext {
	__be16	vlan_etype;
	__be16	vlan_tci;
	__be32	data[2];
};

/**
 * struct ethtool_rx_flow_spec - classification rule for RX flows
 * @flow_type: Type of match to perform, e.g. %TCP_V4_FLOW
 * @h_u: Flow fields to match (dependent on @flow_type)
 * @h_ext: Additional fields to match
 * @m_u: Masks for flow field bits to be matched
 * @m_ext: Masks for additional field bits to be matched
 *	Note, all additional fields must be ignored unless @flow_type
 *	includes the %FLOW_EXT flag.
 * @ring_cookie: RX ring/queue index to deliver to, or %RX_CLS_FLOW_DISC
 *	if packets should be discarded
 * @location: Location of rule in the table.  Locations must be
 *	numbered such that a flow matching multiple rules will be
 *	classified according to the first (lowest numbered) rule.
 */
struct ethtool_rx_flow_spec {
	__u32		flow_type;
	union ethtool_flow_union h_u;
	struct ethtool_flow_ext h_ext;
	union ethtool_flow_union m_u;
	struct ethtool_flow_ext m_ext;
	__u64		ring_cookie;
	__u32		location;
};

/**
 * struct ethtool_rxnfc - command to get or set RX flow classification rules
 * @cmd: Specific command number - %ETHTOOL_GRXFH, %ETHTOOL_SRXFH,
 *	%ETHTOOL_GRXRINGS, %ETHTOOL_GRXCLSRLCNT, %ETHTOOL_GRXCLSRULE,
 *	%ETHTOOL_GRXCLSRLALL, %ETHTOOL_SRXCLSRLDEL or %ETHTOOL_SRXCLSRLINS
 * @flow_type: Type of flow to be affected, e.g. %TCP_V4_FLOW
 * @data: Command-dependent value
 * @fs: Flow classification rule
 * @rule_cnt: Number of rules to be affected
 * @rule_locs: Array of used rule locations
 *
 * For %ETHTOOL_GRXFH and %ETHTOOL_SRXFH, @data is a bitmask indicating
 * the fields included in the flow hash, e.g. %RXH_IP_SRC.  The following
 * structure fields must not be used.
 *
 * For %ETHTOOL_GRXRINGS, @data is set to the number of RX rings/queues
 * on return.
 *
 * For %ETHTOOL_GRXCLSRLCNT, @rule_cnt is set to the number of defined
 * rules on return.  If @data is non-zero on return then it is the
 * size of the rule table, plus the flag %RX_CLS_LOC_SPECIAL if the
 * driver supports any special location values.  If that flag is not
 * set in @data then special location values should not be used.
 *
 * For %ETHTOOL_GRXCLSRULE, @fs.@location specifies the location of an
 * existing rule on entry and @fs contains the rule on return.
 *
 * For %ETHTOOL_GRXCLSRLALL, @rule_cnt specifies the array size of the
 * user buffer for @rule_locs on entry.  On return, @data is the size
 * of the rule table, @rule_cnt is the number of defined rules, and
 * @rule_locs contains the locations of the defined rules.  Drivers
 * must use the second parameter to get_rxnfc() instead of @rule_locs.
 *
 * For %ETHTOOL_SRXCLSRLINS, @fs specifies the rule to add or update.
 * @fs.@location either specifies the location to use or is a special
 * location value with %RX_CLS_LOC_SPECIAL flag set.  On return,
 * @fs.@location is the actual rule location.
 *
 * For %ETHTOOL_SRXCLSRLDEL, @fs.@location specifies the location of an
 * existing rule on entry.
 *
 * A driver supporting the special location values for
 * %ETHTOOL_SRXCLSRLINS may add the rule at any suitable unused
 * location, and may remove a rule at a later location (lower
 * priority) that matches exactly the same set of flows.  The special
 * values are: %RX_CLS_LOC_ANY, selecting any location;
 * %RX_CLS_LOC_FIRST, selecting the first suitable location (maximum
 * priority); and %RX_CLS_LOC_LAST, selecting the last suitable
 * location (minimum priority).  Additional special values may be
 * defined in future and drivers must return -%EINVAL for any
 * unrecognised value.
 */
struct ethtool_rxnfc {
	__u32				cmd;
	__u32				flow_type;
	__u64				data;
	struct ethtool_rx_flow_spec	fs;
	__u32				rule_cnt;
	__u32				rule_locs[0];
};

#ifdef __KERNEL__
#ifdef CONFIG_COMPAT
=======
#ifndef _LINUX_ETHTOOL_H
#define _LINUX_ETHTOOL_H

#include <linux/bitmap.h>
#include <linux/compat.h>
#include <linux/if_ether.h>
#include <linux/netlink.h>
#include <uapi/linux/ethtool.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct compat_ethtool_rx_flow_spec {
	u32		flow_type;
	union ethtool_flow_union h_u;
	struct ethtool_flow_ext h_ext;
	union ethtool_flow_union m_u;
	struct ethtool_flow_ext m_ext;
	compat_u64	ring_cookie;
	u32		location;
};

struct compat_ethtool_rxnfc {
	u32				cmd;
	u32				flow_type;
	compat_u64			data;
	struct compat_ethtool_rx_flow_spec fs;
	u32				rule_cnt;
<<<<<<< HEAD
	u32				rule_locs[0];
};

#endif /* CONFIG_COMPAT */
#endif /* __KERNEL__ */

/**
 * struct ethtool_rxfh_indir - command to get or set RX flow hash indirection
 * @cmd: Specific command number - %ETHTOOL_GRXFHINDIR or %ETHTOOL_SRXFHINDIR
 * @size: On entry, the array size of the user buffer, which may be zero.
 *	On return from %ETHTOOL_GRXFHINDIR, the array size of the hardware
 *	indirection table.
 * @ring_index: RX ring/queue index for each hash value
 *
 * For %ETHTOOL_GRXFHINDIR, a @size of zero means that only the size
 * should be returned.  For %ETHTOOL_SRXFHINDIR, a @size of zero means
 * the table should be reset to default values.  This last feature
 * is not supported by the original implementations.
 */
struct ethtool_rxfh_indir {
	__u32	cmd;
	__u32	size;
	__u32	ring_index[0];
};

/**
 * struct ethtool_rx_ntuple_flow_spec - specification for RX flow filter
 * @flow_type: Type of match to perform, e.g. %TCP_V4_FLOW
 * @h_u: Flow field values to match (dependent on @flow_type)
 * @m_u: Masks for flow field value bits to be ignored
 * @vlan_tag: VLAN tag to match
 * @vlan_tag_mask: Mask for VLAN tag bits to be ignored
 * @data: Driver-dependent data to match
 * @data_mask: Mask for driver-dependent data bits to be ignored
 * @action: RX ring/queue index to deliver to (non-negative) or other action
 *	(negative, e.g. %ETHTOOL_RXNTUPLE_ACTION_DROP)
 *
 * For flow types %TCP_V4_FLOW, %UDP_V4_FLOW and %SCTP_V4_FLOW, where
 * a field value and mask are both zero this is treated as if all mask
 * bits are set i.e. the field is ignored.
 */
struct ethtool_rx_ntuple_flow_spec {
	__u32		 flow_type;
	union {
		struct ethtool_tcpip4_spec		tcp_ip4_spec;
		struct ethtool_tcpip4_spec		udp_ip4_spec;
		struct ethtool_tcpip4_spec		sctp_ip4_spec;
		struct ethtool_ah_espip4_spec		ah_ip4_spec;
		struct ethtool_ah_espip4_spec		esp_ip4_spec;
		struct ethtool_usrip4_spec		usr_ip4_spec;
		struct ethhdr				ether_spec;
		__u8					hdata[72];
	} h_u, m_u;

	__u16	        vlan_tag;
	__u16	        vlan_tag_mask;
	__u64		data;
	__u64		data_mask;

	__s32		action;
#define ETHTOOL_RXNTUPLE_ACTION_DROP	(-1)	/* drop packet */
#define ETHTOOL_RXNTUPLE_ACTION_CLEAR	(-2)	/* clear filter */
};

/**
 * struct ethtool_rx_ntuple - command to set or clear RX flow filter
 * @cmd: Command number - %ETHTOOL_SRXNTUPLE
 * @fs: Flow filter specification
 */
struct ethtool_rx_ntuple {
	__u32					cmd;
	struct ethtool_rx_ntuple_flow_spec	fs;
};

#define ETHTOOL_FLASH_MAX_FILENAME	128
enum ethtool_flash_op_type {
	ETHTOOL_FLASH_ALL_REGIONS	= 0,
};

/* for passing firmware flashing related parameters */
struct ethtool_flash {
	__u32	cmd;
	__u32	region;
	char	data[ETHTOOL_FLASH_MAX_FILENAME];
};

/**
 * struct ethtool_dump - used for retrieving, setting device dump
 * @cmd: Command number - %ETHTOOL_GET_DUMP_FLAG, %ETHTOOL_GET_DUMP_DATA, or
 * 	%ETHTOOL_SET_DUMP
 * @version: FW version of the dump, filled in by driver
 * @flag: driver dependent flag for dump setting, filled in by driver during
 * 	  get and filled in by ethtool for set operation
 * @len: length of dump data, used as the length of the user buffer on entry to
 * 	 %ETHTOOL_GET_DUMP_DATA and this is returned as dump length by driver
 * 	 for %ETHTOOL_GET_DUMP_FLAG command
 * @data: data collected for get dump data operation
 */
struct ethtool_dump {
	__u32	cmd;
	__u32	version;
	__u32	flag;
	__u32	len;
	__u8	data[0];
};

/* for returning and changing feature sets */

/**
 * struct ethtool_get_features_block - block with state of 32 features
 * @available: mask of changeable features
 * @requested: mask of features requested to be enabled if possible
 * @active: mask of currently enabled features
 * @never_changed: mask of features not changeable for any device
 */
struct ethtool_get_features_block {
	__u32	available;
	__u32	requested;
	__u32	active;
	__u32	never_changed;
};

/**
 * struct ethtool_gfeatures - command to get state of device's features
 * @cmd: command number = %ETHTOOL_GFEATURES
 * @size: in: number of elements in the features[] array;
 *       out: number of elements in features[] needed to hold all features
 * @features: state of features
 */
struct ethtool_gfeatures {
	__u32	cmd;
	__u32	size;
	struct ethtool_get_features_block features[0];
};

/**
 * struct ethtool_set_features_block - block with request for 32 features
 * @valid: mask of features to be changed
 * @requested: values of features to be changed
 */
struct ethtool_set_features_block {
	__u32	valid;
	__u32	requested;
};

/**
 * struct ethtool_sfeatures - command to request change in device's features
 * @cmd: command number = %ETHTOOL_SFEATURES
 * @size: array size of the features[] array
 * @features: feature change masks
 */
struct ethtool_sfeatures {
	__u32	cmd;
	__u32	size;
	struct ethtool_set_features_block features[0];
};

/*
 * %ETHTOOL_SFEATURES changes features present in features[].valid to the
 * values of corresponding bits in features[].requested. Bits in .requested
 * not set in .valid or not changeable are ignored.
 *
 * Returns %EINVAL when .valid contains undefined or never-changeable bits
 * or size is not equal to required number of features words (32-bit blocks).
 * Returns >= 0 if request was completed; bits set in the value mean:
 *   %ETHTOOL_F_UNSUPPORTED - there were bits set in .valid that are not
 *	changeable (not present in %ETHTOOL_GFEATURES' features[].available)
 *	those bits were ignored.
 *   %ETHTOOL_F_WISH - some or all changes requested were recorded but the
 *      resulting state of bits masked by .valid is not equal to .requested.
 *      Probably there are other device-specific constraints on some features
 *      in the set. When %ETHTOOL_F_UNSUPPORTED is set, .valid is considered
 *      here as though ignored bits were cleared.
 *   %ETHTOOL_F_COMPAT - some or all changes requested were made by calling
 *      compatibility functions. Requested offload state cannot be properly
 *      managed by kernel.
 *
 * Meaning of bits in the masks are obtained by %ETHTOOL_GSSET_INFO (number of
 * bits in the arrays - always multiple of 32) and %ETHTOOL_GSTRINGS commands
 * for ETH_SS_FEATURES string set. First entry in the table corresponds to least
 * significant bit in features[0] fields. Empty strings mark undefined features.
 */
enum ethtool_sfeatures_retval_bits {
	ETHTOOL_F_UNSUPPORTED__BIT,
	ETHTOOL_F_WISH__BIT,
	ETHTOOL_F_COMPAT__BIT,
};

#define ETHTOOL_F_UNSUPPORTED   (1 << ETHTOOL_F_UNSUPPORTED__BIT)
#define ETHTOOL_F_WISH          (1 << ETHTOOL_F_WISH__BIT)
#define ETHTOOL_F_COMPAT        (1 << ETHTOOL_F_COMPAT__BIT)

#ifdef __KERNEL__

#include <linux/rculist.h>

extern int __ethtool_get_settings(struct net_device *dev,
				  struct ethtool_cmd *cmd);

=======
	u32				rule_locs[];
};

#include <linux/rculist.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * enum ethtool_phys_id_state - indicator state for physical identification
 * @ETHTOOL_ID_INACTIVE: Physical ID indicator should be deactivated
 * @ETHTOOL_ID_ACTIVE: Physical ID indicator should be activated
 * @ETHTOOL_ID_ON: LED should be turned on (used iff %ETHTOOL_ID_ACTIVE
 *	is not supported)
 * @ETHTOOL_ID_OFF: LED should be turned off (used iff %ETHTOOL_ID_ACTIVE
 *	is not supported)
 */
enum ethtool_phys_id_state {
	ETHTOOL_ID_INACTIVE,
	ETHTOOL_ID_ACTIVE,
	ETHTOOL_ID_ON,
	ETHTOOL_ID_OFF
};

<<<<<<< HEAD
struct net_device;

/* Some generic methods drivers may use in their ethtool_ops */
u32 ethtool_op_get_link(struct net_device *dev);
=======
enum {
	ETH_RSS_HASH_TOP_BIT, /* Configurable RSS hash function - Toeplitz */
	ETH_RSS_HASH_XOR_BIT, /* Configurable RSS hash function - Xor */
	ETH_RSS_HASH_CRC32_BIT, /* Configurable RSS hash function - Crc32 */

	/*
	 * Add your fresh new hash function bits above and remember to update
	 * rss_hash_func_strings[] in ethtool.c
	 */
	ETH_RSS_HASH_FUNCS_COUNT
};

/**
 * struct kernel_ethtool_ringparam - RX/TX ring configuration
 * @rx_buf_len: Current length of buffers on the rx ring.
 * @tcp_data_split: Scatter packet headers and data to separate buffers
 * @tx_push: The flag of tx push mode
 * @rx_push: The flag of rx push mode
 * @cqe_size: Size of TX/RX completion queue event
 * @tx_push_buf_len: Size of TX push buffer
 * @tx_push_buf_max_len: Maximum allowed size of TX push buffer
 */
struct kernel_ethtool_ringparam {
	u32	rx_buf_len;
	u8	tcp_data_split;
	u8	tx_push;
	u8	rx_push;
	u32	cqe_size;
	u32	tx_push_buf_len;
	u32	tx_push_buf_max_len;
};

/**
 * enum ethtool_supported_ring_param - indicator caps for setting ring params
 * @ETHTOOL_RING_USE_RX_BUF_LEN: capture for setting rx_buf_len
 * @ETHTOOL_RING_USE_CQE_SIZE: capture for setting cqe_size
 * @ETHTOOL_RING_USE_TX_PUSH: capture for setting tx_push
 * @ETHTOOL_RING_USE_RX_PUSH: capture for setting rx_push
 * @ETHTOOL_RING_USE_TX_PUSH_BUF_LEN: capture for setting tx_push_buf_len
 * @ETHTOOL_RING_USE_TCP_DATA_SPLIT: capture for setting tcp_data_split
 */
enum ethtool_supported_ring_param {
	ETHTOOL_RING_USE_RX_BUF_LEN		= BIT(0),
	ETHTOOL_RING_USE_CQE_SIZE		= BIT(1),
	ETHTOOL_RING_USE_TX_PUSH		= BIT(2),
	ETHTOOL_RING_USE_RX_PUSH		= BIT(3),
	ETHTOOL_RING_USE_TX_PUSH_BUF_LEN	= BIT(4),
	ETHTOOL_RING_USE_TCP_DATA_SPLIT		= BIT(5),
};

#define __ETH_RSS_HASH_BIT(bit)	((u32)1 << (bit))
#define __ETH_RSS_HASH(name)	__ETH_RSS_HASH_BIT(ETH_RSS_HASH_##name##_BIT)

#define ETH_RSS_HASH_TOP	__ETH_RSS_HASH(TOP)
#define ETH_RSS_HASH_XOR	__ETH_RSS_HASH(XOR)
#define ETH_RSS_HASH_CRC32	__ETH_RSS_HASH(CRC32)

#define ETH_RSS_HASH_UNKNOWN	0
#define ETH_RSS_HASH_NO_CHANGE	0

struct net_device;
struct netlink_ext_ack;

/* Link extended state and substate. */
struct ethtool_link_ext_state_info {
	enum ethtool_link_ext_state link_ext_state;
	union {
		enum ethtool_link_ext_substate_autoneg autoneg;
		enum ethtool_link_ext_substate_link_training link_training;
		enum ethtool_link_ext_substate_link_logical_mismatch link_logical_mismatch;
		enum ethtool_link_ext_substate_bad_signal_integrity bad_signal_integrity;
		enum ethtool_link_ext_substate_cable_issue cable_issue;
		enum ethtool_link_ext_substate_module module;
		u32 __link_ext_substate;
	};
};

struct ethtool_link_ext_stats {
	/* Custom Linux statistic for PHY level link down events.
	 * In a simpler world it should be equal to netdev->carrier_down_count
	 * unfortunately netdev also counts local reconfigurations which don't
	 * actually take the physical link down, not to mention NC-SI which,
	 * if present, keeps the link up regardless of host state.
	 * This statistic counts when PHY _actually_ went down, or lost link.
	 *
	 * Note that we need u64 for ethtool_stats_init() and comparisons
	 * to ETHTOOL_STAT_NOT_SET, but only u32 is exposed to the user.
	 */
	u64 link_down_events;
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * ethtool_rxfh_indir_default - get default value for RX flow hash indirection
 * @index: Index in RX flow hash indirection table
 * @n_rx_rings: Number of RX rings to use
 *
 * This function provides the default policy for RX flow hash indirection.
 */
static inline u32 ethtool_rxfh_indir_default(u32 index, u32 n_rx_rings)
{
	return index % n_rx_rings;
}

<<<<<<< HEAD
/**
 * struct ethtool_ops - optional netdev operations
 * @get_settings: Get various device settings including Ethernet link
 *	settings. The @cmd parameter is expected to have been cleared
 *	before get_settings is called. Returns a negative error code or
 *	zero.
 * @set_settings: Set various device settings including Ethernet link
 *	settings.  Returns a negative error code or zero.
 * @get_drvinfo: Report driver/device information.  Should only set the
 *	@driver, @version, @fw_version and @bus_info fields.  If not
 *	implemented, the @driver and @bus_info fields will be filled in
 *	according to the netdev's parent device.
=======
/* declare a link mode bitmap */
#define __ETHTOOL_DECLARE_LINK_MODE_MASK(name)		\
	DECLARE_BITMAP(name, __ETHTOOL_LINK_MODE_MASK_NBITS)

/* drivers must ignore base.cmd and base.link_mode_masks_nwords
 * fields, but they are allowed to overwrite them (will be ignored).
 */
struct ethtool_link_ksettings {
	struct ethtool_link_settings base;
	struct {
		__ETHTOOL_DECLARE_LINK_MODE_MASK(supported);
		__ETHTOOL_DECLARE_LINK_MODE_MASK(advertising);
		__ETHTOOL_DECLARE_LINK_MODE_MASK(lp_advertising);
	} link_modes;
	u32	lanes;
};

/**
 * ethtool_link_ksettings_zero_link_mode - clear link_ksettings link mode mask
 *   @ptr : pointer to struct ethtool_link_ksettings
 *   @name : one of supported/advertising/lp_advertising
 */
#define ethtool_link_ksettings_zero_link_mode(ptr, name)		\
	bitmap_zero((ptr)->link_modes.name, __ETHTOOL_LINK_MODE_MASK_NBITS)

/**
 * ethtool_link_ksettings_add_link_mode - set bit in link_ksettings
 * link mode mask
 *   @ptr : pointer to struct ethtool_link_ksettings
 *   @name : one of supported/advertising/lp_advertising
 *   @mode : one of the ETHTOOL_LINK_MODE_*_BIT
 * (not atomic, no bound checking)
 */
#define ethtool_link_ksettings_add_link_mode(ptr, name, mode)		\
	__set_bit(ETHTOOL_LINK_MODE_ ## mode ## _BIT, (ptr)->link_modes.name)

/**
 * ethtool_link_ksettings_del_link_mode - clear bit in link_ksettings
 * link mode mask
 *   @ptr : pointer to struct ethtool_link_ksettings
 *   @name : one of supported/advertising/lp_advertising
 *   @mode : one of the ETHTOOL_LINK_MODE_*_BIT
 * (not atomic, no bound checking)
 */
#define ethtool_link_ksettings_del_link_mode(ptr, name, mode)		\
	__clear_bit(ETHTOOL_LINK_MODE_ ## mode ## _BIT, (ptr)->link_modes.name)

/**
 * ethtool_link_ksettings_test_link_mode - test bit in ksettings link mode mask
 *   @ptr : pointer to struct ethtool_link_ksettings
 *   @name : one of supported/advertising/lp_advertising
 *   @mode : one of the ETHTOOL_LINK_MODE_*_BIT
 * (not atomic, no bound checking)
 *
 * Returns true/false.
 */
#define ethtool_link_ksettings_test_link_mode(ptr, name, mode)		\
	test_bit(ETHTOOL_LINK_MODE_ ## mode ## _BIT, (ptr)->link_modes.name)

extern int
__ethtool_get_link_ksettings(struct net_device *dev,
			     struct ethtool_link_ksettings *link_ksettings);

struct ethtool_keee {
	__ETHTOOL_DECLARE_LINK_MODE_MASK(supported);
	__ETHTOOL_DECLARE_LINK_MODE_MASK(advertised);
	__ETHTOOL_DECLARE_LINK_MODE_MASK(lp_advertised);
	u32	tx_lpi_timer;
	bool	tx_lpi_enabled;
	bool	eee_active;
	bool	eee_enabled;
};

struct kernel_ethtool_coalesce {
	u8 use_cqe_mode_tx;
	u8 use_cqe_mode_rx;
	u32 tx_aggr_max_bytes;
	u32 tx_aggr_max_frames;
	u32 tx_aggr_time_usecs;
};

/**
 * ethtool_intersect_link_masks - Given two link masks, AND them together
 * @dst: first mask and where result is stored
 * @src: second mask to intersect with
 *
 * Given two link mode masks, AND them together and save the result in dst.
 */
void ethtool_intersect_link_masks(struct ethtool_link_ksettings *dst,
				  struct ethtool_link_ksettings *src);

void ethtool_convert_legacy_u32_to_link_mode(unsigned long *dst,
					     u32 legacy_u32);

/* return false if src had higher bits set. lower bits always updated. */
bool ethtool_convert_link_mode_to_legacy_u32(u32 *legacy_u32,
				     const unsigned long *src);

#define ETHTOOL_COALESCE_RX_USECS		BIT(0)
#define ETHTOOL_COALESCE_RX_MAX_FRAMES		BIT(1)
#define ETHTOOL_COALESCE_RX_USECS_IRQ		BIT(2)
#define ETHTOOL_COALESCE_RX_MAX_FRAMES_IRQ	BIT(3)
#define ETHTOOL_COALESCE_TX_USECS		BIT(4)
#define ETHTOOL_COALESCE_TX_MAX_FRAMES		BIT(5)
#define ETHTOOL_COALESCE_TX_USECS_IRQ		BIT(6)
#define ETHTOOL_COALESCE_TX_MAX_FRAMES_IRQ	BIT(7)
#define ETHTOOL_COALESCE_STATS_BLOCK_USECS	BIT(8)
#define ETHTOOL_COALESCE_USE_ADAPTIVE_RX	BIT(9)
#define ETHTOOL_COALESCE_USE_ADAPTIVE_TX	BIT(10)
#define ETHTOOL_COALESCE_PKT_RATE_LOW		BIT(11)
#define ETHTOOL_COALESCE_RX_USECS_LOW		BIT(12)
#define ETHTOOL_COALESCE_RX_MAX_FRAMES_LOW	BIT(13)
#define ETHTOOL_COALESCE_TX_USECS_LOW		BIT(14)
#define ETHTOOL_COALESCE_TX_MAX_FRAMES_LOW	BIT(15)
#define ETHTOOL_COALESCE_PKT_RATE_HIGH		BIT(16)
#define ETHTOOL_COALESCE_RX_USECS_HIGH		BIT(17)
#define ETHTOOL_COALESCE_RX_MAX_FRAMES_HIGH	BIT(18)
#define ETHTOOL_COALESCE_TX_USECS_HIGH		BIT(19)
#define ETHTOOL_COALESCE_TX_MAX_FRAMES_HIGH	BIT(20)
#define ETHTOOL_COALESCE_RATE_SAMPLE_INTERVAL	BIT(21)
#define ETHTOOL_COALESCE_USE_CQE_RX		BIT(22)
#define ETHTOOL_COALESCE_USE_CQE_TX		BIT(23)
#define ETHTOOL_COALESCE_TX_AGGR_MAX_BYTES	BIT(24)
#define ETHTOOL_COALESCE_TX_AGGR_MAX_FRAMES	BIT(25)
#define ETHTOOL_COALESCE_TX_AGGR_TIME_USECS	BIT(26)
#define ETHTOOL_COALESCE_ALL_PARAMS		GENMASK(26, 0)

#define ETHTOOL_COALESCE_USECS						\
	(ETHTOOL_COALESCE_RX_USECS | ETHTOOL_COALESCE_TX_USECS)
#define ETHTOOL_COALESCE_MAX_FRAMES					\
	(ETHTOOL_COALESCE_RX_MAX_FRAMES | ETHTOOL_COALESCE_TX_MAX_FRAMES)
#define ETHTOOL_COALESCE_USECS_IRQ					\
	(ETHTOOL_COALESCE_RX_USECS_IRQ | ETHTOOL_COALESCE_TX_USECS_IRQ)
#define ETHTOOL_COALESCE_MAX_FRAMES_IRQ		\
	(ETHTOOL_COALESCE_RX_MAX_FRAMES_IRQ |	\
	 ETHTOOL_COALESCE_TX_MAX_FRAMES_IRQ)
#define ETHTOOL_COALESCE_USE_ADAPTIVE					\
	(ETHTOOL_COALESCE_USE_ADAPTIVE_RX | ETHTOOL_COALESCE_USE_ADAPTIVE_TX)
#define ETHTOOL_COALESCE_USECS_LOW_HIGH					\
	(ETHTOOL_COALESCE_RX_USECS_LOW | ETHTOOL_COALESCE_TX_USECS_LOW | \
	 ETHTOOL_COALESCE_RX_USECS_HIGH | ETHTOOL_COALESCE_TX_USECS_HIGH)
#define ETHTOOL_COALESCE_MAX_FRAMES_LOW_HIGH	\
	(ETHTOOL_COALESCE_RX_MAX_FRAMES_LOW |	\
	 ETHTOOL_COALESCE_TX_MAX_FRAMES_LOW |	\
	 ETHTOOL_COALESCE_RX_MAX_FRAMES_HIGH |	\
	 ETHTOOL_COALESCE_TX_MAX_FRAMES_HIGH)
#define ETHTOOL_COALESCE_PKT_RATE_RX_USECS				\
	(ETHTOOL_COALESCE_USE_ADAPTIVE_RX |				\
	 ETHTOOL_COALESCE_RX_USECS_LOW | ETHTOOL_COALESCE_RX_USECS_HIGH | \
	 ETHTOOL_COALESCE_PKT_RATE_LOW | ETHTOOL_COALESCE_PKT_RATE_HIGH | \
	 ETHTOOL_COALESCE_RATE_SAMPLE_INTERVAL)
#define ETHTOOL_COALESCE_USE_CQE					\
	(ETHTOOL_COALESCE_USE_CQE_RX | ETHTOOL_COALESCE_USE_CQE_TX)
#define ETHTOOL_COALESCE_TX_AGGR		\
	(ETHTOOL_COALESCE_TX_AGGR_MAX_BYTES |	\
	 ETHTOOL_COALESCE_TX_AGGR_MAX_FRAMES |	\
	 ETHTOOL_COALESCE_TX_AGGR_TIME_USECS)

#define ETHTOOL_STAT_NOT_SET	(~0ULL)

static inline void ethtool_stats_init(u64 *stats, unsigned int n)
{
	while (n--)
		stats[n] = ETHTOOL_STAT_NOT_SET;
}

/* Basic IEEE 802.3 MAC statistics (30.3.1.1.*), not otherwise exposed
 * via a more targeted API.
 */
struct ethtool_eth_mac_stats {
	enum ethtool_mac_stats_src src;
	struct_group(stats,
		u64 FramesTransmittedOK;
		u64 SingleCollisionFrames;
		u64 MultipleCollisionFrames;
		u64 FramesReceivedOK;
		u64 FrameCheckSequenceErrors;
		u64 AlignmentErrors;
		u64 OctetsTransmittedOK;
		u64 FramesWithDeferredXmissions;
		u64 LateCollisions;
		u64 FramesAbortedDueToXSColls;
		u64 FramesLostDueToIntMACXmitError;
		u64 CarrierSenseErrors;
		u64 OctetsReceivedOK;
		u64 FramesLostDueToIntMACRcvError;
		u64 MulticastFramesXmittedOK;
		u64 BroadcastFramesXmittedOK;
		u64 FramesWithExcessiveDeferral;
		u64 MulticastFramesReceivedOK;
		u64 BroadcastFramesReceivedOK;
		u64 InRangeLengthErrors;
		u64 OutOfRangeLengthField;
		u64 FrameTooLongErrors;
	);
};

/* Basic IEEE 802.3 PHY statistics (30.3.2.1.*), not otherwise exposed
 * via a more targeted API.
 */
struct ethtool_eth_phy_stats {
	enum ethtool_mac_stats_src src;
	struct_group(stats,
		u64 SymbolErrorDuringCarrier;
	);
};

/* Basic IEEE 802.3 MAC Ctrl statistics (30.3.3.*), not otherwise exposed
 * via a more targeted API.
 */
struct ethtool_eth_ctrl_stats {
	enum ethtool_mac_stats_src src;
	struct_group(stats,
		u64 MACControlFramesTransmitted;
		u64 MACControlFramesReceived;
		u64 UnsupportedOpcodesReceived;
	);
};

/**
 * struct ethtool_pause_stats - statistics for IEEE 802.3x pause frames
 * @src: input field denoting whether stats should be queried from the eMAC or
 *	pMAC (if the MM layer is supported). To be ignored otherwise.
 * @tx_pause_frames: transmitted pause frame count. Reported to user space
 *	as %ETHTOOL_A_PAUSE_STAT_TX_FRAMES.
 *
 *	Equivalent to `30.3.4.2 aPAUSEMACCtrlFramesTransmitted`
 *	from the standard.
 *
 * @rx_pause_frames: received pause frame count. Reported to user space
 *	as %ETHTOOL_A_PAUSE_STAT_RX_FRAMES. Equivalent to:
 *
 *	Equivalent to `30.3.4.3 aPAUSEMACCtrlFramesReceived`
 *	from the standard.
 */
struct ethtool_pause_stats {
	enum ethtool_mac_stats_src src;
	struct_group(stats,
		u64 tx_pause_frames;
		u64 rx_pause_frames;
	);
};

#define ETHTOOL_MAX_LANES	8

/**
 * struct ethtool_fec_stats - statistics for IEEE 802.3 FEC
 * @corrected_blocks: number of received blocks corrected by FEC
 *	Reported to user space as %ETHTOOL_A_FEC_STAT_CORRECTED.
 *
 *	Equivalent to `30.5.1.1.17 aFECCorrectedBlocks` from the standard.
 *
 * @uncorrectable_blocks: number of received blocks FEC was not able to correct
 *	Reported to user space as %ETHTOOL_A_FEC_STAT_UNCORR.
 *
 *	Equivalent to `30.5.1.1.18 aFECUncorrectableBlocks` from the standard.
 *
 * @corrected_bits: number of bits corrected by FEC
 *	Similar to @corrected_blocks but counts individual bit changes,
 *	not entire FEC data blocks. This is a non-standard statistic.
 *	Reported to user space as %ETHTOOL_A_FEC_STAT_CORR_BITS.
 *
 * For each of the above fields, the two substructure members are:
 *
 * - @lanes: per-lane/PCS-instance counts as defined by the standard
 * - @total: error counts for the entire port, for drivers incapable of reporting
 *	per-lane stats
 *
 * Drivers should fill in either only total or per-lane statistics, core
 * will take care of adding lane values up to produce the total.
 */
struct ethtool_fec_stats {
	struct ethtool_fec_stat {
		u64 total;
		u64 lanes[ETHTOOL_MAX_LANES];
	} corrected_blocks, uncorrectable_blocks, corrected_bits;
};

/**
 * struct ethtool_rmon_hist_range - byte range for histogram statistics
 * @low: low bound of the bucket (inclusive)
 * @high: high bound of the bucket (inclusive)
 */
struct ethtool_rmon_hist_range {
	u16 low;
	u16 high;
};

#define ETHTOOL_RMON_HIST_MAX	10

/**
 * struct ethtool_rmon_stats - selected RMON (RFC 2819) statistics
 * @src: input field denoting whether stats should be queried from the eMAC or
 *	pMAC (if the MM layer is supported). To be ignored otherwise.
 * @undersize_pkts: Equivalent to `etherStatsUndersizePkts` from the RFC.
 * @oversize_pkts: Equivalent to `etherStatsOversizePkts` from the RFC.
 * @fragments: Equivalent to `etherStatsFragments` from the RFC.
 * @jabbers: Equivalent to `etherStatsJabbers` from the RFC.
 * @hist: Packet counter for packet length buckets (e.g.
 *	`etherStatsPkts128to255Octets` from the RFC).
 * @hist_tx: Tx counters in similar form to @hist, not defined in the RFC.
 *
 * Selection of RMON (RFC 2819) statistics which are not exposed via different
 * APIs, primarily the packet-length-based counters.
 * Unfortunately different designs choose different buckets beyond
 * the 1024B mark (jumbo frame teritory), so the definition of the bucket
 * ranges is left to the driver.
 */
struct ethtool_rmon_stats {
	enum ethtool_mac_stats_src src;
	struct_group(stats,
		u64 undersize_pkts;
		u64 oversize_pkts;
		u64 fragments;
		u64 jabbers;

		u64 hist[ETHTOOL_RMON_HIST_MAX];
		u64 hist_tx[ETHTOOL_RMON_HIST_MAX];
	);
};

#define ETH_MODULE_EEPROM_PAGE_LEN	128
#define ETH_MODULE_MAX_I2C_ADDRESS	0x7f

/**
 * struct ethtool_module_eeprom - EEPROM dump from specified page
 * @offset: Offset within the specified EEPROM page to begin read, in bytes.
 * @length: Number of bytes to read.
 * @page: Page number to read from.
 * @bank: Page bank number to read from, if applicable by EEPROM spec.
 * @i2c_address: I2C address of a page. Value less than 0x7f expected. Most
 *	EEPROMs use 0x50 or 0x51.
 * @data: Pointer to buffer with EEPROM data of @length size.
 *
 * This can be used to manage pages during EEPROM dump in ethtool and pass
 * required information to the driver.
 */
struct ethtool_module_eeprom {
	u32	offset;
	u32	length;
	u8	page;
	u8	bank;
	u8	i2c_address;
	u8	*data;
};

/**
 * struct ethtool_module_power_mode_params - module power mode parameters
 * @policy: The power mode policy enforced by the host for the plug-in module.
 * @mode: The operational power mode of the plug-in module. Should be filled by
 *	device drivers on get operations.
 */
struct ethtool_module_power_mode_params {
	enum ethtool_module_power_mode_policy policy;
	enum ethtool_module_power_mode mode;
};

/**
 * struct ethtool_mm_state - 802.3 MAC merge layer state
 * @verify_time:
 *	wait time between verification attempts in ms (according to clause
 *	30.14.1.6 aMACMergeVerifyTime)
 * @max_verify_time:
 *	maximum accepted value for the @verify_time variable in set requests
 * @verify_status:
 *	state of the verification state machine of the MM layer (according to
 *	clause 30.14.1.2 aMACMergeStatusVerify)
 * @tx_enabled:
 *	set if the MM layer is administratively enabled in the TX direction
 *	(according to clause 30.14.1.3 aMACMergeEnableTx)
 * @tx_active:
 *	set if the MM layer is enabled in the TX direction, which makes FP
 *	possible (according to 30.14.1.5 aMACMergeStatusTx). This should be
 *	true if MM is enabled, and the verification status is either verified,
 *	or disabled.
 * @pmac_enabled:
 *	set if the preemptible MAC is powered on and is able to receive
 *	preemptible packets and respond to verification frames.
 * @verify_enabled:
 *	set if the Verify function of the MM layer (which sends SMD-V
 *	verification requests) is administratively enabled (regardless of
 *	whether it is currently in the ETHTOOL_MM_VERIFY_STATUS_DISABLED state
 *	or not), according to clause 30.14.1.4 aMACMergeVerifyDisableTx (but
 *	using positive rather than negative logic). The device should always
 *	respond to received SMD-V requests as long as @pmac_enabled is set.
 * @tx_min_frag_size:
 *	the minimum size of non-final mPacket fragments that the link partner
 *	supports receiving, expressed in octets. Compared to the definition
 *	from clause 30.14.1.7 aMACMergeAddFragSize which is expressed in the
 *	range 0 to 3 (requiring a translation to the size in octets according
 *	to the formula 64 * (1 + addFragSize) - 4), a value in a continuous and
 *	unbounded range can be specified here.
 * @rx_min_frag_size:
 *	the minimum size of non-final mPacket fragments that this device
 *	supports receiving, expressed in octets.
 */
struct ethtool_mm_state {
	u32 verify_time;
	u32 max_verify_time;
	enum ethtool_mm_verify_status verify_status;
	bool tx_enabled;
	bool tx_active;
	bool pmac_enabled;
	bool verify_enabled;
	u32 tx_min_frag_size;
	u32 rx_min_frag_size;
};

/**
 * struct ethtool_mm_cfg - 802.3 MAC merge layer configuration
 * @verify_time: see struct ethtool_mm_state
 * @verify_enabled: see struct ethtool_mm_state
 * @tx_enabled: see struct ethtool_mm_state
 * @pmac_enabled: see struct ethtool_mm_state
 * @tx_min_frag_size: see struct ethtool_mm_state
 */
struct ethtool_mm_cfg {
	u32 verify_time;
	bool verify_enabled;
	bool tx_enabled;
	bool pmac_enabled;
	u32 tx_min_frag_size;
};

/**
 * struct ethtool_mm_stats - 802.3 MAC merge layer statistics
 * @MACMergeFrameAssErrorCount:
 *	received MAC frames with reassembly errors
 * @MACMergeFrameSmdErrorCount:
 *	received MAC frames/fragments rejected due to unknown or incorrect SMD
 * @MACMergeFrameAssOkCount:
 *	received MAC frames that were successfully reassembled and passed up
 * @MACMergeFragCountRx:
 *	number of additional correct SMD-C mPackets received due to preemption
 * @MACMergeFragCountTx:
 *	number of additional mPackets sent due to preemption
 * @MACMergeHoldCount:
 *	number of times the MM layer entered the HOLD state, which blocks
 *	transmission of preemptible traffic
 */
struct ethtool_mm_stats {
	u64 MACMergeFrameAssErrorCount;
	u64 MACMergeFrameSmdErrorCount;
	u64 MACMergeFrameAssOkCount;
	u64 MACMergeFragCountRx;
	u64 MACMergeFragCountTx;
	u64 MACMergeHoldCount;
};

/**
 * struct ethtool_rxfh_param - RXFH (RSS) parameters
 * @hfunc: Defines the current RSS hash function used by HW (or to be set to).
 *	Valid values are one of the %ETH_RSS_HASH_*.
 * @indir_size: On SET, the array size of the user buffer for the
 *	indirection table, which may be zero, or
 *	%ETH_RXFH_INDIR_NO_CHANGE.  On GET (read from the driver),
 *	the array size of the hardware indirection table.
 * @indir: The indirection table of size @indir_size entries.
 * @key_size: On SET, the array size of the user buffer for the hash key,
 *	which may be zero.  On GET (read from the driver), the size of the
 *	hardware hash key.
 * @key: The hash key of size @key_size bytes.
 * @rss_context: RSS context identifier.  Context 0 is the default for normal
 *	traffic; other contexts can be referenced as the destination for RX flow
 *	classification rules.  On SET, %ETH_RXFH_CONTEXT_ALLOC is used
 *	to allocate a new RSS context; on return this field will
 *	contain the ID of the newly allocated context.
 * @rss_delete: Set to non-ZERO to remove the @rss_context context.
 * @input_xfrm: Defines how the input data is transformed. Valid values are one
 *	of %RXH_XFRM_*.
 */
struct ethtool_rxfh_param {
	u8	hfunc;
	u32	indir_size;
	u32	*indir;
	u32	key_size;
	u8	*key;
	u32	rss_context;
	u8	rss_delete;
	u8	input_xfrm;
};

/**
 * struct ethtool_ops - optional netdev operations
 * @cap_link_lanes_supported: indicates if the driver supports lanes
 *	parameter.
 * @cap_rss_ctx_supported: indicates if the driver supports RSS
 *	contexts.
 * @cap_rss_sym_xor_supported: indicates if the driver supports symmetric-xor
 *	RSS.
 * @supported_coalesce_params: supported types of interrupt coalescing.
 * @supported_ring_params: supported ring params.
 * @get_drvinfo: Report driver/device information. Modern drivers no
 *	longer have to implement this callback. Most fields are
 *	correctly filled in by the core using system information, or
 *	populated using other driver operations.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @get_regs_len: Get buffer length required for @get_regs
 * @get_regs: Get device registers
 * @get_wol: Report whether Wake-on-Lan is enabled
 * @set_wol: Turn Wake-on-Lan on or off.  Returns a negative error code
 *	or zero.
 * @get_msglevel: Report driver message level.  This should be the value
 *	of the @msg_enable field used by netif logging functions.
 * @set_msglevel: Set driver message level
 * @nway_reset: Restart autonegotiation.  Returns a negative error code
 *	or zero.
 * @get_link: Report whether physical link is up.  Will only be called if
 *	the netdev is up.  Should usually be set to ethtool_op_get_link(),
 *	which uses netif_carrier_ok().
<<<<<<< HEAD
=======
 * @get_link_ext_state: Report link extended state. Should set link_ext_state and
 *	link_ext_substate (link_ext_substate of 0 means link_ext_substate is unknown,
 *	do not attach ext_substate attribute to netlink message). If link_ext_state
 *	and link_ext_substate are unknown, return -ENODATA. If not implemented,
 *	link_ext_state and link_ext_substate will not be sent to userspace.
 * @get_link_ext_stats: Read extra link-related counters.
 * @get_eeprom_len: Read range of EEPROM addresses for validation of
 *	@get_eeprom and @set_eeprom requests.
 *	Returns 0 if device does not support EEPROM access.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @get_eeprom: Read data from the device EEPROM.
 *	Should fill in the magic field.  Don't need to check len for zero
 *	or wraparound.  Fill in the data argument with the eeprom values
 *	from offset to offset + len.  Update len to the amount read.
 *	Returns an error or zero.
 * @set_eeprom: Write data to the device EEPROM.
 *	Should validate the magic field.  Don't need to check len for zero
 *	or wraparound.  Update len to the amount written.  Returns an error
 *	or zero.
 * @get_coalesce: Get interrupt coalescing parameters.  Returns a negative
 *	error code or zero.
<<<<<<< HEAD
 * @set_coalesce: Set interrupt coalescing parameters.  Returns a negative
 *	error code or zero.
 * @get_ringparam: Report ring sizes
 * @set_ringparam: Set ring sizes.  Returns a negative error code or zero.
=======
 * @set_coalesce: Set interrupt coalescing parameters.  Supported coalescing
 *	types should be set in @supported_coalesce_params.
 *	Returns a negative error code or zero.
 * @get_ringparam: Report ring sizes
 * @set_ringparam: Set ring sizes.  Returns a negative error code or zero.
 * @get_pause_stats: Report pause frame statistics. Drivers must not zero
 *	statistics which they don't report. The stats structure is initialized
 *	to ETHTOOL_STAT_NOT_SET indicating driver does not report statistics.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @get_pauseparam: Report pause parameters
 * @set_pauseparam: Set pause parameters.  Returns a negative error code
 *	or zero.
 * @self_test: Run specified self-tests
 * @get_strings: Return a set of strings that describe the requested objects
 * @set_phys_id: Identify the physical devices, e.g. by flashing an LED
 *	attached to it.  The implementation may update the indicator
 *	asynchronously or synchronously, but in either case it must return
 *	quickly.  It is initially called with the argument %ETHTOOL_ID_ACTIVE,
 *	and must either activate asynchronous updates and return zero, return
 *	a negative error or return a positive frequency for synchronous
 *	indication (e.g. 1 for one on/off cycle per second).  If it returns
 *	a frequency then it will be called again at intervals with the
 *	argument %ETHTOOL_ID_ON or %ETHTOOL_ID_OFF and should set the state of
 *	the indicator accordingly.  Finally, it is called with the argument
 *	%ETHTOOL_ID_INACTIVE and must deactivate the indicator.  Returns a
 *	negative error code or zero.
 * @get_ethtool_stats: Return extended statistics about the device.
 *	This is only useful if the device maintains statistics not
 *	included in &struct rtnl_link_stats64.
 * @begin: Function to be called before any other operation.  Returns a
 *	negative error code or zero.
 * @complete: Function to be called after any other operation except
 *	@begin.  Will be called even if the other operation failed.
 * @get_priv_flags: Report driver-specific feature flags.
 * @set_priv_flags: Set driver-specific feature flags.  Returns a negative
 *	error code or zero.
 * @get_sset_count: Get number of strings that @get_strings will write.
 * @get_rxnfc: Get RX flow classification rules.  Returns a negative
 *	error code or zero.
 * @set_rxnfc: Set RX flow classification rules.  Returns a negative
 *	error code or zero.
 * @flash_device: Write a firmware image to device's flash memory.
 *	Returns a negative error code or zero.
 * @reset: Reset (part of) the device, as specified by a bitmask of
 *	flags from &enum ethtool_reset_flags.  Returns a negative
 *	error code or zero.
<<<<<<< HEAD
 * @get_rxfh_indir_size: Get the size of the RX flow hash indirection table.
 *	Returns zero if not supported for this specific device.
 * @get_rxfh_indir: Get the contents of the RX flow hash indirection table.
 *	Will not be called if @get_rxfh_indir_size returns zero.
 *	Returns a negative error code or zero.
 * @set_rxfh_indir: Set the contents of the RX flow hash indirection table.
 *	Will not be called if @get_rxfh_indir_size returns zero.
 *	Returns a negative error code or zero.
=======
 * @get_rxfh_key_size: Get the size of the RX flow hash key.
 *	Returns zero if not supported for this specific device.
 * @get_rxfh_indir_size: Get the size of the RX flow hash indirection table.
 *	Returns zero if not supported for this specific device.
 * @get_rxfh: Get the contents of the RX flow hash indirection table, hash key
 *	and/or hash function.
 *	Returns a negative error code or zero.
 * @set_rxfh: Set the contents of the RX flow hash indirection table, hash
 *	key, and/or hash function.  Arguments which are set to %NULL or zero
 *	will remain unchanged.
 *	Returns a negative error code or zero. An error code must be returned
 *	if at least one unsupported change was requested.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @get_channels: Get number of channels.
 * @set_channels: Set number of channels.  Returns a negative error code or
 *	zero.
 * @get_dump_flag: Get dump flag indicating current dump length, version,
 * 		   and flag of the device.
 * @get_dump_data: Get dump data.
 * @set_dump: Set dump specific flags to the device.
<<<<<<< HEAD
=======
 * @get_ts_info: Get the time stamping and PTP hardware clock capabilities.
 *	It may be called with RCU, or rtnl or reference on the device.
 *	Drivers supporting transmit time stamps in software should set this to
 *	ethtool_op_get_ts_info().
 * @get_module_info: Get the size and type of the eeprom contained within
 *	a plug-in module.
 * @get_module_eeprom: Get the eeprom information from the plug-in module
 * @get_eee: Get Energy-Efficient (EEE) supported and status.
 * @set_eee: Set EEE status (enable/disable) as well as LPI timers.
 * @get_tunable: Read the value of a driver / device tunable.
 * @set_tunable: Set the value of a driver / device tunable.
 * @get_per_queue_coalesce: Get interrupt coalescing parameters per queue.
 *	It must check that the given queue number is valid. If neither a RX nor
 *	a TX queue has this number, return -EINVAL. If only a RX queue or a TX
 *	queue has this number, set the inapplicable fields to ~0 and return 0.
 *	Returns a negative error code or zero.
 * @set_per_queue_coalesce: Set interrupt coalescing parameters per queue.
 *	It must check that the given queue number is valid. If neither a RX nor
 *	a TX queue has this number, return -EINVAL. If only a RX queue or a TX
 *	queue has this number, ignore the inapplicable fields. Supported
 *	coalescing types should be set in @supported_coalesce_params.
 *	Returns a negative error code or zero.
 * @get_link_ksettings: Get various device settings including Ethernet link
 *	settings. The %cmd and %link_mode_masks_nwords fields should be
 *	ignored (use %__ETHTOOL_LINK_MODE_MASK_NBITS instead of the latter),
 *	any change to them will be overwritten by kernel. Returns a negative
 *	error code or zero.
 * @set_link_ksettings: Set various device settings including Ethernet link
 *	settings. The %cmd and %link_mode_masks_nwords fields should be
 *	ignored (use %__ETHTOOL_LINK_MODE_MASK_NBITS instead of the latter),
 *	any change to them will be overwritten by kernel. Returns a negative
 *	error code or zero.
 * @get_fec_stats: Report FEC statistics.
 *	Core will sum up per-lane stats to get the total.
 *	Drivers must not zero statistics which they don't report. The stats
 *	structure is initialized to ETHTOOL_STAT_NOT_SET indicating driver does
 *	not report statistics.
 * @get_fecparam: Get the network device Forward Error Correction parameters.
 * @set_fecparam: Set the network device Forward Error Correction parameters.
 * @get_ethtool_phy_stats: Return extended statistics about the PHY device.
 *	This is only useful if the device maintains PHY statistics and
 *	cannot use the standard PHY library helpers.
 * @get_phy_tunable: Read the value of a PHY tunable.
 * @set_phy_tunable: Set the value of a PHY tunable.
 * @get_module_eeprom_by_page: Get a region of plug-in module EEPROM data from
 *	specified page. Returns a negative error code or the amount of bytes
 *	read.
 * @get_eth_phy_stats: Query some of the IEEE 802.3 PHY statistics.
 * @get_eth_mac_stats: Query some of the IEEE 802.3 MAC statistics.
 * @get_eth_ctrl_stats: Query some of the IEEE 802.3 MAC Ctrl statistics.
 * @get_rmon_stats: Query some of the RMON (RFC 2819) statistics.
 *	Set %ranges to a pointer to zero-terminated array of byte ranges.
 * @get_module_power_mode: Get the power mode policy for the plug-in module
 *	used by the network device and its operational power mode, if
 *	plugged-in.
 * @set_module_power_mode: Set the power mode policy for the plug-in module
 *	used by the network device.
 * @get_mm: Query the 802.3 MAC Merge layer state.
 * @set_mm: Set the 802.3 MAC Merge layer parameters.
 * @get_mm_stats: Query the 802.3 MAC Merge layer statistics.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * All operations are optional (i.e. the function pointer may be set
 * to %NULL) and callers must take this into account.  Callers must
 * hold the RTNL lock.
 *
 * See the structures used by these operations for further documentation.
<<<<<<< HEAD
=======
 * Note that for all operations using a structure ending with a zero-
 * length array, the array is allocated separately in the kernel and
 * is passed to the driver as an additional parameter.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * See &struct net_device and &struct net_device_ops for documentation
 * of the generic netdev features interface.
 */
struct ethtool_ops {
<<<<<<< HEAD
	int	(*get_settings)(struct net_device *, struct ethtool_cmd *);
	int	(*set_settings)(struct net_device *, struct ethtool_cmd *);
=======
	u32     cap_link_lanes_supported:1;
	u32     cap_rss_ctx_supported:1;
	u32	cap_rss_sym_xor_supported:1;
	u32	supported_coalesce_params;
	u32	supported_ring_params;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void	(*get_drvinfo)(struct net_device *, struct ethtool_drvinfo *);
	int	(*get_regs_len)(struct net_device *);
	void	(*get_regs)(struct net_device *, struct ethtool_regs *, void *);
	void	(*get_wol)(struct net_device *, struct ethtool_wolinfo *);
	int	(*set_wol)(struct net_device *, struct ethtool_wolinfo *);
	u32	(*get_msglevel)(struct net_device *);
	void	(*set_msglevel)(struct net_device *, u32);
	int	(*nway_reset)(struct net_device *);
	u32	(*get_link)(struct net_device *);
<<<<<<< HEAD
=======
	int	(*get_link_ext_state)(struct net_device *,
				      struct ethtool_link_ext_state_info *);
	void	(*get_link_ext_stats)(struct net_device *dev,
				      struct ethtool_link_ext_stats *stats);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int	(*get_eeprom_len)(struct net_device *);
	int	(*get_eeprom)(struct net_device *,
			      struct ethtool_eeprom *, u8 *);
	int	(*set_eeprom)(struct net_device *,
			      struct ethtool_eeprom *, u8 *);
<<<<<<< HEAD
	int	(*get_coalesce)(struct net_device *, struct ethtool_coalesce *);
	int	(*set_coalesce)(struct net_device *, struct ethtool_coalesce *);
	void	(*get_ringparam)(struct net_device *,
				 struct ethtool_ringparam *);
	int	(*set_ringparam)(struct net_device *,
				 struct ethtool_ringparam *);
=======
	int	(*get_coalesce)(struct net_device *,
				struct ethtool_coalesce *,
				struct kernel_ethtool_coalesce *,
				struct netlink_ext_ack *);
	int	(*set_coalesce)(struct net_device *,
				struct ethtool_coalesce *,
				struct kernel_ethtool_coalesce *,
				struct netlink_ext_ack *);
	void	(*get_ringparam)(struct net_device *,
				 struct ethtool_ringparam *,
				 struct kernel_ethtool_ringparam *,
				 struct netlink_ext_ack *);
	int	(*set_ringparam)(struct net_device *,
				 struct ethtool_ringparam *,
				 struct kernel_ethtool_ringparam *,
				 struct netlink_ext_ack *);
	void	(*get_pause_stats)(struct net_device *dev,
				   struct ethtool_pause_stats *pause_stats);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void	(*get_pauseparam)(struct net_device *,
				  struct ethtool_pauseparam*);
	int	(*set_pauseparam)(struct net_device *,
				  struct ethtool_pauseparam*);
	void	(*self_test)(struct net_device *, struct ethtool_test *, u64 *);
	void	(*get_strings)(struct net_device *, u32 stringset, u8 *);
	int	(*set_phys_id)(struct net_device *, enum ethtool_phys_id_state);
	void	(*get_ethtool_stats)(struct net_device *,
				     struct ethtool_stats *, u64 *);
	int	(*begin)(struct net_device *);
	void	(*complete)(struct net_device *);
	u32	(*get_priv_flags)(struct net_device *);
	int	(*set_priv_flags)(struct net_device *, u32);
	int	(*get_sset_count)(struct net_device *, int);
	int	(*get_rxnfc)(struct net_device *,
			     struct ethtool_rxnfc *, u32 *rule_locs);
	int	(*set_rxnfc)(struct net_device *, struct ethtool_rxnfc *);
	int	(*flash_device)(struct net_device *, struct ethtool_flash *);
	int	(*reset)(struct net_device *, u32 *);
<<<<<<< HEAD
	u32	(*get_rxfh_indir_size)(struct net_device *);
	int	(*get_rxfh_indir)(struct net_device *, u32 *);
	int	(*set_rxfh_indir)(struct net_device *, const u32 *);
=======
	u32	(*get_rxfh_key_size)(struct net_device *);
	u32	(*get_rxfh_indir_size)(struct net_device *);
	int	(*get_rxfh)(struct net_device *, struct ethtool_rxfh_param *);
	int	(*set_rxfh)(struct net_device *, struct ethtool_rxfh_param *,
			    struct netlink_ext_ack *extack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void	(*get_channels)(struct net_device *, struct ethtool_channels *);
	int	(*set_channels)(struct net_device *, struct ethtool_channels *);
	int	(*get_dump_flag)(struct net_device *, struct ethtool_dump *);
	int	(*get_dump_data)(struct net_device *,
				 struct ethtool_dump *, void *);
	int	(*set_dump)(struct net_device *, struct ethtool_dump *);
<<<<<<< HEAD

};
#endif /* __KERNEL__ */

/* CMDs currently supported */
#define ETHTOOL_GSET		0x00000001 /* Get settings. */
#define ETHTOOL_SSET		0x00000002 /* Set settings. */
#define ETHTOOL_GDRVINFO	0x00000003 /* Get driver info. */
#define ETHTOOL_GREGS		0x00000004 /* Get NIC registers. */
#define ETHTOOL_GWOL		0x00000005 /* Get wake-on-lan options. */
#define ETHTOOL_SWOL		0x00000006 /* Set wake-on-lan options. */
#define ETHTOOL_GMSGLVL		0x00000007 /* Get driver message level */
#define ETHTOOL_SMSGLVL		0x00000008 /* Set driver msg level. */
#define ETHTOOL_NWAY_RST	0x00000009 /* Restart autonegotiation. */
/* Get link status for host, i.e. whether the interface *and* the
 * physical port (if there is one) are up (ethtool_value). */
#define ETHTOOL_GLINK		0x0000000a
#define ETHTOOL_GEEPROM		0x0000000b /* Get EEPROM data */
#define ETHTOOL_SEEPROM		0x0000000c /* Set EEPROM data. */
#define ETHTOOL_GCOALESCE	0x0000000e /* Get coalesce config */
#define ETHTOOL_SCOALESCE	0x0000000f /* Set coalesce config. */
#define ETHTOOL_GRINGPARAM	0x00000010 /* Get ring parameters */
#define ETHTOOL_SRINGPARAM	0x00000011 /* Set ring parameters. */
#define ETHTOOL_GPAUSEPARAM	0x00000012 /* Get pause parameters */
#define ETHTOOL_SPAUSEPARAM	0x00000013 /* Set pause parameters. */
#define ETHTOOL_GRXCSUM		0x00000014 /* Get RX hw csum enable (ethtool_value) */
#define ETHTOOL_SRXCSUM		0x00000015 /* Set RX hw csum enable (ethtool_value) */
#define ETHTOOL_GTXCSUM		0x00000016 /* Get TX hw csum enable (ethtool_value) */
#define ETHTOOL_STXCSUM		0x00000017 /* Set TX hw csum enable (ethtool_value) */
#define ETHTOOL_GSG		0x00000018 /* Get scatter-gather enable
					    * (ethtool_value) */
#define ETHTOOL_SSG		0x00000019 /* Set scatter-gather enable
					    * (ethtool_value). */
#define ETHTOOL_TEST		0x0000001a /* execute NIC self-test. */
#define ETHTOOL_GSTRINGS	0x0000001b /* get specified string set */
#define ETHTOOL_PHYS_ID		0x0000001c /* identify the NIC */
#define ETHTOOL_GSTATS		0x0000001d /* get NIC-specific statistics */
#define ETHTOOL_GTSO		0x0000001e /* Get TSO enable (ethtool_value) */
#define ETHTOOL_STSO		0x0000001f /* Set TSO enable (ethtool_value) */
#define ETHTOOL_GPERMADDR	0x00000020 /* Get permanent hardware address */
#define ETHTOOL_GUFO		0x00000021 /* Get UFO enable (ethtool_value) */
#define ETHTOOL_SUFO		0x00000022 /* Set UFO enable (ethtool_value) */
#define ETHTOOL_GGSO		0x00000023 /* Get GSO enable (ethtool_value) */
#define ETHTOOL_SGSO		0x00000024 /* Set GSO enable (ethtool_value) */
#define ETHTOOL_GFLAGS		0x00000025 /* Get flags bitmap(ethtool_value) */
#define ETHTOOL_SFLAGS		0x00000026 /* Set flags bitmap(ethtool_value) */
#define ETHTOOL_GPFLAGS		0x00000027 /* Get driver-private flags bitmap */
#define ETHTOOL_SPFLAGS		0x00000028 /* Set driver-private flags bitmap */

#define ETHTOOL_GRXFH		0x00000029 /* Get RX flow hash configuration */
#define ETHTOOL_SRXFH		0x0000002a /* Set RX flow hash configuration */
#define ETHTOOL_GGRO		0x0000002b /* Get GRO enable (ethtool_value) */
#define ETHTOOL_SGRO		0x0000002c /* Set GRO enable (ethtool_value) */
#define ETHTOOL_GRXRINGS	0x0000002d /* Get RX rings available for LB */
#define ETHTOOL_GRXCLSRLCNT	0x0000002e /* Get RX class rule count */
#define ETHTOOL_GRXCLSRULE	0x0000002f /* Get RX classification rule */
#define ETHTOOL_GRXCLSRLALL	0x00000030 /* Get all RX classification rule */
#define ETHTOOL_SRXCLSRLDEL	0x00000031 /* Delete RX classification rule */
#define ETHTOOL_SRXCLSRLINS	0x00000032 /* Insert RX classification rule */
#define ETHTOOL_FLASHDEV	0x00000033 /* Flash firmware to device */
#define ETHTOOL_RESET		0x00000034 /* Reset hardware */
#define ETHTOOL_SRXNTUPLE	0x00000035 /* Add an n-tuple filter to device */
#define ETHTOOL_GRXNTUPLE	0x00000036 /* deprecated */
#define ETHTOOL_GSSET_INFO	0x00000037 /* Get string set info */
#define ETHTOOL_GRXFHINDIR	0x00000038 /* Get RX flow hash indir'n table */
#define ETHTOOL_SRXFHINDIR	0x00000039 /* Set RX flow hash indir'n table */

#define ETHTOOL_GFEATURES	0x0000003a /* Get device offload settings */
#define ETHTOOL_SFEATURES	0x0000003b /* Change device offload settings */
#define ETHTOOL_GCHANNELS	0x0000003c /* Get no of channels */
#define ETHTOOL_SCHANNELS	0x0000003d /* Set no of channels */
#define ETHTOOL_SET_DUMP	0x0000003e /* Set dump settings */
#define ETHTOOL_GET_DUMP_FLAG	0x0000003f /* Get dump settings */
#define ETHTOOL_GET_DUMP_DATA	0x00000040 /* Get dump data */

/* compatibility with older code */
#define SPARC_ETH_GSET		ETHTOOL_GSET
#define SPARC_ETH_SSET		ETHTOOL_SSET

/* Indicates what features are supported by the interface. */
#define SUPPORTED_10baseT_Half		(1 << 0)
#define SUPPORTED_10baseT_Full		(1 << 1)
#define SUPPORTED_100baseT_Half		(1 << 2)
#define SUPPORTED_100baseT_Full		(1 << 3)
#define SUPPORTED_1000baseT_Half	(1 << 4)
#define SUPPORTED_1000baseT_Full	(1 << 5)
#define SUPPORTED_Autoneg		(1 << 6)
#define SUPPORTED_TP			(1 << 7)
#define SUPPORTED_AUI			(1 << 8)
#define SUPPORTED_MII			(1 << 9)
#define SUPPORTED_FIBRE			(1 << 10)
#define SUPPORTED_BNC			(1 << 11)
#define SUPPORTED_10000baseT_Full	(1 << 12)
#define SUPPORTED_Pause			(1 << 13)
#define SUPPORTED_Asym_Pause		(1 << 14)
#define SUPPORTED_2500baseX_Full	(1 << 15)
#define SUPPORTED_Backplane		(1 << 16)
#define SUPPORTED_1000baseKX_Full	(1 << 17)
#define SUPPORTED_10000baseKX4_Full	(1 << 18)
#define SUPPORTED_10000baseKR_Full	(1 << 19)
#define SUPPORTED_10000baseR_FEC	(1 << 20)
#define SUPPORTED_20000baseMLD2_Full	(1 << 21)
#define SUPPORTED_20000baseKR2_Full	(1 << 22)

/* Indicates what features are advertised by the interface. */
#define ADVERTISED_10baseT_Half		(1 << 0)
#define ADVERTISED_10baseT_Full		(1 << 1)
#define ADVERTISED_100baseT_Half	(1 << 2)
#define ADVERTISED_100baseT_Full	(1 << 3)
#define ADVERTISED_1000baseT_Half	(1 << 4)
#define ADVERTISED_1000baseT_Full	(1 << 5)
#define ADVERTISED_Autoneg		(1 << 6)
#define ADVERTISED_TP			(1 << 7)
#define ADVERTISED_AUI			(1 << 8)
#define ADVERTISED_MII			(1 << 9)
#define ADVERTISED_FIBRE		(1 << 10)
#define ADVERTISED_BNC			(1 << 11)
#define ADVERTISED_10000baseT_Full	(1 << 12)
#define ADVERTISED_Pause		(1 << 13)
#define ADVERTISED_Asym_Pause		(1 << 14)
#define ADVERTISED_2500baseX_Full	(1 << 15)
#define ADVERTISED_Backplane		(1 << 16)
#define ADVERTISED_1000baseKX_Full	(1 << 17)
#define ADVERTISED_10000baseKX4_Full	(1 << 18)
#define ADVERTISED_10000baseKR_Full	(1 << 19)
#define ADVERTISED_10000baseR_FEC	(1 << 20)
#define ADVERTISED_20000baseMLD2_Full	(1 << 21)
#define ADVERTISED_20000baseKR2_Full	(1 << 22)

/* The following are all involved in forcing a particular link
 * mode for the device for setting things.  When getting the
 * devices settings, these indicate the current mode and whether
 * it was forced up into this mode or autonegotiated.
 */

/* The forced speed, 10Mb, 100Mb, gigabit, 2.5Gb, 10GbE. */
#define SPEED_10		10
#define SPEED_100		100
#define SPEED_1000		1000
#define SPEED_2500		2500
#define SPEED_10000		10000
#define SPEED_UNKNOWN		-1

/* Duplex, half or full. */
#define DUPLEX_HALF		0x00
#define DUPLEX_FULL		0x01
#define DUPLEX_UNKNOWN		0xff

/* Which connector port. */
#define PORT_TP			0x00
#define PORT_AUI		0x01
#define PORT_MII		0x02
#define PORT_FIBRE		0x03
#define PORT_BNC		0x04
#define PORT_DA			0x05
#define PORT_NONE		0xef
#define PORT_OTHER		0xff

/* Which transceiver to use. */
#define XCVR_INTERNAL		0x00
#define XCVR_EXTERNAL		0x01
#define XCVR_DUMMY1		0x02
#define XCVR_DUMMY2		0x03
#define XCVR_DUMMY3		0x04

/* Enable or disable autonegotiation.  If this is set to enable,
 * the forced link modes above are completely ignored.
 */
#define AUTONEG_DISABLE		0x00
#define AUTONEG_ENABLE		0x01

/* Mode MDI or MDI-X */
#define ETH_TP_MDI_INVALID	0x00
#define ETH_TP_MDI		0x01
#define ETH_TP_MDI_X		0x02

/* Wake-On-Lan options. */
#define WAKE_PHY		(1 << 0)
#define WAKE_UCAST		(1 << 1)
#define WAKE_MCAST		(1 << 2)
#define WAKE_BCAST		(1 << 3)
#define WAKE_ARP		(1 << 4)
#define WAKE_MAGIC		(1 << 5)
#define WAKE_MAGICSECURE	(1 << 6) /* only meaningful if WAKE_MAGIC */

/* L2-L4 network traffic flow types */
#define	TCP_V4_FLOW	0x01	/* hash or spec (tcp_ip4_spec) */
#define	UDP_V4_FLOW	0x02	/* hash or spec (udp_ip4_spec) */
#define	SCTP_V4_FLOW	0x03	/* hash or spec (sctp_ip4_spec) */
#define	AH_ESP_V4_FLOW	0x04	/* hash only */
#define	TCP_V6_FLOW	0x05	/* hash only */
#define	UDP_V6_FLOW	0x06	/* hash only */
#define	SCTP_V6_FLOW	0x07	/* hash only */
#define	AH_ESP_V6_FLOW	0x08	/* hash only */
#define	AH_V4_FLOW	0x09	/* hash or spec (ah_ip4_spec) */
#define	ESP_V4_FLOW	0x0a	/* hash or spec (esp_ip4_spec) */
#define	AH_V6_FLOW	0x0b	/* hash only */
#define	ESP_V6_FLOW	0x0c	/* hash only */
#define	IP_USER_FLOW	0x0d	/* spec only (usr_ip4_spec) */
#define	IPV4_FLOW	0x10	/* hash only */
#define	IPV6_FLOW	0x11	/* hash only */
#define	ETHER_FLOW	0x12	/* spec only (ether_spec) */
/* Flag to enable additional fields in struct ethtool_rx_flow_spec */
#define	FLOW_EXT	0x80000000

/* L3-L4 network traffic flow hash options */
#define	RXH_L2DA	(1 << 1)
#define	RXH_VLAN	(1 << 2)
#define	RXH_L3_PROTO	(1 << 3)
#define	RXH_IP_SRC	(1 << 4)
#define	RXH_IP_DST	(1 << 5)
#define	RXH_L4_B_0_1	(1 << 6) /* src port in case of TCP/UDP/SCTP */
#define	RXH_L4_B_2_3	(1 << 7) /* dst port in case of TCP/UDP/SCTP */
#define	RXH_DISCARD	(1 << 31)

#define	RX_CLS_FLOW_DISC	0xffffffffffffffffULL

/* Special RX classification rule insert location values */
#define RX_CLS_LOC_SPECIAL	0x80000000	/* flag */
#define RX_CLS_LOC_ANY		0xffffffff
#define RX_CLS_LOC_FIRST	0xfffffffe
#define RX_CLS_LOC_LAST		0xfffffffd

/* Reset flags */
/* The reset() operation must clear the flags for the components which
 * were actually reset.  On successful return, the flags indicate the
 * components which were not reset, either because they do not exist
 * in the hardware or because they cannot be reset independently.  The
 * driver must never reset any components that were not requested.
 */
enum ethtool_reset_flags {
	/* These flags represent components dedicated to the interface
	 * the command is addressed to.  Shift any flag left by
	 * ETH_RESET_SHARED_SHIFT to reset a shared component of the
	 * same type.
	 */
	ETH_RESET_MGMT		= 1 << 0,	/* Management processor */
	ETH_RESET_IRQ		= 1 << 1,	/* Interrupt requester */
	ETH_RESET_DMA		= 1 << 2,	/* DMA engine */
	ETH_RESET_FILTER	= 1 << 3,	/* Filtering/flow direction */
	ETH_RESET_OFFLOAD	= 1 << 4,	/* Protocol offload */
	ETH_RESET_MAC		= 1 << 5,	/* Media access controller */
	ETH_RESET_PHY		= 1 << 6,	/* Transceiver/PHY */
	ETH_RESET_RAM		= 1 << 7,	/* RAM shared between
						 * multiple components */

	ETH_RESET_DEDICATED	= 0x0000ffff,	/* All components dedicated to
						 * this interface */
	ETH_RESET_ALL		= 0xffffffff,	/* All components used by this
						 * interface, even if shared */
};
#define ETH_RESET_SHARED_SHIFT	16

=======
	int	(*get_ts_info)(struct net_device *, struct ethtool_ts_info *);
	int     (*get_module_info)(struct net_device *,
				   struct ethtool_modinfo *);
	int     (*get_module_eeprom)(struct net_device *,
				     struct ethtool_eeprom *, u8 *);
	int	(*get_eee)(struct net_device *dev, struct ethtool_keee *eee);
	int	(*set_eee)(struct net_device *dev, struct ethtool_keee *eee);
	int	(*get_tunable)(struct net_device *,
			       const struct ethtool_tunable *, void *);
	int	(*set_tunable)(struct net_device *,
			       const struct ethtool_tunable *, const void *);
	int	(*get_per_queue_coalesce)(struct net_device *, u32,
					  struct ethtool_coalesce *);
	int	(*set_per_queue_coalesce)(struct net_device *, u32,
					  struct ethtool_coalesce *);
	int	(*get_link_ksettings)(struct net_device *,
				      struct ethtool_link_ksettings *);
	int	(*set_link_ksettings)(struct net_device *,
				      const struct ethtool_link_ksettings *);
	void	(*get_fec_stats)(struct net_device *dev,
				 struct ethtool_fec_stats *fec_stats);
	int	(*get_fecparam)(struct net_device *,
				      struct ethtool_fecparam *);
	int	(*set_fecparam)(struct net_device *,
				      struct ethtool_fecparam *);
	void	(*get_ethtool_phy_stats)(struct net_device *,
					 struct ethtool_stats *, u64 *);
	int	(*get_phy_tunable)(struct net_device *,
				   const struct ethtool_tunable *, void *);
	int	(*set_phy_tunable)(struct net_device *,
				   const struct ethtool_tunable *, const void *);
	int	(*get_module_eeprom_by_page)(struct net_device *dev,
					     const struct ethtool_module_eeprom *page,
					     struct netlink_ext_ack *extack);
	void	(*get_eth_phy_stats)(struct net_device *dev,
				     struct ethtool_eth_phy_stats *phy_stats);
	void	(*get_eth_mac_stats)(struct net_device *dev,
				     struct ethtool_eth_mac_stats *mac_stats);
	void	(*get_eth_ctrl_stats)(struct net_device *dev,
				      struct ethtool_eth_ctrl_stats *ctrl_stats);
	void	(*get_rmon_stats)(struct net_device *dev,
				  struct ethtool_rmon_stats *rmon_stats,
				  const struct ethtool_rmon_hist_range **ranges);
	int	(*get_module_power_mode)(struct net_device *dev,
					 struct ethtool_module_power_mode_params *params,
					 struct netlink_ext_ack *extack);
	int	(*set_module_power_mode)(struct net_device *dev,
					 const struct ethtool_module_power_mode_params *params,
					 struct netlink_ext_ack *extack);
	int	(*get_mm)(struct net_device *dev, struct ethtool_mm_state *state);
	int	(*set_mm)(struct net_device *dev, struct ethtool_mm_cfg *cfg,
			  struct netlink_ext_ack *extack);
	void	(*get_mm_stats)(struct net_device *dev, struct ethtool_mm_stats *stats);
};

int ethtool_check_ops(const struct ethtool_ops *ops);

struct ethtool_rx_flow_rule {
	struct flow_rule	*rule;
	unsigned long		priv[];
};

struct ethtool_rx_flow_spec_input {
	const struct ethtool_rx_flow_spec	*fs;
	u32					rss_ctx;
};

struct ethtool_rx_flow_rule *
ethtool_rx_flow_rule_create(const struct ethtool_rx_flow_spec_input *input);
void ethtool_rx_flow_rule_destroy(struct ethtool_rx_flow_rule *rule);

bool ethtool_virtdev_validate_cmd(const struct ethtool_link_ksettings *cmd);
int ethtool_virtdev_set_link_ksettings(struct net_device *dev,
				       const struct ethtool_link_ksettings *cmd,
				       u32 *dev_speed, u8 *dev_duplex);

struct phy_device;
struct phy_tdr_config;
struct phy_plca_cfg;
struct phy_plca_status;

/**
 * struct ethtool_phy_ops - Optional PHY device options
 * @get_sset_count: Get number of strings that @get_strings will write.
 * @get_strings: Return a set of strings that describe the requested objects
 * @get_stats: Return extended statistics about the PHY device.
 * @get_plca_cfg: Return PLCA configuration.
 * @set_plca_cfg: Set PLCA configuration.
 * @get_plca_status: Get PLCA configuration.
 * @start_cable_test: Start a cable test
 * @start_cable_test_tdr: Start a Time Domain Reflectometry cable test
 *
 * All operations are optional (i.e. the function pointer may be set to %NULL)
 * and callers must take this into account. Callers must hold the RTNL lock.
 */
struct ethtool_phy_ops {
	int (*get_sset_count)(struct phy_device *dev);
	int (*get_strings)(struct phy_device *dev, u8 *data);
	int (*get_stats)(struct phy_device *dev,
			 struct ethtool_stats *stats, u64 *data);
	int (*get_plca_cfg)(struct phy_device *dev,
			    struct phy_plca_cfg *plca_cfg);
	int (*set_plca_cfg)(struct phy_device *dev,
			    const struct phy_plca_cfg *plca_cfg,
			    struct netlink_ext_ack *extack);
	int (*get_plca_status)(struct phy_device *dev,
			       struct phy_plca_status *plca_st);
	int (*start_cable_test)(struct phy_device *phydev,
				struct netlink_ext_ack *extack);
	int (*start_cable_test_tdr)(struct phy_device *phydev,
				    struct netlink_ext_ack *extack,
				    const struct phy_tdr_config *config);
};

/**
 * ethtool_set_ethtool_phy_ops - Set the ethtool_phy_ops singleton
 * @ops: Ethtool PHY operations to set
 */
void ethtool_set_ethtool_phy_ops(const struct ethtool_phy_ops *ops);

/**
 * ethtool_params_from_link_mode - Derive link parameters from a given link mode
 * @link_ksettings: Link parameters to be derived from the link mode
 * @link_mode: Link mode
 */
void
ethtool_params_from_link_mode(struct ethtool_link_ksettings *link_ksettings,
			      enum ethtool_link_mode_bit_indices link_mode);

/**
 * ethtool_get_phc_vclocks - Derive phc vclocks information, and caller
 *                           is responsible to free memory of vclock_index
 * @dev: pointer to net_device structure
 * @vclock_index: pointer to pointer of vclock index
 *
 * Return number of phc vclocks
 */
int ethtool_get_phc_vclocks(struct net_device *dev, int **vclock_index);

/* Some generic methods drivers may use in their ethtool_ops */
u32 ethtool_op_get_link(struct net_device *dev);
int ethtool_op_get_ts_info(struct net_device *dev, struct ethtool_ts_info *eti);

/**
 * ethtool_mm_frag_size_add_to_min - Translate (standard) additional fragment
 *	size expressed as multiplier into (absolute) minimum fragment size
 *	value expressed in octets
 * @val_add: Value of addFragSize multiplier
 */
static inline u32 ethtool_mm_frag_size_add_to_min(u32 val_add)
{
	return (ETH_ZLEN + ETH_FCS_LEN) * (1 + val_add) - ETH_FCS_LEN;
}

/**
 * ethtool_mm_frag_size_min_to_add - Translate (absolute) minimum fragment size
 *	expressed in octets into (standard) additional fragment size expressed
 *	as multiplier
 * @val_min: Value of addFragSize variable in octets
 * @val_add: Pointer where the standard addFragSize value is to be returned
 * @extack: Netlink extended ack
 *
 * Translate a value in octets to one of 0, 1, 2, 3 according to the reverse
 * application of the 802.3 formula 64 * (1 + addFragSize) - 4. To be called
 * by drivers which do not support programming the minimum fragment size to a
 * continuous range. Returns error on other fragment length values.
 */
static inline int ethtool_mm_frag_size_min_to_add(u32 val_min, u32 *val_add,
						  struct netlink_ext_ack *extack)
{
	u32 add_frag_size;

	for (add_frag_size = 0; add_frag_size < 4; add_frag_size++) {
		if (ethtool_mm_frag_size_add_to_min(add_frag_size) == val_min) {
			*val_add = add_frag_size;
			return 0;
		}
	}

	NL_SET_ERR_MSG_MOD(extack,
			   "minFragSize required to be one of 60, 124, 188 or 252");
	return -EINVAL;
}

/**
 * ethtool_get_ts_info_by_layer - Obtains time stamping capabilities from the MAC or PHY layer.
 * @dev: pointer to net_device structure
 * @info: buffer to hold the result
 * Returns zero on success, non-zero otherwise.
 */
int ethtool_get_ts_info_by_layer(struct net_device *dev, struct ethtool_ts_info *info);

/**
 * ethtool_sprintf - Write formatted string to ethtool string data
 * @data: Pointer to a pointer to the start of string to update
 * @fmt: Format of string to write
 *
 * Write formatted string to *data. Update *data to point at start of
 * next string.
 */
extern __printf(2, 3) void ethtool_sprintf(u8 **data, const char *fmt, ...);

/**
 * ethtool_puts - Write string to ethtool string data
 * @data: Pointer to a pointer to the start of string to update
 * @str: String to write
 *
 * Write string to *data without a trailing newline. Update *data
 * to point at start of next string.
 *
 * Prefer this function to ethtool_sprintf() when given only
 * two arguments or if @fmt is just "%s".
 */
extern void ethtool_puts(u8 **data, const char *str);

/* Link mode to forced speed capabilities maps */
struct ethtool_forced_speed_map {
	u32		speed;
	__ETHTOOL_DECLARE_LINK_MODE_MASK(caps);

	const u32	*cap_arr;
	u32		arr_size;
};

#define ETHTOOL_FORCED_SPEED_MAP(prefix, value)				\
{									\
	.speed		= SPEED_##value,				\
	.cap_arr	= prefix##_##value,				\
	.arr_size	= ARRAY_SIZE(prefix##_##value),			\
}

void
ethtool_forced_speed_maps_init(struct ethtool_forced_speed_map *maps, u32 size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _LINUX_ETHTOOL_H */
