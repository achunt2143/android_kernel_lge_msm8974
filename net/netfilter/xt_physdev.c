<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Kernel module to match the bridge port in and
 * out device for IP packets coming into contact with a bridge. */

/* (C) 2001-2003 Bart De Schuymer <bdschuym@pandora.be>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netfilter_bridge.h>
#include <linux/netfilter/xt_physdev.h>
#include <linux/netfilter/x_tables.h>
=======
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/if.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netfilter_bridge.h>
#include <linux/netfilter/x_tables.h>
#include <uapi/linux/netfilter/xt_physdev.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bart De Schuymer <bdschuym@pandora.be>");
MODULE_DESCRIPTION("Xtables: Bridge physical device match");
MODULE_ALIAS("ipt_physdev");
MODULE_ALIAS("ip6t_physdev");


static bool
physdev_mt(const struct sk_buff *skb, struct xt_action_param *par)
{
<<<<<<< HEAD
	static const char nulldevname[IFNAMSIZ] __attribute__((aligned(sizeof(long))));
	const struct xt_physdev_info *info = par->matchinfo;
	unsigned long ret;
	const char *indev, *outdev;
	const struct nf_bridge_info *nf_bridge;
=======
	const struct xt_physdev_info *info = par->matchinfo;
	const struct net_device *physdev;
	unsigned long ret;
	const char *indev, *outdev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Not a bridged IP packet or no info available yet:
	 * LOCAL_OUT/mangle and LOCAL_OUT/nat don't know if
	 * the destination device will be a bridge. */
<<<<<<< HEAD
	if (!(nf_bridge = skb->nf_bridge)) {
=======
	if (!nf_bridge_info_exists(skb)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Return MATCH if the invert flags of the used options are on */
		if ((info->bitmask & XT_PHYSDEV_OP_BRIDGED) &&
		    !(info->invert & XT_PHYSDEV_OP_BRIDGED))
			return false;
		if ((info->bitmask & XT_PHYSDEV_OP_ISIN) &&
		    !(info->invert & XT_PHYSDEV_OP_ISIN))
			return false;
		if ((info->bitmask & XT_PHYSDEV_OP_ISOUT) &&
		    !(info->invert & XT_PHYSDEV_OP_ISOUT))
			return false;
		if ((info->bitmask & XT_PHYSDEV_OP_IN) &&
		    !(info->invert & XT_PHYSDEV_OP_IN))
			return false;
		if ((info->bitmask & XT_PHYSDEV_OP_OUT) &&
		    !(info->invert & XT_PHYSDEV_OP_OUT))
			return false;
		return true;
	}

<<<<<<< HEAD
	/* This only makes sense in the FORWARD and POSTROUTING chains */
	if ((info->bitmask & XT_PHYSDEV_OP_BRIDGED) &&
	    (!!(nf_bridge->mask & BRNF_BRIDGED) ^
	    !(info->invert & XT_PHYSDEV_OP_BRIDGED)))
		return false;

	if ((info->bitmask & XT_PHYSDEV_OP_ISIN &&
	    (!nf_bridge->physindev ^ !!(info->invert & XT_PHYSDEV_OP_ISIN))) ||
	    (info->bitmask & XT_PHYSDEV_OP_ISOUT &&
	    (!nf_bridge->physoutdev ^ !!(info->invert & XT_PHYSDEV_OP_ISOUT))))
=======
	physdev = nf_bridge_get_physoutdev(skb);
	outdev = physdev ? physdev->name : NULL;

	/* This only makes sense in the FORWARD and POSTROUTING chains */
	if ((info->bitmask & XT_PHYSDEV_OP_BRIDGED) &&
	    (!!outdev ^ !(info->invert & XT_PHYSDEV_OP_BRIDGED)))
		return false;

	physdev = nf_bridge_get_physindev(skb, xt_net(par));
	indev = physdev ? physdev->name : NULL;

	if ((info->bitmask & XT_PHYSDEV_OP_ISIN &&
	    (!indev ^ !!(info->invert & XT_PHYSDEV_OP_ISIN))) ||
	    (info->bitmask & XT_PHYSDEV_OP_ISOUT &&
	    (!outdev ^ !!(info->invert & XT_PHYSDEV_OP_ISOUT))))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return false;

	if (!(info->bitmask & XT_PHYSDEV_OP_IN))
		goto match_outdev;
<<<<<<< HEAD
	indev = nf_bridge->physindev ? nf_bridge->physindev->name : nulldevname;
	ret = ifname_compare_aligned(indev, info->physindev, info->in_mask);

	if (!ret ^ !(info->invert & XT_PHYSDEV_OP_IN))
		return false;
=======

	if (indev) {
		ret = ifname_compare_aligned(indev, info->physindev,
					     info->in_mask);

		if (!ret ^ !(info->invert & XT_PHYSDEV_OP_IN))
			return false;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

match_outdev:
	if (!(info->bitmask & XT_PHYSDEV_OP_OUT))
		return true;
<<<<<<< HEAD
	outdev = nf_bridge->physoutdev ?
		 nf_bridge->physoutdev->name : nulldevname;
=======

	if (!outdev)
		return false;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = ifname_compare_aligned(outdev, info->physoutdev, info->out_mask);

	return (!!ret ^ !(info->invert & XT_PHYSDEV_OP_OUT));
}

static int physdev_mt_check(const struct xt_mtchk_param *par)
{
	const struct xt_physdev_info *info = par->matchinfo;
<<<<<<< HEAD
=======
	static bool brnf_probed __read_mostly;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!(info->bitmask & XT_PHYSDEV_OP_MASK) ||
	    info->bitmask & ~XT_PHYSDEV_OP_MASK)
		return -EINVAL;
<<<<<<< HEAD
	if (info->bitmask & XT_PHYSDEV_OP_OUT &&
	    (!(info->bitmask & XT_PHYSDEV_OP_BRIDGED) ||
	     info->invert & XT_PHYSDEV_OP_BRIDGED) &&
	    par->hook_mask & ((1 << NF_INET_LOCAL_OUT) |
	    (1 << NF_INET_FORWARD) | (1 << NF_INET_POST_ROUTING))) {
		pr_info("using --physdev-out in the OUTPUT, FORWARD and "
			"POSTROUTING chains for non-bridged traffic is not "
			"supported anymore.\n");
		if (par->hook_mask & (1 << NF_INET_LOCAL_OUT))
			return -EINVAL;
	}
=======
	if (info->bitmask & (XT_PHYSDEV_OP_OUT | XT_PHYSDEV_OP_ISOUT) &&
	    (!(info->bitmask & XT_PHYSDEV_OP_BRIDGED) ||
	     info->invert & XT_PHYSDEV_OP_BRIDGED) &&
	    par->hook_mask & (1 << NF_INET_LOCAL_OUT)) {
		pr_info_ratelimited("--physdev-out and --physdev-is-out only supported in the FORWARD and POSTROUTING chains with bridged traffic\n");
		return -EINVAL;
	}

	if (!brnf_probed) {
		brnf_probed = true;
		request_module("br_netfilter");
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static struct xt_match physdev_mt_reg __read_mostly = {
	.name       = "physdev",
	.revision   = 0,
	.family     = NFPROTO_UNSPEC,
	.checkentry = physdev_mt_check,
	.match      = physdev_mt,
	.matchsize  = sizeof(struct xt_physdev_info),
	.me         = THIS_MODULE,
};

static int __init physdev_mt_init(void)
{
	return xt_register_match(&physdev_mt_reg);
}

static void __exit physdev_mt_exit(void)
{
	xt_unregister_match(&physdev_mt_reg);
}

module_init(physdev_mt_init);
module_exit(physdev_mt_exit);
