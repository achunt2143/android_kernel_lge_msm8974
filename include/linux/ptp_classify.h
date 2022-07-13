<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * PTP 1588 support
 *
 * This file implements a BPF that recognizes PTP event messages.
 *
 * Copyright (C) 2010 OMICRON electronics GmbH
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _PTP_CLASSIFY_H_
#define _PTP_CLASSIFY_H_

<<<<<<< HEAD
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/ip.h>
#include <linux/filter.h>
#ifdef __KERNEL__
#include <linux/in.h>
#else
#include <netinet/in.h>
#endif
=======
#include <asm/unaligned.h>
#include <linux/ip.h>
#include <linux/ktime.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
#include <net/checksum.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define PTP_CLASS_NONE  0x00 /* not a PTP event message */
#define PTP_CLASS_V1    0x01 /* protocol version 1 */
#define PTP_CLASS_V2    0x02 /* protocol version 2 */
#define PTP_CLASS_VMASK 0x0f /* max protocol version is 15 */
#define PTP_CLASS_IPV4  0x10 /* event in an IPV4 UDP packet */
#define PTP_CLASS_IPV6  0x20 /* event in an IPV6 UDP packet */
<<<<<<< HEAD
#define PTP_CLASS_L2    0x30 /* event in a L2 packet */
#define PTP_CLASS_VLAN  0x40 /* event in a VLAN tagged L2 packet */
#define PTP_CLASS_PMASK 0xf0 /* mask for the packet type field */

#define PTP_CLASS_V1_IPV4 (PTP_CLASS_V1 | PTP_CLASS_IPV4)
#define PTP_CLASS_V1_IPV6 (PTP_CLASS_V1 | PTP_CLASS_IPV6) /*probably DNE*/
=======
#define PTP_CLASS_L2    0x40 /* event in a L2 packet */
#define PTP_CLASS_PMASK	0x70 /* mask for the packet type field */
#define PTP_CLASS_VLAN	0x80 /* event in a VLAN tagged packet */

#define PTP_CLASS_V1_IPV4 (PTP_CLASS_V1 | PTP_CLASS_IPV4)
#define PTP_CLASS_V1_IPV6 (PTP_CLASS_V1 | PTP_CLASS_IPV6) /* probably DNE */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define PTP_CLASS_V2_IPV4 (PTP_CLASS_V2 | PTP_CLASS_IPV4)
#define PTP_CLASS_V2_IPV6 (PTP_CLASS_V2 | PTP_CLASS_IPV6)
#define PTP_CLASS_V2_L2   (PTP_CLASS_V2 | PTP_CLASS_L2)
#define PTP_CLASS_V2_VLAN (PTP_CLASS_V2 | PTP_CLASS_VLAN)
<<<<<<< HEAD

#define PTP_EV_PORT 319
#define PTP_GEN_BIT 0x08 /* indicates general message, if set in message type */

#define OFF_ETYPE	12
#define OFF_IHL		14
#define OFF_FRAG	20
#define OFF_PROTO4	23
#define OFF_NEXT	6
#define OFF_UDP_DST	2

#define OFF_PTP_SOURCE_UUID	22 /* PTPv1 only */
#define OFF_PTP_SEQUENCE_ID	30
#define OFF_PTP_CONTROL		32 /* PTPv1 only */

#define IPV4_HLEN(data) (((struct iphdr *)(data + OFF_IHL))->ihl << 2)

#define IP6_HLEN	40
#define UDP_HLEN	8

#define RELOFF_DST4	(ETH_HLEN + OFF_UDP_DST)
#define OFF_DST6	(ETH_HLEN + IP6_HLEN + OFF_UDP_DST)
#define OFF_PTP6	(ETH_HLEN + IP6_HLEN + UDP_HLEN)

#define OP_AND	(BPF_ALU | BPF_AND  | BPF_K)
#define OP_JEQ	(BPF_JMP | BPF_JEQ  | BPF_K)
#define OP_JSET	(BPF_JMP | BPF_JSET | BPF_K)
#define OP_LDB	(BPF_LD  | BPF_B    | BPF_ABS)
#define OP_LDH	(BPF_LD  | BPF_H    | BPF_ABS)
#define OP_LDHI	(BPF_LD  | BPF_H    | BPF_IND)
#define OP_LDX	(BPF_LDX | BPF_B    | BPF_MSH)
#define OP_OR	(BPF_ALU | BPF_OR   | BPF_K)
#define OP_RETA	(BPF_RET | BPF_A)
#define OP_RETK	(BPF_RET | BPF_K)

static inline int ptp_filter_init(struct sock_filter *f, int len)
{
	if (OP_LDH == f[0].code)
		return sk_chk_filter(f, len);
	else
		return 0;
}

#define PTP_FILTER \
	{OP_LDH,	0,   0, OFF_ETYPE		}, /*              */ \
	{OP_JEQ,	0,  12, ETH_P_IP		}, /* f goto L20   */ \
	{OP_LDB,	0,   0, OFF_PROTO4		}, /*              */ \
	{OP_JEQ,	0,   9, IPPROTO_UDP		}, /* f goto L10   */ \
	{OP_LDH,	0,   0, OFF_FRAG		}, /*              */ \
	{OP_JSET,	7,   0, 0x1fff			}, /* t goto L11   */ \
	{OP_LDX,	0,   0, OFF_IHL			}, /*              */ \
	{OP_LDHI,	0,   0, RELOFF_DST4		}, /*              */ \
	{OP_JEQ,	0,   4, PTP_EV_PORT		}, /* f goto L12   */ \
	{OP_LDHI,	0,   0, ETH_HLEN + UDP_HLEN	}, /*              */ \
	{OP_AND,	0,   0, PTP_CLASS_VMASK		}, /*              */ \
	{OP_OR,		0,   0, PTP_CLASS_IPV4		}, /*              */ \
	{OP_RETA,	0,   0, 0			}, /*              */ \
/*L1x*/	{OP_RETK,	0,   0, PTP_CLASS_NONE		}, /*              */ \
/*L20*/	{OP_JEQ,	0,   9, ETH_P_IPV6		}, /* f goto L40   */ \
	{OP_LDB,	0,   0, ETH_HLEN + OFF_NEXT	}, /*              */ \
	{OP_JEQ,	0,   6, IPPROTO_UDP		}, /* f goto L30   */ \
	{OP_LDH,	0,   0, OFF_DST6		}, /*              */ \
	{OP_JEQ,	0,   4, PTP_EV_PORT		}, /* f goto L31   */ \
	{OP_LDH,	0,   0, OFF_PTP6		}, /*              */ \
	{OP_AND,	0,   0, PTP_CLASS_VMASK		}, /*              */ \
	{OP_OR,		0,   0, PTP_CLASS_IPV6		}, /*              */ \
	{OP_RETA,	0,   0, 0			}, /*              */ \
/*L3x*/	{OP_RETK,	0,   0, PTP_CLASS_NONE		}, /*              */ \
/*L40*/	{OP_JEQ,	0,   9, ETH_P_8021Q		}, /* f goto L50   */ \
	{OP_LDH,	0,   0, OFF_ETYPE + 4		}, /*              */ \
	{OP_JEQ,	0,  15, ETH_P_1588		}, /* f goto L60   */ \
	{OP_LDB,	0,   0, ETH_HLEN + VLAN_HLEN	}, /*              */ \
	{OP_AND,	0,   0, PTP_GEN_BIT		}, /*              */ \
	{OP_JEQ,	0,  12, 0			}, /* f goto L6x   */ \
	{OP_LDH,	0,   0, ETH_HLEN + VLAN_HLEN	}, /*              */ \
	{OP_AND,	0,   0, PTP_CLASS_VMASK		}, /*              */ \
	{OP_OR,		0,   0, PTP_CLASS_VLAN		}, /*              */ \
	{OP_RETA,	0,   0, 0			}, /*              */ \
/*L50*/	{OP_JEQ,	0,   7, ETH_P_1588		}, /* f goto L61   */ \
	{OP_LDB,	0,   0, ETH_HLEN		}, /*              */ \
	{OP_AND,	0,   0, PTP_GEN_BIT		}, /*              */ \
	{OP_JEQ,	0,   4, 0			}, /* f goto L6x   */ \
	{OP_LDH,	0,   0, ETH_HLEN		}, /*              */ \
	{OP_AND,	0,   0, PTP_CLASS_VMASK		}, /*              */ \
	{OP_OR,		0,   0, PTP_CLASS_L2		}, /*              */ \
	{OP_RETA,	0,   0, 0			}, /*              */ \
/*L6x*/	{OP_RETK,	0,   0, PTP_CLASS_NONE		},

#endif
=======
#define PTP_CLASS_L4      (PTP_CLASS_IPV4 | PTP_CLASS_IPV6)

#define PTP_MSGTYPE_SYNC        0x0
#define PTP_MSGTYPE_DELAY_REQ   0x1
#define PTP_MSGTYPE_PDELAY_REQ  0x2
#define PTP_MSGTYPE_PDELAY_RESP 0x3

#define PTP_EV_PORT 319
#define PTP_GEN_PORT 320
#define PTP_GEN_BIT 0x08 /* indicates general message, if set in message type */

#define OFF_PTP_SOURCE_UUID	22 /* PTPv1 only */
#define OFF_PTP_SEQUENCE_ID	30

/* PTP header flag fields */
#define PTP_FLAG_TWOSTEP	BIT(1)

/* Below defines should actually be removed at some point in time. */
#define IP6_HLEN	40
#define UDP_HLEN	8
#define OFF_IHL		14
#define IPV4_HLEN(data) (((struct iphdr *)(data + OFF_IHL))->ihl << 2)

struct clock_identity {
	u8 id[8];
} __packed;

struct port_identity {
	struct clock_identity	clock_identity;
	__be16			port_number;
} __packed;

struct ptp_header {
	u8			tsmt;  /* transportSpecific | messageType */
	u8			ver;   /* reserved          | versionPTP  */
	__be16			message_length;
	u8			domain_number;
	u8			reserved1;
	u8			flag_field[2];
	__be64			correction;
	__be32			reserved2;
	struct port_identity	source_port_identity;
	__be16			sequence_id;
	u8			control;
	u8			log_message_interval;
} __packed;

#if defined(CONFIG_NET_PTP_CLASSIFY)
/**
 * ptp_classify_raw - classify a PTP packet
 * @skb: buffer
 *
 * Runs a minimal BPF dissector to classify a network packet to
 * determine the PTP class. In case the skb does not contain any
 * PTP protocol data, PTP_CLASS_NONE will be returned, otherwise
 * PTP_CLASS_V1_IPV{4,6}, PTP_CLASS_V2_IPV{4,6} or
 * PTP_CLASS_V2_{L2,VLAN}, depending on the packet content.
 */
unsigned int ptp_classify_raw(const struct sk_buff *skb);

/**
 * ptp_parse_header - Get pointer to the PTP v2 header
 * @skb: packet buffer
 * @type: type of the packet (see ptp_classify_raw())
 *
 * This function takes care of the VLAN, UDP, IPv4 and IPv6 headers. The length
 * is checked.
 *
 * Note, internally skb_mac_header() is used. Make sure that the @skb is
 * initialized accordingly.
 *
 * Return: Pointer to the ptp v2 header or NULL if not found
 */
struct ptp_header *ptp_parse_header(struct sk_buff *skb, unsigned int type);

/**
 * ptp_get_msgtype - Extract ptp message type from given header
 * @hdr: ptp header
 * @type: type of the packet (see ptp_classify_raw())
 *
 * This function returns the message type for a given ptp header. It takes care
 * of the different ptp header versions (v1 or v2).
 *
 * Return: The message type
 */
static inline u8 ptp_get_msgtype(const struct ptp_header *hdr,
				 unsigned int type)
{
	u8 msgtype;

	if (unlikely(type & PTP_CLASS_V1)) {
		/* msg type is located at the control field for ptp v1 */
		msgtype = hdr->control;
	} else {
		msgtype = hdr->tsmt & 0x0f;
	}

	return msgtype;
}

/**
 * ptp_check_diff8 - Computes new checksum (when altering a 64-bit field)
 * @old: old field value
 * @new: new field value
 * @oldsum: previous checksum
 *
 * This function can be used to calculate a new checksum when only a single
 * field is changed. Similar as ip_vs_check_diff*() in ip_vs.h.
 *
 * Return: Updated checksum
 */
static inline __wsum ptp_check_diff8(__be64 old, __be64 new, __wsum oldsum)
{
	__be64 diff[2] = { ~old, new };

	return csum_partial(diff, sizeof(diff), oldsum);
}

/**
 * ptp_header_update_correction - Update PTP header's correction field
 * @skb: packet buffer
 * @type: type of the packet (see ptp_classify_raw())
 * @hdr: ptp header
 * @correction: new correction value
 *
 * This updates the correction field of a PTP header and updates the UDP
 * checksum (if UDP is used as transport). It is needed for hardware capable of
 * one-step P2P that does not already modify the correction field of Pdelay_Req
 * event messages on ingress.
 */
static inline
void ptp_header_update_correction(struct sk_buff *skb, unsigned int type,
				  struct ptp_header *hdr, s64 correction)
{
	__be64 correction_old;
	struct udphdr *uhdr;

	/* previous correction value is required for checksum update. */
	memcpy(&correction_old,  &hdr->correction, sizeof(correction_old));

	/* write new correction value */
	put_unaligned_be64((u64)correction, &hdr->correction);

	switch (type & PTP_CLASS_PMASK) {
	case PTP_CLASS_IPV4:
	case PTP_CLASS_IPV6:
		/* locate udp header */
		uhdr = (struct udphdr *)((char *)hdr - sizeof(struct udphdr));
		break;
	default:
		return;
	}

	/* update checksum */
	uhdr->check = csum_fold(ptp_check_diff8(correction_old,
						hdr->correction,
						~csum_unfold(uhdr->check)));
	if (!uhdr->check)
		uhdr->check = CSUM_MANGLED_0;

	skb->ip_summed = CHECKSUM_NONE;
}

/**
 * ptp_msg_is_sync - Evaluates whether the given skb is a PTP Sync message
 * @skb: packet buffer
 * @type: type of the packet (see ptp_classify_raw())
 *
 * This function evaluates whether the given skb is a PTP Sync message.
 *
 * Return: true if sync message, false otherwise
 */
bool ptp_msg_is_sync(struct sk_buff *skb, unsigned int type);

void __init ptp_classifier_init(void);
#else
static inline void ptp_classifier_init(void)
{
}
static inline unsigned int ptp_classify_raw(struct sk_buff *skb)
{
	return PTP_CLASS_NONE;
}
static inline struct ptp_header *ptp_parse_header(struct sk_buff *skb,
						  unsigned int type)
{
	return NULL;
}
static inline u8 ptp_get_msgtype(const struct ptp_header *hdr,
				 unsigned int type)
{
	/* The return is meaningless. The stub function would not be
	 * executed since no available header from ptp_parse_header.
	 */
	return PTP_MSGTYPE_SYNC;
}
static inline bool ptp_msg_is_sync(struct sk_buff *skb, unsigned int type)
{
	return false;
}

static inline
void ptp_header_update_correction(struct sk_buff *skb, unsigned int type,
				  struct ptp_header *hdr, s64 correction)
{
}
#endif
#endif /* _PTP_CLASSIFY_H_ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
