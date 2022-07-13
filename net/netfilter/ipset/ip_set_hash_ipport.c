<<<<<<< HEAD
/* Copyright (C) 2003-2011 Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
=======
// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (C) 2003-2013 Jozsef Kadlecsik <kadlec@netfilter.org> */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Kernel module implementing an IP set type: the hash:ip,port type */

#include <linux/jhash.h>
#include <linux/module.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/errno.h>
#include <linux/random.h>
#include <net/ip.h>
#include <net/ipv6.h>
#include <net/netlink.h>
#include <net/tcp.h>

#include <linux/netfilter.h>
#include <linux/netfilter/ipset/pfxlen.h>
#include <linux/netfilter/ipset/ip_set.h>
<<<<<<< HEAD
#include <linux/netfilter/ipset/ip_set_timeout.h>
#include <linux/netfilter/ipset/ip_set_getport.h>
#include <linux/netfilter/ipset/ip_set_hash.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>");
MODULE_DESCRIPTION("hash:ip,port type of IP sets");
MODULE_ALIAS("ip_set_hash:ip,port");

/* Type specific function prefix */
#define TYPE		hash_ipport

static bool
hash_ipport_same_set(const struct ip_set *a, const struct ip_set *b);

#define hash_ipport4_same_set	hash_ipport_same_set
#define hash_ipport6_same_set	hash_ipport_same_set

/* The type variant functions: IPv4 */

/* Member elements without timeout */
=======
#include <linux/netfilter/ipset/ip_set_getport.h>
#include <linux/netfilter/ipset/ip_set_hash.h>

#define IPSET_TYPE_REV_MIN	0
/*				1    SCTP and UDPLITE support added */
/*				2    Counters support added */
/*				3    Comments support added */
/*				4    Forceadd support added */
/*				5    skbinfo support added */
/*				6    bucketsize, initval support added */
#define IPSET_TYPE_REV_MAX	7 /* bitmask support added */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jozsef Kadlecsik <kadlec@netfilter.org>");
IP_SET_MODULE_DESC("hash:ip,port", IPSET_TYPE_REV_MIN, IPSET_TYPE_REV_MAX);
MODULE_ALIAS("ip_set_hash:ip,port");

/* Type specific function prefix */
#define HTYPE		hash_ipport
#define IP_SET_HASH_WITH_NETMASK
#define IP_SET_HASH_WITH_BITMASK

/* IPv4 variant */

/* Member elements */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct hash_ipport4_elem {
	__be32 ip;
	__be16 port;
	u8 proto;
	u8 padding;
};

<<<<<<< HEAD
/* Member elements with timeout support */
struct hash_ipport4_telem {
	__be32 ip;
	__be16 port;
	u8 proto;
	u8 padding;
	unsigned long timeout;
};

static inline bool
=======
/* Common functions */

static bool
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
hash_ipport4_data_equal(const struct hash_ipport4_elem *ip1,
			const struct hash_ipport4_elem *ip2,
			u32 *multi)
{
	return ip1->ip == ip2->ip &&
	       ip1->port == ip2->port &&
	       ip1->proto == ip2->proto;
}

<<<<<<< HEAD
static inline bool
hash_ipport4_data_isnull(const struct hash_ipport4_elem *elem)
{
	return elem->proto == 0;
}

static inline void
hash_ipport4_data_copy(struct hash_ipport4_elem *dst,
		       const struct hash_ipport4_elem *src)
{
	dst->ip = src->ip;
	dst->port = src->port;
	dst->proto = src->proto;
}

static inline void
hash_ipport4_data_zero_out(struct hash_ipport4_elem *elem)
{
	elem->proto = 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static bool
hash_ipport4_data_list(struct sk_buff *skb,
		       const struct hash_ipport4_elem *data)
{
<<<<<<< HEAD
	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, data->ip);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, data->port);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
	return 0;

nla_put_failure:
	return 1;
}

static bool
hash_ipport4_data_tlist(struct sk_buff *skb,
			const struct hash_ipport4_elem *data)
{
	const struct hash_ipport4_telem *tdata =
		(const struct hash_ipport4_telem *)data;

	NLA_PUT_IPADDR4(skb, IPSET_ATTR_IP, tdata->ip);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, tdata->port);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
	NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
		      htonl(ip_set_timeout_get(tdata->timeout)));

	return 0;

nla_put_failure:
	return 1;
}

#define PF		4
#define HOST_MASK	32
#include <linux/netfilter/ipset/ip_set_ahash.h>

static inline void
hash_ipport4_data_next(struct ip_set_hash *h,
		       const struct hash_ipport4_elem *d)
{
	h->next.ip = ntohl(d->ip);
	h->next.port = ntohs(d->port);
}

static int
hash_ipport4_kadt(struct ip_set *set, const struct sk_buff *skb,
		  const struct xt_action_param *par,
		  enum ipset_adt adt, const struct ip_set_adt_opt *opt)
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipport4_elem data = { };

	if (!ip_set_get_ip4_port(skb, opt->flags & IPSET_DIM_TWO_SRC,
				 &data.port, &data.proto))
		return -EINVAL;

	ip4addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &data.ip);

	return adtfn(set, &data, opt_timeout(opt, h), opt->cmdflags);
=======
	if (nla_put_ipaddr4(skb, IPSET_ATTR_IP, data->ip) ||
	    nla_put_net16(skb, IPSET_ATTR_PORT, data->port) ||
	    nla_put_u8(skb, IPSET_ATTR_PROTO, data->proto))
		goto nla_put_failure;
	return false;

nla_put_failure:
	return true;
}

static void
hash_ipport4_data_next(struct hash_ipport4_elem *next,
		       const struct hash_ipport4_elem *d)
{
	next->ip = d->ip;
	next->port = d->port;
}

#define MTYPE		hash_ipport4
#define HOST_MASK	32
#include "ip_set_hash_gen.h"

static int
hash_ipport4_kadt(struct ip_set *set, const struct sk_buff *skb,
		  const struct xt_action_param *par,
		  enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipport4_elem e = { .ip = 0 };
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);
	const struct MTYPE *h = set->data;

	if (!ip_set_get_ip4_port(skb, opt->flags & IPSET_DIM_TWO_SRC,
				 &e.port, &e.proto))
		return -EINVAL;

	ip4addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &e.ip);
	e.ip &= h->bitmask.ip;
	if (e.ip == 0)
		return -EINVAL;
	return adtfn(set, &e, &ext, &opt->ext, opt->cmdflags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int
hash_ipport4_uadt(struct ip_set *set, struct nlattr *tb[],
		  enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
<<<<<<< HEAD
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipport4_elem data = { };
	u32 ip, ip_to = 0, p = 0, port, port_to;
	u32 timeout = h->timeout;
	bool with_ports = false;
	int ret;

	if (unlikely(!tb[IPSET_ATTR_IP] ||
		     !ip_set_attr_netorder(tb, IPSET_ATTR_PORT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PORT_TO) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT)))
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

	ret = ip_set_get_ipaddr4(tb[IPSET_ATTR_IP], &data.ip);
	if (ret)
		return ret;

	if (tb[IPSET_ATTR_PORT])
		data.port = nla_get_be16(tb[IPSET_ATTR_PORT]);
	else
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_PROTO]) {
		data.proto = nla_get_u8(tb[IPSET_ATTR_PROTO]);
		with_ports = ip_set_proto_with_ports(data.proto);

		if (data.proto == 0)
			return -IPSET_ERR_INVALID_PROTO;
	} else
		return -IPSET_ERR_MISSING_PROTO;

	if (!(with_ports || data.proto == IPPROTO_ICMP))
		data.port = 0;

	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!with_timeout(h->timeout))
			return -IPSET_ERR_TIMEOUT;
		timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
	}

	if (adt == IPSET_TEST ||
	    !(tb[IPSET_ATTR_IP_TO] || tb[IPSET_ATTR_CIDR] ||
	      tb[IPSET_ATTR_PORT_TO])) {
		ret = adtfn(set, &data, timeout, flags);
		return ip_set_eexist(ret, flags) ? 0 : ret;
	}

	ip = ntohl(data.ip);
=======
	struct hash_ipport4 *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipport4_elem e = { .ip = 0 };
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	u32 ip, ip_to = 0, p = 0, port, port_to, i = 0;
	bool with_ports = false;
	int ret;

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

	if (unlikely(!tb[IPSET_ATTR_IP] ||
		     !ip_set_attr_netorder(tb, IPSET_ATTR_PORT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PORT_TO)))
		return -IPSET_ERR_PROTOCOL;

	ret = ip_set_get_ipaddr4(tb[IPSET_ATTR_IP], &e.ip);
	if (ret)
		return ret;

	ret = ip_set_get_extensions(set, tb, &ext);
	if (ret)
		return ret;

	e.ip &= h->bitmask.ip;
	if (e.ip == 0)
		return -EINVAL;

	e.port = nla_get_be16(tb[IPSET_ATTR_PORT]);

	if (tb[IPSET_ATTR_PROTO]) {
		e.proto = nla_get_u8(tb[IPSET_ATTR_PROTO]);
		with_ports = ip_set_proto_with_ports(e.proto);

		if (e.proto == 0)
			return -IPSET_ERR_INVALID_PROTO;
	} else {
		return -IPSET_ERR_MISSING_PROTO;
	}

	if (!(with_ports || e.proto == IPPROTO_ICMP))
		e.port = 0;

	if (adt == IPSET_TEST ||
	    !(tb[IPSET_ATTR_IP_TO] || tb[IPSET_ATTR_CIDR] ||
	      tb[IPSET_ATTR_PORT_TO])) {
		ret = adtfn(set, &e, &ext, &ext, flags);
		return ip_set_eexist(ret, flags) ? 0 : ret;
	}

	ip_to = ip = ntohl(e.ip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (tb[IPSET_ATTR_IP_TO]) {
		ret = ip_set_get_hostipaddr4(tb[IPSET_ATTR_IP_TO], &ip_to);
		if (ret)
			return ret;
		if (ip > ip_to)
			swap(ip, ip_to);
	} else if (tb[IPSET_ATTR_CIDR]) {
		u8 cidr = nla_get_u8(tb[IPSET_ATTR_CIDR]);

<<<<<<< HEAD
		if (cidr > 32)
			return -IPSET_ERR_INVALID_CIDR;
		ip_set_mask_from_to(ip, ip_to, cidr);
	} else
		ip_to = ip;

	port_to = port = ntohs(data.port);
=======
		if (!cidr || cidr > HOST_MASK)
			return -IPSET_ERR_INVALID_CIDR;
		ip_set_mask_from_to(ip, ip_to, cidr);
	}

	port_to = port = ntohs(e.port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (with_ports && tb[IPSET_ATTR_PORT_TO]) {
		port_to = ip_set_get_h16(tb[IPSET_ATTR_PORT_TO]);
		if (port > port_to)
			swap(port, port_to);
	}

	if (retried)
<<<<<<< HEAD
		ip = h->next.ip;
	for (; !before(ip_to, ip); ip++) {
		p = retried && ip == h->next.ip ? h->next.port : port;
		for (; p <= port_to; p++) {
			data.ip = htonl(ip);
			data.port = htons(p);
			ret = adtfn(set, &data, timeout, flags);

			if (ret && !ip_set_eexist(ret, flags))
				return ret;
			else
				ret = 0;
=======
		ip = ntohl(h->next.ip);
	for (; ip <= ip_to; ip++) {
		p = retried && ip == ntohl(h->next.ip) ? ntohs(h->next.port)
						       : port;
		for (; p <= port_to; p++, i++) {
			e.ip = htonl(ip);
			e.port = htons(p);
			if (i > IPSET_MAX_RANGE) {
				hash_ipport4_data_next(&h->next, &e);
				return -ERANGE;
			}
			ret = adtfn(set, &e, &ext, &ext, flags);

			if (ret && !ip_set_eexist(ret, flags))
				return ret;

			ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	return ret;
}

<<<<<<< HEAD
static bool
hash_ipport_same_set(const struct ip_set *a, const struct ip_set *b)
{
	const struct ip_set_hash *x = a->data;
	const struct ip_set_hash *y = b->data;

	/* Resizing changes htable_bits, so we ignore it */
	return x->maxelem == y->maxelem &&
	       x->timeout == y->timeout;
}

/* The type variant functions: IPv6 */
=======
/* IPv6 variant */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct hash_ipport6_elem {
	union nf_inet_addr ip;
	__be16 port;
	u8 proto;
	u8 padding;
};

<<<<<<< HEAD
struct hash_ipport6_telem {
	union nf_inet_addr ip;
	__be16 port;
	u8 proto;
	u8 padding;
	unsigned long timeout;
};

static inline bool
=======
/* Common functions */

static bool
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
hash_ipport6_data_equal(const struct hash_ipport6_elem *ip1,
			const struct hash_ipport6_elem *ip2,
			u32 *multi)
{
<<<<<<< HEAD
	return ipv6_addr_cmp(&ip1->ip.in6, &ip2->ip.in6) == 0 &&
=======
	return ipv6_addr_equal(&ip1->ip.in6, &ip2->ip.in6) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       ip1->port == ip2->port &&
	       ip1->proto == ip2->proto;
}

<<<<<<< HEAD
static inline bool
hash_ipport6_data_isnull(const struct hash_ipport6_elem *elem)
{
	return elem->proto == 0;
}

static inline void
hash_ipport6_data_copy(struct hash_ipport6_elem *dst,
		       const struct hash_ipport6_elem *src)
{
	memcpy(dst, src, sizeof(*dst));
}

static inline void
hash_ipport6_data_zero_out(struct hash_ipport6_elem *elem)
{
	elem->proto = 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static bool
hash_ipport6_data_list(struct sk_buff *skb,
		       const struct hash_ipport6_elem *data)
{
<<<<<<< HEAD
	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP, &data->ip);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, data->port);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
	return 0;

nla_put_failure:
	return 1;
}

static bool
hash_ipport6_data_tlist(struct sk_buff *skb,
			const struct hash_ipport6_elem *data)
{
	const struct hash_ipport6_telem *e =
		(const struct hash_ipport6_telem *)data;

	NLA_PUT_IPADDR6(skb, IPSET_ATTR_IP, &e->ip);
	NLA_PUT_NET16(skb, IPSET_ATTR_PORT, data->port);
	NLA_PUT_U8(skb, IPSET_ATTR_PROTO, data->proto);
	NLA_PUT_NET32(skb, IPSET_ATTR_TIMEOUT,
		      htonl(ip_set_timeout_get(e->timeout)));
	return 0;

nla_put_failure:
	return 1;
}

#undef PF
#undef HOST_MASK

#define PF		6
#define HOST_MASK	128
#include <linux/netfilter/ipset/ip_set_ahash.h>

static inline void
hash_ipport6_data_next(struct ip_set_hash *h,
		       const struct hash_ipport6_elem *d)
{
	h->next.port = ntohs(d->port);
}

static int
hash_ipport6_kadt(struct ip_set *set, const struct sk_buff *skb,
		  const struct xt_action_param *par,
		  enum ipset_adt adt, const struct ip_set_adt_opt *opt)
{
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipport6_elem data = { };

	if (!ip_set_get_ip6_port(skb, opt->flags & IPSET_DIM_TWO_SRC,
				 &data.port, &data.proto))
		return -EINVAL;

	ip6addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &data.ip.in6);

	return adtfn(set, &data, opt_timeout(opt, h), opt->cmdflags);
=======
	if (nla_put_ipaddr6(skb, IPSET_ATTR_IP, &data->ip.in6) ||
	    nla_put_net16(skb, IPSET_ATTR_PORT, data->port) ||
	    nla_put_u8(skb, IPSET_ATTR_PROTO, data->proto))
		goto nla_put_failure;
	return false;

nla_put_failure:
	return true;
}

static void
hash_ipport6_data_next(struct hash_ipport6_elem *next,
		       const struct hash_ipport6_elem *d)
{
	next->port = d->port;
}

#undef MTYPE
#undef HOST_MASK

#define MTYPE		hash_ipport6
#define HOST_MASK	128
#define IP_SET_EMIT_CREATE
#include "ip_set_hash_gen.h"

static int
hash_ipport6_kadt(struct ip_set *set, const struct sk_buff *skb,
		  const struct xt_action_param *par,
		  enum ipset_adt adt, struct ip_set_adt_opt *opt)
{
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipport6_elem e = { .ip = { .all = { 0 } } };
	struct ip_set_ext ext = IP_SET_INIT_KEXT(skb, opt, set);
	const struct MTYPE *h = set->data;

	if (!ip_set_get_ip6_port(skb, opt->flags & IPSET_DIM_TWO_SRC,
				 &e.port, &e.proto))
		return -EINVAL;

	ip6addrptr(skb, opt->flags & IPSET_DIM_ONE_SRC, &e.ip.in6);
	nf_inet_addr_mask_inplace(&e.ip, &h->bitmask);
	if (ipv6_addr_any(&e.ip.in6))
		return -EINVAL;

	return adtfn(set, &e, &ext, &opt->ext, opt->cmdflags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int
hash_ipport6_uadt(struct ip_set *set, struct nlattr *tb[],
		  enum ipset_adt adt, u32 *lineno, u32 flags, bool retried)
{
<<<<<<< HEAD
	const struct ip_set_hash *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipport6_elem data = { };
	u32 port, port_to;
	u32 timeout = h->timeout;
	bool with_ports = false;
	int ret;

	if (unlikely(!tb[IPSET_ATTR_IP] ||
		     !ip_set_attr_netorder(tb, IPSET_ATTR_PORT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PORT_TO) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT) ||
		     tb[IPSET_ATTR_IP_TO] ||
		     tb[IPSET_ATTR_CIDR]))
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

	ret = ip_set_get_ipaddr6(tb[IPSET_ATTR_IP], &data.ip);
	if (ret)
		return ret;

	if (tb[IPSET_ATTR_PORT])
		data.port = nla_get_be16(tb[IPSET_ATTR_PORT]);
	else
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_PROTO]) {
		data.proto = nla_get_u8(tb[IPSET_ATTR_PROTO]);
		with_ports = ip_set_proto_with_ports(data.proto);

		if (data.proto == 0)
			return -IPSET_ERR_INVALID_PROTO;
	} else
		return -IPSET_ERR_MISSING_PROTO;

	if (!(with_ports || data.proto == IPPROTO_ICMPV6))
		data.port = 0;

	if (tb[IPSET_ATTR_TIMEOUT]) {
		if (!with_timeout(h->timeout))
			return -IPSET_ERR_TIMEOUT;
		timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);
	}

	if (adt == IPSET_TEST || !with_ports || !tb[IPSET_ATTR_PORT_TO]) {
		ret = adtfn(set, &data, timeout, flags);
		return ip_set_eexist(ret, flags) ? 0 : ret;
	}

	port = ntohs(data.port);
=======
	const struct hash_ipport6 *h = set->data;
	ipset_adtfn adtfn = set->variant->adt[adt];
	struct hash_ipport6_elem e = { .ip = { .all = { 0 } } };
	struct ip_set_ext ext = IP_SET_INIT_UEXT(set);
	u32 port, port_to;
	bool with_ports = false;
	int ret;

	if (tb[IPSET_ATTR_LINENO])
		*lineno = nla_get_u32(tb[IPSET_ATTR_LINENO]);

	if (unlikely(!tb[IPSET_ATTR_IP] ||
		     !ip_set_attr_netorder(tb, IPSET_ATTR_PORT) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_PORT_TO)))
		return -IPSET_ERR_PROTOCOL;
	if (unlikely(tb[IPSET_ATTR_IP_TO]))
		return -IPSET_ERR_HASH_RANGE_UNSUPPORTED;
	if (unlikely(tb[IPSET_ATTR_CIDR])) {
		u8 cidr = nla_get_u8(tb[IPSET_ATTR_CIDR]);

		if (cidr != HOST_MASK)
			return -IPSET_ERR_INVALID_CIDR;
	}

	ret = ip_set_get_ipaddr6(tb[IPSET_ATTR_IP], &e.ip);
	if (ret)
		return ret;

	ret = ip_set_get_extensions(set, tb, &ext);
	if (ret)
		return ret;

	nf_inet_addr_mask_inplace(&e.ip, &h->bitmask);
	if (ipv6_addr_any(&e.ip.in6))
		return -EINVAL;

	e.port = nla_get_be16(tb[IPSET_ATTR_PORT]);

	if (tb[IPSET_ATTR_PROTO]) {
		e.proto = nla_get_u8(tb[IPSET_ATTR_PROTO]);
		with_ports = ip_set_proto_with_ports(e.proto);

		if (e.proto == 0)
			return -IPSET_ERR_INVALID_PROTO;
	} else {
		return -IPSET_ERR_MISSING_PROTO;
	}

	if (!(with_ports || e.proto == IPPROTO_ICMPV6))
		e.port = 0;

	if (adt == IPSET_TEST || !with_ports || !tb[IPSET_ATTR_PORT_TO]) {
		ret = adtfn(set, &e, &ext, &ext, flags);
		return ip_set_eexist(ret, flags) ? 0 : ret;
	}

	port = ntohs(e.port);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	port_to = ip_set_get_h16(tb[IPSET_ATTR_PORT_TO]);
	if (port > port_to)
		swap(port, port_to);

	if (retried)
<<<<<<< HEAD
		port = h->next.port;
	for (; port <= port_to; port++) {
		data.port = htons(port);
		ret = adtfn(set, &data, timeout, flags);

		if (ret && !ip_set_eexist(ret, flags))
			return ret;
		else
			ret = 0;
=======
		port = ntohs(h->next.port);
	for (; port <= port_to; port++) {
		e.port = htons(port);
		ret = adtfn(set, &e, &ext, &ext, flags);

		if (ret && !ip_set_eexist(ret, flags))
			return ret;

		ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return ret;
}

<<<<<<< HEAD
/* Create hash:ip type of sets */

static int
hash_ipport_create(struct ip_set *set, struct nlattr *tb[], u32 flags)
{
	struct ip_set_hash *h;
	u32 hashsize = IPSET_DEFAULT_HASHSIZE, maxelem = IPSET_DEFAULT_MAXELEM;
	u8 hbits;
	size_t hsize;

	if (!(set->family == NFPROTO_IPV4 || set->family == NFPROTO_IPV6))
		return -IPSET_ERR_INVALID_FAMILY;

	if (unlikely(!ip_set_optattr_netorder(tb, IPSET_ATTR_HASHSIZE) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_MAXELEM) ||
		     !ip_set_optattr_netorder(tb, IPSET_ATTR_TIMEOUT)))
		return -IPSET_ERR_PROTOCOL;

	if (tb[IPSET_ATTR_HASHSIZE]) {
		hashsize = ip_set_get_h32(tb[IPSET_ATTR_HASHSIZE]);
		if (hashsize < IPSET_MIMINAL_HASHSIZE)
			hashsize = IPSET_MIMINAL_HASHSIZE;
	}

	if (tb[IPSET_ATTR_MAXELEM])
		maxelem = ip_set_get_h32(tb[IPSET_ATTR_MAXELEM]);

	h = kzalloc(sizeof(*h), GFP_KERNEL);
	if (!h)
		return -ENOMEM;

	h->maxelem = maxelem;
	get_random_bytes(&h->initval, sizeof(h->initval));
	h->timeout = IPSET_NO_TIMEOUT;

	hbits = htable_bits(hashsize);
	hsize = htable_size(hbits);
	if (hsize == 0) {
		kfree(h);
		return -ENOMEM;
	}
	h->table = ip_set_alloc(hsize);
	if (!h->table) {
		kfree(h);
		return -ENOMEM;
	}
	h->table->htable_bits = hbits;

	set->data = h;

	if (tb[IPSET_ATTR_TIMEOUT]) {
		h->timeout = ip_set_timeout_uget(tb[IPSET_ATTR_TIMEOUT]);

		set->variant = set->family == NFPROTO_IPV4
			? &hash_ipport4_tvariant : &hash_ipport6_tvariant;

		if (set->family == NFPROTO_IPV4)
			hash_ipport4_gc_init(set);
		else
			hash_ipport6_gc_init(set);
	} else {
		set->variant = set->family == NFPROTO_IPV4
			? &hash_ipport4_variant : &hash_ipport6_variant;
	}

	pr_debug("create %s hashsize %u (%u) maxelem %u: %p(%p)\n",
		 set->name, jhash_size(h->table->htable_bits),
		 h->table->htable_bits, h->maxelem, set->data, h->table);

	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct ip_set_type hash_ipport_type __read_mostly = {
	.name		= "hash:ip,port",
	.protocol	= IPSET_PROTOCOL,
	.features	= IPSET_TYPE_IP | IPSET_TYPE_PORT,
	.dimension	= IPSET_DIM_TWO,
	.family		= NFPROTO_UNSPEC,
<<<<<<< HEAD
	.revision_min	= 0,
	.revision_max	= 1,	/* SCTP and UDPLITE support added */
=======
	.revision_min	= IPSET_TYPE_REV_MIN,
	.revision_max	= IPSET_TYPE_REV_MAX,
	.create_flags[IPSET_TYPE_REV_MAX] = IPSET_CREATE_FLAG_BUCKETSIZE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.create		= hash_ipport_create,
	.create_policy	= {
		[IPSET_ATTR_HASHSIZE]	= { .type = NLA_U32 },
		[IPSET_ATTR_MAXELEM]	= { .type = NLA_U32 },
<<<<<<< HEAD
		[IPSET_ATTR_PROBES]	= { .type = NLA_U8 },
		[IPSET_ATTR_RESIZE]	= { .type = NLA_U8  },
		[IPSET_ATTR_PROTO]	= { .type = NLA_U8 },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
=======
		[IPSET_ATTR_INITVAL]	= { .type = NLA_U32 },
		[IPSET_ATTR_BUCKETSIZE]	= { .type = NLA_U8 },
		[IPSET_ATTR_RESIZE]	= { .type = NLA_U8  },
		[IPSET_ATTR_PROTO]	= { .type = NLA_U8 },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
		[IPSET_ATTR_CADT_FLAGS]	= { .type = NLA_U32 },
		[IPSET_ATTR_NETMASK]	= { .type = NLA_U8 },
		[IPSET_ATTR_BITMASK]	= { .type = NLA_NESTED },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	.adt_policy	= {
		[IPSET_ATTR_IP]		= { .type = NLA_NESTED },
		[IPSET_ATTR_IP_TO]	= { .type = NLA_NESTED },
		[IPSET_ATTR_PORT]	= { .type = NLA_U16 },
		[IPSET_ATTR_PORT_TO]	= { .type = NLA_U16 },
		[IPSET_ATTR_CIDR]	= { .type = NLA_U8 },
		[IPSET_ATTR_PROTO]	= { .type = NLA_U8 },
		[IPSET_ATTR_TIMEOUT]	= { .type = NLA_U32 },
		[IPSET_ATTR_LINENO]	= { .type = NLA_U32 },
<<<<<<< HEAD
=======
		[IPSET_ATTR_BYTES]	= { .type = NLA_U64 },
		[IPSET_ATTR_PACKETS]	= { .type = NLA_U64 },
		[IPSET_ATTR_COMMENT]	= { .type = NLA_NUL_STRING,
					    .len  = IPSET_MAX_COMMENT_SIZE },
		[IPSET_ATTR_SKBMARK]	= { .type = NLA_U64 },
		[IPSET_ATTR_SKBPRIO]	= { .type = NLA_U32 },
		[IPSET_ATTR_SKBQUEUE]	= { .type = NLA_U16 },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	.me		= THIS_MODULE,
};

static int __init
hash_ipport_init(void)
{
	return ip_set_type_register(&hash_ipport_type);
}

static void __exit
hash_ipport_fini(void)
{
<<<<<<< HEAD
=======
	rcu_barrier();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ip_set_type_unregister(&hash_ipport_type);
}

module_init(hash_ipport_init);
module_exit(hash_ipport_fini);
