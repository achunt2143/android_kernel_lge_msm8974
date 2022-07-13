<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Amanda extension for IP connection tracking
 *
 * (C) 2002 by Brian J. Murrell <netfilter@interlinx.bc.ca>
 * based on HW's ip_conntrack_irc.c as well as other modules
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
 * (C) 2006 Patrick McHardy <kaber@trash.net>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/textsearch.h>
#include <linux/skbuff.h>
#include <linux/in.h>
#include <linux/udp.h>
#include <linux/netfilter.h>
#include <linux/gfp.h>

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_conntrack_ecache.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <linux/netfilter/nf_conntrack_amanda.h>

static unsigned int master_timeout __read_mostly = 300;
static char *ts_algo = "kmp";

<<<<<<< HEAD
=======
#define HELPER_NAME "amanda"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_AUTHOR("Brian J. Murrell <netfilter@interlinx.bc.ca>");
MODULE_DESCRIPTION("Amanda connection tracking module");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ip_conntrack_amanda");
<<<<<<< HEAD
MODULE_ALIAS_NFCT_HELPER("amanda");
=======
MODULE_ALIAS_NFCT_HELPER(HELPER_NAME);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

module_param(master_timeout, uint, 0600);
MODULE_PARM_DESC(master_timeout, "timeout for the master connection");
module_param(ts_algo, charp, 0400);
MODULE_PARM_DESC(ts_algo, "textsearch algorithm to use (default kmp)");

unsigned int (*nf_nat_amanda_hook)(struct sk_buff *skb,
				   enum ip_conntrack_info ctinfo,
				   unsigned int protoff,
				   unsigned int matchoff,
				   unsigned int matchlen,
				   struct nf_conntrack_expect *exp)
				   __read_mostly;
EXPORT_SYMBOL_GPL(nf_nat_amanda_hook);

enum amanda_strings {
	SEARCH_CONNECT,
	SEARCH_NEWLINE,
	SEARCH_DATA,
	SEARCH_MESG,
	SEARCH_INDEX,
<<<<<<< HEAD
=======
	SEARCH_STATE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct {
	const char		*string;
	size_t			len;
	struct ts_config	*ts;
} search[] __read_mostly = {
	[SEARCH_CONNECT] = {
		.string	= "CONNECT ",
		.len	= 8,
	},
	[SEARCH_NEWLINE] = {
		.string	= "\n",
		.len	= 1,
	},
	[SEARCH_DATA] = {
		.string	= "DATA ",
		.len	= 5,
	},
	[SEARCH_MESG] = {
		.string	= "MESG ",
		.len	= 5,
	},
	[SEARCH_INDEX] = {
		.string = "INDEX ",
		.len	= 6,
	},
<<<<<<< HEAD
=======
	[SEARCH_STATE] = {
		.string = "STATE ",
		.len	= 6,
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int amanda_help(struct sk_buff *skb,
		       unsigned int protoff,
		       struct nf_conn *ct,
		       enum ip_conntrack_info ctinfo)
{
<<<<<<< HEAD
	struct ts_state ts;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nf_conntrack_expect *exp;
	struct nf_conntrack_tuple *tuple;
	unsigned int dataoff, start, stop, off, i;
	char pbuf[sizeof("65535")], *tmp;
	u_int16_t len;
	__be16 port;
	int ret = NF_ACCEPT;
	typeof(nf_nat_amanda_hook) nf_nat_amanda;

	/* Only look at packets from the Amanda server */
	if (CTINFO2DIR(ctinfo) == IP_CT_DIR_ORIGINAL)
		return NF_ACCEPT;

	/* increase the UDP timeout of the master connection as replies from
	 * Amanda clients to the server can be quite delayed */
	nf_ct_refresh(ct, skb, master_timeout * HZ);

	/* No data? */
	dataoff = protoff + sizeof(struct udphdr);
	if (dataoff >= skb->len) {
<<<<<<< HEAD
		if (net_ratelimit())
			printk(KERN_ERR "amanda_help: skblen = %u\n", skb->len);
		return NF_ACCEPT;
	}

	memset(&ts, 0, sizeof(ts));
	start = skb_find_text(skb, dataoff, skb->len,
			      search[SEARCH_CONNECT].ts, &ts);
=======
		net_err_ratelimited("amanda_help: skblen = %u\n", skb->len);
		return NF_ACCEPT;
	}

	start = skb_find_text(skb, dataoff, skb->len,
			      search[SEARCH_CONNECT].ts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (start == UINT_MAX)
		goto out;
	start += dataoff + search[SEARCH_CONNECT].len;

<<<<<<< HEAD
	memset(&ts, 0, sizeof(ts));
	stop = skb_find_text(skb, start, skb->len,
			     search[SEARCH_NEWLINE].ts, &ts);
=======
	stop = skb_find_text(skb, start, skb->len,
			     search[SEARCH_NEWLINE].ts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (stop == UINT_MAX)
		goto out;
	stop += start;

<<<<<<< HEAD
	for (i = SEARCH_DATA; i <= SEARCH_INDEX; i++) {
		memset(&ts, 0, sizeof(ts));
		off = skb_find_text(skb, start, stop, search[i].ts, &ts);
=======
	for (i = SEARCH_DATA; i <= SEARCH_STATE; i++) {
		off = skb_find_text(skb, start, stop, search[i].ts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (off == UINT_MAX)
			continue;
		off += start + search[i].len;

		len = min_t(unsigned int, sizeof(pbuf) - 1, stop - off);
		if (skb_copy_bits(skb, off, pbuf, len))
			break;
		pbuf[len] = '\0';

		port = htons(simple_strtoul(pbuf, &tmp, 10));
		len = tmp - pbuf;
		if (port == 0 || len > 5)
			break;

		exp = nf_ct_expect_alloc(ct);
		if (exp == NULL) {
<<<<<<< HEAD
=======
			nf_ct_helper_log(skb, ct, "cannot alloc expectation");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = NF_DROP;
			goto out;
		}
		tuple = &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple;
		nf_ct_expect_init(exp, NF_CT_EXPECT_CLASS_DEFAULT,
				  nf_ct_l3num(ct),
				  &tuple->src.u3, &tuple->dst.u3,
				  IPPROTO_TCP, NULL, &port);

		nf_nat_amanda = rcu_dereference(nf_nat_amanda_hook);
		if (nf_nat_amanda && ct->status & IPS_NAT_MASK)
			ret = nf_nat_amanda(skb, ctinfo, protoff,
					    off - dataoff, len, exp);
<<<<<<< HEAD
		else if (nf_ct_expect_related(exp) != 0)
			ret = NF_DROP;
=======
		else if (nf_ct_expect_related(exp, 0) != 0) {
			nf_ct_helper_log(skb, ct, "cannot add expectation");
			ret = NF_DROP;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		nf_ct_expect_put(exp);
	}

out:
	return ret;
}

static const struct nf_conntrack_expect_policy amanda_exp_policy = {
<<<<<<< HEAD
	.max_expected		= 3,
=======
	.max_expected		= 4,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.timeout		= 180,
};

static struct nf_conntrack_helper amanda_helper[2] __read_mostly = {
	{
<<<<<<< HEAD
		.name			= "amanda",
=======
		.name			= HELPER_NAME,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.me			= THIS_MODULE,
		.help			= amanda_help,
		.tuple.src.l3num	= AF_INET,
		.tuple.src.u.udp.port	= cpu_to_be16(10080),
		.tuple.dst.protonum	= IPPROTO_UDP,
		.expect_policy		= &amanda_exp_policy,
<<<<<<< HEAD
=======
		.nat_mod_name		= NF_NAT_HELPER_NAME(HELPER_NAME),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	{
		.name			= "amanda",
		.me			= THIS_MODULE,
		.help			= amanda_help,
		.tuple.src.l3num	= AF_INET6,
		.tuple.src.u.udp.port	= cpu_to_be16(10080),
		.tuple.dst.protonum	= IPPROTO_UDP,
		.expect_policy		= &amanda_exp_policy,
<<<<<<< HEAD
=======
		.nat_mod_name		= NF_NAT_HELPER_NAME(HELPER_NAME),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static void __exit nf_conntrack_amanda_fini(void)
{
	int i;

<<<<<<< HEAD
	nf_conntrack_helper_unregister(&amanda_helper[0]);
	nf_conntrack_helper_unregister(&amanda_helper[1]);
=======
	nf_conntrack_helpers_unregister(amanda_helper,
					ARRAY_SIZE(amanda_helper));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < ARRAY_SIZE(search); i++)
		textsearch_destroy(search[i].ts);
}

static int __init nf_conntrack_amanda_init(void)
{
	int ret, i;

<<<<<<< HEAD
=======
	NF_CT_HELPER_BUILD_BUG_ON(0);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < ARRAY_SIZE(search); i++) {
		search[i].ts = textsearch_prepare(ts_algo, search[i].string,
						  search[i].len,
						  GFP_KERNEL, TS_AUTOLOAD);
		if (IS_ERR(search[i].ts)) {
			ret = PTR_ERR(search[i].ts);
			goto err1;
		}
	}
<<<<<<< HEAD
	ret = nf_conntrack_helper_register(&amanda_helper[0]);
	if (ret < 0)
		goto err1;
	ret = nf_conntrack_helper_register(&amanda_helper[1]);
	if (ret < 0)
		goto err2;
	return 0;

err2:
	nf_conntrack_helper_unregister(&amanda_helper[0]);
=======
	ret = nf_conntrack_helpers_register(amanda_helper,
					    ARRAY_SIZE(amanda_helper));
	if (ret < 0)
		goto err1;
	return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err1:
	while (--i >= 0)
		textsearch_destroy(search[i].ts);

	return ret;
}

module_init(nf_conntrack_amanda_init);
module_exit(nf_conntrack_amanda_fini);
