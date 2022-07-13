<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	PF_INET6 socket protocol family
 *	Linux INET6 implementation
 *
 *	Authors:
 *	Pedro Roque		<roque@di.fc.ul.pt>
 *
 *	Adapted from linux/net/ipv4/af_inet.c
 *
<<<<<<< HEAD
 * 	Fixes:
 *	piggy, Karl Knutson	:	Socket protocol table
 * 	Hideaki YOSHIFUJI	:	sin6_scope_id support
 * 	Arnaldo Melo		: 	check proc_net_create return, cleanups
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */

=======
 *	Fixes:
 *	piggy, Karl Knutson	:	Socket protocol table
 *	Hideaki YOSHIFUJI	:	sin6_scope_id support
 *	Arnaldo Melo		:	check proc_net_create return, cleanups
 */

#define pr_fmt(fmt) "IPv6: " fmt
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/module.h>
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/slab.h>

#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/icmpv6.h>
#include <linux/netfilter_ipv6.h>

#include <net/ip.h>
#include <net/ipv6.h>
#include <net/udp.h>
#include <net/udplite.h>
#include <net/tcp.h>
<<<<<<< HEAD
#include <net/ipip.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/ping.h>
#include <net/protocol.h>
#include <net/inet_common.h>
#include <net/route.h>
#include <net/transp_v6.h>
#include <net/ip6_route.h>
#include <net/addrconf.h>
<<<<<<< HEAD
#ifdef CONFIG_IPV6_TUNNEL
#include <net/ip6_tunnel.h>
#endif

#include <asm/uaccess.h>
#include <linux/mroute6.h>

#ifdef CONFIG_ANDROID_PARANOID_NETWORK
#include <linux/android_aid.h>

static inline int current_has_network(void)
{
	return in_egroup_p(AID_INET) || capable(CAP_NET_RAW);
}
#else
static inline int current_has_network(void)
{
	return 1;
}
#endif
=======
#include <net/ipv6_stubs.h>
#include <net/ndisc.h>
#ifdef CONFIG_IPV6_TUNNEL
#include <net/ip6_tunnel.h>
#endif
#include <net/calipso.h>
#include <net/seg6.h>
#include <net/rpl.h>
#include <net/compat.h>
#include <net/xfrm.h>
#include <net/ioam6.h>
#include <net/rawv6.h>
#include <net/rps.h>

#include <linux/uaccess.h>
#include <linux/mroute6.h>

#include "ip6_offload.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Cast of dozens");
MODULE_DESCRIPTION("IPv6 protocol stack for Linux");
MODULE_LICENSE("GPL");

/* The inetsw6 table contains everything that inet6_create needs to
 * build a new socket.
 */
static struct list_head inetsw6[SOCK_MAX];
static DEFINE_SPINLOCK(inetsw6_lock);

struct ipv6_params ipv6_defaults = {
	.disable_ipv6 = 0,
	.autoconf = 1,
};

<<<<<<< HEAD
static int disable_ipv6_mod = 0;
=======
static int disable_ipv6_mod;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

module_param_named(disable, disable_ipv6_mod, int, 0444);
MODULE_PARM_DESC(disable, "Disable IPv6 module such that it is non-functional");

module_param_named(disable_ipv6, ipv6_defaults.disable_ipv6, int, 0444);
MODULE_PARM_DESC(disable_ipv6, "Disable IPv6 on all interfaces");

module_param_named(autoconf, ipv6_defaults.autoconf, int, 0444);
MODULE_PARM_DESC(autoconf, "Enable IPv6 address autoconfiguration on all interfaces");

<<<<<<< HEAD
static __inline__ struct ipv6_pinfo *inet6_sk_generic(struct sock *sk)
{
	const int offset = sk->sk_prot->obj_size - sizeof(struct ipv6_pinfo);
=======
bool ipv6_mod_enabled(void)
{
	return disable_ipv6_mod == 0;
}
EXPORT_SYMBOL_GPL(ipv6_mod_enabled);

static struct ipv6_pinfo *inet6_sk_generic(struct sock *sk)
{
	const int offset = sk->sk_prot->ipv6_pinfo_offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return (struct ipv6_pinfo *)(((u8 *)sk) + offset);
}

<<<<<<< HEAD
=======
void inet6_sock_destruct(struct sock *sk)
{
	inet6_cleanup_sock(sk);
	inet_sock_destruct(sk);
}
EXPORT_SYMBOL_GPL(inet6_sock_destruct);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int inet6_create(struct net *net, struct socket *sock, int protocol,
			int kern)
{
	struct inet_sock *inet;
	struct ipv6_pinfo *np;
	struct sock *sk;
	struct inet_protosw *answer;
	struct proto *answer_prot;
	unsigned char answer_flags;
<<<<<<< HEAD
	char answer_no_check;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int try_loading_module = 0;
	int err;

	if (protocol < 0 || protocol >= IPPROTO_MAX)
		return -EINVAL;

<<<<<<< HEAD
	if (!current_has_network())
		return -EACCES;

	if (sock->type != SOCK_RAW &&
	    sock->type != SOCK_DGRAM &&
	    !inet_ehash_secret)
		build_ehash_secret();

	if (protocol < 0 || protocol >= IPPROTO_MAX)
		return -EINVAL;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Look for the requested type/protocol pair. */
lookup_protocol:
	err = -ESOCKTNOSUPPORT;
	rcu_read_lock();
	list_for_each_entry_rcu(answer, &inetsw6[sock->type], list) {

		err = 0;
		/* Check the non-wild match. */
		if (protocol == answer->protocol) {
			if (protocol != IPPROTO_IP)
				break;
		} else {
			/* Check for the two wild cases. */
			if (IPPROTO_IP == protocol) {
				protocol = answer->protocol;
				break;
			}
			if (IPPROTO_IP == answer->protocol)
				break;
		}
		err = -EPROTONOSUPPORT;
	}

	if (err) {
		if (try_loading_module < 2) {
			rcu_read_unlock();
			/*
			 * Be more specific, e.g. net-pf-10-proto-132-type-1
			 * (net-pf-PF_INET6-proto-IPPROTO_SCTP-type-SOCK_STREAM)
			 */
			if (++try_loading_module == 1)
				request_module("net-pf-%d-proto-%d-type-%d",
						PF_INET6, protocol, sock->type);
			/*
			 * Fall back to generic, e.g. net-pf-10-proto-132
			 * (net-pf-PF_INET6-proto-IPPROTO_SCTP)
			 */
			else
				request_module("net-pf-%d-proto-%d",
						PF_INET6, protocol);
			goto lookup_protocol;
		} else
			goto out_rcu_unlock;
	}

	err = -EPERM;
<<<<<<< HEAD
	if (sock->type == SOCK_RAW && !kern && !capable(CAP_NET_RAW))
=======
	if (sock->type == SOCK_RAW && !kern &&
	    !ns_capable(net->user_ns, CAP_NET_RAW))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_rcu_unlock;

	sock->ops = answer->ops;
	answer_prot = answer->prot;
<<<<<<< HEAD
	answer_no_check = answer->no_check;
	answer_flags = answer->flags;
	rcu_read_unlock();

	WARN_ON(answer_prot->slab == NULL);

	err = -ENOBUFS;
	sk = sk_alloc(net, PF_INET6, GFP_KERNEL, answer_prot);
	if (sk == NULL)
=======
	answer_flags = answer->flags;
	rcu_read_unlock();

	WARN_ON(!answer_prot->slab);

	err = -ENOBUFS;
	sk = sk_alloc(net, PF_INET6, GFP_KERNEL, answer_prot, kern);
	if (!sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	sock_init_data(sock, sk);

	err = 0;
<<<<<<< HEAD
	sk->sk_no_check = answer_no_check;
	if (INET_PROTOSW_REUSE & answer_flags)
		sk->sk_reuse = 1;

	inet = inet_sk(sk);
	inet->is_icsk = (INET_PROTOSW_ICSK & answer_flags) != 0;
=======
	if (INET_PROTOSW_REUSE & answer_flags)
		sk->sk_reuse = SK_CAN_REUSE;

	if (INET_PROTOSW_ICSK & answer_flags)
		inet_init_csk_locks(sk);

	inet = inet_sk(sk);
	inet_assign_bit(IS_ICSK, sk, INET_PROTOSW_ICSK & answer_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (SOCK_RAW == sock->type) {
		inet->inet_num = protocol;
		if (IPPROTO_RAW == protocol)
<<<<<<< HEAD
			inet->hdrincl = 1;
	}

	sk->sk_destruct		= inet_sock_destruct;
=======
			inet_set_bit(HDRINCL, sk);
	}

	sk->sk_destruct		= inet6_sock_destruct;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sk->sk_family		= PF_INET6;
	sk->sk_protocol		= protocol;

	sk->sk_backlog_rcv	= answer->prot->backlog_rcv;

	inet_sk(sk)->pinet6 = np = inet6_sk_generic(sk);
	np->hop_limit	= -1;
	np->mcast_hops	= IPV6_DEFAULT_MCASTHOPS;
<<<<<<< HEAD
	np->mc_loop	= 1;
	np->pmtudisc	= IPV6_PMTUDISC_WANT;
	np->ipv6only	= net->ipv6.sysctl.bindv6only;
=======
	inet6_set_bit(MC6_LOOP, sk);
	inet6_set_bit(MC6_ALL, sk);
	np->pmtudisc	= IPV6_PMTUDISC_WANT;
	inet6_assign_bit(REPFLOW, sk, net->ipv6.sysctl.flowlabel_reflect &
				     FLOWLABEL_REFLECT_ESTABLISHED);
	sk->sk_ipv6only	= net->ipv6.sysctl.bindv6only;
	sk->sk_txrehash = READ_ONCE(net->core.sysctl_txrehash);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Init the ipv4 part of the socket since we can have sockets
	 * using v6 API for ipv4.
	 */
	inet->uc_ttl	= -1;

<<<<<<< HEAD
	inet->mc_loop	= 1;
	inet->mc_ttl	= 1;
	inet->mc_index	= 0;
	inet->mc_list	= NULL;
	inet->rcv_tos	= 0;

	if (ipv4_config.no_pmtu_disc)
		inet->pmtudisc = IP_PMTUDISC_DONT;
	else
		inet->pmtudisc = IP_PMTUDISC_WANT;
	/*
	 * Increment only the relevant sk_prot->socks debug field, this changes
	 * the previous behaviour of incrementing both the equivalent to
	 * answer->prot->socks (inet6_sock_nr) and inet_sock_nr.
	 *
	 * This allows better debug granularity as we'll know exactly how many
	 * UDPv6, TCPv6, etc socks were allocated, not the sum of all IPv6
	 * transport protocol socks. -acme
	 */
	sk_refcnt_debug_inc(sk);
=======
	inet_set_bit(MC_LOOP, sk);
	inet->mc_ttl	= 1;
	inet->mc_index	= 0;
	RCU_INIT_POINTER(inet->mc_list, NULL);
	inet->rcv_tos	= 0;

	if (READ_ONCE(net->ipv4.sysctl_ip_no_pmtu_disc))
		inet->pmtudisc = IP_PMTUDISC_DONT;
	else
		inet->pmtudisc = IP_PMTUDISC_WANT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (inet->inet_num) {
		/* It assumes that any protocol which allows
		 * the user to assign a number at socket
		 * creation time automatically shares.
		 */
		inet->inet_sport = htons(inet->inet_num);
<<<<<<< HEAD
		sk->sk_prot->hash(sk);
=======
		err = sk->sk_prot->hash(sk);
		if (err) {
			sk_common_release(sk);
			goto out;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (sk->sk_prot->init) {
		err = sk->sk_prot->init(sk);
		if (err) {
			sk_common_release(sk);
			goto out;
		}
	}
<<<<<<< HEAD
=======

	if (!kern) {
		err = BPF_CGROUP_RUN_PROG_INET_SOCK(sk);
		if (err) {
			sk_common_release(sk);
			goto out;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return err;
out_rcu_unlock:
	rcu_read_unlock();
	goto out;
}

<<<<<<< HEAD

/* bind for INET6 API */
int inet6_bind(struct socket *sock, struct sockaddr *uaddr, int addr_len)
{
	struct sockaddr_in6 *addr=(struct sockaddr_in6 *)uaddr;
	struct sock *sk = sock->sk;
=======
static int __inet6_bind(struct sock *sk, struct sockaddr *uaddr, int addr_len,
			u32 flags)
{
	struct sockaddr_in6 *addr = (struct sockaddr_in6 *)uaddr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct inet_sock *inet = inet_sk(sk);
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct net *net = sock_net(sk);
	__be32 v4addr = 0;
	unsigned short snum;
<<<<<<< HEAD
	int addr_type = 0;
	int err = 0;

	/* If the socket has its own bind function then use it. */
	if (sk->sk_prot->bind)
		return sk->sk_prot->bind(sk, uaddr, addr_len);

	if (addr_len < SIN6_LEN_RFC2133)
		return -EINVAL;

=======
	bool saved_ipv6only;
	int addr_type = 0;
	int err = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (addr->sin6_family != AF_INET6)
		return -EAFNOSUPPORT;

	addr_type = ipv6_addr_type(&addr->sin6_addr);
<<<<<<< HEAD
	if ((addr_type & IPV6_ADDR_MULTICAST) && sock->type == SOCK_STREAM)
		return -EINVAL;

	snum = ntohs(addr->sin6_port);
	if (snum && snum < PROT_SOCK && !capable(CAP_NET_BIND_SERVICE))
		return -EACCES;

	lock_sock(sk);
=======
	if ((addr_type & IPV6_ADDR_MULTICAST) && sk->sk_type == SOCK_STREAM)
		return -EINVAL;

	snum = ntohs(addr->sin6_port);
	if (!(flags & BIND_NO_CAP_NET_BIND_SERVICE) &&
	    snum && inet_port_requires_bind_service(net, snum) &&
	    !ns_capable(net->user_ns, CAP_NET_BIND_SERVICE))
		return -EACCES;

	if (flags & BIND_WITH_LOCK)
		lock_sock(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check these errors (active socket, double bind). */
	if (sk->sk_state != TCP_CLOSE || inet->inet_num) {
		err = -EINVAL;
		goto out;
	}

	/* Check if the address belongs to the host. */
	if (addr_type == IPV6_ADDR_MAPPED) {
<<<<<<< HEAD
=======
		struct net_device *dev = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		int chk_addr_ret;

		/* Binding to v4-mapped address on a v6-only socket
		 * makes no sense
		 */
<<<<<<< HEAD
		if (np->ipv6only) {
=======
		if (ipv6_only_sock(sk)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EINVAL;
			goto out;
		}

<<<<<<< HEAD
		/* Reproduce AF_INET checks to make the bindings consistent */
		v4addr = addr->sin6_addr.s6_addr32[3];
		chk_addr_ret = inet_addr_type(net, v4addr);
		if (!sysctl_ip_nonlocal_bind &&
		    !(inet->freebind || inet->transparent) &&
		    v4addr != htonl(INADDR_ANY) &&
		    chk_addr_ret != RTN_LOCAL &&
		    chk_addr_ret != RTN_MULTICAST &&
		    chk_addr_ret != RTN_BROADCAST) {
=======
		rcu_read_lock();
		if (sk->sk_bound_dev_if) {
			dev = dev_get_by_index_rcu(net, sk->sk_bound_dev_if);
			if (!dev) {
				err = -ENODEV;
				goto out_unlock;
			}
		}

		/* Reproduce AF_INET checks to make the bindings consistent */
		v4addr = addr->sin6_addr.s6_addr32[3];
		chk_addr_ret = inet_addr_type_dev_table(net, dev, v4addr);
		rcu_read_unlock();

		if (!inet_addr_valid_or_nonlocal(net, inet, v4addr,
						 chk_addr_ret)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EADDRNOTAVAIL;
			goto out;
		}
	} else {
		if (addr_type != IPV6_ADDR_ANY) {
			struct net_device *dev = NULL;

			rcu_read_lock();
<<<<<<< HEAD
			if (addr_type & IPV6_ADDR_LINKLOCAL) {
=======
			if (__ipv6_addr_needs_scope_id(addr_type)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				if (addr_len >= sizeof(struct sockaddr_in6) &&
				    addr->sin6_scope_id) {
					/* Override any existing binding, if another one
					 * is supplied by user.
					 */
					sk->sk_bound_dev_if = addr->sin6_scope_id;
				}

				/* Binding to link-local address requires an interface */
				if (!sk->sk_bound_dev_if) {
					err = -EINVAL;
					goto out_unlock;
				}
<<<<<<< HEAD
=======
			}

			if (sk->sk_bound_dev_if) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				dev = dev_get_by_index_rcu(net, sk->sk_bound_dev_if);
				if (!dev) {
					err = -ENODEV;
					goto out_unlock;
				}
			}

			/* ipv4 addr of the socket is invalid.  Only the
			 * unspecified and mapped address have a v4 equivalent.
			 */
			v4addr = LOOPBACK4_IPV6;
			if (!(addr_type & IPV6_ADDR_MULTICAST))	{
<<<<<<< HEAD
				if (!(inet->freebind || inet->transparent) &&
=======
				if (!ipv6_can_nonlocal_bind(net, inet) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    !ipv6_chk_addr(net, &addr->sin6_addr,
						   dev, 0)) {
					err = -EADDRNOTAVAIL;
					goto out_unlock;
				}
			}
			rcu_read_unlock();
		}
	}

	inet->inet_rcv_saddr = v4addr;
	inet->inet_saddr = v4addr;

<<<<<<< HEAD
	np->rcv_saddr = addr->sin6_addr;
=======
	sk->sk_v6_rcv_saddr = addr->sin6_addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!(addr_type & IPV6_ADDR_MULTICAST))
		np->saddr = addr->sin6_addr;

<<<<<<< HEAD
	/* Make sure we are allowed to bind here. */
	if (sk->sk_prot->get_port(sk, snum)) {
		inet_reset_saddr(sk);
		err = -EADDRINUSE;
		goto out;
	}

	if (addr_type != IPV6_ADDR_ANY) {
		sk->sk_userlocks |= SOCK_BINDADDR_LOCK;
		if (addr_type != IPV6_ADDR_MAPPED)
			np->ipv6only = 1;
	}
=======
	saved_ipv6only = sk->sk_ipv6only;
	if (addr_type != IPV6_ADDR_ANY && addr_type != IPV6_ADDR_MAPPED)
		sk->sk_ipv6only = 1;

	/* Make sure we are allowed to bind here. */
	if (snum || !(inet_test_bit(BIND_ADDRESS_NO_PORT, sk) ||
		      (flags & BIND_FORCE_ADDRESS_NO_PORT))) {
		err = sk->sk_prot->get_port(sk, snum);
		if (err) {
			sk->sk_ipv6only = saved_ipv6only;
			inet_reset_saddr(sk);
			goto out;
		}
		if (!(flags & BIND_FROM_BPF)) {
			err = BPF_CGROUP_RUN_PROG_INET6_POST_BIND(sk);
			if (err) {
				sk->sk_ipv6only = saved_ipv6only;
				inet_reset_saddr(sk);
				if (sk->sk_prot->put_port)
					sk->sk_prot->put_port(sk);
				goto out;
			}
		}
	}

	if (addr_type != IPV6_ADDR_ANY)
		sk->sk_userlocks |= SOCK_BINDADDR_LOCK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (snum)
		sk->sk_userlocks |= SOCK_BINDPORT_LOCK;
	inet->inet_sport = htons(inet->inet_num);
	inet->inet_dport = 0;
	inet->inet_daddr = 0;
out:
<<<<<<< HEAD
	release_sock(sk);
=======
	if (flags & BIND_WITH_LOCK)
		release_sock(sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
out_unlock:
	rcu_read_unlock();
	goto out;
}

<<<<<<< HEAD
=======
int inet6_bind_sk(struct sock *sk, struct sockaddr *uaddr, int addr_len)
{
	u32 flags = BIND_WITH_LOCK;
	const struct proto *prot;
	int err = 0;

	/* IPV6_ADDRFORM can change sk->sk_prot under us. */
	prot = READ_ONCE(sk->sk_prot);
	/* If the socket has its own bind function then use it. */
	if (prot->bind)
		return prot->bind(sk, uaddr, addr_len);

	if (addr_len < SIN6_LEN_RFC2133)
		return -EINVAL;

	/* BPF prog is run before any checks are done so that if the prog
	 * changes context in a wrong way it will be caught.
	 */
	err = BPF_CGROUP_RUN_PROG_INET_BIND_LOCK(sk, uaddr, &addr_len,
						 CGROUP_INET6_BIND, &flags);
	if (err)
		return err;

	return __inet6_bind(sk, uaddr, addr_len, flags);
}

/* bind for INET6 API */
int inet6_bind(struct socket *sock, struct sockaddr *uaddr, int addr_len)
{
	return inet6_bind_sk(sock->sk, uaddr, addr_len);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL(inet6_bind);

int inet6_release(struct socket *sock)
{
	struct sock *sk = sock->sk;

<<<<<<< HEAD
	if (sk == NULL)
=======
	if (!sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	/* Free mc lists */
	ipv6_sock_mc_close(sk);

	/* Free ac lists */
	ipv6_sock_ac_close(sk);

	return inet_release(sock);
}
<<<<<<< HEAD

EXPORT_SYMBOL(inet6_release);

void inet6_destroy_sock(struct sock *sk)
=======
EXPORT_SYMBOL(inet6_release);

void inet6_cleanup_sock(struct sock *sk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct sk_buff *skb;
	struct ipv6_txoptions *opt;

	/* Release rx options */

<<<<<<< HEAD
	if ((skb = xchg(&np->pktoptions, NULL)) != NULL)
		kfree_skb(skb);

	if ((skb = xchg(&np->rxpmtu, NULL)) != NULL)
		kfree_skb(skb);
=======
	skb = xchg(&np->pktoptions, NULL);
	kfree_skb(skb);

	skb = xchg(&np->rxpmtu, NULL);
	kfree_skb(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Free flowlabels */
	fl6_free_socklist(sk);

	/* Free tx options */

<<<<<<< HEAD
	if ((opt = xchg(&np->opt, NULL)) != NULL)
		sock_kfree_s(sk, opt, opt->tot_len);
}

EXPORT_SYMBOL_GPL(inet6_destroy_sock);
=======
	opt = xchg((__force struct ipv6_txoptions **)&np->opt, NULL);
	if (opt) {
		atomic_sub(opt->tot_len, &sk->sk_omem_alloc);
		txopt_put(opt);
	}
}
EXPORT_SYMBOL_GPL(inet6_cleanup_sock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	This does both peername and sockname.
 */
<<<<<<< HEAD

int inet6_getname(struct socket *sock, struct sockaddr *uaddr,
		 int *uaddr_len, int peer)
{
	struct sockaddr_in6 *sin=(struct sockaddr_in6 *)uaddr;
=======
int inet6_getname(struct socket *sock, struct sockaddr *uaddr,
		  int peer)
{
	struct sockaddr_in6 *sin = (struct sockaddr_in6 *)uaddr;
	int sin_addr_len = sizeof(*sin);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sock *sk = sock->sk;
	struct inet_sock *inet = inet_sk(sk);
	struct ipv6_pinfo *np = inet6_sk(sk);

	sin->sin6_family = AF_INET6;
	sin->sin6_flowinfo = 0;
	sin->sin6_scope_id = 0;
<<<<<<< HEAD
	if (peer) {
		if (!inet->inet_dport)
			return -ENOTCONN;
		if (((1 << sk->sk_state) & (TCPF_CLOSE | TCPF_SYN_SENT)) &&
		    peer == 1)
			return -ENOTCONN;
		sin->sin6_port = inet->inet_dport;
		sin->sin6_addr = np->daddr;
		if (np->sndflow)
			sin->sin6_flowinfo = np->flow_label;
	} else {
		if (ipv6_addr_any(&np->rcv_saddr))
			sin->sin6_addr = np->saddr;
		else
			sin->sin6_addr = np->rcv_saddr;

		sin->sin6_port = inet->inet_sport;
	}
	if (ipv6_addr_type(&sin->sin6_addr) & IPV6_ADDR_LINKLOCAL)
		sin->sin6_scope_id = sk->sk_bound_dev_if;
	*uaddr_len = sizeof(*sin);
	return 0;
}

EXPORT_SYMBOL(inet6_getname);

int inet6_killaddr_ioctl(struct net *net, void __user *arg) {
	struct in6_ifreq ireq;
	struct sockaddr_in6 sin6;

	if (!capable(CAP_NET_ADMIN))
		return -EACCES;

	if (copy_from_user(&ireq, arg, sizeof(struct in6_ifreq)))
		return -EFAULT;

	sin6.sin6_family = AF_INET6;
	sin6.sin6_addr = ireq.ifr6_addr;
	return tcp_nuke_addr(net, (struct sockaddr *) &sin6);
}

int inet6_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	struct sock *sk = sock->sk;
	struct net *net = sock_net(sk);

	switch(cmd)
	{
	case SIOCGSTAMP:
		return sock_get_timestamp(sk, (struct timeval __user *)arg);

	case SIOCGSTAMPNS:
		return sock_get_timestampns(sk, (struct timespec __user *)arg);

	case SIOCADDRT:
	case SIOCDELRT:

		return ipv6_route_ioctl(net, cmd, (void __user *)arg);

	case SIOCSIFADDR:
		return addrconf_add_ifaddr(net, (void __user *) arg);
	case SIOCDIFADDR:
		return addrconf_del_ifaddr(net, (void __user *) arg);
	case SIOCSIFDSTADDR:
		return addrconf_set_dstaddr(net, (void __user *) arg);
	case SIOCKILLADDR:
		return inet6_killaddr_ioctl(net, (void __user *) arg);
	default:
		if (!sk->sk_prot->ioctl)
			return -ENOIOCTLCMD;
		return sk->sk_prot->ioctl(sk, cmd, arg);
=======
	lock_sock(sk);
	if (peer) {
		if (!inet->inet_dport ||
		    (((1 << sk->sk_state) & (TCPF_CLOSE | TCPF_SYN_SENT)) &&
		    peer == 1)) {
			release_sock(sk);
			return -ENOTCONN;
		}
		sin->sin6_port = inet->inet_dport;
		sin->sin6_addr = sk->sk_v6_daddr;
		if (inet6_test_bit(SNDFLOW, sk))
			sin->sin6_flowinfo = np->flow_label;
		BPF_CGROUP_RUN_SA_PROG(sk, (struct sockaddr *)sin, &sin_addr_len,
				       CGROUP_INET6_GETPEERNAME);
	} else {
		if (ipv6_addr_any(&sk->sk_v6_rcv_saddr))
			sin->sin6_addr = np->saddr;
		else
			sin->sin6_addr = sk->sk_v6_rcv_saddr;
		sin->sin6_port = inet->inet_sport;
		BPF_CGROUP_RUN_SA_PROG(sk, (struct sockaddr *)sin, &sin_addr_len,
				       CGROUP_INET6_GETSOCKNAME);
	}
	sin->sin6_scope_id = ipv6_iface_scope_id(&sin->sin6_addr,
						 sk->sk_bound_dev_if);
	release_sock(sk);
	return sin_addr_len;
}
EXPORT_SYMBOL(inet6_getname);

int inet6_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	struct sock *sk = sock->sk;
	struct net *net = sock_net(sk);
	const struct proto *prot;

	switch (cmd) {
	case SIOCADDRT:
	case SIOCDELRT: {
		struct in6_rtmsg rtmsg;

		if (copy_from_user(&rtmsg, argp, sizeof(rtmsg)))
			return -EFAULT;
		return ipv6_route_ioctl(net, cmd, &rtmsg);
	}
	case SIOCSIFADDR:
		return addrconf_add_ifaddr(net, argp);
	case SIOCDIFADDR:
		return addrconf_del_ifaddr(net, argp);
	case SIOCSIFDSTADDR:
		return addrconf_set_dstaddr(net, argp);
	default:
		/* IPV6_ADDRFORM can change sk->sk_prot under us. */
		prot = READ_ONCE(sk->sk_prot);
		if (!prot->ioctl)
			return -ENOIOCTLCMD;
		return sk_ioctl(sk, cmd, (void __user *)arg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	/*NOTREACHED*/
	return 0;
}
<<<<<<< HEAD

EXPORT_SYMBOL(inet6_ioctl);

=======
EXPORT_SYMBOL(inet6_ioctl);

#ifdef CONFIG_COMPAT
struct compat_in6_rtmsg {
	struct in6_addr		rtmsg_dst;
	struct in6_addr		rtmsg_src;
	struct in6_addr		rtmsg_gateway;
	u32			rtmsg_type;
	u16			rtmsg_dst_len;
	u16			rtmsg_src_len;
	u32			rtmsg_metric;
	u32			rtmsg_info;
	u32			rtmsg_flags;
	s32			rtmsg_ifindex;
};

static int inet6_compat_routing_ioctl(struct sock *sk, unsigned int cmd,
		struct compat_in6_rtmsg __user *ur)
{
	struct in6_rtmsg rt;

	if (copy_from_user(&rt.rtmsg_dst, &ur->rtmsg_dst,
			3 * sizeof(struct in6_addr)) ||
	    get_user(rt.rtmsg_type, &ur->rtmsg_type) ||
	    get_user(rt.rtmsg_dst_len, &ur->rtmsg_dst_len) ||
	    get_user(rt.rtmsg_src_len, &ur->rtmsg_src_len) ||
	    get_user(rt.rtmsg_metric, &ur->rtmsg_metric) ||
	    get_user(rt.rtmsg_info, &ur->rtmsg_info) ||
	    get_user(rt.rtmsg_flags, &ur->rtmsg_flags) ||
	    get_user(rt.rtmsg_ifindex, &ur->rtmsg_ifindex))
		return -EFAULT;


	return ipv6_route_ioctl(sock_net(sk), cmd, &rt);
}

int inet6_compat_ioctl(struct socket *sock, unsigned int cmd, unsigned long arg)
{
	void __user *argp = compat_ptr(arg);
	struct sock *sk = sock->sk;

	switch (cmd) {
	case SIOCADDRT:
	case SIOCDELRT:
		return inet6_compat_routing_ioctl(sk, cmd, argp);
	default:
		return -ENOIOCTLCMD;
	}
}
EXPORT_SYMBOL_GPL(inet6_compat_ioctl);
#endif /* CONFIG_COMPAT */

INDIRECT_CALLABLE_DECLARE(int udpv6_sendmsg(struct sock *, struct msghdr *,
					    size_t));
int inet6_sendmsg(struct socket *sock, struct msghdr *msg, size_t size)
{
	struct sock *sk = sock->sk;
	const struct proto *prot;

	if (unlikely(inet_send_prepare(sk)))
		return -EAGAIN;

	/* IPV6_ADDRFORM can change sk->sk_prot under us. */
	prot = READ_ONCE(sk->sk_prot);
	return INDIRECT_CALL_2(prot->sendmsg, tcp_sendmsg, udpv6_sendmsg,
			       sk, msg, size);
}

INDIRECT_CALLABLE_DECLARE(int udpv6_recvmsg(struct sock *, struct msghdr *,
					    size_t, int, int *));
int inet6_recvmsg(struct socket *sock, struct msghdr *msg, size_t size,
		  int flags)
{
	struct sock *sk = sock->sk;
	const struct proto *prot;
	int addr_len = 0;
	int err;

	if (likely(!(flags & MSG_ERRQUEUE)))
		sock_rps_record_flow(sk);

	/* IPV6_ADDRFORM can change sk->sk_prot under us. */
	prot = READ_ONCE(sk->sk_prot);
	err = INDIRECT_CALL_2(prot->recvmsg, tcp_recvmsg, udpv6_recvmsg,
			      sk, msg, size, flags, &addr_len);
	if (err >= 0)
		msg->msg_namelen = addr_len;
	return err;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
const struct proto_ops inet6_stream_ops = {
	.family		   = PF_INET6,
	.owner		   = THIS_MODULE,
	.release	   = inet6_release,
	.bind		   = inet6_bind,
	.connect	   = inet_stream_connect,	/* ok		*/
	.socketpair	   = sock_no_socketpair,	/* a do nothing	*/
	.accept		   = inet_accept,		/* ok		*/
	.getname	   = inet6_getname,
	.poll		   = tcp_poll,			/* ok		*/
	.ioctl		   = inet6_ioctl,		/* must change  */
<<<<<<< HEAD
=======
	.gettstamp	   = sock_gettstamp,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.listen		   = inet_listen,		/* ok		*/
	.shutdown	   = inet_shutdown,		/* ok		*/
	.setsockopt	   = sock_common_setsockopt,	/* ok		*/
	.getsockopt	   = sock_common_getsockopt,	/* ok		*/
<<<<<<< HEAD
	.sendmsg	   = inet_sendmsg,		/* ok		*/
	.recvmsg	   = inet_recvmsg,		/* ok		*/
	.mmap		   = sock_no_mmap,
	.sendpage	   = inet_sendpage,
	.splice_read	   = tcp_splice_read,
#ifdef CONFIG_COMPAT
	.compat_setsockopt = compat_sock_common_setsockopt,
	.compat_getsockopt = compat_sock_common_getsockopt,
#endif
=======
	.sendmsg	   = inet6_sendmsg,		/* retpoline's sake */
	.recvmsg	   = inet6_recvmsg,		/* retpoline's sake */
#ifdef CONFIG_MMU
	.mmap		   = tcp_mmap,
#endif
	.splice_eof	   = inet_splice_eof,
	.sendmsg_locked    = tcp_sendmsg_locked,
	.splice_read	   = tcp_splice_read,
	.read_sock	   = tcp_read_sock,
	.read_skb	   = tcp_read_skb,
	.peek_len	   = tcp_peek_len,
#ifdef CONFIG_COMPAT
	.compat_ioctl	   = inet6_compat_ioctl,
#endif
	.set_rcvlowat	   = tcp_set_rcvlowat,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const struct proto_ops inet6_dgram_ops = {
	.family		   = PF_INET6,
	.owner		   = THIS_MODULE,
	.release	   = inet6_release,
	.bind		   = inet6_bind,
	.connect	   = inet_dgram_connect,	/* ok		*/
	.socketpair	   = sock_no_socketpair,	/* a do nothing	*/
	.accept		   = sock_no_accept,		/* a do nothing	*/
	.getname	   = inet6_getname,
	.poll		   = udp_poll,			/* ok		*/
	.ioctl		   = inet6_ioctl,		/* must change  */
<<<<<<< HEAD
=======
	.gettstamp	   = sock_gettstamp,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.listen		   = sock_no_listen,		/* ok		*/
	.shutdown	   = inet_shutdown,		/* ok		*/
	.setsockopt	   = sock_common_setsockopt,	/* ok		*/
	.getsockopt	   = sock_common_getsockopt,	/* ok		*/
<<<<<<< HEAD
	.sendmsg	   = inet_sendmsg,		/* ok		*/
	.recvmsg	   = inet_recvmsg,		/* ok		*/
	.mmap		   = sock_no_mmap,
	.sendpage	   = sock_no_sendpage,
#ifdef CONFIG_COMPAT
	.compat_setsockopt = compat_sock_common_setsockopt,
	.compat_getsockopt = compat_sock_common_getsockopt,
=======
	.sendmsg	   = inet6_sendmsg,		/* retpoline's sake */
	.recvmsg	   = inet6_recvmsg,		/* retpoline's sake */
	.read_skb	   = udp_read_skb,
	.mmap		   = sock_no_mmap,
	.set_peek_off	   = udp_set_peek_off,
#ifdef CONFIG_COMPAT
	.compat_ioctl	   = inet6_compat_ioctl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
};

static const struct net_proto_family inet6_family_ops = {
	.family = PF_INET6,
	.create = inet6_create,
	.owner	= THIS_MODULE,
};

int inet6_register_protosw(struct inet_protosw *p)
{
	struct list_head *lh;
	struct inet_protosw *answer;
	struct list_head *last_perm;
	int protocol = p->protocol;
	int ret;

	spin_lock_bh(&inetsw6_lock);

	ret = -EINVAL;
	if (p->type >= SOCK_MAX)
		goto out_illegal;

	/* If we are trying to override a permanent protocol, bail. */
	answer = NULL;
	ret = -EPERM;
	last_perm = &inetsw6[p->type];
	list_for_each(lh, &inetsw6[p->type]) {
		answer = list_entry(lh, struct inet_protosw, list);

		/* Check only the non-wild match. */
		if (INET_PROTOSW_PERMANENT & answer->flags) {
			if (protocol == answer->protocol)
				break;
			last_perm = lh;
		}

		answer = NULL;
	}
	if (answer)
		goto out_permanent;

	/* Add the new entry after the last permanent entry if any, so that
	 * the new entry does not override a permanent entry when matched with
	 * a wild-card protocol. But it is allowed to override any existing
	 * non-permanent entry.  This means that when we remove this entry, the
	 * system automatically returns to the old behavior.
	 */
	list_add_rcu(&p->list, last_perm);
	ret = 0;
out:
	spin_unlock_bh(&inetsw6_lock);
	return ret;

out_permanent:
<<<<<<< HEAD
	printk(KERN_ERR "Attempt to override permanent protocol %d.\n",
	       protocol);
	goto out;

out_illegal:
	printk(KERN_ERR
	       "Ignoring attempt to register invalid socket type %d.\n",
	       p->type);
	goto out;
}

=======
	pr_err("Attempt to override permanent protocol %d\n", protocol);
	goto out;

out_illegal:
	pr_err("Ignoring attempt to register invalid socket type %d\n",
	       p->type);
	goto out;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL(inet6_register_protosw);

void
inet6_unregister_protosw(struct inet_protosw *p)
{
	if (INET_PROTOSW_PERMANENT & p->flags) {
<<<<<<< HEAD
		printk(KERN_ERR
		       "Attempt to unregister permanent protocol %d.\n",
=======
		pr_err("Attempt to unregister permanent protocol %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       p->protocol);
	} else {
		spin_lock_bh(&inetsw6_lock);
		list_del_rcu(&p->list);
		spin_unlock_bh(&inetsw6_lock);

		synchronize_net();
	}
}
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL(inet6_unregister_protosw);

int inet6_sk_rebuild_header(struct sock *sk)
{
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct dst_entry *dst;

	dst = __sk_dst_check(sk, np->dst_cookie);

<<<<<<< HEAD
	if (dst == NULL) {
=======
	if (!dst) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct inet_sock *inet = inet_sk(sk);
		struct in6_addr *final_p, final;
		struct flowi6 fl6;

		memset(&fl6, 0, sizeof(fl6));
		fl6.flowi6_proto = sk->sk_protocol;
<<<<<<< HEAD
		fl6.daddr = np->daddr;
=======
		fl6.daddr = sk->sk_v6_daddr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		fl6.saddr = np->saddr;
		fl6.flowlabel = np->flow_label;
		fl6.flowi6_oif = sk->sk_bound_dev_if;
		fl6.flowi6_mark = sk->sk_mark;
		fl6.fl6_dport = inet->inet_dport;
		fl6.fl6_sport = inet->inet_sport;
		fl6.flowi6_uid = sk->sk_uid;
<<<<<<< HEAD
		security_sk_classify_flow(sk, flowi6_to_flowi(&fl6));

		final_p = fl6_update_dst(&fl6, np->opt, &final);

		dst = ip6_dst_lookup_flow(sk, &fl6, final_p, false);
		if (IS_ERR(dst)) {
			sk->sk_route_caps = 0;
			sk->sk_err_soft = -PTR_ERR(dst);
			return PTR_ERR(dst);
		}

		__ip6_dst_store(sk, dst, NULL, NULL);
=======
		security_sk_classify_flow(sk, flowi6_to_flowi_common(&fl6));

		rcu_read_lock();
		final_p = fl6_update_dst(&fl6, rcu_dereference(np->opt),
					 &final);
		rcu_read_unlock();

		dst = ip6_dst_lookup_flow(sock_net(sk), sk, &fl6, final_p);
		if (IS_ERR(dst)) {
			sk->sk_route_caps = 0;
			WRITE_ONCE(sk->sk_err_soft, -PTR_ERR(dst));
			return PTR_ERR(dst);
		}

		ip6_dst_store(sk, dst, NULL, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}
<<<<<<< HEAD

EXPORT_SYMBOL_GPL(inet6_sk_rebuild_header);

int ipv6_opt_accepted(struct sock *sk, struct sk_buff *skb)
{
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct inet6_skb_parm *opt = IP6CB(skb);

	if (np->rxopt.all) {
		if ((opt->hop && (np->rxopt.bits.hopopts ||
				  np->rxopt.bits.ohopopts)) ||
		    ((IPV6_FLOWINFO_MASK &
		      *(__be32 *)skb_network_header(skb)) &&
=======
EXPORT_SYMBOL_GPL(inet6_sk_rebuild_header);

bool ipv6_opt_accepted(const struct sock *sk, const struct sk_buff *skb,
		       const struct inet6_skb_parm *opt)
{
	const struct ipv6_pinfo *np = inet6_sk(sk);

	if (np->rxopt.all) {
		if (((opt->flags & IP6SKB_HOPBYHOP) &&
		     (np->rxopt.bits.hopopts || np->rxopt.bits.ohopopts)) ||
		    (ip6_flowinfo((struct ipv6hdr *) skb_network_header(skb)) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		     np->rxopt.bits.rxflow) ||
		    (opt->srcrt && (np->rxopt.bits.srcrt ||
		     np->rxopt.bits.osrcrt)) ||
		    ((opt->dst1 || opt->dst0) &&
		     (np->rxopt.bits.dstopts || np->rxopt.bits.odstopts)))
<<<<<<< HEAD
			return 1;
	}
	return 0;
}

EXPORT_SYMBOL_GPL(ipv6_opt_accepted);

static int ipv6_gso_pull_exthdrs(struct sk_buff *skb, int proto)
{
	const struct inet6_protocol *ops = NULL;

	for (;;) {
		struct ipv6_opt_hdr *opth;
		int len;

		if (proto != NEXTHDR_HOP) {
			ops = rcu_dereference(inet6_protos[proto]);

			if (unlikely(!ops))
				break;

			if (!(ops->flags & INET6_PROTO_GSO_EXTHDR))
				break;
		}

		if (unlikely(!pskb_may_pull(skb, 8)))
			break;

		opth = (void *)skb->data;
		len = ipv6_optlen(opth);

		if (unlikely(!pskb_may_pull(skb, len)))
			break;

		proto = opth->nexthdr;
		__skb_pull(skb, len);
	}

	return proto;
}

static int ipv6_gso_send_check(struct sk_buff *skb)
{
	const struct ipv6hdr *ipv6h;
	const struct inet6_protocol *ops;
	int err = -EINVAL;

	if (unlikely(!pskb_may_pull(skb, sizeof(*ipv6h))))
		goto out;

	ipv6h = ipv6_hdr(skb);
	__skb_pull(skb, sizeof(*ipv6h));
	err = -EPROTONOSUPPORT;

	rcu_read_lock();
	ops = rcu_dereference(inet6_protos[
		ipv6_gso_pull_exthdrs(skb, ipv6h->nexthdr)]);

	if (likely(ops && ops->gso_send_check)) {
		skb_reset_transport_header(skb);
		err = ops->gso_send_check(skb);
	}
	rcu_read_unlock();

out:
	return err;
}

static struct sk_buff *ipv6_gso_segment(struct sk_buff *skb,
	netdev_features_t features)
{
	struct sk_buff *segs = ERR_PTR(-EINVAL);
	struct ipv6hdr *ipv6h;
	const struct inet6_protocol *ops;
	int proto;
	struct frag_hdr *fptr;
	u8 *prevhdr;
	int offset = 0;

	if (!(features & NETIF_F_V6_CSUM))
		features &= ~NETIF_F_SG;

	if (unlikely(skb_shinfo(skb)->gso_type &
		     ~(SKB_GSO_UDP |
		       SKB_GSO_DODGY |
		       SKB_GSO_TCP_ECN |
		       SKB_GSO_TCPV6 |
		       0)))
		goto out;

	if (unlikely(!pskb_may_pull(skb, sizeof(*ipv6h))))
		goto out;

	ipv6h = ipv6_hdr(skb);
	__skb_pull(skb, sizeof(*ipv6h));
	segs = ERR_PTR(-EPROTONOSUPPORT);

	proto = ipv6_gso_pull_exthdrs(skb, ipv6h->nexthdr);
	rcu_read_lock();
	ops = rcu_dereference(inet6_protos[proto]);
	if (likely(ops && ops->gso_segment)) {
		skb_reset_transport_header(skb);
		segs = ops->gso_segment(skb, features);
	}
	rcu_read_unlock();

	if (IS_ERR(segs))
		goto out;

	for (skb = segs; skb; skb = skb->next) {
		ipv6h = ipv6_hdr(skb);
		ipv6h->payload_len = htons(skb->len - skb->mac_len -
					   sizeof(*ipv6h));
		if (proto == IPPROTO_UDP) {
			int err = ip6_find_1stfragopt(skb, &prevhdr);
			if (err < 0)
				return ERR_PTR(err);
			fptr = (struct frag_hdr *)(skb_network_header(skb) +
				err);
			fptr->frag_off = htons(offset);
			if (skb->next != NULL)
				fptr->frag_off |= htons(IP6_MF);
			offset += (ntohs(ipv6h->payload_len) -
				   sizeof(struct frag_hdr));
		}
	}

out:
	return segs;
}

struct ipv6_gro_cb {
	struct napi_gro_cb napi;
	int proto;
};

#define IPV6_GRO_CB(skb) ((struct ipv6_gro_cb *)(skb)->cb)

static struct sk_buff **ipv6_gro_receive(struct sk_buff **head,
					 struct sk_buff *skb)
{
	const struct inet6_protocol *ops;
	struct sk_buff **pp = NULL;
	struct sk_buff *p;
	struct ipv6hdr *iph;
	unsigned int nlen;
	unsigned int hlen;
	unsigned int off;
	int flush = 1;
	int proto;
	__wsum csum;

	off = skb_gro_offset(skb);
	hlen = off + sizeof(*iph);
	iph = skb_gro_header_fast(skb, off);
	if (skb_gro_header_hard(skb, hlen)) {
		iph = skb_gro_header_slow(skb, hlen, off);
		if (unlikely(!iph))
			goto out;
	}

	skb_gro_pull(skb, sizeof(*iph));
	skb_set_transport_header(skb, skb_gro_offset(skb));

	flush += ntohs(iph->payload_len) != skb_gro_len(skb);

	rcu_read_lock();
	proto = iph->nexthdr;
	ops = rcu_dereference(inet6_protos[proto]);
	if (!ops || !ops->gro_receive) {
		__pskb_pull(skb, skb_gro_offset(skb));
		proto = ipv6_gso_pull_exthdrs(skb, proto);
		skb_gro_pull(skb, -skb_transport_offset(skb));
		skb_reset_transport_header(skb);
		__skb_push(skb, skb_gro_offset(skb));

		ops = rcu_dereference(inet6_protos[proto]);
		if (!ops || !ops->gro_receive)
			goto out_unlock;

		iph = ipv6_hdr(skb);
	}

	IPV6_GRO_CB(skb)->proto = proto;

	flush--;
	nlen = skb_network_header_len(skb);

	for (p = *head; p; p = p->next) {
		struct ipv6hdr *iph2;

		if (!NAPI_GRO_CB(p)->same_flow)
			continue;

		iph2 = ipv6_hdr(p);

		/* All fields must match except length. */
		if (nlen != skb_network_header_len(p) ||
		    memcmp(iph, iph2, offsetof(struct ipv6hdr, payload_len)) ||
		    memcmp(&iph->nexthdr, &iph2->nexthdr,
			   nlen - offsetof(struct ipv6hdr, nexthdr))) {
			NAPI_GRO_CB(p)->same_flow = 0;
			continue;
		}

		NAPI_GRO_CB(p)->flush |= flush;
	}

	NAPI_GRO_CB(skb)->flush |= flush;

	csum = skb->csum;
	skb_postpull_rcsum(skb, iph, skb_network_header_len(skb));

	pp = ops->gro_receive(head, skb);

	skb->csum = csum;

out_unlock:
	rcu_read_unlock();

out:
	NAPI_GRO_CB(skb)->flush |= flush;

	return pp;
}

static int ipv6_gro_complete(struct sk_buff *skb)
{
	const struct inet6_protocol *ops;
	struct ipv6hdr *iph = ipv6_hdr(skb);
	int err = -ENOSYS;

	iph->payload_len = htons(skb->len - skb_network_offset(skb) -
				 sizeof(*iph));

	rcu_read_lock();
	ops = rcu_dereference(inet6_protos[IPV6_GRO_CB(skb)->proto]);
	if (WARN_ON(!ops || !ops->gro_complete))
		goto out_unlock;

	err = ops->gro_complete(skb);

out_unlock:
	rcu_read_unlock();

	return err;
}

static struct packet_type ipv6_packet_type __read_mostly = {
	.type = cpu_to_be16(ETH_P_IPV6),
	.func = ipv6_rcv,
	.gso_send_check = ipv6_gso_send_check,
	.gso_segment = ipv6_gso_segment,
	.gro_receive = ipv6_gro_receive,
	.gro_complete = ipv6_gro_complete,
=======
			return true;
	}
	return false;
}
EXPORT_SYMBOL_GPL(ipv6_opt_accepted);

static struct packet_type ipv6_packet_type __read_mostly = {
	.type = cpu_to_be16(ETH_P_IPV6),
	.func = ipv6_rcv,
	.list_func = ipv6_list_rcv,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init ipv6_packet_init(void)
{
	dev_add_pack(&ipv6_packet_type);
	return 0;
}

static void ipv6_packet_cleanup(void)
{
	dev_remove_pack(&ipv6_packet_type);
}

static int __net_init ipv6_init_mibs(struct net *net)
{
	int i;

<<<<<<< HEAD
	if (snmp_mib_init((void __percpu **)net->mib.udp_stats_in6,
			  sizeof(struct udp_mib),
			  __alignof__(struct udp_mib)) < 0)
		return -ENOMEM;
	if (snmp_mib_init((void __percpu **)net->mib.udplite_stats_in6,
			  sizeof(struct udp_mib),
			  __alignof__(struct udp_mib)) < 0)
		goto err_udplite_mib;
	if (snmp_mib_init((void __percpu **)net->mib.ipv6_statistics,
			  sizeof(struct ipstats_mib),
			  __alignof__(struct ipstats_mib)) < 0)
=======
	net->mib.udp_stats_in6 = alloc_percpu(struct udp_mib);
	if (!net->mib.udp_stats_in6)
		return -ENOMEM;
	net->mib.udplite_stats_in6 = alloc_percpu(struct udp_mib);
	if (!net->mib.udplite_stats_in6)
		goto err_udplite_mib;
	net->mib.ipv6_statistics = alloc_percpu(struct ipstats_mib);
	if (!net->mib.ipv6_statistics)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err_ip_mib;

	for_each_possible_cpu(i) {
		struct ipstats_mib *af_inet6_stats;
<<<<<<< HEAD
		af_inet6_stats = per_cpu_ptr(net->mib.ipv6_statistics[0], i);
		u64_stats_init(&af_inet6_stats->syncp);
#if SNMP_ARRAY_SZ == 2
		af_inet6_stats = per_cpu_ptr(net->mib.ipv6_statistics[1], i);
		u64_stats_init(&af_inet6_stats->syncp);
#endif
	}


	if (snmp_mib_init((void __percpu **)net->mib.icmpv6_statistics,
			  sizeof(struct icmpv6_mib),
			  __alignof__(struct icmpv6_mib)) < 0)
=======
		af_inet6_stats = per_cpu_ptr(net->mib.ipv6_statistics, i);
		u64_stats_init(&af_inet6_stats->syncp);
	}


	net->mib.icmpv6_statistics = alloc_percpu(struct icmpv6_mib);
	if (!net->mib.icmpv6_statistics)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err_icmp_mib;
	net->mib.icmpv6msg_statistics = kzalloc(sizeof(struct icmpv6msg_mib),
						GFP_KERNEL);
	if (!net->mib.icmpv6msg_statistics)
		goto err_icmpmsg_mib;
	return 0;

err_icmpmsg_mib:
<<<<<<< HEAD
	snmp_mib_free((void __percpu **)net->mib.icmpv6_statistics);
err_icmp_mib:
	snmp_mib_free((void __percpu **)net->mib.ipv6_statistics);
err_ip_mib:
	snmp_mib_free((void __percpu **)net->mib.udplite_stats_in6);
err_udplite_mib:
	snmp_mib_free((void __percpu **)net->mib.udp_stats_in6);
=======
	free_percpu(net->mib.icmpv6_statistics);
err_icmp_mib:
	free_percpu(net->mib.ipv6_statistics);
err_ip_mib:
	free_percpu(net->mib.udplite_stats_in6);
err_udplite_mib:
	free_percpu(net->mib.udp_stats_in6);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -ENOMEM;
}

static void ipv6_cleanup_mibs(struct net *net)
{
<<<<<<< HEAD
	snmp_mib_free((void __percpu **)net->mib.udp_stats_in6);
	snmp_mib_free((void __percpu **)net->mib.udplite_stats_in6);
	snmp_mib_free((void __percpu **)net->mib.ipv6_statistics);
	snmp_mib_free((void __percpu **)net->mib.icmpv6_statistics);
=======
	free_percpu(net->mib.udp_stats_in6);
	free_percpu(net->mib.udplite_stats_in6);
	free_percpu(net->mib.ipv6_statistics);
	free_percpu(net->mib.icmpv6_statistics);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(net->mib.icmpv6msg_statistics);
}

static int __net_init inet6_net_init(struct net *net)
{
	int err = 0;

	net->ipv6.sysctl.bindv6only = 0;
	net->ipv6.sysctl.icmpv6_time = 1*HZ;
<<<<<<< HEAD
=======
	net->ipv6.sysctl.icmpv6_echo_ignore_all = 0;
	net->ipv6.sysctl.icmpv6_echo_ignore_multicast = 0;
	net->ipv6.sysctl.icmpv6_echo_ignore_anycast = 0;
	net->ipv6.sysctl.icmpv6_error_anycast_as_unicast = 0;

	/* By default, rate limit error messages.
	 * Except for pmtu discovery, it would break it.
	 * proc_do_large_bitmap needs pointer to the bitmap.
	 */
	bitmap_set(net->ipv6.sysctl.icmpv6_ratemask, 0, ICMPV6_ERRMSG_MAX + 1);
	bitmap_clear(net->ipv6.sysctl.icmpv6_ratemask, ICMPV6_PKT_TOOBIG, 1);
	net->ipv6.sysctl.icmpv6_ratemask_ptr = net->ipv6.sysctl.icmpv6_ratemask;

	net->ipv6.sysctl.flowlabel_consistency = 1;
	net->ipv6.sysctl.auto_flowlabels = IP6_DEFAULT_AUTO_FLOW_LABELS;
	net->ipv6.sysctl.idgen_retries = 3;
	net->ipv6.sysctl.idgen_delay = 1 * HZ;
	net->ipv6.sysctl.flowlabel_state_ranges = 0;
	net->ipv6.sysctl.max_dst_opts_cnt = IP6_DEFAULT_MAX_DST_OPTS_CNT;
	net->ipv6.sysctl.max_hbh_opts_cnt = IP6_DEFAULT_MAX_HBH_OPTS_CNT;
	net->ipv6.sysctl.max_dst_opts_len = IP6_DEFAULT_MAX_DST_OPTS_LEN;
	net->ipv6.sysctl.max_hbh_opts_len = IP6_DEFAULT_MAX_HBH_OPTS_LEN;
	net->ipv6.sysctl.fib_notify_on_flag_change = 0;
	atomic_set(&net->ipv6.fib6_sernum, 1);

	net->ipv6.sysctl.ioam6_id = IOAM6_DEFAULT_ID;
	net->ipv6.sysctl.ioam6_id_wide = IOAM6_DEFAULT_ID_WIDE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = ipv6_init_mibs(net);
	if (err)
		return err;
#ifdef CONFIG_PROC_FS
	err = udp6_proc_init(net);
	if (err)
		goto out;
	err = tcp6_proc_init(net);
	if (err)
		goto proc_tcp6_fail;
	err = ac6_proc_init(net);
	if (err)
		goto proc_ac6_fail;
#endif
	return err;

#ifdef CONFIG_PROC_FS
proc_ac6_fail:
	tcp6_proc_exit(net);
proc_tcp6_fail:
	udp6_proc_exit(net);
out:
	ipv6_cleanup_mibs(net);
	return err;
#endif
}

static void __net_exit inet6_net_exit(struct net *net)
{
#ifdef CONFIG_PROC_FS
	udp6_proc_exit(net);
	tcp6_proc_exit(net);
	ac6_proc_exit(net);
#endif
	ipv6_cleanup_mibs(net);
}

static struct pernet_operations inet6_net_ops = {
	.init = inet6_net_init,
	.exit = inet6_net_exit,
};

<<<<<<< HEAD
static int __init inet6_init(void)
{
	struct sk_buff *dummy_skb;
	struct list_head *r;
	int err = 0;

	BUILD_BUG_ON(sizeof(struct inet6_skb_parm) > sizeof(dummy_skb->cb));

	/* Register the socket-side information for inet6_create.  */
	for(r = &inetsw6[0]; r < &inetsw6[SOCK_MAX]; ++r)
		INIT_LIST_HEAD(r);

	if (disable_ipv6_mod) {
		printk(KERN_INFO
		       "IPv6: Loaded, but administratively disabled, "
		       "reboot required to enable\n");
=======
static int ipv6_route_input(struct sk_buff *skb)
{
	ip6_route_input(skb);
	return skb_dst(skb)->error;
}

static const struct ipv6_stub ipv6_stub_impl = {
	.ipv6_sock_mc_join = ipv6_sock_mc_join,
	.ipv6_sock_mc_drop = ipv6_sock_mc_drop,
	.ipv6_dst_lookup_flow = ip6_dst_lookup_flow,
	.ipv6_route_input  = ipv6_route_input,
	.fib6_get_table	   = fib6_get_table,
	.fib6_table_lookup = fib6_table_lookup,
	.fib6_lookup       = fib6_lookup,
	.fib6_select_path  = fib6_select_path,
	.ip6_mtu_from_fib6 = ip6_mtu_from_fib6,
	.fib6_nh_init	   = fib6_nh_init,
	.fib6_nh_release   = fib6_nh_release,
	.fib6_nh_release_dsts = fib6_nh_release_dsts,
	.fib6_update_sernum = fib6_update_sernum_stub,
	.fib6_rt_update	   = fib6_rt_update,
	.ip6_del_rt	   = ip6_del_rt,
	.udpv6_encap_enable = udpv6_encap_enable,
	.ndisc_send_na = ndisc_send_na,
#if IS_ENABLED(CONFIG_XFRM)
	.xfrm6_local_rxpmtu = xfrm6_local_rxpmtu,
	.xfrm6_udp_encap_rcv = xfrm6_udp_encap_rcv,
	.xfrm6_gro_udp_encap_rcv = xfrm6_gro_udp_encap_rcv,
	.xfrm6_rcv_encap = xfrm6_rcv_encap,
#endif
	.nd_tbl	= &nd_tbl,
	.ipv6_fragment = ip6_fragment,
	.ipv6_dev_find = ipv6_dev_find,
};

static const struct ipv6_bpf_stub ipv6_bpf_stub_impl = {
	.inet6_bind = __inet6_bind,
	.udp6_lib_lookup = __udp6_lib_lookup,
	.ipv6_setsockopt = do_ipv6_setsockopt,
	.ipv6_getsockopt = do_ipv6_getsockopt,
	.ipv6_dev_get_saddr = ipv6_dev_get_saddr,
};

static int __init inet6_init(void)
{
	struct list_head *r;
	int err = 0;

	sock_skb_cb_check_size(sizeof(struct inet6_skb_parm));

	/* Register the socket-side information for inet6_create.  */
	for (r = &inetsw6[0]; r < &inetsw6[SOCK_MAX]; ++r)
		INIT_LIST_HEAD(r);

	raw_hashinfo_init(&raw_v6_hashinfo);

	if (disable_ipv6_mod) {
		pr_info("Loaded, but administratively disabled, reboot required to enable\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	err = proto_register(&tcpv6_prot, 1);
	if (err)
		goto out;

	err = proto_register(&udpv6_prot, 1);
	if (err)
		goto out_unregister_tcp_proto;

	err = proto_register(&udplitev6_prot, 1);
	if (err)
		goto out_unregister_udp_proto;

	err = proto_register(&rawv6_prot, 1);
	if (err)
		goto out_unregister_udplite_proto;

	err = proto_register(&pingv6_prot, 1);
	if (err)
<<<<<<< HEAD
		goto out_unregister_ping_proto;
=======
		goto out_unregister_raw_proto;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* We MUST register RAW sockets before we create the ICMP6,
	 * IGMP6, or NDISC control sockets.
	 */
	err = rawv6_init();
	if (err)
<<<<<<< HEAD
		goto out_unregister_raw_proto;
=======
		goto out_unregister_ping_proto;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Register the family here so that the init calls below will
	 * be able to create sockets. (?? is this dangerous ??)
	 */
	err = sock_register(&inet6_family_ops);
	if (err)
		goto out_sock_register_fail;

<<<<<<< HEAD
#ifdef CONFIG_SYSCTL
	err = ipv6_static_sysctl_register();
	if (err)
		goto static_sysctl_fail;
#endif
	tcpv6_prot.sysctl_mem = init_net.ipv4.sysctl_tcp_mem;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 *	ipngwg API draft makes clear that the correct semantics
	 *	for TCP and UDP is to consider one TCP and UDP instance
	 *	in a host available by both INET and INET6 APIs and
	 *	able to communicate via both network protocols.
	 */

	err = register_pernet_subsys(&inet6_net_ops);
	if (err)
		goto register_pernet_fail;
<<<<<<< HEAD
	err = icmpv6_init();
	if (err)
		goto icmp_fail;
	err = ip6_mr_init();
	if (err)
		goto ipmr_fail;
=======
	err = ip6_mr_init();
	if (err)
		goto ipmr_fail;
	err = icmpv6_init();
	if (err)
		goto icmp_fail;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = ndisc_init();
	if (err)
		goto ndisc_fail;
	err = igmp6_init();
	if (err)
		goto igmp_fail;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = ipv6_netfilter_init();
	if (err)
		goto netfilter_fail;
	/* Create /proc/foo6 entries. */
#ifdef CONFIG_PROC_FS
	err = -ENOMEM;
	if (raw6_proc_init())
		goto proc_raw6_fail;
	if (udplite6_proc_init())
		goto proc_udplite6_fail;
	if (ipv6_misc_proc_init())
		goto proc_misc6_fail;
	if (if6_proc_init())
		goto proc_if6_fail;
#endif
	err = ip6_route_init();
	if (err)
		goto ip6_route_fail;
	err = ndisc_late_init();
	if (err)
		goto ndisc_late_fail;
	err = ip6_flowlabel_init();
	if (err)
		goto ip6_flowlabel_fail;
<<<<<<< HEAD
=======
	err = ipv6_anycast_init();
	if (err)
		goto ipv6_anycast_fail;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = addrconf_init();
	if (err)
		goto addrconf_fail;

	/* Init v6 extension headers. */
	err = ipv6_exthdrs_init();
	if (err)
		goto ipv6_exthdrs_fail;

	err = ipv6_frag_init();
	if (err)
		goto ipv6_frag_fail;

	/* Init v6 transport protocols. */
	err = udpv6_init();
	if (err)
		goto udpv6_fail;

	err = udplitev6_init();
	if (err)
		goto udplitev6_fail;

<<<<<<< HEAD
=======
	err = udpv6_offload_init();
	if (err)
		goto udpv6_offload_fail;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = tcpv6_init();
	if (err)
		goto tcpv6_fail;

	err = ipv6_packet_init();
	if (err)
		goto ipv6_packet_fail;

	err = pingv6_init();
	if (err)
		goto pingv6_fail;

<<<<<<< HEAD
=======
	err = calipso_init();
	if (err)
		goto calipso_fail;

	err = seg6_init();
	if (err)
		goto seg6_fail;

	err = rpl_init();
	if (err)
		goto rpl_fail;

	err = ioam6_init();
	if (err)
		goto ioam6_fail;

	err = igmp6_late_init();
	if (err)
		goto igmp6_late_err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_SYSCTL
	err = ipv6_sysctl_register();
	if (err)
		goto sysctl_fail;
#endif
<<<<<<< HEAD
=======

	/* ensure that ipv6 stubs are visible only after ipv6 is ready */
	wmb();
	ipv6_stub = &ipv6_stub_impl;
	ipv6_bpf_stub = &ipv6_bpf_stub_impl;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return err;

#ifdef CONFIG_SYSCTL
sysctl_fail:
<<<<<<< HEAD
	ipv6_packet_cleanup();
#endif
pingv6_fail:
	pingv6_exit();
ipv6_packet_fail:
	tcpv6_exit();
tcpv6_fail:
=======
	igmp6_late_cleanup();
#endif
igmp6_late_err:
	ioam6_exit();
ioam6_fail:
	rpl_exit();
rpl_fail:
	seg6_exit();
seg6_fail:
	calipso_exit();
calipso_fail:
	pingv6_exit();
pingv6_fail:
	ipv6_packet_cleanup();
ipv6_packet_fail:
	tcpv6_exit();
tcpv6_fail:
	udpv6_offload_exit();
udpv6_offload_fail:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	udplitev6_exit();
udplitev6_fail:
	udpv6_exit();
udpv6_fail:
	ipv6_frag_exit();
ipv6_frag_fail:
	ipv6_exthdrs_exit();
ipv6_exthdrs_fail:
	addrconf_cleanup();
addrconf_fail:
<<<<<<< HEAD
=======
	ipv6_anycast_cleanup();
ipv6_anycast_fail:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ip6_flowlabel_cleanup();
ip6_flowlabel_fail:
	ndisc_late_cleanup();
ndisc_late_fail:
	ip6_route_cleanup();
ip6_route_fail:
#ifdef CONFIG_PROC_FS
	if6_proc_exit();
proc_if6_fail:
	ipv6_misc_proc_exit();
proc_misc6_fail:
	udplite6_proc_exit();
proc_udplite6_fail:
	raw6_proc_exit();
proc_raw6_fail:
#endif
	ipv6_netfilter_fini();
netfilter_fail:
	igmp6_cleanup();
igmp_fail:
	ndisc_cleanup();
ndisc_fail:
<<<<<<< HEAD
	ip6_mr_cleanup();
ipmr_fail:
	icmpv6_cleanup();
icmp_fail:
	unregister_pernet_subsys(&inet6_net_ops);
register_pernet_fail:
#ifdef CONFIG_SYSCTL
	ipv6_static_sysctl_unregister();
static_sysctl_fail:
#endif
=======
	icmpv6_cleanup();
icmp_fail:
	ip6_mr_cleanup();
ipmr_fail:
	unregister_pernet_subsys(&inet6_net_ops);
register_pernet_fail:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sock_unregister(PF_INET6);
	rtnl_unregister_all(PF_INET6);
out_sock_register_fail:
	rawv6_exit();
out_unregister_ping_proto:
	proto_unregister(&pingv6_prot);
out_unregister_raw_proto:
	proto_unregister(&rawv6_prot);
out_unregister_udplite_proto:
	proto_unregister(&udplitev6_prot);
out_unregister_udp_proto:
	proto_unregister(&udpv6_prot);
out_unregister_tcp_proto:
	proto_unregister(&tcpv6_prot);
	goto out;
}
module_init(inet6_init);

<<<<<<< HEAD
static void __exit inet6_exit(void)
{
	if (disable_ipv6_mod)
		return;

	/* First of all disallow new sockets creation. */
	sock_unregister(PF_INET6);
	/* Disallow any further netlink messages */
	rtnl_unregister_all(PF_INET6);

#ifdef CONFIG_SYSCTL
	ipv6_sysctl_unregister();
#endif
	udpv6_exit();
	udplitev6_exit();
	tcpv6_exit();

	/* Cleanup code parts. */
	ipv6_packet_cleanup();
	ipv6_frag_exit();
	ipv6_exthdrs_exit();
	addrconf_cleanup();
	ip6_flowlabel_cleanup();
	ndisc_late_cleanup();
	ip6_route_cleanup();
#ifdef CONFIG_PROC_FS

	/* Cleanup code parts. */
	if6_proc_exit();
	ipv6_misc_proc_exit();
	udplite6_proc_exit();
	raw6_proc_exit();
#endif
	ipv6_netfilter_fini();
	igmp6_cleanup();
	ndisc_cleanup();
	ip6_mr_cleanup();
	icmpv6_cleanup();
	rawv6_exit();

	unregister_pernet_subsys(&inet6_net_ops);
#ifdef CONFIG_SYSCTL
	ipv6_static_sysctl_unregister();
#endif
	proto_unregister(&rawv6_prot);
	proto_unregister(&udplitev6_prot);
	proto_unregister(&udpv6_prot);
	proto_unregister(&tcpv6_prot);

	rcu_barrier(); /* Wait for completion of call_rcu()'s */
}
module_exit(inet6_exit);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_ALIAS_NETPROTO(PF_INET6);
