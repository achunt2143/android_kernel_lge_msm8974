<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * IP Payload Compression Protocol (IPComp) - RFC3173.
 *
 * Copyright (c) 2003 James Morris <jmorris@intercode.com.au>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Todo:
 *   - Tunable compression parameters.
 *   - Compression stats.
 *   - Adaptive compression.
 */
#include <linux/module.h>
#include <linux/err.h>
#include <linux/rtnetlink.h>
#include <net/ip.h>
#include <net/xfrm.h>
#include <net/icmp.h>
#include <net/ipcomp.h>
#include <net/protocol.h>
#include <net/sock.h>

<<<<<<< HEAD
static void ipcomp4_err(struct sk_buff *skb, u32 info)
=======
static int ipcomp4_err(struct sk_buff *skb, u32 info)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net *net = dev_net(skb->dev);
	__be32 spi;
	const struct iphdr *iph = (const struct iphdr *)skb->data;
	struct ip_comp_hdr *ipch = (struct ip_comp_hdr *)(skb->data+(iph->ihl<<2));
	struct xfrm_state *x;

<<<<<<< HEAD
	if (icmp_hdr(skb)->type != ICMP_DEST_UNREACH ||
	    icmp_hdr(skb)->code != ICMP_FRAG_NEEDED)
		return;
=======
	switch (icmp_hdr(skb)->type) {
	case ICMP_DEST_UNREACH:
		if (icmp_hdr(skb)->code != ICMP_FRAG_NEEDED)
			return 0;
		break;
	case ICMP_REDIRECT:
		break;
	default:
		return 0;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spi = htonl(ntohs(ipch->cpi));
	x = xfrm_state_lookup(net, skb->mark, (const xfrm_address_t *)&iph->daddr,
			      spi, IPPROTO_COMP, AF_INET);
	if (!x)
<<<<<<< HEAD
		return;
	NETDEBUG(KERN_DEBUG "pmtu discovery on SA IPCOMP/%08x/%pI4\n",
		 spi, &iph->daddr);
	xfrm_state_put(x);
=======
		return 0;

	if (icmp_hdr(skb)->type == ICMP_DEST_UNREACH)
		ipv4_update_pmtu(skb, net, info, 0, IPPROTO_COMP);
	else
		ipv4_redirect(skb, net, 0, IPPROTO_COMP);
	xfrm_state_put(x);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* We always hold one tunnel user reference to indicate a tunnel */
static struct xfrm_state *ipcomp_tunnel_create(struct xfrm_state *x)
{
	struct net *net = xs_net(x);
	struct xfrm_state *t;

	t = xfrm_state_alloc(net);
<<<<<<< HEAD
	if (t == NULL)
=======
	if (!t)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	t->id.proto = IPPROTO_IPIP;
	t->id.spi = x->props.saddr.a4;
	t->id.daddr.a4 = x->id.daddr.a4;
	memcpy(&t->sel, &x->sel, sizeof(t->sel));
	t->props.family = AF_INET;
	t->props.mode = x->props.mode;
	t->props.saddr.a4 = x->props.saddr.a4;
	t->props.flags = x->props.flags;
<<<<<<< HEAD
	memcpy(&t->mark, &x->mark, sizeof(t->mark));
=======
	t->props.extra_flags = x->props.extra_flags;
	memcpy(&t->mark, &x->mark, sizeof(t->mark));
	t->if_id = x->if_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (xfrm_init_state(t))
		goto error;

	atomic_set(&t->tunnel_users, 1);
out:
	return t;

error:
	t->km.state = XFRM_STATE_DEAD;
	xfrm_state_put(t);
	t = NULL;
	goto out;
}

/*
 * Must be protected by xfrm_cfg_mutex.  State and tunnel user references are
 * always incremented on success.
 */
static int ipcomp_tunnel_attach(struct xfrm_state *x)
{
	struct net *net = xs_net(x);
	int err = 0;
	struct xfrm_state *t;
	u32 mark = x->mark.v & x->mark.m;

	t = xfrm_state_lookup(net, mark, (xfrm_address_t *)&x->id.daddr.a4,
			      x->props.saddr.a4, IPPROTO_IPIP, AF_INET);
	if (!t) {
		t = ipcomp_tunnel_create(x);
		if (!t) {
			err = -EINVAL;
			goto out;
		}
		xfrm_state_insert(t);
		xfrm_state_hold(t);
	}
	x->tunnel = t;
	atomic_inc(&t->tunnel_users);
out:
	return err;
}

<<<<<<< HEAD
static int ipcomp4_init_state(struct xfrm_state *x)
=======
static int ipcomp4_init_state(struct xfrm_state *x,
			      struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err = -EINVAL;

	x->props.header_len = 0;
	switch (x->props.mode) {
	case XFRM_MODE_TRANSPORT:
		break;
	case XFRM_MODE_TUNNEL:
		x->props.header_len += sizeof(struct iphdr);
		break;
	default:
<<<<<<< HEAD
		goto out;
	}

	err = ipcomp_init_state(x);
=======
		NL_SET_ERR_MSG(extack, "Unsupported XFRM mode for IPcomp");
		goto out;
	}

	err = ipcomp_init_state(x, extack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		goto out;

	if (x->props.mode == XFRM_MODE_TUNNEL) {
		err = ipcomp_tunnel_attach(x);
<<<<<<< HEAD
		if (err)
			goto out;
=======
		if (err) {
			NL_SET_ERR_MSG(extack, "Kernel error: failed to initialize the associated state");
			goto out;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	err = 0;
out:
	return err;
}

<<<<<<< HEAD
static const struct xfrm_type ipcomp_type = {
	.description	= "IPCOMP4",
=======
static int ipcomp4_rcv_cb(struct sk_buff *skb, int err)
{
	return 0;
}

static const struct xfrm_type ipcomp_type = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.owner		= THIS_MODULE,
	.proto	     	= IPPROTO_COMP,
	.init_state	= ipcomp4_init_state,
	.destructor	= ipcomp_destroy,
	.input		= ipcomp_input,
	.output		= ipcomp_output
};

<<<<<<< HEAD
static const struct net_protocol ipcomp4_protocol = {
	.handler	=	xfrm4_rcv,
	.err_handler	=	ipcomp4_err,
	.no_policy	=	1,
=======
static struct xfrm4_protocol ipcomp4_protocol = {
	.handler	=	xfrm4_rcv,
	.input_handler	=	xfrm_input,
	.cb_handler	=	ipcomp4_rcv_cb,
	.err_handler	=	ipcomp4_err,
	.priority	=	0,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init ipcomp4_init(void)
{
	if (xfrm_register_type(&ipcomp_type, AF_INET) < 0) {
		pr_info("%s: can't add xfrm type\n", __func__);
		return -EAGAIN;
	}
<<<<<<< HEAD
	if (inet_add_protocol(&ipcomp4_protocol, IPPROTO_COMP) < 0) {
=======
	if (xfrm4_protocol_register(&ipcomp4_protocol, IPPROTO_COMP) < 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pr_info("%s: can't add protocol\n", __func__);
		xfrm_unregister_type(&ipcomp_type, AF_INET);
		return -EAGAIN;
	}
	return 0;
}

static void __exit ipcomp4_fini(void)
{
<<<<<<< HEAD
	if (inet_del_protocol(&ipcomp4_protocol, IPPROTO_COMP) < 0)
		pr_info("%s: can't remove protocol\n", __func__);
	if (xfrm_unregister_type(&ipcomp_type, AF_INET) < 0)
		pr_info("%s: can't remove xfrm type\n", __func__);
=======
	if (xfrm4_protocol_deregister(&ipcomp4_protocol, IPPROTO_COMP) < 0)
		pr_info("%s: can't remove protocol\n", __func__);
	xfrm_unregister_type(&ipcomp_type, AF_INET);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(ipcomp4_init);
module_exit(ipcomp4_fini);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("IP Payload Compression Protocol (IPComp/IPv4) - RFC3173");
MODULE_AUTHOR("James Morris <jmorris@intercode.com.au>");

MODULE_ALIAS_XFRM_TYPE(AF_INET, XFRM_PROTO_COMP);
