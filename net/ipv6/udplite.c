<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  UDPLITEv6   An implementation of the UDP-Lite protocol over IPv6.
 *              See also net/ipv4/udplite.c
 *
 *  Authors:    Gerrit Renker       <gerrit@erg.abdn.ac.uk>
 *
 *  Changes:
 *  Fixes:
<<<<<<< HEAD
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 */
#include <linux/export.h>
#include "udp_impl.h"

=======
 */
#define pr_fmt(fmt) "UDPLite6: " fmt

#include <linux/export.h>
#include <linux/proc_fs.h>
#include "udp_impl.h"

static int udplitev6_sk_init(struct sock *sk)
{
	udpv6_init_sock(sk);
	pr_warn_once("UDP-Lite is deprecated and scheduled to be removed in 2025, "
		     "please contact the netdev mailing list\n");
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int udplitev6_rcv(struct sk_buff *skb)
{
	return __udp6_lib_rcv(skb, &udplite_table, IPPROTO_UDPLITE);
}

<<<<<<< HEAD
static void udplitev6_err(struct sk_buff *skb,
			  struct inet6_skb_parm *opt,
			  u8 type, u8 code, int offset, __be32 info)
{
	__udp6_lib_err(skb, opt, type, code, offset, info, &udplite_table);
=======
static int udplitev6_err(struct sk_buff *skb,
			  struct inet6_skb_parm *opt,
			  u8 type, u8 code, int offset, __be32 info)
{
	return __udp6_lib_err(skb, opt, type, code, offset, info,
			      &udplite_table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct inet6_protocol udplitev6_protocol = {
	.handler	=	udplitev6_rcv,
	.err_handler	=	udplitev6_err,
	.flags		=	INET6_PROTO_NOPOLICY|INET6_PROTO_FINAL,
};

struct proto udplitev6_prot = {
	.name		   = "UDPLITEv6",
	.owner		   = THIS_MODULE,
	.close		   = udp_lib_close,
	.connect	   = ip6_datagram_connect,
	.disconnect	   = udp_disconnect,
	.ioctl		   = udp_ioctl,
<<<<<<< HEAD
	.init		   = udplite_sk_init,
=======
	.init		   = udplitev6_sk_init,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.destroy	   = udpv6_destroy_sock,
	.setsockopt	   = udpv6_setsockopt,
	.getsockopt	   = udpv6_getsockopt,
	.sendmsg	   = udpv6_sendmsg,
	.recvmsg	   = udpv6_recvmsg,
<<<<<<< HEAD
	.backlog_rcv	   = udpv6_queue_rcv_skb,
	.hash		   = udp_lib_hash,
	.unhash		   = udp_lib_unhash,
	.get_port	   = udp_v6_get_port,
	.obj_size	   = sizeof(struct udp6_sock),
	.slab_flags	   = SLAB_DESTROY_BY_RCU,
	.h.udp_table	   = &udplite_table,
#ifdef CONFIG_COMPAT
	.compat_setsockopt = compat_udpv6_setsockopt,
	.compat_getsockopt = compat_udpv6_getsockopt,
#endif
	.clear_sk	   = udp_v6_clear_sk,
=======
	.hash		   = udp_lib_hash,
	.unhash		   = udp_lib_unhash,
	.rehash		   = udp_v6_rehash,
	.get_port	   = udp_v6_get_port,

	.memory_allocated  = &udp_memory_allocated,
	.per_cpu_fw_alloc  = &udp_memory_per_cpu_fw_alloc,

	.sysctl_mem	   = sysctl_udp_mem,
	.sysctl_wmem_offset = offsetof(struct net, ipv4.sysctl_udp_wmem_min),
	.sysctl_rmem_offset = offsetof(struct net, ipv4.sysctl_udp_rmem_min),
	.obj_size	   = sizeof(struct udp6_sock),
	.ipv6_pinfo_offset = offsetof(struct udp6_sock, inet6),
	.h.udp_table	   = &udplite_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct inet_protosw udplite6_protosw = {
	.type		= SOCK_DGRAM,
	.protocol	= IPPROTO_UDPLITE,
	.prot		= &udplitev6_prot,
	.ops		= &inet6_dgram_ops,
<<<<<<< HEAD
	.no_check	= 0,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.flags		= INET_PROTOSW_PERMANENT,
};

int __init udplitev6_init(void)
{
	int ret;

	ret = inet6_add_protocol(&udplitev6_protocol, IPPROTO_UDPLITE);
	if (ret)
		goto out;

	ret = inet6_register_protosw(&udplite6_protosw);
	if (ret)
		goto out_udplitev6_protocol;
out:
	return ret;

out_udplitev6_protocol:
	inet6_del_protocol(&udplitev6_protocol, IPPROTO_UDPLITE);
	goto out;
}

void udplitev6_exit(void)
{
	inet6_unregister_protosw(&udplite6_protosw);
	inet6_del_protocol(&udplitev6_protocol, IPPROTO_UDPLITE);
}

#ifdef CONFIG_PROC_FS
<<<<<<< HEAD

static const struct file_operations udplite6_afinfo_seq_fops = {
	.owner    = THIS_MODULE,
	.open     = udp_seq_open,
	.read     = seq_read,
	.llseek   = seq_lseek,
	.release  = seq_release_net
};

static struct udp_seq_afinfo udplite6_seq_afinfo = {
	.name		= "udplite6",
	.family		= AF_INET6,
	.udp_table	= &udplite_table,
	.seq_fops	= &udplite6_afinfo_seq_fops,
	.seq_ops	= {
		.show		= udp6_seq_show,
	},
=======
static struct udp_seq_afinfo udplite6_seq_afinfo = {
	.family		= AF_INET6,
	.udp_table	= &udplite_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __net_init udplite6_proc_init_net(struct net *net)
{
<<<<<<< HEAD
	return udp_proc_register(net, &udplite6_seq_afinfo);
=======
	if (!proc_create_net_data("udplite6", 0444, net->proc_net,
			&udp6_seq_ops, sizeof(struct udp_iter_state),
			&udplite6_seq_afinfo))
		return -ENOMEM;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __net_exit udplite6_proc_exit_net(struct net *net)
{
<<<<<<< HEAD
	udp_proc_unregister(net, &udplite6_seq_afinfo);
=======
	remove_proc_entry("udplite6", net->proc_net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pernet_operations udplite6_net_ops = {
	.init = udplite6_proc_init_net,
	.exit = udplite6_proc_exit_net,
};

int __init udplite6_proc_init(void)
{
	return register_pernet_subsys(&udplite6_net_ops);
}

void udplite6_proc_exit(void)
{
	unregister_pernet_subsys(&udplite6_net_ops);
}
#endif
