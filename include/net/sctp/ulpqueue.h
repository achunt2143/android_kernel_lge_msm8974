<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* SCTP kernel implementation
 * (C) Copyright IBM Corp. 2001, 2004
 * Copyright (c) 1999-2000 Cisco, Inc.
 * Copyright (c) 1999-2001 Motorola, Inc.
 * Copyright (c) 2001 Intel Corp.
 * Copyright (c) 2001 Nokia, Inc.
 * Copyright (c) 2001 La Monte H.P. Yarroll
 *
 * These are the definitions needed for the sctp_ulpq type.  The
 * sctp_ulpq is the interface between the Upper Layer Protocol, or ULP,
 * and the core SCTP state machine.  This is the component which handles
 * reassembly and ordering.
 *
<<<<<<< HEAD
 * This SCTP implementation is free software;
 * you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This SCTP implementation  is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 *                 ************************
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU CC; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Please send any bug reports or fixes you make to the
 * email addresses:
 *    lksctp developers <lksctp-developers@lists.sourceforge.net>
 *
 * Or submit a bug report through the following website:
 *    http://www.sf.net/projects/lksctp
=======
 * Please send any bug reports or fixes you make to the
 * email addresses:
 *    lksctp developers <linux-sctp@vger.kernel.org>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Written or modified by:
 *   Jon Grimm             <jgrimm@us.ibm.com>
 *   La Monte H.P. Yarroll <piggy@acm.org>
 *   Sridhar Samudrala     <sri@us.ibm.com>
<<<<<<< HEAD
 *
 * Any bugs reported given to us we will try to fix... any fixes shared will
 * be incorporated into the next SCTP release.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __sctp_ulpqueue_h__
#define __sctp_ulpqueue_h__

/* A structure to carry information to the ULP (e.g. Sockets API) */
struct sctp_ulpq {
<<<<<<< HEAD
	char malloced;
	char pd_mode;
	struct sctp_association *asoc;
	struct sk_buff_head reasm;
=======
	char pd_mode;
	struct sctp_association *asoc;
	struct sk_buff_head reasm;
	struct sk_buff_head reasm_uo;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sk_buff_head lobby;
};

/* Prototypes. */
<<<<<<< HEAD
struct sctp_ulpq *sctp_ulpq_init(struct sctp_ulpq *,
				 struct sctp_association *);
=======
void sctp_ulpq_init(struct sctp_ulpq *ulpq, struct sctp_association *asoc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void sctp_ulpq_flush(struct sctp_ulpq *ulpq);
void sctp_ulpq_free(struct sctp_ulpq *);

/* Add a new DATA chunk for processing. */
int sctp_ulpq_tail_data(struct sctp_ulpq *, struct sctp_chunk *, gfp_t);

/* Add a new event for propagation to the ULP. */
<<<<<<< HEAD
int sctp_ulpq_tail_event(struct sctp_ulpq *, struct sctp_ulpevent *ev);
=======
int sctp_ulpq_tail_event(struct sctp_ulpq *, struct sk_buff_head *skb_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Renege previously received chunks.  */
void sctp_ulpq_renege(struct sctp_ulpq *, struct sctp_chunk *, gfp_t);

/* Perform partial delivery. */
<<<<<<< HEAD
void sctp_ulpq_partial_delivery(struct sctp_ulpq *, struct sctp_chunk *, gfp_t);
=======
void sctp_ulpq_partial_delivery(struct sctp_ulpq *, gfp_t);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Abort the partial delivery. */
void sctp_ulpq_abort_pd(struct sctp_ulpq *, gfp_t);

/* Clear the partial data delivery condition on this socket. */
int sctp_clear_pd(struct sock *sk, struct sctp_association *asoc);

/* Skip over an SSN. */
void sctp_ulpq_skip(struct sctp_ulpq *ulpq, __u16 sid, __u16 ssn);

void sctp_ulpq_reasm_flushtsn(struct sctp_ulpq *, __u32);
<<<<<<< HEAD
#endif /* __sctp_ulpqueue_h__ */







=======

__u16 sctp_ulpq_renege_list(struct sctp_ulpq *ulpq,
			    struct sk_buff_head *list, __u16 needed);

#endif /* __sctp_ulpqueue_h__ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
