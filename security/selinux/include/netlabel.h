<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * SELinux interface to the NetLabel subsystem
 *
 * Author: Paul Moore <paul@paul-moore.com>
<<<<<<< HEAD
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 * (c) Copyright Hewlett-Packard Development Company, L.P., 2006
<<<<<<< HEAD
 *
 * This program is free software;  you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;  without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program;  if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _SELINUX_NETLABEL_H_
#define _SELINUX_NETLABEL_H_

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/net.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include <net/request_sock.h>
<<<<<<< HEAD
=======
#include <net/sctp/structs.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "avc.h"
#include "objsec.h"

#ifdef CONFIG_NETLABEL
void selinux_netlbl_cache_invalidate(void);

<<<<<<< HEAD
void selinux_netlbl_err(struct sk_buff *skb, int error, int gateway);
=======
void selinux_netlbl_err(struct sk_buff *skb, u16 family, int error,
			int gateway);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void selinux_netlbl_sk_security_free(struct sk_security_struct *sksec);
void selinux_netlbl_sk_security_reset(struct sk_security_struct *sksec);

<<<<<<< HEAD
int selinux_netlbl_skbuff_getsid(struct sk_buff *skb,
				 u16 family,
				 u32 *type,
				 u32 *sid);
int selinux_netlbl_skbuff_setsid(struct sk_buff *skb,
				 u16 family,
				 u32 sid);

int selinux_netlbl_inet_conn_request(struct request_sock *req, u16 family);
void selinux_netlbl_inet_csk_clone(struct sock *sk, u16 family);
int selinux_netlbl_socket_post_create(struct sock *sk, u16 family);
int selinux_netlbl_sock_rcv_skb(struct sk_security_struct *sksec,
				struct sk_buff *skb,
				u16 family,
				struct common_audit_data *ad);
int selinux_netlbl_socket_setsockopt(struct socket *sock,
				     int level,
				     int optname);
int selinux_netlbl_socket_connect(struct sock *sk, struct sockaddr *addr);
=======
int selinux_netlbl_skbuff_getsid(struct sk_buff *skb, u16 family, u32 *type,
				 u32 *sid);
int selinux_netlbl_skbuff_setsid(struct sk_buff *skb, u16 family, u32 sid);
int selinux_netlbl_sctp_assoc_request(struct sctp_association *asoc,
				      struct sk_buff *skb);
int selinux_netlbl_inet_conn_request(struct request_sock *req, u16 family);
void selinux_netlbl_inet_csk_clone(struct sock *sk, u16 family);
void selinux_netlbl_sctp_sk_clone(struct sock *sk, struct sock *newsk);
int selinux_netlbl_socket_post_create(struct sock *sk, u16 family);
int selinux_netlbl_sock_rcv_skb(struct sk_security_struct *sksec,
				struct sk_buff *skb, u16 family,
				struct common_audit_data *ad);
int selinux_netlbl_socket_setsockopt(struct socket *sock, int level,
				     int optname);
int selinux_netlbl_socket_connect(struct sock *sk, struct sockaddr *addr);
int selinux_netlbl_socket_connect_locked(struct sock *sk,
					 struct sockaddr *addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#else
static inline void selinux_netlbl_cache_invalidate(void)
{
	return;
}

<<<<<<< HEAD
static inline void selinux_netlbl_err(struct sk_buff *skb,
				      int error,
				      int gateway)
=======
static inline void selinux_netlbl_err(struct sk_buff *skb, u16 family,
				      int error, int gateway)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return;
}

<<<<<<< HEAD
static inline void selinux_netlbl_sk_security_free(
					       struct sk_security_struct *sksec)
=======
static inline void
selinux_netlbl_sk_security_free(struct sk_security_struct *sksec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return;
}

<<<<<<< HEAD
static inline void selinux_netlbl_sk_security_reset(
					       struct sk_security_struct *sksec)
=======
static inline void
selinux_netlbl_sk_security_reset(struct sk_security_struct *sksec)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return;
}

<<<<<<< HEAD
static inline int selinux_netlbl_skbuff_getsid(struct sk_buff *skb,
					       u16 family,
					       u32 *type,
					       u32 *sid)
=======
static inline int selinux_netlbl_skbuff_getsid(struct sk_buff *skb, u16 family,
					       u32 *type, u32 *sid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	*type = NETLBL_NLTYPE_NONE;
	*sid = SECSID_NULL;
	return 0;
}
<<<<<<< HEAD
static inline int selinux_netlbl_skbuff_setsid(struct sk_buff *skb,
					       u16 family,
=======
static inline int selinux_netlbl_skbuff_setsid(struct sk_buff *skb, u16 family,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					       u32 sid)
{
	return 0;
}

<<<<<<< HEAD
static inline int selinux_netlbl_conn_setsid(struct sock *sk,
					     struct sockaddr *addr)
{
	return 0;
}

=======
static inline int
selinux_netlbl_sctp_assoc_request(struct sctp_association *asoc,
				  struct sk_buff *skb)
{
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int selinux_netlbl_inet_conn_request(struct request_sock *req,
						   u16 family)
{
	return 0;
}
static inline void selinux_netlbl_inet_csk_clone(struct sock *sk, u16 family)
{
	return;
}
<<<<<<< HEAD
static inline int selinux_netlbl_socket_post_create(struct sock *sk,
						    u16 family)
=======
static inline void selinux_netlbl_sctp_sk_clone(struct sock *sk,
						struct sock *newsk)
{
	return;
}
static inline int selinux_netlbl_socket_post_create(struct sock *sk, u16 family)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}
static inline int selinux_netlbl_sock_rcv_skb(struct sk_security_struct *sksec,
<<<<<<< HEAD
					      struct sk_buff *skb,
					      u16 family,
=======
					      struct sk_buff *skb, u16 family,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					      struct common_audit_data *ad)
{
	return 0;
}
static inline int selinux_netlbl_socket_setsockopt(struct socket *sock,
<<<<<<< HEAD
						   int level,
						   int optname)
=======
						   int level, int optname)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}
static inline int selinux_netlbl_socket_connect(struct sock *sk,
						struct sockaddr *addr)
{
	return 0;
}
<<<<<<< HEAD
=======
static inline int selinux_netlbl_socket_connect_locked(struct sock *sk,
						       struct sockaddr *addr)
{
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_NETLABEL */

#endif
