<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	IPv6 Address [auto]configuration
 *	Linux INET6 implementation
 *
 *	Authors:
 *	Pedro Roque		<roque@di.fc.ul.pt>
 *	Alexey Kuznetsov	<kuznet@ms2.inr.ac.ru>
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 *	Changes:
 *
 *	Janos Farkas			:	delete timer on ifdown
 *	<chexum@bankinf.banki.hu>
 *	Andi Kleen			:	kill double kfree on module
 *						unload.
 *	Maciej W. Rozycki		:	FDDI support
 *	sekiya@USAGI			:	Don't send too many RS
 *						packets.
 *	yoshfuji@USAGI			:       Fixed interval between DAD
 *						packets.
 *	YOSHIFUJI Hideaki @USAGI	:	improved accuracy of
 *						address validation timer.
 *	YOSHIFUJI Hideaki @USAGI	:	Privacy Extensions (RFC3041)
 *						support.
 *	Yuji SEKIYA @USAGI		:	Don't assign a same IPv6
 *						address on a same interface.
 *	YOSHIFUJI Hideaki @USAGI	:	ARCnet support
 *	YOSHIFUJI Hideaki @USAGI	:	convert /proc/net/if_inet6 to
 *						seq_file.
 *	YOSHIFUJI Hideaki @USAGI	:	improved source address
 *						selection; consider scope,
 *						status etc.
<<<<<<< HEAD
 *	Harout S. Hedeshian		:	procfs flag to toggle automatic
 *						addition of prefix route
 */

#include <linux/errno.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/net.h>
=======
 */

#define pr_fmt(fmt) "IPv6: " fmt

#include <linux/errno.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/socket.h>
#include <linux/sockios.h>
#include <linux/net.h>
#include <linux/inet.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/in6.h>
#include <linux/netdevice.h>
#include <linux/if_addr.h>
#include <linux/if_arp.h>
#include <linux/if_arcnet.h>
#include <linux/if_infiniband.h>
#include <linux/route.h>
#include <linux/inetdevice.h>
#include <linux/init.h>
#include <linux/slab.h>
#ifdef CONFIG_SYSCTL
#include <linux/sysctl.h>
#endif
#include <linux/capability.h>
#include <linux/delay.h>
#include <linux/notifier.h>
#include <linux/string.h>
<<<<<<< HEAD
=======
#include <linux/hash.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <net/net_namespace.h>
#include <net/sock.h>
#include <net/snmp.h>

<<<<<<< HEAD
=======
#include <net/6lowpan.h>
#include <net/firewire.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/ipv6.h>
#include <net/protocol.h>
#include <net/ndisc.h>
#include <net/ip6_route.h>
#include <net/addrconf.h>
#include <net/tcp.h>
#include <net/ip.h>
#include <net/netlink.h>
#include <net/pkt_sched.h>
<<<<<<< HEAD
#include <linux/if_tunnel.h>
#include <linux/rtnetlink.h>

#ifdef CONFIG_IPV6_PRIVACY
#include <linux/random.h>
#endif

=======
#include <net/l3mdev.h>
#include <linux/if_tunnel.h>
#include <linux/rtnetlink.h>
#include <linux/netconf.h>
#include <linux/random.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/uaccess.h>
#include <asm/unaligned.h>

#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/export.h>
<<<<<<< HEAD

/* Set to 3 to get tracing... */
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
#define ACONF_DEBUG 2 // The original value.
//#define ACONF_DEBUG 3 // To debug...
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21

#if ACONF_DEBUG >= 3
#define ADBG(x) printk x
#else
#define ADBG(x)
#endif

#define	INFINITY_LIFE_TIME	0xFFFFFFFF
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
//The value of global scope is 1.
//The value of link-local scope is 33.
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
#define LGE_DATA_GLOBAL_SCOPE 1
#define LGE_DATA_LINK_LOCAL_SCOPE 33

//The value which is 100 equals 1 second.
//So value which is 5 equals 50 milli-seconds.
//The 50 milli-seconds is requirements of LGU+.
#define LGE_DATA_WAITING_TIME_FOR_DAD_OF_LGU 5
#endif
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
=======
#include <linux/ioam6.h>

#define	INFINITY_LIFE_TIME	0xFFFFFFFF

#define IPV6_MAX_STRLEN \
	sizeof("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255")

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline u32 cstamp_delta(unsigned long cstamp)
{
	return (cstamp - INITIAL_JIFFIES) * 100UL / HZ;
}

<<<<<<< HEAD
#define ADDRCONF_TIMER_FUZZ_MINUS	(HZ > 50 ? HZ/50 : 1)
#define ADDRCONF_TIMER_FUZZ		(HZ / 4)
#define ADDRCONF_TIMER_FUZZ_MAX		(HZ)

#ifdef CONFIG_SYSCTL
static void addrconf_sysctl_register(struct inet6_dev *idev);
static void addrconf_sysctl_unregister(struct inet6_dev *idev);
#else
static inline void addrconf_sysctl_register(struct inet6_dev *idev)
{
=======
static inline s32 rfc3315_s14_backoff_init(s32 irt)
{
	/* multiply 'initial retransmission time' by 0.9 .. 1.1 */
	u64 tmp = get_random_u32_inclusive(900000, 1100000) * (u64)irt;
	do_div(tmp, 1000000);
	return (s32)tmp;
}

static inline s32 rfc3315_s14_backoff_update(s32 rt, s32 mrt)
{
	/* multiply 'retransmission timeout' by 1.9 .. 2.1 */
	u64 tmp = get_random_u32_inclusive(1900000, 2100000) * (u64)rt;
	do_div(tmp, 1000000);
	if ((s32)tmp > mrt) {
		/* multiply 'maximum retransmission time' by 0.9 .. 1.1 */
		tmp = get_random_u32_inclusive(900000, 1100000) * (u64)mrt;
		do_div(tmp, 1000000);
	}
	return (s32)tmp;
}

#ifdef CONFIG_SYSCTL
static int addrconf_sysctl_register(struct inet6_dev *idev);
static void addrconf_sysctl_unregister(struct inet6_dev *idev);
#else
static inline int addrconf_sysctl_register(struct inet6_dev *idev)
{
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void addrconf_sysctl_unregister(struct inet6_dev *idev)
{
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_IPV6_PRIVACY
static int __ipv6_regen_rndid(struct inet6_dev *idev);
static int __ipv6_try_regen_rndid(struct inet6_dev *idev, struct in6_addr *tmpaddr);
static void ipv6_regen_rndid(unsigned long data);
#endif

static int ipv6_generate_eui64(u8 *eui, struct net_device *dev);
static int ipv6_count_addresses(struct inet6_dev *idev);

/*
 *	Configured unicast address hash table
 */
static struct hlist_head inet6_addr_lst[IN6_ADDR_HSIZE];
static DEFINE_SPINLOCK(addrconf_hash_lock);

static void addrconf_verify(unsigned long);

static DEFINE_TIMER(addr_chk_timer, addrconf_verify, 0, 0);
static DEFINE_SPINLOCK(addrconf_verify_lock);
=======
static void ipv6_gen_rnd_iid(struct in6_addr *addr);

static int ipv6_generate_eui64(u8 *eui, struct net_device *dev);
static int ipv6_count_addresses(const struct inet6_dev *idev);
static int ipv6_generate_stable_address(struct in6_addr *addr,
					u8 dad_count,
					const struct inet6_dev *idev);

#define IN6_ADDR_HSIZE_SHIFT	8
#define IN6_ADDR_HSIZE		(1 << IN6_ADDR_HSIZE_SHIFT)

static void addrconf_verify(struct net *net);
static void addrconf_verify_rtnl(struct net *net);

static struct workqueue_struct *addrconf_wq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void addrconf_join_anycast(struct inet6_ifaddr *ifp);
static void addrconf_leave_anycast(struct inet6_ifaddr *ifp);

static void addrconf_type_change(struct net_device *dev,
				 unsigned long event);
<<<<<<< HEAD
static int addrconf_ifdown(struct net_device *dev, int how);

static void addrconf_dad_start(struct inet6_ifaddr *ifp, u32 flags);
static void addrconf_dad_timer(unsigned long data);
static void addrconf_dad_completed(struct inet6_ifaddr *ifp);
static void addrconf_dad_run(struct inet6_dev *idev);
static void addrconf_rs_timer(unsigned long data);
=======
static int addrconf_ifdown(struct net_device *dev, bool unregister);

static struct fib6_info *addrconf_get_prefix_route(const struct in6_addr *pfx,
						  int plen,
						  const struct net_device *dev,
						  u32 flags, u32 noflags,
						  bool no_gw);

static void addrconf_dad_start(struct inet6_ifaddr *ifp);
static void addrconf_dad_work(struct work_struct *w);
static void addrconf_dad_completed(struct inet6_ifaddr *ifp, bool bump_id,
				   bool send_na);
static void addrconf_dad_run(struct inet6_dev *idev, bool restart);
static void addrconf_rs_timer(struct timer_list *t);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void __ipv6_ifa_notify(int event, struct inet6_ifaddr *ifa);
static void ipv6_ifa_notify(int event, struct inet6_ifaddr *ifa);

static void inet6_prefix_notify(int event, struct inet6_dev *idev,
				struct prefix_info *pinfo);
<<<<<<< HEAD
static bool ipv6_chk_same_addr(struct net *net, const struct in6_addr *addr,
			       struct net_device *dev);

static ATOMIC_NOTIFIER_HEAD(inet6addr_chain);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct ipv6_devconf ipv6_devconf __read_mostly = {
	.forwarding		= 0,
	.hop_limit		= IPV6_DEFAULT_HOPLIMIT,
	.mtu6			= IPV6_MIN_MTU,
	.accept_ra		= 1,
	.accept_redirects	= 1,
	.autoconf		= 1,
	.force_mld_version	= 0,
<<<<<<< HEAD
	.dad_transmits		= 1,
	.rtr_solicits		= MAX_RTR_SOLICITATIONS,
	.rtr_solicit_interval	= RTR_SOLICITATION_INTERVAL,
	.rtr_solicit_delay	= MAX_RTR_SOLICITATION_DELAY,
#ifdef CONFIG_IPV6_PRIVACY
	.use_tempaddr 		= 0,
	.temp_valid_lft		= TEMP_VALID_LIFETIME,
	.temp_prefered_lft	= TEMP_PREFERRED_LIFETIME,
	.regen_max_retry	= REGEN_MAX_RETRY,
	.max_desync_factor	= MAX_DESYNC_FACTOR,
#endif
	.max_addresses		= IPV6_MAX_ADDRESSES,
	.accept_ra_defrtr	= 1,
	.accept_ra_pinfo	= 1,
#ifdef CONFIG_LGE_DHCPV6_WIFI
	.ra_info_flag		= 0,
#endif		
=======
	.mldv1_unsolicited_report_interval = 10 * HZ,
	.mldv2_unsolicited_report_interval = HZ,
	.dad_transmits		= 1,
	.rtr_solicits		= MAX_RTR_SOLICITATIONS,
	.rtr_solicit_interval	= RTR_SOLICITATION_INTERVAL,
	.rtr_solicit_max_interval = RTR_SOLICITATION_MAX_INTERVAL,
	.rtr_solicit_delay	= MAX_RTR_SOLICITATION_DELAY,
	.use_tempaddr		= 0,
	.temp_valid_lft		= TEMP_VALID_LIFETIME,
	.temp_prefered_lft	= TEMP_PREFERRED_LIFETIME,
	.regen_min_advance	= REGEN_MIN_ADVANCE,
	.regen_max_retry	= REGEN_MAX_RETRY,
	.max_desync_factor	= MAX_DESYNC_FACTOR,
	.max_addresses		= IPV6_MAX_ADDRESSES,
	.accept_ra_defrtr	= 1,
	.ra_defrtr_metric	= IP6_RT_PRIO_USER,
	.accept_ra_from_local	= 0,
	.accept_ra_min_hop_limit= 1,
	.accept_ra_min_lft	= 0,
	.accept_ra_pinfo	= 1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_IPV6_ROUTER_PREF
	.accept_ra_rtr_pref	= 1,
	.rtr_probe_interval	= 60 * HZ,
#ifdef CONFIG_IPV6_ROUTE_INFO
<<<<<<< HEAD
	.accept_ra_rt_info_max_plen = 0,
#endif
#endif
	.accept_ra_rt_table	= 0,
	.proxy_ndp		= 0,
	.accept_source_route	= 0,	/* we do not accept RH0 by default. */
	.disable_ipv6		= 0,
	.accept_dad		= 1,
	.accept_ra_prefix_route = 1,
	.accept_ra_mtu		= 1,
=======
	.accept_ra_rt_info_min_plen = 0,
	.accept_ra_rt_info_max_plen = 0,
#endif
#endif
	.proxy_ndp		= 0,
	.accept_source_route	= 0,	/* we do not accept RH0 by default. */
	.disable_ipv6		= 0,
	.accept_dad		= 0,
	.suppress_frag_ndisc	= 1,
	.accept_ra_mtu		= 1,
	.stable_secret		= {
		.initialized = false,
	},
	.use_oif_addrs_only	= 0,
	.ignore_routes_with_linkdown = 0,
	.keep_addr_on_down	= 0,
	.seg6_enabled		= 0,
#ifdef CONFIG_IPV6_SEG6_HMAC
	.seg6_require_hmac	= 0,
#endif
	.enhanced_dad           = 1,
	.addr_gen_mode		= IN6_ADDR_GEN_MODE_EUI64,
	.disable_policy		= 0,
	.rpl_seg_enabled	= 0,
	.ioam6_enabled		= 0,
	.ioam6_id               = IOAM6_DEFAULT_IF_ID,
	.ioam6_id_wide		= IOAM6_DEFAULT_IF_ID_WIDE,
	.ndisc_evict_nocarrier	= 1,
	.ra_honor_pio_life	= 0,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct ipv6_devconf ipv6_devconf_dflt __read_mostly = {
	.forwarding		= 0,
	.hop_limit		= IPV6_DEFAULT_HOPLIMIT,
	.mtu6			= IPV6_MIN_MTU,
	.accept_ra		= 1,
	.accept_redirects	= 1,
	.autoconf		= 1,
<<<<<<< HEAD
	.dad_transmits		= 1,
	.rtr_solicits		= MAX_RTR_SOLICITATIONS,
	.rtr_solicit_interval	= RTR_SOLICITATION_INTERVAL,
	.rtr_solicit_delay	= MAX_RTR_SOLICITATION_DELAY,
#ifdef CONFIG_IPV6_PRIVACY
	.use_tempaddr		= 0,
	.temp_valid_lft		= TEMP_VALID_LIFETIME,
	.temp_prefered_lft	= TEMP_PREFERRED_LIFETIME,
	.regen_max_retry	= REGEN_MAX_RETRY,
	.max_desync_factor	= MAX_DESYNC_FACTOR,
#endif
	.max_addresses		= IPV6_MAX_ADDRESSES,
	.accept_ra_defrtr	= 1,
	.accept_ra_pinfo	= 1,
#ifdef CONFIG_LGE_DHCPV6_WIFI
	.ra_info_flag		= 0,
#endif	
=======
	.force_mld_version	= 0,
	.mldv1_unsolicited_report_interval = 10 * HZ,
	.mldv2_unsolicited_report_interval = HZ,
	.dad_transmits		= 1,
	.rtr_solicits		= MAX_RTR_SOLICITATIONS,
	.rtr_solicit_interval	= RTR_SOLICITATION_INTERVAL,
	.rtr_solicit_max_interval = RTR_SOLICITATION_MAX_INTERVAL,
	.rtr_solicit_delay	= MAX_RTR_SOLICITATION_DELAY,
	.use_tempaddr		= 0,
	.temp_valid_lft		= TEMP_VALID_LIFETIME,
	.temp_prefered_lft	= TEMP_PREFERRED_LIFETIME,
	.regen_min_advance	= REGEN_MIN_ADVANCE,
	.regen_max_retry	= REGEN_MAX_RETRY,
	.max_desync_factor	= MAX_DESYNC_FACTOR,
	.max_addresses		= IPV6_MAX_ADDRESSES,
	.accept_ra_defrtr	= 1,
	.ra_defrtr_metric	= IP6_RT_PRIO_USER,
	.accept_ra_from_local	= 0,
	.accept_ra_min_hop_limit= 1,
	.accept_ra_min_lft	= 0,
	.accept_ra_pinfo	= 1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_IPV6_ROUTER_PREF
	.accept_ra_rtr_pref	= 1,
	.rtr_probe_interval	= 60 * HZ,
#ifdef CONFIG_IPV6_ROUTE_INFO
<<<<<<< HEAD
	.accept_ra_rt_info_max_plen = 0,
#endif
#endif
	.accept_ra_rt_table	= 0,
=======
	.accept_ra_rt_info_min_plen = 0,
	.accept_ra_rt_info_max_plen = 0,
#endif
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.proxy_ndp		= 0,
	.accept_source_route	= 0,	/* we do not accept RH0 by default. */
	.disable_ipv6		= 0,
	.accept_dad		= 1,
<<<<<<< HEAD
	.accept_ra_prefix_route = 1,
	.accept_ra_mtu		= 1,
};

/* IPv6 Wildcard Address and Loopback Address defined by RFC2553 */
const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;
const struct in6_addr in6addr_loopback = IN6ADDR_LOOPBACK_INIT;
const struct in6_addr in6addr_linklocal_allnodes = IN6ADDR_LINKLOCAL_ALLNODES_INIT;
const struct in6_addr in6addr_linklocal_allrouters = IN6ADDR_LINKLOCAL_ALLROUTERS_INIT;

/* Check if a valid qdisc is available */
static inline bool addrconf_qdisc_ok(const struct net_device *dev)
{
	return !qdisc_tx_is_noop(dev);
}

/* Check if a route is valid prefix route */
static inline int addrconf_is_prefix_route(const struct rt6_info *rt)
{
	return (rt->rt6i_flags & (RTF_GATEWAY | RTF_DEFAULT)) == 0;
}

static void addrconf_del_timer(struct inet6_ifaddr *ifp)
{
	if (del_timer(&ifp->timer))
		__in6_ifa_put(ifp);
}

enum addrconf_timer_t {
	AC_NONE,
	AC_DAD,
	AC_RS,
};
static void addrconf_mod_timer(struct inet6_ifaddr *ifp,
			       enum addrconf_timer_t what,
			       unsigned long when)
{
	if (!del_timer(&ifp->timer))
		in6_ifa_hold(ifp);

	switch (what) {
	case AC_DAD:
		ifp->timer.function = addrconf_dad_timer;
		break;
	case AC_RS:
		ifp->timer.function = addrconf_rs_timer;
		break;
	default:
		break;
	}
	ifp->timer.expires = jiffies + when;
	add_timer(&ifp->timer);
=======
	.suppress_frag_ndisc	= 1,
	.accept_ra_mtu		= 1,
	.stable_secret		= {
		.initialized = false,
	},
	.use_oif_addrs_only	= 0,
	.ignore_routes_with_linkdown = 0,
	.keep_addr_on_down	= 0,
	.seg6_enabled		= 0,
#ifdef CONFIG_IPV6_SEG6_HMAC
	.seg6_require_hmac	= 0,
#endif
	.enhanced_dad           = 1,
	.addr_gen_mode		= IN6_ADDR_GEN_MODE_EUI64,
	.disable_policy		= 0,
	.rpl_seg_enabled	= 0,
	.ioam6_enabled		= 0,
	.ioam6_id               = IOAM6_DEFAULT_IF_ID,
	.ioam6_id_wide		= IOAM6_DEFAULT_IF_ID_WIDE,
	.ndisc_evict_nocarrier	= 1,
	.ra_honor_pio_life	= 0,
};

/* Check if link is ready: is it up and is a valid qdisc available */
static inline bool addrconf_link_ready(const struct net_device *dev)
{
	return netif_oper_up(dev) && !qdisc_tx_is_noop(dev);
}

static void addrconf_del_rs_timer(struct inet6_dev *idev)
{
	if (del_timer(&idev->rs_timer))
		__in6_dev_put(idev);
}

static void addrconf_del_dad_work(struct inet6_ifaddr *ifp)
{
	if (cancel_delayed_work(&ifp->dad_work))
		__in6_ifa_put(ifp);
}

static void addrconf_mod_rs_timer(struct inet6_dev *idev,
				  unsigned long when)
{
	if (!mod_timer(&idev->rs_timer, jiffies + when))
		in6_dev_hold(idev);
}

static void addrconf_mod_dad_work(struct inet6_ifaddr *ifp,
				   unsigned long delay)
{
	in6_ifa_hold(ifp);
	if (mod_delayed_work(addrconf_wq, &ifp->dad_work, delay))
		in6_ifa_put(ifp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int snmp6_alloc_dev(struct inet6_dev *idev)
{
	int i;

<<<<<<< HEAD
	if (snmp_mib_init((void __percpu **)idev->stats.ipv6,
			  sizeof(struct ipstats_mib),
			  __alignof__(struct ipstats_mib)) < 0)
=======
	idev->stats.ipv6 = alloc_percpu_gfp(struct ipstats_mib, GFP_KERNEL_ACCOUNT);
	if (!idev->stats.ipv6)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err_ip;

	for_each_possible_cpu(i) {
		struct ipstats_mib *addrconf_stats;
<<<<<<< HEAD
		addrconf_stats = per_cpu_ptr(idev->stats.ipv6[0], i);
		u64_stats_init(&addrconf_stats->syncp);
#if SNMP_ARRAY_SZ == 2
		addrconf_stats = per_cpu_ptr(idev->stats.ipv6[1], i);
		u64_stats_init(&addrconf_stats->syncp);
#endif
=======
		addrconf_stats = per_cpu_ptr(idev->stats.ipv6, i);
		u64_stats_init(&addrconf_stats->syncp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}


	idev->stats.icmpv6dev = kzalloc(sizeof(struct icmpv6_mib_device),
					GFP_KERNEL);
	if (!idev->stats.icmpv6dev)
		goto err_icmp;
	idev->stats.icmpv6msgdev = kzalloc(sizeof(struct icmpv6msg_mib_device),
<<<<<<< HEAD
					   GFP_KERNEL);
=======
					   GFP_KERNEL_ACCOUNT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!idev->stats.icmpv6msgdev)
		goto err_icmpmsg;

	return 0;

err_icmpmsg:
	kfree(idev->stats.icmpv6dev);
err_icmp:
<<<<<<< HEAD
	snmp_mib_free((void __percpu **)idev->stats.ipv6);
=======
	free_percpu(idev->stats.ipv6);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_ip:
	return -ENOMEM;
}

<<<<<<< HEAD
static void snmp6_free_dev(struct inet6_dev *idev)
{
	kfree(idev->stats.icmpv6msgdev);
	kfree(idev->stats.icmpv6dev);
	snmp_mib_free((void __percpu **)idev->stats.ipv6);
}

/* Nobody refers to this device, we may destroy it. */

void in6_dev_finish_destroy(struct inet6_dev *idev)
{
	struct net_device *dev = idev->dev;

	WARN_ON(!list_empty(&idev->addr_list));
	WARN_ON(idev->mc_list != NULL);

#ifdef NET_REFCNT_DEBUG
	printk(KERN_DEBUG "in6_dev_finish_destroy: %s\n", dev ? dev->name : "NIL");
#endif
	dev_put(dev);
	if (!idev->dead) {
		pr_warning("Freeing alive inet6 device %p\n", idev);
		return;
	}
	snmp6_free_dev(idev);
	kfree_rcu(idev, rcu);
}

EXPORT_SYMBOL(in6_dev_finish_destroy);

static struct inet6_dev * ipv6_add_dev(struct net_device *dev)
{
	struct inet6_dev *ndev;

	ASSERT_RTNL();

	if (dev->mtu < IPV6_MIN_MTU)
		return NULL;

	ndev = kzalloc(sizeof(struct inet6_dev), GFP_KERNEL);

	if (ndev == NULL)
		return NULL;
=======
static struct inet6_dev *ipv6_add_dev(struct net_device *dev)
{
	struct inet6_dev *ndev;
	int err = -ENOMEM;

	ASSERT_RTNL();

	if (dev->mtu < IPV6_MIN_MTU && dev != blackhole_netdev)
		return ERR_PTR(-EINVAL);

	ndev = kzalloc(sizeof(*ndev), GFP_KERNEL_ACCOUNT);
	if (!ndev)
		return ERR_PTR(err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rwlock_init(&ndev->lock);
	ndev->dev = dev;
	INIT_LIST_HEAD(&ndev->addr_list);
<<<<<<< HEAD

	memcpy(&ndev->cnf, dev_net(dev)->ipv6.devconf_dflt, sizeof(ndev->cnf));
	ndev->cnf.mtu6 = dev->mtu;
	ndev->cnf.sysctl = NULL;
	ndev->nd_parms = neigh_parms_alloc(dev, &nd_tbl);
	if (ndev->nd_parms == NULL) {
		kfree(ndev);
		return NULL;
=======
	timer_setup(&ndev->rs_timer, addrconf_rs_timer, 0);
	memcpy(&ndev->cnf, dev_net(dev)->ipv6.devconf_dflt, sizeof(ndev->cnf));

	if (ndev->cnf.stable_secret.initialized)
		ndev->cnf.addr_gen_mode = IN6_ADDR_GEN_MODE_STABLE_PRIVACY;

	ndev->cnf.mtu6 = dev->mtu;
	ndev->ra_mtu = 0;
	ndev->nd_parms = neigh_parms_alloc(dev, &nd_tbl);
	if (!ndev->nd_parms) {
		kfree(ndev);
		return ERR_PTR(err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (ndev->cnf.forwarding)
		dev_disable_lro(dev);
	/* We refer to the device */
<<<<<<< HEAD
	dev_hold(dev);

	if (snmp6_alloc_dev(ndev) < 0) {
		ADBG((KERN_WARNING
			"%s(): cannot allocate memory for statistics; dev=%s.\n",
			__func__, dev->name));
		neigh_parms_release(&nd_tbl, ndev->nd_parms);
		dev_put(dev);
		kfree(ndev);
		return NULL;
	}

	if (snmp6_register_dev(ndev) < 0) {
		ADBG((KERN_WARNING
			"%s(): cannot create /proc/net/dev_snmp6/%s\n",
			__func__, dev->name));
		neigh_parms_release(&nd_tbl, ndev->nd_parms);
		ndev->dead = 1;
		in6_dev_finish_destroy(ndev);
		return NULL;
	}

	/* One reference from device.  We must do this before
	 * we invoke __ipv6_regen_rndid().
	 */
	in6_dev_hold(ndev);
=======
	netdev_hold(dev, &ndev->dev_tracker, GFP_KERNEL);

	if (snmp6_alloc_dev(ndev) < 0) {
		netdev_dbg(dev, "%s: cannot allocate memory for statistics\n",
			   __func__);
		neigh_parms_release(&nd_tbl, ndev->nd_parms);
		netdev_put(dev, &ndev->dev_tracker);
		kfree(ndev);
		return ERR_PTR(err);
	}

	if (dev != blackhole_netdev) {
		if (snmp6_register_dev(ndev) < 0) {
			netdev_dbg(dev, "%s: cannot create /proc/net/dev_snmp6/%s\n",
				   __func__, dev->name);
			goto err_release;
		}
	}
	/* One reference from device. */
	refcount_set(&ndev->refcnt, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (dev->flags & (IFF_NOARP | IFF_LOOPBACK))
		ndev->cnf.accept_dad = -1;

<<<<<<< HEAD
#if defined(CONFIG_IPV6_SIT) || defined(CONFIG_IPV6_SIT_MODULE)
	if (dev->type == ARPHRD_SIT && (dev->priv_flags & IFF_ISATAP)) {
		printk(KERN_INFO
		       "%s: Disabled Multicast RS\n",
		       dev->name);
=======
#if IS_ENABLED(CONFIG_IPV6_SIT)
	if (dev->type == ARPHRD_SIT && (dev->priv_flags & IFF_ISATAP)) {
		pr_info("%s: Disabled Multicast RS\n", dev->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ndev->cnf.rtr_solicits = 0;
	}
#endif

<<<<<<< HEAD
#ifdef CONFIG_IPV6_PRIVACY
	INIT_LIST_HEAD(&ndev->tempaddr_list);
	setup_timer(&ndev->regen_timer, ipv6_regen_rndid, (unsigned long)ndev);
=======
	INIT_LIST_HEAD(&ndev->tempaddr_list);
	ndev->desync_factor = U32_MAX;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((dev->flags&IFF_LOOPBACK) ||
	    dev->type == ARPHRD_TUNNEL ||
	    dev->type == ARPHRD_TUNNEL6 ||
	    dev->type == ARPHRD_SIT ||
	    dev->type == ARPHRD_NONE) {
		ndev->cnf.use_tempaddr = -1;
<<<<<<< HEAD
	} else {
		in6_dev_hold(ndev);
		ipv6_regen_rndid((unsigned long) ndev);
	}
#endif

	if (netif_running(dev) && addrconf_qdisc_ok(dev))
=======
	}

	ndev->token = in6addr_any;

	if (netif_running(dev) && addrconf_link_ready(dev))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ndev->if_flags |= IF_READY;

	ipv6_mc_init_dev(ndev);
	ndev->tstamp = jiffies;
<<<<<<< HEAD
	addrconf_sysctl_register(ndev);
	/* protected by rtnl_lock */
	rcu_assign_pointer(dev->ip6_ptr, ndev);

	/* Join all-node multicast group */
	ipv6_dev_mc_inc(dev, &in6addr_linklocal_allnodes);

	/* Join all-router multicast group if forwarding is set */
	if (ndev->cnf.forwarding && (dev->flags & IFF_MULTICAST))
		ipv6_dev_mc_inc(dev, &in6addr_linklocal_allrouters);

	return ndev;
}

static struct inet6_dev * ipv6_find_idev(struct net_device *dev)
=======
	if (dev != blackhole_netdev) {
		err = addrconf_sysctl_register(ndev);
		if (err) {
			ipv6_mc_destroy_dev(ndev);
			snmp6_unregister_dev(ndev);
			goto err_release;
		}
	}
	/* protected by rtnl_lock */
	rcu_assign_pointer(dev->ip6_ptr, ndev);

	if (dev != blackhole_netdev) {
		/* Join interface-local all-node multicast group */
		ipv6_dev_mc_inc(dev, &in6addr_interfacelocal_allnodes);

		/* Join all-node multicast group */
		ipv6_dev_mc_inc(dev, &in6addr_linklocal_allnodes);

		/* Join all-router multicast group if forwarding is set */
		if (ndev->cnf.forwarding && (dev->flags & IFF_MULTICAST))
			ipv6_dev_mc_inc(dev, &in6addr_linklocal_allrouters);
	}
	return ndev;

err_release:
	neigh_parms_release(&nd_tbl, ndev->nd_parms);
	ndev->dead = 1;
	in6_dev_finish_destroy(ndev);
	return ERR_PTR(err);
}

static struct inet6_dev *ipv6_find_idev(struct net_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inet6_dev *idev;

	ASSERT_RTNL();

	idev = __in6_dev_get(dev);
	if (!idev) {
		idev = ipv6_add_dev(dev);
<<<<<<< HEAD
		if (!idev)
			return NULL;
=======
		if (IS_ERR(idev))
			return idev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (dev->flags&IFF_UP)
		ipv6_mc_up(idev);
	return idev;
}

<<<<<<< HEAD
=======
static int inet6_netconf_msgsize_devconf(int type)
{
	int size =  NLMSG_ALIGN(sizeof(struct netconfmsg))
		    + nla_total_size(4);	/* NETCONFA_IFINDEX */
	bool all = false;

	if (type == NETCONFA_ALL)
		all = true;

	if (all || type == NETCONFA_FORWARDING)
		size += nla_total_size(4);
#ifdef CONFIG_IPV6_MROUTE
	if (all || type == NETCONFA_MC_FORWARDING)
		size += nla_total_size(4);
#endif
	if (all || type == NETCONFA_PROXY_NEIGH)
		size += nla_total_size(4);

	if (all || type == NETCONFA_IGNORE_ROUTES_WITH_LINKDOWN)
		size += nla_total_size(4);

	return size;
}

static int inet6_netconf_fill_devconf(struct sk_buff *skb, int ifindex,
				      struct ipv6_devconf *devconf, u32 portid,
				      u32 seq, int event, unsigned int flags,
				      int type)
{
	struct nlmsghdr  *nlh;
	struct netconfmsg *ncm;
	bool all = false;

	nlh = nlmsg_put(skb, portid, seq, event, sizeof(struct netconfmsg),
			flags);
	if (!nlh)
		return -EMSGSIZE;

	if (type == NETCONFA_ALL)
		all = true;

	ncm = nlmsg_data(nlh);
	ncm->ncm_family = AF_INET6;

	if (nla_put_s32(skb, NETCONFA_IFINDEX, ifindex) < 0)
		goto nla_put_failure;

	if (!devconf)
		goto out;

	if ((all || type == NETCONFA_FORWARDING) &&
	    nla_put_s32(skb, NETCONFA_FORWARDING,
			READ_ONCE(devconf->forwarding)) < 0)
		goto nla_put_failure;
#ifdef CONFIG_IPV6_MROUTE
	if ((all || type == NETCONFA_MC_FORWARDING) &&
	    nla_put_s32(skb, NETCONFA_MC_FORWARDING,
			atomic_read(&devconf->mc_forwarding)) < 0)
		goto nla_put_failure;
#endif
	if ((all || type == NETCONFA_PROXY_NEIGH) &&
	    nla_put_s32(skb, NETCONFA_PROXY_NEIGH,
			READ_ONCE(devconf->proxy_ndp)) < 0)
		goto nla_put_failure;

	if ((all || type == NETCONFA_IGNORE_ROUTES_WITH_LINKDOWN) &&
	    nla_put_s32(skb, NETCONFA_IGNORE_ROUTES_WITH_LINKDOWN,
			READ_ONCE(devconf->ignore_routes_with_linkdown)) < 0)
		goto nla_put_failure;

out:
	nlmsg_end(skb, nlh);
	return 0;

nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}

void inet6_netconf_notify_devconf(struct net *net, int event, int type,
				  int ifindex, struct ipv6_devconf *devconf)
{
	struct sk_buff *skb;
	int err = -ENOBUFS;

	skb = nlmsg_new(inet6_netconf_msgsize_devconf(type), GFP_KERNEL);
	if (!skb)
		goto errout;

	err = inet6_netconf_fill_devconf(skb, ifindex, devconf, 0, 0,
					 event, 0, type);
	if (err < 0) {
		/* -EMSGSIZE implies BUG in inet6_netconf_msgsize_devconf() */
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(skb);
		goto errout;
	}
	rtnl_notify(skb, net, 0, RTNLGRP_IPV6_NETCONF, NULL, GFP_KERNEL);
	return;
errout:
	rtnl_set_sk_err(net, RTNLGRP_IPV6_NETCONF, err);
}

static const struct nla_policy devconf_ipv6_policy[NETCONFA_MAX+1] = {
	[NETCONFA_IFINDEX]	= { .len = sizeof(int) },
	[NETCONFA_FORWARDING]	= { .len = sizeof(int) },
	[NETCONFA_PROXY_NEIGH]	= { .len = sizeof(int) },
	[NETCONFA_IGNORE_ROUTES_WITH_LINKDOWN]	= { .len = sizeof(int) },
};

static int inet6_netconf_valid_get_req(struct sk_buff *skb,
				       const struct nlmsghdr *nlh,
				       struct nlattr **tb,
				       struct netlink_ext_ack *extack)
{
	int i, err;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(struct netconfmsg))) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid header for netconf get request");
		return -EINVAL;
	}

	if (!netlink_strict_get_check(skb))
		return nlmsg_parse_deprecated(nlh, sizeof(struct netconfmsg),
					      tb, NETCONFA_MAX,
					      devconf_ipv6_policy, extack);

	err = nlmsg_parse_deprecated_strict(nlh, sizeof(struct netconfmsg),
					    tb, NETCONFA_MAX,
					    devconf_ipv6_policy, extack);
	if (err)
		return err;

	for (i = 0; i <= NETCONFA_MAX; i++) {
		if (!tb[i])
			continue;

		switch (i) {
		case NETCONFA_IFINDEX:
			break;
		default:
			NL_SET_ERR_MSG_MOD(extack, "Unsupported attribute in netconf get request");
			return -EINVAL;
		}
	}

	return 0;
}

static int inet6_netconf_get_devconf(struct sk_buff *in_skb,
				     struct nlmsghdr *nlh,
				     struct netlink_ext_ack *extack)
{
	struct net *net = sock_net(in_skb->sk);
	struct nlattr *tb[NETCONFA_MAX+1];
	struct inet6_dev *in6_dev = NULL;
	struct net_device *dev = NULL;
	struct sk_buff *skb;
	struct ipv6_devconf *devconf;
	int ifindex;
	int err;

	err = inet6_netconf_valid_get_req(in_skb, nlh, tb, extack);
	if (err < 0)
		return err;

	if (!tb[NETCONFA_IFINDEX])
		return -EINVAL;

	err = -EINVAL;
	ifindex = nla_get_s32(tb[NETCONFA_IFINDEX]);
	switch (ifindex) {
	case NETCONFA_IFINDEX_ALL:
		devconf = net->ipv6.devconf_all;
		break;
	case NETCONFA_IFINDEX_DEFAULT:
		devconf = net->ipv6.devconf_dflt;
		break;
	default:
		dev = dev_get_by_index(net, ifindex);
		if (!dev)
			return -EINVAL;
		in6_dev = in6_dev_get(dev);
		if (!in6_dev)
			goto errout;
		devconf = &in6_dev->cnf;
		break;
	}

	err = -ENOBUFS;
	skb = nlmsg_new(inet6_netconf_msgsize_devconf(NETCONFA_ALL), GFP_KERNEL);
	if (!skb)
		goto errout;

	err = inet6_netconf_fill_devconf(skb, ifindex, devconf,
					 NETLINK_CB(in_skb).portid,
					 nlh->nlmsg_seq, RTM_NEWNETCONF, 0,
					 NETCONFA_ALL);
	if (err < 0) {
		/* -EMSGSIZE implies BUG in inet6_netconf_msgsize_devconf() */
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(skb);
		goto errout;
	}
	err = rtnl_unicast(skb, net, NETLINK_CB(in_skb).portid);
errout:
	if (in6_dev)
		in6_dev_put(in6_dev);
	dev_put(dev);
	return err;
}

/* Combine dev_addr_genid and dev_base_seq to detect changes.
 */
static u32 inet6_base_seq(const struct net *net)
{
	u32 res = atomic_read(&net->ipv6.dev_addr_genid) +
		  READ_ONCE(net->dev_base_seq);

	/* Must not return 0 (see nl_dump_check_consistent()).
	 * Chose a value far away from 0.
	 */
	if (!res)
		res = 0x80000000;
	return res;
}

static int inet6_netconf_dump_devconf(struct sk_buff *skb,
				      struct netlink_callback *cb)
{
	const struct nlmsghdr *nlh = cb->nlh;
	struct net *net = sock_net(skb->sk);
	struct {
		unsigned long ifindex;
		unsigned int all_default;
	} *ctx = (void *)cb->ctx;
	struct net_device *dev;
	struct inet6_dev *idev;
	int err = 0;

	if (cb->strict_check) {
		struct netlink_ext_ack *extack = cb->extack;
		struct netconfmsg *ncm;

		if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*ncm))) {
			NL_SET_ERR_MSG_MOD(extack, "Invalid header for netconf dump request");
			return -EINVAL;
		}

		if (nlmsg_attrlen(nlh, sizeof(*ncm))) {
			NL_SET_ERR_MSG_MOD(extack, "Invalid data after header in netconf dump request");
			return -EINVAL;
		}
	}

	rcu_read_lock();
	for_each_netdev_dump(net, dev, ctx->ifindex) {
		idev = __in6_dev_get(dev);
		if (!idev)
			continue;
		err = inet6_netconf_fill_devconf(skb, dev->ifindex,
					         &idev->cnf,
						 NETLINK_CB(cb->skb).portid,
						 nlh->nlmsg_seq,
						 RTM_NEWNETCONF,
						 NLM_F_MULTI,
						 NETCONFA_ALL);
		if (err < 0)
			goto done;
	}
	if (ctx->all_default == 0) {
		err = inet6_netconf_fill_devconf(skb, NETCONFA_IFINDEX_ALL,
						 net->ipv6.devconf_all,
						 NETLINK_CB(cb->skb).portid,
						 nlh->nlmsg_seq,
						 RTM_NEWNETCONF, NLM_F_MULTI,
						 NETCONFA_ALL);
		if (err < 0)
			goto done;
		ctx->all_default++;
	}
	if (ctx->all_default == 1) {
		err = inet6_netconf_fill_devconf(skb, NETCONFA_IFINDEX_DEFAULT,
						 net->ipv6.devconf_dflt,
						 NETLINK_CB(cb->skb).portid,
						 nlh->nlmsg_seq,
						 RTM_NEWNETCONF, NLM_F_MULTI,
						 NETCONFA_ALL);
		if (err < 0)
			goto done;
		ctx->all_default++;
	}
done:
	rcu_read_unlock();
	return err;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_SYSCTL
static void dev_forward_change(struct inet6_dev *idev)
{
	struct net_device *dev;
	struct inet6_ifaddr *ifa;
<<<<<<< HEAD
=======
	LIST_HEAD(tmp_addr_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!idev)
		return;
	dev = idev->dev;
	if (idev->cnf.forwarding)
		dev_disable_lro(dev);
<<<<<<< HEAD
	if (dev && (dev->flags & IFF_MULTICAST)) {
		if (idev->cnf.forwarding)
			ipv6_dev_mc_inc(dev, &in6addr_linklocal_allrouters);
		else
			ipv6_dev_mc_dec(dev, &in6addr_linklocal_allrouters);
	}

	list_for_each_entry(ifa, &idev->addr_list, if_list) {
		if (ifa->flags&IFA_F_TENTATIVE)
			continue;
=======
	if (dev->flags & IFF_MULTICAST) {
		if (idev->cnf.forwarding) {
			ipv6_dev_mc_inc(dev, &in6addr_linklocal_allrouters);
			ipv6_dev_mc_inc(dev, &in6addr_interfacelocal_allrouters);
			ipv6_dev_mc_inc(dev, &in6addr_sitelocal_allrouters);
		} else {
			ipv6_dev_mc_dec(dev, &in6addr_linklocal_allrouters);
			ipv6_dev_mc_dec(dev, &in6addr_interfacelocal_allrouters);
			ipv6_dev_mc_dec(dev, &in6addr_sitelocal_allrouters);
		}
	}

	read_lock_bh(&idev->lock);
	list_for_each_entry(ifa, &idev->addr_list, if_list) {
		if (ifa->flags&IFA_F_TENTATIVE)
			continue;
		list_add_tail(&ifa->if_list_aux, &tmp_addr_list);
	}
	read_unlock_bh(&idev->lock);

	while (!list_empty(&tmp_addr_list)) {
		ifa = list_first_entry(&tmp_addr_list,
				       struct inet6_ifaddr, if_list_aux);
		list_del(&ifa->if_list_aux);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (idev->cnf.forwarding)
			addrconf_join_anycast(ifa);
		else
			addrconf_leave_anycast(ifa);
	}
<<<<<<< HEAD
=======

	inet6_netconf_notify_devconf(dev_net(dev), RTM_NEWNETCONF,
				     NETCONFA_FORWARDING,
				     dev->ifindex, &idev->cnf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


static void addrconf_forward_change(struct net *net, __s32 newf)
{
	struct net_device *dev;
	struct inet6_dev *idev;

	for_each_netdev(net, dev) {
		idev = __in6_dev_get(dev);
		if (idev) {
			int changed = (!idev->cnf.forwarding) ^ (!newf);
<<<<<<< HEAD
			idev->cnf.forwarding = newf;
=======

			WRITE_ONCE(idev->cnf.forwarding, newf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (changed)
				dev_forward_change(idev);
		}
	}
}

static int addrconf_fixup_forwarding(struct ctl_table *table, int *p, int newf)
{
	struct net *net;
	int old;

	if (!rtnl_trylock())
		return restart_syscall();

	net = (struct net *)table->extra2;
	old = *p;
<<<<<<< HEAD
	*p = newf;

	if (p == &net->ipv6.devconf_dflt->forwarding) {
=======
	WRITE_ONCE(*p, newf);

	if (p == &net->ipv6.devconf_dflt->forwarding) {
		if ((!newf) ^ (!old))
			inet6_netconf_notify_devconf(net, RTM_NEWNETCONF,
						     NETCONFA_FORWARDING,
						     NETCONFA_IFINDEX_DEFAULT,
						     net->ipv6.devconf_dflt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rtnl_unlock();
		return 0;
	}

	if (p == &net->ipv6.devconf_all->forwarding) {
<<<<<<< HEAD
		net->ipv6.devconf_dflt->forwarding = newf;
		addrconf_forward_change(net, newf);
=======
		int old_dflt = net->ipv6.devconf_dflt->forwarding;

		WRITE_ONCE(net->ipv6.devconf_dflt->forwarding, newf);
		if ((!newf) ^ (!old_dflt))
			inet6_netconf_notify_devconf(net, RTM_NEWNETCONF,
						     NETCONFA_FORWARDING,
						     NETCONFA_IFINDEX_DEFAULT,
						     net->ipv6.devconf_dflt);

		addrconf_forward_change(net, newf);
		if ((!newf) ^ (!old))
			inet6_netconf_notify_devconf(net, RTM_NEWNETCONF,
						     NETCONFA_FORWARDING,
						     NETCONFA_IFINDEX_ALL,
						     net->ipv6.devconf_all);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if ((!newf) ^ (!old))
		dev_forward_change((struct inet6_dev *)table->extra1);
	rtnl_unlock();

	if (newf)
		rt6_purge_dflt_routers(net);
	return 1;
}
<<<<<<< HEAD
=======

static void addrconf_linkdown_change(struct net *net, __s32 newf)
{
	struct net_device *dev;
	struct inet6_dev *idev;

	for_each_netdev(net, dev) {
		idev = __in6_dev_get(dev);
		if (idev) {
			int changed = (!idev->cnf.ignore_routes_with_linkdown) ^ (!newf);

			WRITE_ONCE(idev->cnf.ignore_routes_with_linkdown, newf);
			if (changed)
				inet6_netconf_notify_devconf(dev_net(dev),
							     RTM_NEWNETCONF,
							     NETCONFA_IGNORE_ROUTES_WITH_LINKDOWN,
							     dev->ifindex,
							     &idev->cnf);
		}
	}
}

static int addrconf_fixup_linkdown(struct ctl_table *table, int *p, int newf)
{
	struct net *net;
	int old;

	if (!rtnl_trylock())
		return restart_syscall();

	net = (struct net *)table->extra2;
	old = *p;
	WRITE_ONCE(*p, newf);

	if (p == &net->ipv6.devconf_dflt->ignore_routes_with_linkdown) {
		if ((!newf) ^ (!old))
			inet6_netconf_notify_devconf(net,
						     RTM_NEWNETCONF,
						     NETCONFA_IGNORE_ROUTES_WITH_LINKDOWN,
						     NETCONFA_IFINDEX_DEFAULT,
						     net->ipv6.devconf_dflt);
		rtnl_unlock();
		return 0;
	}

	if (p == &net->ipv6.devconf_all->ignore_routes_with_linkdown) {
		WRITE_ONCE(net->ipv6.devconf_dflt->ignore_routes_with_linkdown, newf);
		addrconf_linkdown_change(net, newf);
		if ((!newf) ^ (!old))
			inet6_netconf_notify_devconf(net,
						     RTM_NEWNETCONF,
						     NETCONFA_IGNORE_ROUTES_WITH_LINKDOWN,
						     NETCONFA_IFINDEX_ALL,
						     net->ipv6.devconf_all);
	}
	rtnl_unlock();

	return 1;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

/* Nobody refers to this ifaddr, destroy it */
void inet6_ifa_finish_destroy(struct inet6_ifaddr *ifp)
{
	WARN_ON(!hlist_unhashed(&ifp->addr_lst));

#ifdef NET_REFCNT_DEBUG
<<<<<<< HEAD
	printk(KERN_DEBUG "inet6_ifa_finish_destroy\n");
=======
	pr_debug("%s\n", __func__);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

	in6_dev_put(ifp->idev);

<<<<<<< HEAD
	if (del_timer(&ifp->timer))
		pr_notice("Timer is still running, when freeing ifa=%p\n", ifp);

	if (ifp->state != INET6_IFADDR_STATE_DEAD) {
		pr_warning("Freeing alive inet6 address %p\n", ifp);
		return;
	}
	dst_release(&ifp->rt->dst);
=======
	if (cancel_delayed_work(&ifp->dad_work))
		pr_notice("delayed DAD work was pending while freeing ifa=%p\n",
			  ifp);

	if (ifp->state != INET6_IFADDR_STATE_DEAD) {
		pr_warn("Freeing alive inet6 address %p\n", ifp);
		return;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	kfree_rcu(ifp, rcu);
}

static void
ipv6_link_dev_addr(struct inet6_dev *idev, struct inet6_ifaddr *ifp)
{
	struct list_head *p;
	int ifp_scope = ipv6_addr_src_scope(&ifp->addr);

	/*
	 * Each device address list is sorted in order of scope -
	 * global before linklocal.
	 */
	list_for_each(p, &idev->addr_list) {
		struct inet6_ifaddr *ifa
			= list_entry(p, struct inet6_ifaddr, if_list);
		if (ifp_scope >= ipv6_addr_src_scope(&ifa->addr))
			break;
	}

<<<<<<< HEAD
	list_add_tail(&ifp->if_list, p);
}

static u32 ipv6_addr_hash(const struct in6_addr *addr)
{
	/*
	 * We perform the hash function over the last 64 bits of the address
	 * This will include the IEEE address token on links that support it.
	 */
	return jhash_2words((__force u32)addr->s6_addr32[2],
			    (__force u32)addr->s6_addr32[3], 0)
		& (IN6_ADDR_HSIZE - 1);
=======
	list_add_tail_rcu(&ifp->if_list, p);
}

static u32 inet6_addr_hash(const struct net *net, const struct in6_addr *addr)
{
	u32 val = ipv6_addr_hash(addr) ^ net_hash_mix(net);

	return hash_32(val, IN6_ADDR_HSIZE_SHIFT);
}

static bool ipv6_chk_same_addr(struct net *net, const struct in6_addr *addr,
			       struct net_device *dev, unsigned int hash)
{
	struct inet6_ifaddr *ifp;

	hlist_for_each_entry(ifp, &net->ipv6.inet6_addr_lst[hash], addr_lst) {
		if (ipv6_addr_equal(&ifp->addr, addr)) {
			if (!dev || ifp->idev->dev == dev)
				return true;
		}
	}
	return false;
}

static int ipv6_add_addr_hash(struct net_device *dev, struct inet6_ifaddr *ifa)
{
	struct net *net = dev_net(dev);
	unsigned int hash = inet6_addr_hash(net, &ifa->addr);
	int err = 0;

	spin_lock_bh(&net->ipv6.addrconf_hash_lock);

	/* Ignore adding duplicate addresses on an interface */
	if (ipv6_chk_same_addr(net, &ifa->addr, dev, hash)) {
		netdev_dbg(dev, "ipv6_add_addr: already assigned\n");
		err = -EEXIST;
	} else {
		hlist_add_head_rcu(&ifa->addr_lst, &net->ipv6.inet6_addr_lst[hash]);
	}

	spin_unlock_bh(&net->ipv6.addrconf_hash_lock);

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* On success it returns ifp with increased reference count */

static struct inet6_ifaddr *
<<<<<<< HEAD
ipv6_add_addr(struct inet6_dev *idev, const struct in6_addr *addr, int pfxlen,
	      int scope, u32 flags)
{
	struct inet6_ifaddr *ifa = NULL;
	struct rt6_info *rt;
	unsigned int hash;
	int err = 0;
	int addr_type = ipv6_addr_type(addr);

	if (addr_type == IPV6_ADDR_ANY ||
	    addr_type & IPV6_ADDR_MULTICAST ||
	    (!(idev->dev->flags & IFF_LOOPBACK) &&
	     addr_type & IPV6_ADDR_LOOPBACK))
		return ERR_PTR(-EADDRNOTAVAIL);

	rcu_read_lock_bh();
	if (idev->dead) {
		err = -ENODEV;			/*XXX*/
		goto out2;
	}

	if (idev->cnf.disable_ipv6) {
		err = -EACCES;
		goto out2;
	}

	spin_lock(&addrconf_hash_lock);

	/* Ignore adding duplicate addresses on an interface */
	if (ipv6_chk_same_addr(dev_net(idev->dev), addr, idev->dev)) {
		ADBG(("ipv6_add_addr: already assigned\n"));
		err = -EEXIST;
		goto out;
	}

	ifa = kzalloc(sizeof(struct inet6_ifaddr), GFP_ATOMIC);

	if (ifa == NULL) {
		ADBG(("ipv6_add_addr: malloc failed\n"));
=======
ipv6_add_addr(struct inet6_dev *idev, struct ifa6_config *cfg,
	      bool can_block, struct netlink_ext_ack *extack)
{
	gfp_t gfp_flags = can_block ? GFP_KERNEL : GFP_ATOMIC;
	int addr_type = ipv6_addr_type(cfg->pfx);
	struct net *net = dev_net(idev->dev);
	struct inet6_ifaddr *ifa = NULL;
	struct fib6_info *f6i = NULL;
	int err = 0;

	if (addr_type == IPV6_ADDR_ANY) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid address");
		return ERR_PTR(-EADDRNOTAVAIL);
	} else if (addr_type & IPV6_ADDR_MULTICAST &&
		   !(cfg->ifa_flags & IFA_F_MCAUTOJOIN)) {
		NL_SET_ERR_MSG_MOD(extack, "Cannot assign multicast address without \"IFA_F_MCAUTOJOIN\" flag");
		return ERR_PTR(-EADDRNOTAVAIL);
	} else if (!(idev->dev->flags & IFF_LOOPBACK) &&
		   !netif_is_l3_master(idev->dev) &&
		   addr_type & IPV6_ADDR_LOOPBACK) {
		NL_SET_ERR_MSG_MOD(extack, "Cannot assign loopback address on this device");
		return ERR_PTR(-EADDRNOTAVAIL);
	}

	if (idev->dead) {
		NL_SET_ERR_MSG_MOD(extack, "device is going away");
		err = -ENODEV;
		goto out;
	}

	if (idev->cnf.disable_ipv6) {
		NL_SET_ERR_MSG_MOD(extack, "IPv6 is disabled on this device");
		err = -EACCES;
		goto out;
	}

	/* validator notifier needs to be blocking;
	 * do not call in atomic context
	 */
	if (can_block) {
		struct in6_validator_info i6vi = {
			.i6vi_addr = *cfg->pfx,
			.i6vi_dev = idev,
			.extack = extack,
		};

		err = inet6addr_validator_notifier_call_chain(NETDEV_UP, &i6vi);
		err = notifier_to_errno(err);
		if (err < 0)
			goto out;
	}

	ifa = kzalloc(sizeof(*ifa), gfp_flags | __GFP_ACCOUNT);
	if (!ifa) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -ENOBUFS;
		goto out;
	}

<<<<<<< HEAD
	rt = addrconf_dst_alloc(idev, addr, false);
	if (IS_ERR(rt)) {
		err = PTR_ERR(rt);
		goto out;
	}

	ifa->addr = *addr;

	spin_lock_init(&ifa->lock);
	spin_lock_init(&ifa->state_lock);
	init_timer(&ifa->timer);
	INIT_HLIST_NODE(&ifa->addr_lst);
	ifa->timer.data = (unsigned long) ifa;
	ifa->scope = scope;
	ifa->prefix_len = pfxlen;
	ifa->flags = flags | IFA_F_TENTATIVE;
	ifa->cstamp = ifa->tstamp = jiffies;

	ifa->rt = rt;

	ifa->idev = idev;
	in6_dev_hold(idev);
	/* For caller */
	in6_ifa_hold(ifa);

	/* Add to big hash table */
	hash = ipv6_addr_hash(addr);

	hlist_add_head_rcu(&ifa->addr_lst, &inet6_addr_lst[hash]);
#ifdef CONFIG_LGP_DATA_BUGFIX_IPV6_ADDRCONF_KERNEL_CRASH
	//spin_unlock(&addrconf_hash_lock);
#else
	spin_unlock(&addrconf_hash_lock);
#endif
	write_lock(&idev->lock);
	/* Add to inet6_dev unicast addr list. */
	ipv6_link_dev_addr(idev, ifa);

#ifdef CONFIG_IPV6_PRIVACY
=======
	f6i = addrconf_f6i_alloc(net, idev, cfg->pfx, false, gfp_flags, extack);
	if (IS_ERR(f6i)) {
		err = PTR_ERR(f6i);
		f6i = NULL;
		goto out;
	}

	neigh_parms_data_state_setall(idev->nd_parms);

	ifa->addr = *cfg->pfx;
	if (cfg->peer_pfx)
		ifa->peer_addr = *cfg->peer_pfx;

	spin_lock_init(&ifa->lock);
	INIT_DELAYED_WORK(&ifa->dad_work, addrconf_dad_work);
	INIT_HLIST_NODE(&ifa->addr_lst);
	ifa->scope = cfg->scope;
	ifa->prefix_len = cfg->plen;
	ifa->rt_priority = cfg->rt_priority;
	ifa->flags = cfg->ifa_flags;
	ifa->ifa_proto = cfg->ifa_proto;
	/* No need to add the TENTATIVE flag for addresses with NODAD */
	if (!(cfg->ifa_flags & IFA_F_NODAD))
		ifa->flags |= IFA_F_TENTATIVE;
	ifa->valid_lft = cfg->valid_lft;
	ifa->prefered_lft = cfg->preferred_lft;
	ifa->cstamp = ifa->tstamp = jiffies;
	ifa->tokenized = false;

	ifa->rt = f6i;

	ifa->idev = idev;
	in6_dev_hold(idev);

	/* For caller */
	refcount_set(&ifa->refcnt, 1);

	rcu_read_lock();

	err = ipv6_add_addr_hash(idev->dev, ifa);
	if (err < 0) {
		rcu_read_unlock();
		goto out;
	}

	write_lock_bh(&idev->lock);

	/* Add to inet6_dev unicast addr list. */
	ipv6_link_dev_addr(idev, ifa);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ifa->flags&IFA_F_TEMPORARY) {
		list_add(&ifa->tmp_list, &idev->tempaddr_list);
		in6_ifa_hold(ifa);
	}
<<<<<<< HEAD
#endif

	in6_ifa_hold(ifa);
	write_unlock(&idev->lock);
#ifdef CONFIG_LGP_DATA_BUGFIX_IPV6_ADDRCONF_KERNEL_CRASH
	spin_unlock(&addrconf_hash_lock);
#endif
out2:
	rcu_read_unlock_bh();

	if (likely(err == 0))
		atomic_notifier_call_chain(&inet6addr_chain, NETDEV_UP, ifa);
	else {
		kfree(ifa);
=======

	in6_ifa_hold(ifa);
	write_unlock_bh(&idev->lock);

	rcu_read_unlock();

	inet6addr_notifier_call_chain(NETDEV_UP, ifa);
out:
	if (unlikely(err < 0)) {
		fib6_info_release(f6i);

		if (ifa) {
			if (ifa->idev)
				in6_dev_put(ifa->idev);
			kfree(ifa);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ifa = ERR_PTR(err);
	}

	return ifa;
<<<<<<< HEAD
out:
	spin_unlock(&addrconf_hash_lock);
	goto out2;
}

=======
}

enum cleanup_prefix_rt_t {
	CLEANUP_PREFIX_RT_NOP,    /* no cleanup action for prefix route */
	CLEANUP_PREFIX_RT_DEL,    /* delete the prefix route */
	CLEANUP_PREFIX_RT_EXPIRE, /* update the lifetime of the prefix route */
};

/*
 * Check, whether the prefix for ifp would still need a prefix route
 * after deleting ifp. The function returns one of the CLEANUP_PREFIX_RT_*
 * constants.
 *
 * 1) we don't purge prefix if address was not permanent.
 *    prefix is managed by its own lifetime.
 * 2) we also don't purge, if the address was IFA_F_NOPREFIXROUTE.
 * 3) if there are no addresses, delete prefix.
 * 4) if there are still other permanent address(es),
 *    corresponding prefix is still permanent.
 * 5) if there are still other addresses with IFA_F_NOPREFIXROUTE,
 *    don't purge the prefix, assume user space is managing it.
 * 6) otherwise, update prefix lifetime to the
 *    longest valid lifetime among the corresponding
 *    addresses on the device.
 *    Note: subsequent RA will update lifetime.
 **/
static enum cleanup_prefix_rt_t
check_cleanup_prefix_route(struct inet6_ifaddr *ifp, unsigned long *expires)
{
	struct inet6_ifaddr *ifa;
	struct inet6_dev *idev = ifp->idev;
	unsigned long lifetime;
	enum cleanup_prefix_rt_t action = CLEANUP_PREFIX_RT_DEL;

	*expires = jiffies;

	list_for_each_entry(ifa, &idev->addr_list, if_list) {
		if (ifa == ifp)
			continue;
		if (ifa->prefix_len != ifp->prefix_len ||
		    !ipv6_prefix_equal(&ifa->addr, &ifp->addr,
				       ifp->prefix_len))
			continue;
		if (ifa->flags & (IFA_F_PERMANENT | IFA_F_NOPREFIXROUTE))
			return CLEANUP_PREFIX_RT_NOP;

		action = CLEANUP_PREFIX_RT_EXPIRE;

		spin_lock(&ifa->lock);

		lifetime = addrconf_timeout_fixup(ifa->valid_lft, HZ);
		/*
		 * Note: Because this address is
		 * not permanent, lifetime <
		 * LONG_MAX / HZ here.
		 */
		if (time_before(*expires, ifa->tstamp + lifetime * HZ))
			*expires = ifa->tstamp + lifetime * HZ;
		spin_unlock(&ifa->lock);
	}

	return action;
}

static void
cleanup_prefix_route(struct inet6_ifaddr *ifp, unsigned long expires,
		     bool del_rt, bool del_peer)
{
	struct fib6_table *table;
	struct fib6_info *f6i;

	f6i = addrconf_get_prefix_route(del_peer ? &ifp->peer_addr : &ifp->addr,
					ifp->prefix_len,
					ifp->idev->dev, 0, RTF_DEFAULT, true);
	if (f6i) {
		if (del_rt)
			ip6_del_rt(dev_net(ifp->idev->dev), f6i, false);
		else {
			if (!(f6i->fib6_flags & RTF_EXPIRES)) {
				table = f6i->fib6_table;
				spin_lock_bh(&table->tb6_lock);

				fib6_set_expires(f6i, expires);
				fib6_add_gc_list(f6i);

				spin_unlock_bh(&table->tb6_lock);
			}
			fib6_info_release(f6i);
		}
	}
}


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* This function wants to get referenced ifp and releases it before return */

static void ipv6_del_addr(struct inet6_ifaddr *ifp)
{
<<<<<<< HEAD
	struct inet6_ifaddr *ifa, *ifn;
	struct inet6_dev *idev = ifp->idev;
	int state;
	int deleted = 0, onlink = 0;
	unsigned long expires = jiffies;

	spin_lock_bh(&ifp->state_lock);
	state = ifp->state;
	ifp->state = INET6_IFADDR_STATE_DEAD;
	spin_unlock_bh(&ifp->state_lock);
=======
	enum cleanup_prefix_rt_t action = CLEANUP_PREFIX_RT_NOP;
	struct net *net = dev_net(ifp->idev->dev);
	unsigned long expires;
	int state;

	ASSERT_RTNL();

	spin_lock_bh(&ifp->lock);
	state = ifp->state;
	ifp->state = INET6_IFADDR_STATE_DEAD;
	spin_unlock_bh(&ifp->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (state == INET6_IFADDR_STATE_DEAD)
		goto out;

<<<<<<< HEAD
	spin_lock_bh(&addrconf_hash_lock);
	hlist_del_init_rcu(&ifp->addr_lst);
#ifdef CONFIG_LGP_DATA_BUGFIX_IPV6_ADDRCONF_KERNEL_CRASH
	//spin_unlock_bh(&addrconf_hash_lock);
#else
	spin_unlock_bh(&addrconf_hash_lock);
#endif

	write_lock_bh(&idev->lock);
#ifdef CONFIG_IPV6_PRIVACY
=======
	spin_lock_bh(&net->ipv6.addrconf_hash_lock);
	hlist_del_init_rcu(&ifp->addr_lst);
	spin_unlock_bh(&net->ipv6.addrconf_hash_lock);

	write_lock_bh(&ifp->idev->lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ifp->flags&IFA_F_TEMPORARY) {
		list_del(&ifp->tmp_list);
		if (ifp->ifpub) {
			in6_ifa_put(ifp->ifpub);
			ifp->ifpub = NULL;
		}
		__in6_ifa_put(ifp);
	}
<<<<<<< HEAD
#endif

	list_for_each_entry_safe(ifa, ifn, &idev->addr_list, if_list) {
		if (ifa == ifp) {
			list_del_init(&ifp->if_list);
			__in6_ifa_put(ifp);

			if (!(ifp->flags & IFA_F_PERMANENT) || onlink > 0)
				break;
			deleted = 1;
			continue;
		} else if (ifp->flags & IFA_F_PERMANENT) {
			if (ipv6_prefix_equal(&ifa->addr, &ifp->addr,
					      ifp->prefix_len)) {
				if (ifa->flags & IFA_F_PERMANENT) {
					onlink = 1;
					if (deleted)
						break;
				} else {
					unsigned long lifetime;

					if (!onlink)
						onlink = -1;

					spin_lock(&ifa->lock);

					lifetime = addrconf_timeout_fixup(ifa->valid_lft, HZ);
					/*
					 * Note: Because this address is
					 * not permanent, lifetime <
					 * LONG_MAX / HZ here.
					 */
					if (time_before(expires,
							ifa->tstamp + lifetime * HZ))
						expires = ifa->tstamp + lifetime * HZ;
					spin_unlock(&ifa->lock);
				}
			}
		}
	}
	write_unlock_bh(&idev->lock);
#ifdef CONFIG_LGP_DATA_BUGFIX_IPV6_ADDRCONF_KERNEL_CRASH
	spin_unlock_bh(&addrconf_hash_lock);
#endif

	addrconf_del_timer(ifp);

	ipv6_ifa_notify(RTM_DELADDR, ifp);

	atomic_notifier_call_chain(&inet6addr_chain, NETDEV_DOWN, ifp);

	/*
	 * Purge or update corresponding prefix
	 *
	 * 1) we don't purge prefix here if address was not permanent.
	 *    prefix is managed by its own lifetime.
	 * 2) if there're no addresses, delete prefix.
	 * 3) if there're still other permanent address(es),
	 *    corresponding prefix is still permanent.
	 * 4) otherwise, update prefix lifetime to the
	 *    longest valid lifetime among the corresponding
	 *    addresses on the device.
	 *    Note: subsequent RA will update lifetime.
	 *
	 * --yoshfuji
	 */
	if ((ifp->flags & IFA_F_PERMANENT) && onlink < 1) {
		struct in6_addr prefix;
		struct rt6_info *rt;
		struct net *net = dev_net(ifp->idev->dev);
		struct flowi6 fl6 = {};

		ipv6_addr_prefix(&prefix, &ifp->addr, ifp->prefix_len);
		fl6.flowi6_oif = ifp->idev->dev->ifindex;
		fl6.daddr = prefix;
		rt = (struct rt6_info *)ip6_route_lookup(net, &fl6,
							 RT6_LOOKUP_F_IFACE);

		if (rt != net->ipv6.ip6_null_entry &&
		    addrconf_is_prefix_route(rt)) {
			if (onlink == 0) {
				ip6_del_rt(rt);
				rt = NULL;
			} else if (!(rt->rt6i_flags & RTF_EXPIRES)) {
				rt6_set_expires(rt, expires);
			}
		}
		dst_release(&rt->dst);
=======

	if (ifp->flags & IFA_F_PERMANENT && !(ifp->flags & IFA_F_NOPREFIXROUTE))
		action = check_cleanup_prefix_route(ifp, &expires);

	list_del_rcu(&ifp->if_list);
	__in6_ifa_put(ifp);

	write_unlock_bh(&ifp->idev->lock);

	addrconf_del_dad_work(ifp);

	ipv6_ifa_notify(RTM_DELADDR, ifp);

	inet6addr_notifier_call_chain(NETDEV_DOWN, ifp);

	if (action != CLEANUP_PREFIX_RT_NOP) {
		cleanup_prefix_route(ifp, expires,
			action == CLEANUP_PREFIX_RT_DEL, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* clean up prefsrc entries */
	rt6_remove_prefsrc(ifp);
out:
	in6_ifa_put(ifp);
}

<<<<<<< HEAD
#ifdef CONFIG_IPV6_PRIVACY
static int ipv6_create_tempaddr(struct inet6_ifaddr *ifp, struct inet6_ifaddr *ift)
{
	struct inet6_dev *idev = ifp->idev;
	struct in6_addr addr, *tmpaddr;
	unsigned long tmp_prefered_lft, tmp_valid_lft, tmp_tstamp, age;
	unsigned long regen_advance;
	int tmp_plen;
	int ret = 0;
	int max_addresses;
	u32 addr_flags;
	unsigned long now = jiffies;

	write_lock(&idev->lock);
	if (ift) {
		spin_lock_bh(&ift->lock);
		memcpy(&addr.s6_addr[8], &ift->addr.s6_addr[8], 8);
		spin_unlock_bh(&ift->lock);
		tmpaddr = &addr;
	} else {
		tmpaddr = NULL;
	}
retry:
	in6_dev_hold(idev);
	if (idev->cnf.use_tempaddr <= 0) {
		write_unlock(&idev->lock);
		printk(KERN_INFO
			"ipv6_create_tempaddr(): use_tempaddr is disabled.\n");
=======
static unsigned long ipv6_get_regen_advance(const struct inet6_dev *idev)
{
	return READ_ONCE(idev->cnf.regen_min_advance) +
		READ_ONCE(idev->cnf.regen_max_retry) *
		READ_ONCE(idev->cnf.dad_transmits) *
		max(NEIGH_VAR(idev->nd_parms, RETRANS_TIME), HZ/100) / HZ;
}

static int ipv6_create_tempaddr(struct inet6_ifaddr *ifp, bool block)
{
	struct inet6_dev *idev = ifp->idev;
	unsigned long tmp_tstamp, age;
	unsigned long regen_advance;
	unsigned long now = jiffies;
	u32 if_public_preferred_lft;
	s32 cnf_temp_preferred_lft;
	struct inet6_ifaddr *ift;
	struct ifa6_config cfg;
	long max_desync_factor;
	struct in6_addr addr;
	int ret = 0;

	write_lock_bh(&idev->lock);

retry:
	in6_dev_hold(idev);
	if (READ_ONCE(idev->cnf.use_tempaddr) <= 0) {
		write_unlock_bh(&idev->lock);
		pr_info("%s: use_tempaddr is disabled\n", __func__);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		in6_dev_put(idev);
		ret = -1;
		goto out;
	}
	spin_lock_bh(&ifp->lock);
<<<<<<< HEAD
	if (ifp->regen_count++ >= idev->cnf.regen_max_retry) {
		idev->cnf.use_tempaddr = -1;	/*XXX*/
		spin_unlock_bh(&ifp->lock);
		write_unlock(&idev->lock);
		printk(KERN_WARNING
			"ipv6_create_tempaddr(): regeneration time exceeded. disabled temporary address support.\n");
=======
	if (ifp->regen_count++ >= READ_ONCE(idev->cnf.regen_max_retry)) {
		WRITE_ONCE(idev->cnf.use_tempaddr, -1);	/*XXX*/
		spin_unlock_bh(&ifp->lock);
		write_unlock_bh(&idev->lock);
		pr_warn("%s: regeneration time exceeded - disabled temporary address support\n",
			__func__);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		in6_dev_put(idev);
		ret = -1;
		goto out;
	}
	in6_ifa_hold(ifp);
	memcpy(addr.s6_addr, ifp->addr.s6_addr, 8);
<<<<<<< HEAD
	if (__ipv6_try_regen_rndid(idev, tmpaddr) < 0) {
		spin_unlock_bh(&ifp->lock);
		write_unlock(&idev->lock);
		printk(KERN_WARNING
			"ipv6_create_tempaddr(): regeneration of randomized interface id failed.\n");
		in6_ifa_put(ifp);
		in6_dev_put(idev);
		ret = -1;
		goto out;
	}
	memcpy(&addr.s6_addr[8], idev->rndid, 8);
	age = (now - ifp->tstamp) / HZ;
	tmp_valid_lft = min_t(__u32,
			      ifp->valid_lft,
			      idev->cnf.temp_valid_lft + age);
	tmp_prefered_lft = min_t(__u32,
				 ifp->prefered_lft,
				 idev->cnf.temp_prefered_lft + age -
				 idev->cnf.max_desync_factor);
	tmp_plen = ifp->prefix_len;
	max_addresses = idev->cnf.max_addresses;
	tmp_tstamp = ifp->tstamp;
	spin_unlock_bh(&ifp->lock);

	regen_advance = idev->cnf.regen_max_retry *
	                idev->cnf.dad_transmits *
	                idev->nd_parms->retrans_time / HZ;
	write_unlock(&idev->lock);

	/* A temporary address is created only if this calculated Preferred
	 * Lifetime is greater than REGEN_ADVANCE time units.  In particular,
	 * an implementation must not create a temporary address with a zero
	 * Preferred Lifetime.
=======
	ipv6_gen_rnd_iid(&addr);

	age = (now - ifp->tstamp) / HZ;

	regen_advance = ipv6_get_regen_advance(idev);

	/* recalculate max_desync_factor each time and update
	 * idev->desync_factor if it's larger
	 */
	cnf_temp_preferred_lft = READ_ONCE(idev->cnf.temp_prefered_lft);
	max_desync_factor = min_t(long,
				  READ_ONCE(idev->cnf.max_desync_factor),
				  cnf_temp_preferred_lft - regen_advance);

	if (unlikely(idev->desync_factor > max_desync_factor)) {
		if (max_desync_factor > 0) {
			get_random_bytes(&idev->desync_factor,
					 sizeof(idev->desync_factor));
			idev->desync_factor %= max_desync_factor;
		} else {
			idev->desync_factor = 0;
		}
	}

	if_public_preferred_lft = ifp->prefered_lft;

	memset(&cfg, 0, sizeof(cfg));
	cfg.valid_lft = min_t(__u32, ifp->valid_lft,
			      READ_ONCE(idev->cnf.temp_valid_lft) + age);
	cfg.preferred_lft = cnf_temp_preferred_lft + age - idev->desync_factor;
	cfg.preferred_lft = min_t(__u32, if_public_preferred_lft, cfg.preferred_lft);
	cfg.preferred_lft = min_t(__u32, cfg.valid_lft, cfg.preferred_lft);

	cfg.plen = ifp->prefix_len;
	tmp_tstamp = ifp->tstamp;
	spin_unlock_bh(&ifp->lock);

	write_unlock_bh(&idev->lock);

	/* From RFC 4941:
	 *
	 *     A temporary address is created only if this calculated Preferred
	 *     Lifetime is greater than REGEN_ADVANCE time units.  In
	 *     particular, an implementation must not create a temporary address
	 *     with a zero Preferred Lifetime.
	 *
	 *     ...
	 *
	 *     When creating a temporary address, the lifetime values MUST be
	 *     derived from the corresponding prefix as follows:
	 *
	 *     ...
	 *
	 *     *  Its Preferred Lifetime is the lower of the Preferred Lifetime
	 *        of the public address or TEMP_PREFERRED_LIFETIME -
	 *        DESYNC_FACTOR.
	 *
	 * To comply with the RFC's requirements, clamp the preferred lifetime
	 * to a minimum of regen_advance, unless that would exceed valid_lft or
	 * ifp->prefered_lft.
	 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Use age calculation as in addrconf_verify to avoid unnecessary
	 * temporary addresses being generated.
	 */
	age = (now - tmp_tstamp + ADDRCONF_TIMER_FUZZ_MINUS) / HZ;
<<<<<<< HEAD
	if (tmp_prefered_lft <= regen_advance + age) {
		in6_ifa_put(ifp);
		in6_dev_put(idev);
		ret = -1;
		goto out;
	}

	addr_flags = IFA_F_TEMPORARY;
	/* set in addrconf_prefix_rcv() */
	if (ifp->flags & IFA_F_OPTIMISTIC)
		addr_flags |= IFA_F_OPTIMISTIC;

	ift = ipv6_add_addr(idev, &addr, tmp_plen,
			    ipv6_addr_type(&addr)&IPV6_ADDR_SCOPE_MASK,
			    addr_flags);
	if (IS_ERR(ift)) {
		in6_ifa_put(ifp);
		in6_dev_put(idev);
		printk(KERN_INFO
			"ipv6_create_tempaddr(): retry temporary address regeneration.\n");
		tmpaddr = &addr;
		write_lock(&idev->lock);
=======
	if (cfg.preferred_lft <= regen_advance + age) {
		cfg.preferred_lft = regen_advance + age + 1;
		if (cfg.preferred_lft > cfg.valid_lft ||
		    cfg.preferred_lft > if_public_preferred_lft) {
			in6_ifa_put(ifp);
			in6_dev_put(idev);
			ret = -1;
			goto out;
		}
	}

	cfg.ifa_flags = IFA_F_TEMPORARY;
	/* set in addrconf_prefix_rcv() */
	if (ifp->flags & IFA_F_OPTIMISTIC)
		cfg.ifa_flags |= IFA_F_OPTIMISTIC;

	cfg.pfx = &addr;
	cfg.scope = ipv6_addr_scope(cfg.pfx);

	ift = ipv6_add_addr(idev, &cfg, block, NULL);
	if (IS_ERR(ift)) {
		in6_ifa_put(ifp);
		in6_dev_put(idev);
		pr_info("%s: retry temporary address regeneration\n", __func__);
		write_lock_bh(&idev->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto retry;
	}

	spin_lock_bh(&ift->lock);
	ift->ifpub = ifp;
<<<<<<< HEAD
	ift->valid_lft = tmp_valid_lft;
	ift->prefered_lft = tmp_prefered_lft;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ift->cstamp = now;
	ift->tstamp = tmp_tstamp;
	spin_unlock_bh(&ift->lock);

<<<<<<< HEAD
	addrconf_dad_start(ift, 0);
=======
	addrconf_dad_start(ift);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	in6_ifa_put(ift);
	in6_dev_put(idev);
out:
	return ret;
}
<<<<<<< HEAD
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	Choose an appropriate source address (RFC3484)
 */
enum {
	IPV6_SADDR_RULE_INIT = 0,
	IPV6_SADDR_RULE_LOCAL,
	IPV6_SADDR_RULE_SCOPE,
	IPV6_SADDR_RULE_PREFERRED,
#ifdef CONFIG_IPV6_MIP6
	IPV6_SADDR_RULE_HOA,
#endif
	IPV6_SADDR_RULE_OIF,
	IPV6_SADDR_RULE_LABEL,
<<<<<<< HEAD
#ifdef CONFIG_IPV6_PRIVACY
	IPV6_SADDR_RULE_PRIVACY,
#endif
=======
	IPV6_SADDR_RULE_PRIVACY,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	IPV6_SADDR_RULE_ORCHID,
	IPV6_SADDR_RULE_PREFIX,
#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	IPV6_SADDR_RULE_NOT_OPTIMISTIC,
#endif
	IPV6_SADDR_RULE_MAX
};

struct ipv6_saddr_score {
	int			rule;
	int			addr_type;
	struct inet6_ifaddr	*ifa;
	DECLARE_BITMAP(scorebits, IPV6_SADDR_RULE_MAX);
	int			scopedist;
	int			matchlen;
};

struct ipv6_saddr_dst {
	const struct in6_addr *addr;
	int ifindex;
	int scope;
	int label;
	unsigned int prefs;
};

static inline int ipv6_saddr_preferred(int type)
{
	if (type & (IPV6_ADDR_MAPPED|IPV6_ADDR_COMPATv4|IPV6_ADDR_LOOPBACK))
		return 1;
	return 0;
}

<<<<<<< HEAD
static inline bool ipv6_use_optimistic_addr(struct inet6_dev *idev)
{
#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	return idev && idev->cnf.optimistic_dad && idev->cnf.use_optimistic;
=======
static bool ipv6_use_optimistic_addr(const struct net *net,
				     const struct inet6_dev *idev)
{
#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	if (!idev)
		return false;
	if (!READ_ONCE(net->ipv6.devconf_all->optimistic_dad) &&
	    !READ_ONCE(idev->cnf.optimistic_dad))
		return false;
	if (!READ_ONCE(net->ipv6.devconf_all->use_optimistic) &&
	    !READ_ONCE(idev->cnf.use_optimistic))
		return false;

	return true;
#else
	return false;
#endif
}

static bool ipv6_allow_optimistic_dad(const struct net *net,
				      const struct inet6_dev *idev)
{
#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	if (!idev)
		return false;
	if (!READ_ONCE(net->ipv6.devconf_all->optimistic_dad) &&
	    !READ_ONCE(idev->cnf.optimistic_dad))
		return false;

	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
	return false;
#endif
}

static int ipv6_get_saddr_eval(struct net *net,
			       struct ipv6_saddr_score *score,
			       struct ipv6_saddr_dst *dst,
			       int i)
{
	int ret;

	if (i <= score->rule) {
		switch (i) {
		case IPV6_SADDR_RULE_SCOPE:
			ret = score->scopedist;
			break;
		case IPV6_SADDR_RULE_PREFIX:
			ret = score->matchlen;
			break;
		default:
			ret = !!test_bit(i, score->scorebits);
		}
		goto out;
	}

	switch (i) {
	case IPV6_SADDR_RULE_INIT:
		/* Rule 0: remember if hiscore is not ready yet */
		ret = !!score->ifa;
		break;
	case IPV6_SADDR_RULE_LOCAL:
		/* Rule 1: Prefer same address */
		ret = ipv6_addr_equal(&score->ifa->addr, dst->addr);
		break;
	case IPV6_SADDR_RULE_SCOPE:
		/* Rule 2: Prefer appropriate scope
		 *
		 *      ret
		 *       ^
		 *    -1 |  d 15
		 *    ---+--+-+---> scope
		 *       |
		 *       |             d is scope of the destination.
		 *  B-d  |  \
		 *       |   \      <- smaller scope is better if
<<<<<<< HEAD
		 *  B-15 |    \        if scope is enough for destinaion.
=======
		 *  B-15 |    \        if scope is enough for destination.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 *       |             ret = B - scope (-1 <= scope >= d <= 15).
		 * d-C-1 | /
		 *       |/         <- greater is better
		 *   -C  /             if scope is not enough for destination.
		 *      /|             ret = scope - C (-1 <= d < scope <= 15).
		 *
		 * d - C - 1 < B -15 (for all -1 <= d <= 15).
		 * C > d + 14 - B >= 15 + 14 - B = 29 - B.
		 * Assume B = 0 and we get C > 29.
		 */
		ret = __ipv6_addr_src_scope(score->addr_type);
		if (ret >= dst->scope)
			ret = -ret;
		else
			ret -= 128;	/* 30 is enough */
		score->scopedist = ret;
		break;
	case IPV6_SADDR_RULE_PREFERRED:
	    {
		/* Rule 3: Avoid deprecated and optimistic addresses */
		u8 avoid = IFA_F_DEPRECATED;

<<<<<<< HEAD
		if (!ipv6_use_optimistic_addr(score->ifa->idev))
=======
		if (!ipv6_use_optimistic_addr(net, score->ifa->idev))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			avoid |= IFA_F_OPTIMISTIC;
		ret = ipv6_saddr_preferred(score->addr_type) ||
		      !(score->ifa->flags & avoid);
		break;
	    }
#ifdef CONFIG_IPV6_MIP6
	case IPV6_SADDR_RULE_HOA:
	    {
		/* Rule 4: Prefer home address */
		int prefhome = !(dst->prefs & IPV6_PREFER_SRC_COA);
		ret = !(score->ifa->flags & IFA_F_HOMEADDRESS) ^ prefhome;
		break;
	    }
#endif
	case IPV6_SADDR_RULE_OIF:
		/* Rule 5: Prefer outgoing interface */
		ret = (!dst->ifindex ||
		       dst->ifindex == score->ifa->idev->dev->ifindex);
		break;
	case IPV6_SADDR_RULE_LABEL:
		/* Rule 6: Prefer matching label */
		ret = ipv6_addr_label(net,
				      &score->ifa->addr, score->addr_type,
				      score->ifa->idev->dev->ifindex) == dst->label;
		break;
<<<<<<< HEAD
#ifdef CONFIG_IPV6_PRIVACY
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case IPV6_SADDR_RULE_PRIVACY:
	    {
		/* Rule 7: Prefer public address
		 * Note: prefer temporary address if use_tempaddr >= 2
		 */
		int preftmp = dst->prefs & (IPV6_PREFER_SRC_PUBLIC|IPV6_PREFER_SRC_TMP) ?
				!!(dst->prefs & IPV6_PREFER_SRC_TMP) :
<<<<<<< HEAD
				score->ifa->idev->cnf.use_tempaddr >= 2;
		ret = (!(score->ifa->flags & IFA_F_TEMPORARY)) ^ preftmp;
		break;
	    }
#endif
=======
				READ_ONCE(score->ifa->idev->cnf.use_tempaddr) >= 2;
		ret = (!(score->ifa->flags & IFA_F_TEMPORARY)) ^ preftmp;
		break;
	    }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case IPV6_SADDR_RULE_ORCHID:
		/* Rule 8-: Prefer ORCHID vs ORCHID or
		 *	    non-ORCHID vs non-ORCHID
		 */
		ret = !(ipv6_addr_orchid(&score->ifa->addr) ^
			ipv6_addr_orchid(dst->addr));
		break;
	case IPV6_SADDR_RULE_PREFIX:
		/* Rule 8: Use longest matching prefix */
<<<<<<< HEAD
		score->matchlen = ret = ipv6_addr_diff(&score->ifa->addr,
						       dst->addr);
=======
		ret = ipv6_addr_diff(&score->ifa->addr, dst->addr);
		if (ret > score->ifa->prefix_len)
			ret = score->ifa->prefix_len;
		score->matchlen = ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	case IPV6_SADDR_RULE_NOT_OPTIMISTIC:
		/* Optimistic addresses still have lower precedence than other
		 * preferred addresses.
		 */
		ret = !(score->ifa->flags & IFA_F_OPTIMISTIC);
		break;
#endif
	default:
		ret = 0;
	}

	if (ret)
		__set_bit(i, score->scorebits);
	score->rule = i;
out:
	return ret;
}

<<<<<<< HEAD
=======
static int __ipv6_dev_get_saddr(struct net *net,
				struct ipv6_saddr_dst *dst,
				struct inet6_dev *idev,
				struct ipv6_saddr_score *scores,
				int hiscore_idx)
{
	struct ipv6_saddr_score *score = &scores[1 - hiscore_idx], *hiscore = &scores[hiscore_idx];

	list_for_each_entry_rcu(score->ifa, &idev->addr_list, if_list) {
		int i;

		/*
		 * - Tentative Address (RFC2462 section 5.4)
		 *  - A tentative address is not considered
		 *    "assigned to an interface" in the traditional
		 *    sense, unless it is also flagged as optimistic.
		 * - Candidate Source Address (section 4)
		 *  - In any case, anycast addresses, multicast
		 *    addresses, and the unspecified address MUST
		 *    NOT be included in a candidate set.
		 */
		if ((score->ifa->flags & IFA_F_TENTATIVE) &&
		    (!(score->ifa->flags & IFA_F_OPTIMISTIC)))
			continue;

		score->addr_type = __ipv6_addr_type(&score->ifa->addr);

		if (unlikely(score->addr_type == IPV6_ADDR_ANY ||
			     score->addr_type & IPV6_ADDR_MULTICAST)) {
			net_dbg_ratelimited("ADDRCONF: unspecified / multicast address assigned as unicast address on %s",
					    idev->dev->name);
			continue;
		}

		score->rule = -1;
		bitmap_zero(score->scorebits, IPV6_SADDR_RULE_MAX);

		for (i = 0; i < IPV6_SADDR_RULE_MAX; i++) {
			int minihiscore, miniscore;

			minihiscore = ipv6_get_saddr_eval(net, hiscore, dst, i);
			miniscore = ipv6_get_saddr_eval(net, score, dst, i);

			if (minihiscore > miniscore) {
				if (i == IPV6_SADDR_RULE_SCOPE &&
				    score->scopedist > 0) {
					/*
					 * special case:
					 * each remaining entry
					 * has too small (not enough)
					 * scope, because ifa entries
					 * are sorted by their scope
					 * values.
					 */
					goto out;
				}
				break;
			} else if (minihiscore < miniscore) {
				swap(hiscore, score);
				hiscore_idx = 1 - hiscore_idx;

				/* restore our iterator */
				score->ifa = hiscore->ifa;

				break;
			}
		}
	}
out:
	return hiscore_idx;
}

static int ipv6_get_saddr_master(struct net *net,
				 const struct net_device *dst_dev,
				 const struct net_device *master,
				 struct ipv6_saddr_dst *dst,
				 struct ipv6_saddr_score *scores,
				 int hiscore_idx)
{
	struct inet6_dev *idev;

	idev = __in6_dev_get(dst_dev);
	if (idev)
		hiscore_idx = __ipv6_dev_get_saddr(net, dst, idev,
						   scores, hiscore_idx);

	idev = __in6_dev_get(master);
	if (idev)
		hiscore_idx = __ipv6_dev_get_saddr(net, dst, idev,
						   scores, hiscore_idx);

	return hiscore_idx;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int ipv6_dev_get_saddr(struct net *net, const struct net_device *dst_dev,
		       const struct in6_addr *daddr, unsigned int prefs,
		       struct in6_addr *saddr)
{
<<<<<<< HEAD
	struct ipv6_saddr_score scores[2],
				*score = &scores[0], *hiscore = &scores[1];
	struct ipv6_saddr_dst dst;
	struct net_device *dev;
	int dst_type;
=======
	struct ipv6_saddr_score scores[2], *hiscore;
	struct ipv6_saddr_dst dst;
	struct inet6_dev *idev;
	struct net_device *dev;
	int dst_type;
	bool use_oif_addr = false;
	int hiscore_idx = 0;
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dst_type = __ipv6_addr_type(daddr);
	dst.addr = daddr;
	dst.ifindex = dst_dev ? dst_dev->ifindex : 0;
	dst.scope = __ipv6_addr_src_scope(dst_type);
	dst.label = ipv6_addr_label(net, daddr, dst_type, dst.ifindex);
	dst.prefs = prefs;

<<<<<<< HEAD
	hiscore->rule = -1;
	hiscore->ifa = NULL;

	rcu_read_lock();

	for_each_netdev_rcu(net, dev) {
		struct inet6_dev *idev;

		/* Candidate Source Address (section 4)
		 *  - multicast and link-local destination address,
		 *    the set of candidate source address MUST only
		 *    include addresses assigned to interfaces
		 *    belonging to the same link as the outgoing
		 *    interface.
		 * (- For site-local destination addresses, the
		 *    set of candidate source addresses MUST only
		 *    include addresses assigned to interfaces
		 *    belonging to the same site as the outgoing
		 *    interface.)
		 */
		if (((dst_type & IPV6_ADDR_MULTICAST) ||
		     dst.scope <= IPV6_ADDR_SCOPE_LINKLOCAL) &&
		    dst.ifindex && dev->ifindex != dst.ifindex)
			continue;

		idev = __in6_dev_get(dev);
		if (!idev)
			continue;

		read_lock_bh(&idev->lock);
		list_for_each_entry(score->ifa, &idev->addr_list, if_list) {
			int i;

			/*
			 * - Tentative Address (RFC2462 section 5.4)
			 *  - A tentative address is not considered
			 *    "assigned to an interface" in the traditional
			 *    sense, unless it is also flagged as optimistic.
			 * - Candidate Source Address (section 4)
			 *  - In any case, anycast addresses, multicast
			 *    addresses, and the unspecified address MUST
			 *    NOT be included in a candidate set.
			 */
			if ((score->ifa->flags & IFA_F_TENTATIVE) &&
			    (!(score->ifa->flags & IFA_F_OPTIMISTIC)))
				continue;

			score->addr_type = __ipv6_addr_type(&score->ifa->addr);

			if (unlikely(score->addr_type == IPV6_ADDR_ANY ||
				     score->addr_type & IPV6_ADDR_MULTICAST)) {
				LIMIT_NETDEBUG(KERN_DEBUG
					       "ADDRCONF: unspecified / multicast address "
					       "assigned as unicast address on %s",
					       dev->name);
				continue;
			}

			score->rule = -1;
			bitmap_zero(score->scorebits, IPV6_SADDR_RULE_MAX);

			for (i = 0; i < IPV6_SADDR_RULE_MAX; i++) {
				int minihiscore, miniscore;

				minihiscore = ipv6_get_saddr_eval(net, hiscore, &dst, i);
				miniscore = ipv6_get_saddr_eval(net, score, &dst, i);

				if (minihiscore > miniscore) {
					if (i == IPV6_SADDR_RULE_SCOPE &&
					    score->scopedist > 0) {
						/*
						 * special case:
						 * each remaining entry
						 * has too small (not enough)
						 * scope, because ifa entries
						 * are sorted by their scope
						 * values.
						 */
						goto try_nextdev;
					}
					break;
				} else if (minihiscore < miniscore) {
					if (hiscore->ifa)
						in6_ifa_put(hiscore->ifa);

					in6_ifa_hold(score->ifa);

					swap(hiscore, score);

					/* restore our iterator */
					score->ifa = hiscore->ifa;

					break;
				}
			}
		}
try_nextdev:
		read_unlock_bh(&idev->lock);
	}
	rcu_read_unlock();

	if (!hiscore->ifa)
		return -EADDRNOTAVAIL;

	*saddr = hiscore->ifa->addr;
	in6_ifa_put(hiscore->ifa);
	return 0;
}
EXPORT_SYMBOL(ipv6_dev_get_saddr);

int __ipv6_get_lladdr(struct inet6_dev *idev, struct in6_addr *addr,
		      unsigned char banned_flags)
=======
	scores[hiscore_idx].rule = -1;
	scores[hiscore_idx].ifa = NULL;

	rcu_read_lock();

	/* Candidate Source Address (section 4)
	 *  - multicast and link-local destination address,
	 *    the set of candidate source address MUST only
	 *    include addresses assigned to interfaces
	 *    belonging to the same link as the outgoing
	 *    interface.
	 * (- For site-local destination addresses, the
	 *    set of candidate source addresses MUST only
	 *    include addresses assigned to interfaces
	 *    belonging to the same site as the outgoing
	 *    interface.)
	 *  - "It is RECOMMENDED that the candidate source addresses
	 *    be the set of unicast addresses assigned to the
	 *    interface that will be used to send to the destination
	 *    (the 'outgoing' interface)." (RFC 6724)
	 */
	if (dst_dev) {
		idev = __in6_dev_get(dst_dev);
		if ((dst_type & IPV6_ADDR_MULTICAST) ||
		    dst.scope <= IPV6_ADDR_SCOPE_LINKLOCAL ||
		    (idev && READ_ONCE(idev->cnf.use_oif_addrs_only))) {
			use_oif_addr = true;
		}
	}

	if (use_oif_addr) {
		if (idev)
			hiscore_idx = __ipv6_dev_get_saddr(net, &dst, idev, scores, hiscore_idx);
	} else {
		const struct net_device *master;
		int master_idx = 0;

		/* if dst_dev exists and is enslaved to an L3 device, then
		 * prefer addresses from dst_dev and then the master over
		 * any other enslaved devices in the L3 domain.
		 */
		master = l3mdev_master_dev_rcu(dst_dev);
		if (master) {
			master_idx = master->ifindex;

			hiscore_idx = ipv6_get_saddr_master(net, dst_dev,
							    master, &dst,
							    scores, hiscore_idx);

			if (scores[hiscore_idx].ifa)
				goto out;
		}

		for_each_netdev_rcu(net, dev) {
			/* only consider addresses on devices in the
			 * same L3 domain
			 */
			if (l3mdev_master_ifindex_rcu(dev) != master_idx)
				continue;
			idev = __in6_dev_get(dev);
			if (!idev)
				continue;
			hiscore_idx = __ipv6_dev_get_saddr(net, &dst, idev, scores, hiscore_idx);
		}
	}

out:
	hiscore = &scores[hiscore_idx];
	if (!hiscore->ifa)
		ret = -EADDRNOTAVAIL;
	else
		*saddr = hiscore->ifa->addr;

	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL(ipv6_dev_get_saddr);

static int __ipv6_get_lladdr(struct inet6_dev *idev, struct in6_addr *addr,
			      u32 banned_flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inet6_ifaddr *ifp;
	int err = -EADDRNOTAVAIL;

<<<<<<< HEAD
	list_for_each_entry(ifp, &idev->addr_list, if_list) {
=======
	list_for_each_entry_reverse(ifp, &idev->addr_list, if_list) {
		if (ifp->scope > IFA_LINK)
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ifp->scope == IFA_LINK &&
		    !(ifp->flags & banned_flags)) {
			*addr = ifp->addr;
			err = 0;
			break;
		}
	}
	return err;
}

int ipv6_get_lladdr(struct net_device *dev, struct in6_addr *addr,
<<<<<<< HEAD
		    unsigned char banned_flags)
=======
		    u32 banned_flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inet6_dev *idev;
	int err = -EADDRNOTAVAIL;

	rcu_read_lock();
	idev = __in6_dev_get(dev);
	if (idev) {
		read_lock_bh(&idev->lock);
		err = __ipv6_get_lladdr(idev, addr, banned_flags);
		read_unlock_bh(&idev->lock);
	}
	rcu_read_unlock();
	return err;
}

<<<<<<< HEAD
static int ipv6_count_addresses(struct inet6_dev *idev)
{
	int cnt = 0;
	struct inet6_ifaddr *ifp;

	read_lock_bh(&idev->lock);
	list_for_each_entry(ifp, &idev->addr_list, if_list)
		cnt++;
	read_unlock_bh(&idev->lock);
=======
static int ipv6_count_addresses(const struct inet6_dev *idev)
{
	const struct inet6_ifaddr *ifp;
	int cnt = 0;

	rcu_read_lock();
	list_for_each_entry_rcu(ifp, &idev->addr_list, if_list)
		cnt++;
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return cnt;
}

int ipv6_chk_addr(struct net *net, const struct in6_addr *addr,
<<<<<<< HEAD
		  struct net_device *dev, int strict)
{
	struct inet6_ifaddr *ifp;
	struct hlist_node *node;
	unsigned int hash = ipv6_addr_hash(addr);

	rcu_read_lock_bh();
	hlist_for_each_entry_rcu(ifp, node, &inet6_addr_lst[hash], addr_lst) {
		if (!net_eq(dev_net(ifp->idev->dev), net))
			continue;
		if (ipv6_addr_equal(&ifp->addr, addr) &&
		    (!(ifp->flags&IFA_F_TENTATIVE) ||
		     (ipv6_use_optimistic_addr(ifp->idev) &&
		      ifp->flags&IFA_F_OPTIMISTIC)) &&
		    (dev == NULL || ifp->idev->dev == dev ||
		     !(ifp->scope&(IFA_LINK|IFA_HOST) || strict))) {
			rcu_read_unlock_bh();
			return 1;
		}
	}

	rcu_read_unlock_bh();
	return 0;
}
EXPORT_SYMBOL(ipv6_chk_addr);

static bool ipv6_chk_same_addr(struct net *net, const struct in6_addr *addr,
			       struct net_device *dev)
{
	unsigned int hash = ipv6_addr_hash(addr);
	struct inet6_ifaddr *ifp;
	struct hlist_node *node;

	hlist_for_each_entry(ifp, node, &inet6_addr_lst[hash], addr_lst) {
		if (!net_eq(dev_net(ifp->idev->dev), net))
			continue;
		if (ipv6_addr_equal(&ifp->addr, addr)) {
			if (dev == NULL || ifp->idev->dev == dev)
				return true;
		}
	}
	return false;
}

int ipv6_chk_prefix(const struct in6_addr *addr, struct net_device *dev)
{
	struct inet6_dev *idev;
	struct inet6_ifaddr *ifa;
=======
		  const struct net_device *dev, int strict)
{
	return ipv6_chk_addr_and_flags(net, addr, dev, !dev,
				       strict, IFA_F_TENTATIVE);
}
EXPORT_SYMBOL(ipv6_chk_addr);

/* device argument is used to find the L3 domain of interest. If
 * skip_dev_check is set, then the ifp device is not checked against
 * the passed in dev argument. So the 2 cases for addresses checks are:
 *   1. does the address exist in the L3 domain that dev is part of
 *      (skip_dev_check = true), or
 *
 *   2. does the address exist on the specific device
 *      (skip_dev_check = false)
 */
static struct net_device *
__ipv6_chk_addr_and_flags(struct net *net, const struct in6_addr *addr,
			  const struct net_device *dev, bool skip_dev_check,
			  int strict, u32 banned_flags)
{
	unsigned int hash = inet6_addr_hash(net, addr);
	struct net_device *l3mdev, *ndev;
	struct inet6_ifaddr *ifp;
	u32 ifp_flags;

	rcu_read_lock();

	l3mdev = l3mdev_master_dev_rcu(dev);
	if (skip_dev_check)
		dev = NULL;

	hlist_for_each_entry_rcu(ifp, &net->ipv6.inet6_addr_lst[hash], addr_lst) {
		ndev = ifp->idev->dev;

		if (l3mdev_master_dev_rcu(ndev) != l3mdev)
			continue;

		/* Decouple optimistic from tentative for evaluation here.
		 * Ban optimistic addresses explicitly, when required.
		 */
		ifp_flags = (ifp->flags&IFA_F_OPTIMISTIC)
			    ? (ifp->flags&~IFA_F_TENTATIVE)
			    : ifp->flags;
		if (ipv6_addr_equal(&ifp->addr, addr) &&
		    !(ifp_flags&banned_flags) &&
		    (!dev || ndev == dev ||
		     !(ifp->scope&(IFA_LINK|IFA_HOST) || strict))) {
			rcu_read_unlock();
			return ndev;
		}
	}

	rcu_read_unlock();
	return NULL;
}

int ipv6_chk_addr_and_flags(struct net *net, const struct in6_addr *addr,
			    const struct net_device *dev, bool skip_dev_check,
			    int strict, u32 banned_flags)
{
	return __ipv6_chk_addr_and_flags(net, addr, dev, skip_dev_check,
					 strict, banned_flags) ? 1 : 0;
}
EXPORT_SYMBOL(ipv6_chk_addr_and_flags);


/* Compares an address/prefix_len with addresses on device @dev.
 * If one is found it returns true.
 */
bool ipv6_chk_custom_prefix(const struct in6_addr *addr,
	const unsigned int prefix_len, struct net_device *dev)
{
	const struct inet6_ifaddr *ifa;
	const struct inet6_dev *idev;
	bool ret = false;

	rcu_read_lock();
	idev = __in6_dev_get(dev);
	if (idev) {
		list_for_each_entry_rcu(ifa, &idev->addr_list, if_list) {
			ret = ipv6_prefix_equal(addr, &ifa->addr, prefix_len);
			if (ret)
				break;
		}
	}
	rcu_read_unlock();

	return ret;
}
EXPORT_SYMBOL(ipv6_chk_custom_prefix);

int ipv6_chk_prefix(const struct in6_addr *addr, struct net_device *dev)
{
	const struct inet6_ifaddr *ifa;
	const struct inet6_dev *idev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int	onlink;

	onlink = 0;
	rcu_read_lock();
	idev = __in6_dev_get(dev);
	if (idev) {
<<<<<<< HEAD
		read_lock_bh(&idev->lock);
		list_for_each_entry(ifa, &idev->addr_list, if_list) {
=======
		list_for_each_entry_rcu(ifa, &idev->addr_list, if_list) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			onlink = ipv6_prefix_equal(addr, &ifa->addr,
						   ifa->prefix_len);
			if (onlink)
				break;
		}
<<<<<<< HEAD
		read_unlock_bh(&idev->lock);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	rcu_read_unlock();
	return onlink;
}
<<<<<<< HEAD

EXPORT_SYMBOL(ipv6_chk_prefix);

struct inet6_ifaddr *ipv6_get_ifaddr(struct net *net, const struct in6_addr *addr,
				     struct net_device *dev, int strict)
{
	struct inet6_ifaddr *ifp, *result = NULL;
	unsigned int hash = ipv6_addr_hash(addr);
	struct hlist_node *node;

	rcu_read_lock_bh();
	hlist_for_each_entry_rcu_bh(ifp, node, &inet6_addr_lst[hash], addr_lst) {
		if (!net_eq(dev_net(ifp->idev->dev), net))
			continue;
		if (ipv6_addr_equal(&ifp->addr, addr)) {
			if (dev == NULL || ifp->idev->dev == dev ||
			    !(ifp->scope&(IFA_LINK|IFA_HOST) || strict)) {
				result = ifp;
				in6_ifa_hold(ifp);
				break;
			}
		}
	}
	rcu_read_unlock_bh();
=======
EXPORT_SYMBOL(ipv6_chk_prefix);

/**
 * ipv6_dev_find - find the first device with a given source address.
 * @net: the net namespace
 * @addr: the source address
 * @dev: used to find the L3 domain of interest
 *
 * The caller should be protected by RCU, or RTNL.
 */
struct net_device *ipv6_dev_find(struct net *net, const struct in6_addr *addr,
				 struct net_device *dev)
{
	return __ipv6_chk_addr_and_flags(net, addr, dev, !dev, 1,
					 IFA_F_TENTATIVE);
}
EXPORT_SYMBOL(ipv6_dev_find);

struct inet6_ifaddr *ipv6_get_ifaddr(struct net *net, const struct in6_addr *addr,
				     struct net_device *dev, int strict)
{
	unsigned int hash = inet6_addr_hash(net, addr);
	struct inet6_ifaddr *ifp, *result = NULL;

	rcu_read_lock();
	hlist_for_each_entry_rcu(ifp, &net->ipv6.inet6_addr_lst[hash], addr_lst) {
		if (ipv6_addr_equal(&ifp->addr, addr)) {
			if (!dev || ifp->idev->dev == dev ||
			    !(ifp->scope&(IFA_LINK|IFA_HOST) || strict)) {
				if (in6_ifa_hold_safe(ifp)) {
					result = ifp;
					break;
				}
			}
		}
	}
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return result;
}

/* Gets referenced address, destroys ifaddr */

static void addrconf_dad_stop(struct inet6_ifaddr *ifp, int dad_failed)
{
<<<<<<< HEAD
	if (ifp->flags&IFA_F_PERMANENT) {
		spin_lock_bh(&ifp->lock);
		addrconf_del_timer(ifp);
		ifp->flags |= IFA_F_TENTATIVE;
		if (dad_failed)
			ifp->flags |= IFA_F_DADFAILED;
		spin_unlock_bh(&ifp->lock);
		if (dad_failed)
			ipv6_ifa_notify(0, ifp);
		in6_ifa_put(ifp);
#ifdef CONFIG_IPV6_PRIVACY
	} else if (ifp->flags&IFA_F_TEMPORARY) {
=======
	if (dad_failed)
		ifp->flags |= IFA_F_DADFAILED;

	if (ifp->flags&IFA_F_TEMPORARY) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct inet6_ifaddr *ifpub;
		spin_lock_bh(&ifp->lock);
		ifpub = ifp->ifpub;
		if (ifpub) {
			in6_ifa_hold(ifpub);
			spin_unlock_bh(&ifp->lock);
<<<<<<< HEAD
			ipv6_create_tempaddr(ifpub, ifp);
=======
			ipv6_create_tempaddr(ifpub, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			in6_ifa_put(ifpub);
		} else {
			spin_unlock_bh(&ifp->lock);
		}
		ipv6_del_addr(ifp);
<<<<<<< HEAD
#endif
	} else
		ipv6_del_addr(ifp);
=======
	} else if (ifp->flags&IFA_F_PERMANENT || !dad_failed) {
		spin_lock_bh(&ifp->lock);
		addrconf_del_dad_work(ifp);
		ifp->flags |= IFA_F_TENTATIVE;
		if (dad_failed)
			ifp->flags &= ~IFA_F_OPTIMISTIC;
		spin_unlock_bh(&ifp->lock);
		if (dad_failed)
			ipv6_ifa_notify(0, ifp);
		in6_ifa_put(ifp);
	} else {
		ipv6_del_addr(ifp);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int addrconf_dad_end(struct inet6_ifaddr *ifp)
{
	int err = -ENOENT;

<<<<<<< HEAD
	spin_lock(&ifp->state_lock);
=======
	spin_lock_bh(&ifp->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ifp->state == INET6_IFADDR_STATE_DAD) {
		ifp->state = INET6_IFADDR_STATE_POSTDAD;
		err = 0;
	}
<<<<<<< HEAD
	spin_unlock(&ifp->state_lock);
=======
	spin_unlock_bh(&ifp->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

<<<<<<< HEAD
void addrconf_dad_failure(struct inet6_ifaddr *ifp)
{
	struct inet6_dev *idev = ifp->idev;
=======
void addrconf_dad_failure(struct sk_buff *skb, struct inet6_ifaddr *ifp)
{
	struct inet6_dev *idev = ifp->idev;
	struct net *net = dev_net(idev->dev);
	int max_addresses;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (addrconf_dad_end(ifp)) {
		in6_ifa_put(ifp);
		return;
	}

<<<<<<< HEAD
	if (net_ratelimit())
		printk(KERN_INFO "%s: IPv6 duplicate address %pI6c detected!\n",
			ifp->idev->dev->name, &ifp->addr);

	if (idev->cnf.accept_dad > 1 && !idev->cnf.disable_ipv6) {
		struct in6_addr addr;

		addr.s6_addr32[0] = htonl(0xfe800000);
		addr.s6_addr32[1] = 0;

		if (!ipv6_generate_eui64(addr.s6_addr + 8, idev->dev) &&
		    ipv6_addr_equal(&ifp->addr, &addr)) {
			/* DAD failed for link-local based on MAC address */
			idev->cnf.disable_ipv6 = 1;

			printk(KERN_INFO "%s: IPv6 being disabled!\n",
				ifp->idev->dev->name);
		}
	}

	addrconf_dad_stop(ifp, 1);
}

/* Join to solicited addr multicast group. */

=======
	net_info_ratelimited("%s: IPv6 duplicate address %pI6c used by %pM detected!\n",
			     ifp->idev->dev->name, &ifp->addr, eth_hdr(skb)->h_source);

	spin_lock_bh(&ifp->lock);

	if (ifp->flags & IFA_F_STABLE_PRIVACY) {
		struct in6_addr new_addr;
		struct inet6_ifaddr *ifp2;
		int retries = ifp->stable_privacy_retry + 1;
		struct ifa6_config cfg = {
			.pfx = &new_addr,
			.plen = ifp->prefix_len,
			.ifa_flags = ifp->flags,
			.valid_lft = ifp->valid_lft,
			.preferred_lft = ifp->prefered_lft,
			.scope = ifp->scope,
		};

		if (retries > net->ipv6.sysctl.idgen_retries) {
			net_info_ratelimited("%s: privacy stable address generation failed because of DAD conflicts!\n",
					     ifp->idev->dev->name);
			goto errdad;
		}

		new_addr = ifp->addr;
		if (ipv6_generate_stable_address(&new_addr, retries,
						 idev))
			goto errdad;

		spin_unlock_bh(&ifp->lock);

		max_addresses = READ_ONCE(idev->cnf.max_addresses);
		if (max_addresses &&
		    ipv6_count_addresses(idev) >= max_addresses)
			goto lock_errdad;

		net_info_ratelimited("%s: generating new stable privacy address because of DAD conflict\n",
				     ifp->idev->dev->name);

		ifp2 = ipv6_add_addr(idev, &cfg, false, NULL);
		if (IS_ERR(ifp2))
			goto lock_errdad;

		spin_lock_bh(&ifp2->lock);
		ifp2->stable_privacy_retry = retries;
		ifp2->state = INET6_IFADDR_STATE_PREDAD;
		spin_unlock_bh(&ifp2->lock);

		addrconf_mod_dad_work(ifp2, net->ipv6.sysctl.idgen_delay);
		in6_ifa_put(ifp2);
lock_errdad:
		spin_lock_bh(&ifp->lock);
	}

errdad:
	/* transition from _POSTDAD to _ERRDAD */
	ifp->state = INET6_IFADDR_STATE_ERRDAD;
	spin_unlock_bh(&ifp->lock);

	addrconf_mod_dad_work(ifp, 0);
	in6_ifa_put(ifp);
}

/* Join to solicited addr multicast group.
 * caller must hold RTNL */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void addrconf_join_solict(struct net_device *dev, const struct in6_addr *addr)
{
	struct in6_addr maddr;

	if (dev->flags&(IFF_LOOPBACK|IFF_NOARP))
		return;

	addrconf_addr_solict_mult(addr, &maddr);
	ipv6_dev_mc_inc(dev, &maddr);
}

<<<<<<< HEAD
=======
/* caller must hold RTNL */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void addrconf_leave_solict(struct inet6_dev *idev, const struct in6_addr *addr)
{
	struct in6_addr maddr;

	if (idev->dev->flags&(IFF_LOOPBACK|IFF_NOARP))
		return;

	addrconf_addr_solict_mult(addr, &maddr);
	__ipv6_dev_mc_dec(idev, &maddr);
}

<<<<<<< HEAD
static void addrconf_join_anycast(struct inet6_ifaddr *ifp)
{
	struct in6_addr addr;
	if (ifp->prefix_len == 127) /* RFC 6164 */
=======
/* caller must hold RTNL */
static void addrconf_join_anycast(struct inet6_ifaddr *ifp)
{
	struct in6_addr addr;

	if (ifp->prefix_len >= 127) /* RFC 6164 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	ipv6_addr_prefix(&addr, &ifp->addr, ifp->prefix_len);
	if (ipv6_addr_any(&addr))
		return;
<<<<<<< HEAD
	ipv6_dev_ac_inc(ifp->idev->dev, &addr);
}

static void addrconf_leave_anycast(struct inet6_ifaddr *ifp)
{
	struct in6_addr addr;
	if (ifp->prefix_len == 127) /* RFC 6164 */
=======
	__ipv6_dev_ac_inc(ifp->idev, &addr);
}

/* caller must hold RTNL */
static void addrconf_leave_anycast(struct inet6_ifaddr *ifp)
{
	struct in6_addr addr;

	if (ifp->prefix_len >= 127) /* RFC 6164 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	ipv6_addr_prefix(&addr, &ifp->addr, ifp->prefix_len);
	if (ipv6_addr_any(&addr))
		return;
	__ipv6_dev_ac_dec(ifp->idev, &addr);
}

<<<<<<< HEAD
static int addrconf_ifid_eui48(u8 *eui, struct net_device *dev)
{
	if (dev->addr_len != ETH_ALEN)
		return -1;
	memcpy(eui, dev->dev_addr, 3);
	memcpy(eui + 5, dev->dev_addr + 3, 3);

	/*
	 * The zSeries OSA network cards can be shared among various
	 * OS instances, but the OSA cards have only one MAC address.
	 * This leads to duplicate address conflicts in conjunction
	 * with IPv6 if more than one instance uses the same card.
	 *
	 * The driver for these cards can deliver a unique 16-bit
	 * identifier for each instance sharing the same card.  It is
	 * placed instead of 0xFFFE in the interface identifier.  The
	 * "u" bit of the interface identifier is not inverted in this
	 * case.  Hence the resulting interface identifier has local
	 * scope according to RFC2373.
	 */
	if (dev->dev_id) {
		eui[3] = (dev->dev_id >> 8) & 0xFF;
		eui[4] = dev->dev_id & 0xFF;
	} else {
		eui[3] = 0xFF;
		eui[4] = 0xFE;
		eui[0] ^= 2;
	}
=======
static int addrconf_ifid_6lowpan(u8 *eui, struct net_device *dev)
{
	switch (dev->addr_len) {
	case ETH_ALEN:
		memcpy(eui, dev->dev_addr, 3);
		eui[3] = 0xFF;
		eui[4] = 0xFE;
		memcpy(eui + 5, dev->dev_addr + 3, 3);
		break;
	case EUI64_ADDR_LEN:
		memcpy(eui, dev->dev_addr, EUI64_ADDR_LEN);
		eui[0] ^= 2;
		break;
	default:
		return -1;
	}

	return 0;
}

static int addrconf_ifid_ieee1394(u8 *eui, struct net_device *dev)
{
	const union fwnet_hwaddr *ha;

	if (dev->addr_len != FWNET_ALEN)
		return -1;

	ha = (const union fwnet_hwaddr *)dev->dev_addr;

	memcpy(eui, &ha->uc.uniq_id, sizeof(ha->uc.uniq_id));
	eui[0] ^= 2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int addrconf_ifid_arcnet(u8 *eui, struct net_device *dev)
{
	/* XXX: inherit EUI-64 from other interface -- yoshfuji */
	if (dev->addr_len != ARCNET_ALEN)
		return -1;
	memset(eui, 0, 7);
<<<<<<< HEAD
	eui[7] = *(u8*)dev->dev_addr;
=======
	eui[7] = *(u8 *)dev->dev_addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int addrconf_ifid_infiniband(u8 *eui, struct net_device *dev)
{
	if (dev->addr_len != INFINIBAND_ALEN)
		return -1;
	memcpy(eui, dev->dev_addr + 12, 8);
	eui[0] |= 2;
	return 0;
}

static int __ipv6_isatap_ifid(u8 *eui, __be32 addr)
{
	if (addr == 0)
		return -1;
	eui[0] = (ipv4_is_zeronet(addr) || ipv4_is_private_10(addr) ||
		  ipv4_is_loopback(addr) || ipv4_is_linklocal_169(addr) ||
		  ipv4_is_private_172(addr) || ipv4_is_test_192(addr) ||
		  ipv4_is_anycast_6to4(addr) || ipv4_is_private_192(addr) ||
		  ipv4_is_test_198(addr) || ipv4_is_multicast(addr) ||
		  ipv4_is_lbcast(addr)) ? 0x00 : 0x02;
	eui[1] = 0;
	eui[2] = 0x5E;
	eui[3] = 0xFE;
	memcpy(eui + 4, &addr, 4);
	return 0;
}

static int addrconf_ifid_sit(u8 *eui, struct net_device *dev)
{
	if (dev->priv_flags & IFF_ISATAP)
		return __ipv6_isatap_ifid(eui, *(__be32 *)dev->dev_addr);
	return -1;
}

static int addrconf_ifid_gre(u8 *eui, struct net_device *dev)
{
	return __ipv6_isatap_ifid(eui, *(__be32 *)dev->dev_addr);
}

<<<<<<< HEAD
=======
static int addrconf_ifid_ip6tnl(u8 *eui, struct net_device *dev)
{
	memcpy(eui, dev->perm_addr, 3);
	memcpy(eui + 5, dev->perm_addr + 3, 3);
	eui[3] = 0xFF;
	eui[4] = 0xFE;
	eui[0] ^= 2;
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int ipv6_generate_eui64(u8 *eui, struct net_device *dev)
{
	switch (dev->type) {
	case ARPHRD_ETHER:
	case ARPHRD_FDDI:
<<<<<<< HEAD
	case ARPHRD_IEEE802_TR:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return addrconf_ifid_eui48(eui, dev);
	case ARPHRD_ARCNET:
		return addrconf_ifid_arcnet(eui, dev);
	case ARPHRD_INFINIBAND:
		return addrconf_ifid_infiniband(eui, dev);
	case ARPHRD_SIT:
		return addrconf_ifid_sit(eui, dev);
	case ARPHRD_IPGRE:
<<<<<<< HEAD
		return addrconf_ifid_gre(eui, dev);
	case ARPHRD_RAWIP: {
		struct in6_addr lladdr;

		if (ipv6_get_lladdr(dev, &lladdr, IFA_F_TENTATIVE))
			get_random_bytes(eui, 8);
		else
			memcpy(eui, lladdr.s6_addr + 8, 8);

		return 0;
	}
=======
	case ARPHRD_TUNNEL:
		return addrconf_ifid_gre(eui, dev);
	case ARPHRD_6LOWPAN:
		return addrconf_ifid_6lowpan(eui, dev);
	case ARPHRD_IEEE1394:
		return addrconf_ifid_ieee1394(eui, dev);
	case ARPHRD_TUNNEL6:
	case ARPHRD_IP6GRE:
	case ARPHRD_RAWIP:
		return addrconf_ifid_ip6tnl(eui, dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return -1;
}

static int ipv6_inherit_eui64(u8 *eui, struct inet6_dev *idev)
{
	int err = -1;
	struct inet6_ifaddr *ifp;

	read_lock_bh(&idev->lock);
<<<<<<< HEAD
	list_for_each_entry(ifp, &idev->addr_list, if_list) {
=======
	list_for_each_entry_reverse(ifp, &idev->addr_list, if_list) {
		if (ifp->scope > IFA_LINK)
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ifp->scope == IFA_LINK && !(ifp->flags&IFA_F_TENTATIVE)) {
			memcpy(eui, ifp->addr.s6_addr+8, 8);
			err = 0;
			break;
		}
	}
	read_unlock_bh(&idev->lock);
	return err;
}

<<<<<<< HEAD
#ifdef CONFIG_IPV6_PRIVACY
/* (re)generation of randomized interface identifier (RFC 3041 3.2, 3.5) */
static int __ipv6_regen_rndid(struct inet6_dev *idev)
{
regen:
	get_random_bytes(idev->rndid, sizeof(idev->rndid));
	idev->rndid[0] &= ~0x02;

	/*
	 * <draft-ietf-ipngwg-temp-addresses-v2-00.txt>:
	 * check if generated address is not inappropriate
	 *
	 *  - Reserved subnet anycast (RFC 2526)
	 *	11111101 11....11 1xxxxxxx
	 *  - ISATAP (RFC4214) 6.1
	 *	00-00-5E-FE-xx-xx-xx-xx
	 *  - value 0
	 *  - XXX: already assigned to an address on the device
	 */
	if (idev->rndid[0] == 0xfd &&
	    (idev->rndid[1]&idev->rndid[2]&idev->rndid[3]&idev->rndid[4]&idev->rndid[5]&idev->rndid[6]) == 0xff &&
	    (idev->rndid[7]&0x80))
		goto regen;
	if ((idev->rndid[0]|idev->rndid[1]) == 0) {
		if (idev->rndid[2] == 0x5e && idev->rndid[3] == 0xfe)
			goto regen;
		if ((idev->rndid[2]|idev->rndid[3]|idev->rndid[4]|idev->rndid[5]|idev->rndid[6]|idev->rndid[7]) == 0x00)
			goto regen;
	}

	return 0;
}

static void ipv6_regen_rndid(unsigned long data)
{
	struct inet6_dev *idev = (struct inet6_dev *) data;
	unsigned long expires;

	rcu_read_lock_bh();
	write_lock_bh(&idev->lock);

	if (idev->dead)
		goto out;

	if (__ipv6_regen_rndid(idev) < 0)
		goto out;

	expires = jiffies +
		idev->cnf.temp_prefered_lft * HZ -
		idev->cnf.regen_max_retry * idev->cnf.dad_transmits * idev->nd_parms->retrans_time -
		idev->cnf.max_desync_factor * HZ;
	if (time_before(expires, jiffies)) {
		printk(KERN_WARNING
			"ipv6_regen_rndid(): too short regeneration interval; timer disabled for %s.\n",
			idev->dev->name);
		goto out;
	}

	if (!mod_timer(&idev->regen_timer, expires))
		in6_dev_hold(idev);

out:
	write_unlock_bh(&idev->lock);
	rcu_read_unlock_bh();
	in6_dev_put(idev);
}

static int __ipv6_try_regen_rndid(struct inet6_dev *idev, struct in6_addr *tmpaddr) {
	int ret = 0;

	if (tmpaddr && memcmp(idev->rndid, &tmpaddr->s6_addr[8], 8) == 0)
		ret = __ipv6_regen_rndid(idev);
	return ret;
}
#endif

u32 addrconf_rt_table(const struct net_device *dev, u32 default_table) {
	/* Determines into what table to put autoconf PIO/RIO/default routes
	 * learned on this device.
	 *
	 * - If 0, use the same table for every device. This puts routes into
	 *   one of RT_TABLE_{PREFIX,INFO,DFLT} depending on the type of route
	 *   (but note that these three are currently all equal to
	 *   RT6_TABLE_MAIN).
	 * - If > 0, use the specified table.
	 * - If < 0, put routes into table dev->ifindex + (-rt_table).
	 */
	struct inet6_dev *idev = in6_dev_get(dev);
	u32 table;
	int sysctl = idev->cnf.accept_ra_rt_table;
	if (sysctl == 0) {
		table = default_table;
	} else if (sysctl > 0) {
		table = (u32) sysctl;
	} else {
		table = (unsigned) dev->ifindex + (-sysctl);
	}
	in6_dev_put(idev);
	return table;
=======
/* Generation of a randomized Interface Identifier
 * draft-ietf-6man-rfc4941bis, Section 3.3.1
 */

static void ipv6_gen_rnd_iid(struct in6_addr *addr)
{
regen:
	get_random_bytes(&addr->s6_addr[8], 8);

	/* <draft-ietf-6man-rfc4941bis-08.txt>, Section 3.3.1:
	 * check if generated address is not inappropriate:
	 *
	 * - Reserved IPv6 Interface Identifiers
	 * - XXX: already assigned to an address on the device
	 */

	/* Subnet-router anycast: 0000:0000:0000:0000 */
	if (!(addr->s6_addr32[2] | addr->s6_addr32[3]))
		goto regen;

	/* IANA Ethernet block: 0200:5EFF:FE00:0000-0200:5EFF:FE00:5212
	 * Proxy Mobile IPv6:   0200:5EFF:FE00:5213
	 * IANA Ethernet block: 0200:5EFF:FE00:5214-0200:5EFF:FEFF:FFFF
	 */
	if (ntohl(addr->s6_addr32[2]) == 0x02005eff &&
	    (ntohl(addr->s6_addr32[3]) & 0Xff000000) == 0xfe000000)
		goto regen;

	/* Reserved subnet anycast addresses */
	if (ntohl(addr->s6_addr32[2]) == 0xfdffffff &&
	    ntohl(addr->s6_addr32[3]) >= 0Xffffff80)
		goto regen;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Add prefix route.
 */

static void
<<<<<<< HEAD
addrconf_prefix_route(struct in6_addr *pfx, int plen, struct net_device *dev,
		      unsigned long expires, u32 flags)
{
	struct fib6_config cfg = {
		.fc_table = addrconf_rt_table(dev, RT6_TABLE_PREFIX),
		.fc_metric = IP6_RT_PRIO_ADDRCONF,
=======
addrconf_prefix_route(struct in6_addr *pfx, int plen, u32 metric,
		      struct net_device *dev, unsigned long expires,
		      u32 flags, gfp_t gfp_flags)
{
	struct fib6_config cfg = {
		.fc_table = l3mdev_fib_table(dev) ? : RT6_TABLE_PREFIX,
		.fc_metric = metric ? : IP6_RT_PRIO_ADDRCONF,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.fc_ifindex = dev->ifindex,
		.fc_expires = expires,
		.fc_dst_len = plen,
		.fc_flags = RTF_UP | flags,
		.fc_nlinfo.nl_net = dev_net(dev),
		.fc_protocol = RTPROT_KERNEL,
<<<<<<< HEAD
=======
		.fc_type = RTN_UNICAST,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	};

	cfg.fc_dst = *pfx;

	/* Prevent useless cloning on PtP SIT.
	   This thing is done here expecting that the whole
	   class of non-broadcast devices need not cloning.
	 */
<<<<<<< HEAD
#if defined(CONFIG_IPV6_SIT) || defined(CONFIG_IPV6_SIT_MODULE)
=======
#if IS_ENABLED(CONFIG_IPV6_SIT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (dev->type == ARPHRD_SIT && (dev->flags & IFF_POINTOPOINT))
		cfg.fc_flags |= RTF_NONEXTHOP;
#endif

<<<<<<< HEAD
	ip6_route_add(&cfg);
}


static struct rt6_info *addrconf_get_prefix_route(const struct in6_addr *pfx,
						  int plen,
						  const struct net_device *dev,
						  u32 flags, u32 noflags)
{
	struct fib6_node *fn;
	struct rt6_info *rt = NULL;
	struct fib6_table *table;

	table = fib6_get_table(dev_net(dev),
			       addrconf_rt_table(dev, RT6_TABLE_PREFIX));
	if (table == NULL)
		return NULL;

	write_lock_bh(&table->tb6_lock);
	fn = fib6_locate(&table->tb6_root, pfx, plen, NULL, 0);
	if (!fn)
		goto out;
	for (rt = fn->leaf; rt; rt = rt->dst.rt6_next) {
		if (rt->dst.dev->ifindex != dev->ifindex)
			continue;
		if ((rt->rt6i_flags & flags) != flags)
			continue;
		if ((rt->rt6i_flags & noflags) != 0)
			continue;
		dst_hold(&rt->dst);
		break;
	}
out:
	write_unlock_bh(&table->tb6_lock);
=======
	ip6_route_add(&cfg, gfp_flags, NULL);
}


static struct fib6_info *addrconf_get_prefix_route(const struct in6_addr *pfx,
						  int plen,
						  const struct net_device *dev,
						  u32 flags, u32 noflags,
						  bool no_gw)
{
	struct fib6_node *fn;
	struct fib6_info *rt = NULL;
	struct fib6_table *table;
	u32 tb_id = l3mdev_fib_table(dev) ? : RT6_TABLE_PREFIX;

	table = fib6_get_table(dev_net(dev), tb_id);
	if (!table)
		return NULL;

	rcu_read_lock();
	fn = fib6_locate(&table->tb6_root, pfx, plen, NULL, 0, true);
	if (!fn)
		goto out;

	for_each_fib6_node_rt_rcu(fn) {
		/* prefix routes only use builtin fib6_nh */
		if (rt->nh)
			continue;

		if (rt->fib6_nh->fib_nh_dev->ifindex != dev->ifindex)
			continue;
		if (no_gw && rt->fib6_nh->fib_nh_gw_family)
			continue;
		if ((rt->fib6_flags & flags) != flags)
			continue;
		if ((rt->fib6_flags & noflags) != 0)
			continue;
		if (!fib6_info_hold_safe(rt))
			continue;
		break;
	}
out:
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rt;
}


/* Create "default" multicast route to the interface */

static void addrconf_add_mroute(struct net_device *dev)
{
	struct fib6_config cfg = {
<<<<<<< HEAD
		.fc_table = RT6_TABLE_LOCAL,
=======
		.fc_table = l3mdev_fib_table(dev) ? : RT6_TABLE_LOCAL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.fc_metric = IP6_RT_PRIO_ADDRCONF,
		.fc_ifindex = dev->ifindex,
		.fc_dst_len = 8,
		.fc_flags = RTF_UP,
<<<<<<< HEAD
		.fc_nlinfo.nl_net = dev_net(dev),
=======
		.fc_type = RTN_MULTICAST,
		.fc_nlinfo.nl_net = dev_net(dev),
		.fc_protocol = RTPROT_KERNEL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	};

	ipv6_addr_set(&cfg.fc_dst, htonl(0xFF000000), 0, 0, 0);

<<<<<<< HEAD
	ip6_route_add(&cfg);
}

#if defined(CONFIG_IPV6_SIT) || defined(CONFIG_IPV6_SIT_MODULE)
static void sit_route_add(struct net_device *dev)
{
	struct fib6_config cfg = {
		.fc_table = RT6_TABLE_MAIN,
		.fc_metric = IP6_RT_PRIO_ADDRCONF,
		.fc_ifindex = dev->ifindex,
		.fc_dst_len = 96,
		.fc_flags = RTF_UP | RTF_NONEXTHOP,
		.fc_nlinfo.nl_net = dev_net(dev),
	};

	/* prefix length - 96 bits "::d.d.d.d" */
	ip6_route_add(&cfg);
}
#endif

static void addrconf_add_lroute(struct net_device *dev)
{
	struct in6_addr addr;

	ipv6_addr_set(&addr,  htonl(0xFE800000), 0, 0, 0);
	addrconf_prefix_route(&addr, 64, dev, 0, 0);
=======
	ip6_route_add(&cfg, GFP_KERNEL, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct inet6_dev *addrconf_add_dev(struct net_device *dev)
{
	struct inet6_dev *idev;

	ASSERT_RTNL();

	idev = ipv6_find_idev(dev);
<<<<<<< HEAD
	if (!idev)
		return ERR_PTR(-ENOBUFS);
=======
	if (IS_ERR(idev))
		return idev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (idev->cnf.disable_ipv6)
		return ERR_PTR(-EACCES);

	/* Add default multicast route */
<<<<<<< HEAD
	if (!(dev->flags & IFF_LOOPBACK))
		addrconf_add_mroute(dev);

	/* Add link local route */
	addrconf_add_lroute(dev);
	return idev;
}

void addrconf_prefix_rcv(struct net_device *dev, u8 *opt, int len, bool sllao)
{
	struct prefix_info *pinfo;
	__u32 valid_lft;
	__u32 prefered_lft;
	int addr_type;
	struct inet6_dev *in6_dev;
	struct net *net = dev_net(dev);

// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
	ADBG((KERN_DEBUG "[LGE_DATA][%s()] The prefix is received now !", __func__));
#endif
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21    

	pinfo = (struct prefix_info *) opt;

	if (len < sizeof(struct prefix_info)) {
		ADBG(("addrconf: prefix option too short\n"));
=======
	if (!(dev->flags & IFF_LOOPBACK) && !netif_is_l3_master(dev))
		addrconf_add_mroute(dev);

	return idev;
}

static void manage_tempaddrs(struct inet6_dev *idev,
			     struct inet6_ifaddr *ifp,
			     __u32 valid_lft, __u32 prefered_lft,
			     bool create, unsigned long now)
{
	u32 flags;
	struct inet6_ifaddr *ift;

	read_lock_bh(&idev->lock);
	/* update all temporary addresses in the list */
	list_for_each_entry(ift, &idev->tempaddr_list, tmp_list) {
		int age, max_valid, max_prefered;

		if (ifp != ift->ifpub)
			continue;

		/* RFC 4941 section 3.3:
		 * If a received option will extend the lifetime of a public
		 * address, the lifetimes of temporary addresses should
		 * be extended, subject to the overall constraint that no
		 * temporary addresses should ever remain "valid" or "preferred"
		 * for a time longer than (TEMP_VALID_LIFETIME) or
		 * (TEMP_PREFERRED_LIFETIME - DESYNC_FACTOR), respectively.
		 */
		age = (now - ift->cstamp) / HZ;
		max_valid = READ_ONCE(idev->cnf.temp_valid_lft) - age;
		if (max_valid < 0)
			max_valid = 0;

		max_prefered = READ_ONCE(idev->cnf.temp_prefered_lft) -
			       idev->desync_factor - age;
		if (max_prefered < 0)
			max_prefered = 0;

		if (valid_lft > max_valid)
			valid_lft = max_valid;

		if (prefered_lft > max_prefered)
			prefered_lft = max_prefered;

		spin_lock(&ift->lock);
		flags = ift->flags;
		ift->valid_lft = valid_lft;
		ift->prefered_lft = prefered_lft;
		ift->tstamp = now;
		if (prefered_lft > 0)
			ift->flags &= ~IFA_F_DEPRECATED;

		spin_unlock(&ift->lock);
		if (!(flags&IFA_F_TENTATIVE))
			ipv6_ifa_notify(0, ift);
	}

	/* Also create a temporary address if it's enabled but no temporary
	 * address currently exists.
	 * However, we get called with valid_lft == 0, prefered_lft == 0, create == false
	 * as part of cleanup (ie. deleting the mngtmpaddr).
	 * We don't want that to result in creating a new temporary ip address.
	 */
	if (list_empty(&idev->tempaddr_list) && (valid_lft || prefered_lft))
		create = true;

	if (create && READ_ONCE(idev->cnf.use_tempaddr) > 0) {
		/* When a new public address is created as described
		 * in [ADDRCONF], also create a new temporary address.
		 */
		read_unlock_bh(&idev->lock);
		ipv6_create_tempaddr(ifp, false);
	} else {
		read_unlock_bh(&idev->lock);
	}
}

static bool is_addr_mode_generate_stable(struct inet6_dev *idev)
{
	return idev->cnf.addr_gen_mode == IN6_ADDR_GEN_MODE_STABLE_PRIVACY ||
	       idev->cnf.addr_gen_mode == IN6_ADDR_GEN_MODE_RANDOM;
}

int addrconf_prefix_rcv_add_addr(struct net *net, struct net_device *dev,
				 const struct prefix_info *pinfo,
				 struct inet6_dev *in6_dev,
				 const struct in6_addr *addr, int addr_type,
				 u32 addr_flags, bool sllao, bool tokenized,
				 __u32 valid_lft, u32 prefered_lft)
{
	struct inet6_ifaddr *ifp = ipv6_get_ifaddr(net, addr, dev, 1);
	int create = 0, update_lft = 0;

	if (!ifp && valid_lft) {
		int max_addresses = READ_ONCE(in6_dev->cnf.max_addresses);
		struct ifa6_config cfg = {
			.pfx = addr,
			.plen = pinfo->prefix_len,
			.ifa_flags = addr_flags,
			.valid_lft = valid_lft,
			.preferred_lft = prefered_lft,
			.scope = addr_type & IPV6_ADDR_SCOPE_MASK,
			.ifa_proto = IFAPROT_KERNEL_RA
		};

#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
		if ((READ_ONCE(net->ipv6.devconf_all->optimistic_dad) ||
		     READ_ONCE(in6_dev->cnf.optimistic_dad)) &&
		    !net->ipv6.devconf_all->forwarding && sllao)
			cfg.ifa_flags |= IFA_F_OPTIMISTIC;
#endif

		/* Do not allow to create too much of autoconfigured
		 * addresses; this would be too easy way to crash kernel.
		 */
		if (!max_addresses ||
		    ipv6_count_addresses(in6_dev) < max_addresses)
			ifp = ipv6_add_addr(in6_dev, &cfg, false, NULL);

		if (IS_ERR_OR_NULL(ifp))
			return -1;

		create = 1;
		spin_lock_bh(&ifp->lock);
		ifp->flags |= IFA_F_MANAGETEMPADDR;
		ifp->cstamp = jiffies;
		ifp->tokenized = tokenized;
		spin_unlock_bh(&ifp->lock);
		addrconf_dad_start(ifp);
	}

	if (ifp) {
		u32 flags;
		unsigned long now;
		u32 stored_lft;

		/* update lifetime (RFC2462 5.5.3 e) */
		spin_lock_bh(&ifp->lock);
		now = jiffies;
		if (ifp->valid_lft > (now - ifp->tstamp) / HZ)
			stored_lft = ifp->valid_lft - (now - ifp->tstamp) / HZ;
		else
			stored_lft = 0;

		/* RFC4862 Section 5.5.3e:
		 * "Note that the preferred lifetime of the
		 *  corresponding address is always reset to
		 *  the Preferred Lifetime in the received
		 *  Prefix Information option, regardless of
		 *  whether the valid lifetime is also reset or
		 *  ignored."
		 *
		 * So we should always update prefered_lft here.
		 */
		update_lft = !create && stored_lft;

		if (update_lft && !READ_ONCE(in6_dev->cnf.ra_honor_pio_life)) {
			const u32 minimum_lft = min_t(u32,
				stored_lft, MIN_VALID_LIFETIME);
			valid_lft = max(valid_lft, minimum_lft);
		}

		if (update_lft) {
			ifp->valid_lft = valid_lft;
			ifp->prefered_lft = prefered_lft;
			WRITE_ONCE(ifp->tstamp, now);
			flags = ifp->flags;
			ifp->flags &= ~IFA_F_DEPRECATED;
			spin_unlock_bh(&ifp->lock);

			if (!(flags&IFA_F_TENTATIVE))
				ipv6_ifa_notify(0, ifp);
		} else
			spin_unlock_bh(&ifp->lock);

		manage_tempaddrs(in6_dev, ifp, valid_lft, prefered_lft,
				 create, now);

		in6_ifa_put(ifp);
		addrconf_verify(net);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(addrconf_prefix_rcv_add_addr);

void addrconf_prefix_rcv(struct net_device *dev, u8 *opt, int len, bool sllao)
{
	struct prefix_info *pinfo;
	struct fib6_table *table;
	__u32 valid_lft;
	__u32 prefered_lft;
	int addr_type, err;
	u32 addr_flags = 0;
	struct inet6_dev *in6_dev;
	struct net *net = dev_net(dev);

	pinfo = (struct prefix_info *) opt;

	if (len < sizeof(struct prefix_info)) {
		netdev_dbg(dev, "addrconf: prefix option too short\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	/*
	 *	Validation checks ([ADDRCONF], page 19)
	 */

	addr_type = ipv6_addr_type(&pinfo->prefix);

	if (addr_type & (IPV6_ADDR_MULTICAST|IPV6_ADDR_LINKLOCAL))
		return;

	valid_lft = ntohl(pinfo->valid);
	prefered_lft = ntohl(pinfo->prefered);

	if (prefered_lft > valid_lft) {
<<<<<<< HEAD
		if (net_ratelimit())
			printk(KERN_WARNING "addrconf: prefix option has invalid lifetime\n");
=======
		net_warn_ratelimited("addrconf: prefix option has invalid lifetime\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	in6_dev = in6_dev_get(dev);

<<<<<<< HEAD
	if (in6_dev == NULL) {
		if (net_ratelimit())
			printk(KERN_DEBUG "addrconf: device %s not configured\n", dev->name);
		return;
	}

=======
	if (!in6_dev) {
		net_dbg_ratelimited("addrconf: device %s not configured\n",
				    dev->name);
		return;
	}

	if (valid_lft != 0 && valid_lft < in6_dev->cnf.accept_ra_min_lft)
		goto put;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 *	Two things going on here:
	 *	1) Add routes for on-link prefixes
	 *	2) Configure prefixes with the auto flag set
	 */

	if (pinfo->onlink) {
<<<<<<< HEAD
		struct rt6_info *rt;
=======
		struct fib6_info *rt;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		unsigned long rt_expires;

		/* Avoid arithmetic overflow. Really, we could
		 * save rt_expires in seconds, likely valid_lft,
		 * but it would require division in fib gc, that it
		 * not good.
		 */
		if (HZ > USER_HZ)
			rt_expires = addrconf_timeout_fixup(valid_lft, HZ);
		else
			rt_expires = addrconf_timeout_fixup(valid_lft, USER_HZ);

		if (addrconf_finite_timeout(rt_expires))
			rt_expires *= HZ;

		rt = addrconf_get_prefix_route(&pinfo->prefix,
					       pinfo->prefix_len,
					       dev,
					       RTF_ADDRCONF | RTF_PREFIX_RT,
<<<<<<< HEAD
					       RTF_GATEWAY | RTF_DEFAULT);
=======
					       RTF_DEFAULT, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (rt) {
			/* Autoconf prefix route */
			if (valid_lft == 0) {
<<<<<<< HEAD
				ip6_del_rt(rt);
				rt = NULL;
			} else if (addrconf_finite_timeout(rt_expires)) {
				/* not infinity */
				rt6_set_expires(rt, jiffies + rt_expires);
			} else {
				rt6_clean_expires(rt);
=======
				ip6_del_rt(net, rt, false);
				rt = NULL;
			} else {
				table = rt->fib6_table;
				spin_lock_bh(&table->tb6_lock);

				if (addrconf_finite_timeout(rt_expires)) {
					/* not infinity */
					fib6_set_expires(rt, jiffies + rt_expires);
					fib6_add_gc_list(rt);
				} else {
					fib6_clean_expires(rt);
					fib6_remove_gc_list(rt);
				}

				spin_unlock_bh(&table->tb6_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		} else if (valid_lft) {
			clock_t expires = 0;
			int flags = RTF_ADDRCONF | RTF_PREFIX_RT;
			if (addrconf_finite_timeout(rt_expires)) {
				/* not infinity */
				flags |= RTF_EXPIRES;
				expires = jiffies_to_clock_t(rt_expires);
			}
<<<<<<< HEAD
			if (dev->ip6_ptr->cnf.accept_ra_prefix_route) {
				addrconf_prefix_route(&pinfo->prefix,
					pinfo->prefix_len, dev, expires, flags);
			}
		}
		if (rt)
			dst_release(&rt->dst);
=======
			addrconf_prefix_route(&pinfo->prefix, pinfo->prefix_len,
					      0, dev, expires, flags,
					      GFP_ATOMIC);
		}
		fib6_info_release(rt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Try to figure out our local address for this prefix */

	if (pinfo->autoconf && in6_dev->cnf.autoconf) {
<<<<<<< HEAD
		struct inet6_ifaddr * ifp;
		struct in6_addr addr;
		int create = 0, update_lft = 0;

		if (pinfo->prefix_len == 64) {
			memcpy(&addr, &pinfo->prefix, 8);
			if (ipv6_generate_eui64(addr.s6_addr + 8, dev) &&
			    ipv6_inherit_eui64(addr.s6_addr + 8, in6_dev)) {
				in6_dev_put(in6_dev);
				return;
			}
			goto ok;
		}
		if (net_ratelimit())
			printk(KERN_DEBUG "IPv6 addrconf: prefix with wrong length %d\n",
			       pinfo->prefix_len);
		in6_dev_put(in6_dev);
		return;

ok:

		ifp = ipv6_get_ifaddr(net, &addr, dev, 1);

		if (ifp == NULL && valid_lft) {
			int max_addresses = in6_dev->cnf.max_addresses;
			u32 addr_flags = 0;

#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
			if (in6_dev->cnf.optimistic_dad &&
			    !net->ipv6.devconf_all->forwarding && sllao)
				addr_flags = IFA_F_OPTIMISTIC;
#endif

			/* Do not allow to create too much of autoconfigured
			 * addresses; this would be too easy way to crash kernel.
			 */
			if (!max_addresses ||
			    ipv6_count_addresses(in6_dev) < max_addresses)
				ifp = ipv6_add_addr(in6_dev, &addr, pinfo->prefix_len,
						    addr_type&IPV6_ADDR_SCOPE_MASK,
						    addr_flags);

			if (!ifp || IS_ERR(ifp)) {
				in6_dev_put(in6_dev);
				return;
			}

			update_lft = create = 1;
			ifp->cstamp = jiffies;
			addrconf_dad_start(ifp, RTF_ADDRCONF|RTF_PREFIX_RT);
		}

		if (ifp) {
			int flags;
			unsigned long now;
#ifdef CONFIG_IPV6_PRIVACY
			struct inet6_ifaddr *ift;
#endif
			u32 stored_lft;

			/* update lifetime (RFC2462 5.5.3 e) */
			spin_lock(&ifp->lock);
			now = jiffies;
			if (ifp->valid_lft > (now - ifp->tstamp) / HZ)
				stored_lft = ifp->valid_lft - (now - ifp->tstamp) / HZ;
			else
				stored_lft = 0;
			if (!update_lft && stored_lft) {
				if (valid_lft > MIN_VALID_LIFETIME ||
				    valid_lft > stored_lft)
					update_lft = 1;
				else if (stored_lft <= MIN_VALID_LIFETIME) {
					/* valid_lft <= stored_lft is always true */
					/*
					 * RFC 4862 Section 5.5.3e:
					 * "Note that the preferred lifetime of
					 *  the corresponding address is always
					 *  reset to the Preferred Lifetime in
					 *  the received Prefix Information
					 *  option, regardless of whether the
					 *  valid lifetime is also reset or
					 *  ignored."
					 *
					 *  So if the preferred lifetime in
					 *  this advertisement is different
					 *  than what we have stored, but the
					 *  valid lifetime is invalid, just
					 *  reset prefered_lft.
					 *
					 *  We must set the valid lifetime
					 *  to the stored lifetime since we'll
					 *  be updating the timestamp below,
					 *  else we'll set it back to the
					 *  minimum.
					 */
					if (prefered_lft != ifp->prefered_lft) {
						valid_lft = stored_lft;
						update_lft = 1;
					}
				} else {
					valid_lft = MIN_VALID_LIFETIME;
					if (valid_lft < prefered_lft)
						prefered_lft = valid_lft;
					update_lft = 1;
				}
			}

			if (update_lft) {
				ifp->valid_lft = valid_lft;
				ifp->prefered_lft = prefered_lft;
				ifp->tstamp = now;
				flags = ifp->flags;
				ifp->flags &= ~IFA_F_DEPRECATED;
				spin_unlock(&ifp->lock);

				if (!(flags&IFA_F_TENTATIVE))
					ipv6_ifa_notify(0, ifp);
			} else
				spin_unlock(&ifp->lock);

#ifdef CONFIG_IPV6_PRIVACY
			read_lock_bh(&in6_dev->lock);
			/* update all temporary addresses in the list */
			list_for_each_entry(ift, &in6_dev->tempaddr_list,
					    tmp_list) {
				int age, max_valid, max_prefered;

				if (ifp != ift->ifpub)
					continue;

				/*
				 * RFC 4941 section 3.3:
				 * If a received option will extend the lifetime
				 * of a public address, the lifetimes of
				 * temporary addresses should be extended,
				 * subject to the overall constraint that no
				 * temporary addresses should ever remain
				 * "valid" or "preferred" for a time longer than
				 * (TEMP_VALID_LIFETIME) or
				 * (TEMP_PREFERRED_LIFETIME - DESYNC_FACTOR),
				 * respectively.
				 */
				age = (now - ift->cstamp) / HZ;
				max_valid = in6_dev->cnf.temp_valid_lft - age;
				if (max_valid < 0)
					max_valid = 0;

				max_prefered = in6_dev->cnf.temp_prefered_lft -
					       in6_dev->cnf.max_desync_factor -
					       age;
				if (max_prefered < 0)
					max_prefered = 0;

				if (valid_lft > max_valid)
					valid_lft = max_valid;

				if (prefered_lft > max_prefered)
					prefered_lft = max_prefered;

				spin_lock(&ift->lock);
				flags = ift->flags;
				ift->valid_lft = valid_lft;
				ift->prefered_lft = prefered_lft;
				ift->tstamp = now;
				if (prefered_lft > 0)
					ift->flags &= ~IFA_F_DEPRECATED;

				spin_unlock(&ift->lock);
				if (!(flags&IFA_F_TENTATIVE))
					ipv6_ifa_notify(0, ift);
			}

			if ((create || list_empty(&in6_dev->tempaddr_list)) && in6_dev->cnf.use_tempaddr > 0) {
				/*
				 * When a new public address is created as
				 * described in [ADDRCONF], also create a new
				 * temporary address. Also create a temporary
				 * address if it's enabled but no temporary
				 * address currently exists.
				 */
				read_unlock_bh(&in6_dev->lock);
				ipv6_create_tempaddr(ifp, NULL);
			} else {
				read_unlock_bh(&in6_dev->lock);
			}
#endif
			in6_ifa_put(ifp);
			addrconf_verify(0);
		}
	}
	inet6_prefix_notify(RTM_NEWPREFIX, in6_dev, pinfo);
	in6_dev_put(in6_dev);
}

=======
		struct in6_addr addr;
		bool tokenized = false, dev_addr_generated = false;

		if (pinfo->prefix_len == 64) {
			memcpy(&addr, &pinfo->prefix, 8);

			if (!ipv6_addr_any(&in6_dev->token)) {
				read_lock_bh(&in6_dev->lock);
				memcpy(addr.s6_addr + 8,
				       in6_dev->token.s6_addr + 8, 8);
				read_unlock_bh(&in6_dev->lock);
				tokenized = true;
			} else if (is_addr_mode_generate_stable(in6_dev) &&
				   !ipv6_generate_stable_address(&addr, 0,
								 in6_dev)) {
				addr_flags |= IFA_F_STABLE_PRIVACY;
				goto ok;
			} else if (ipv6_generate_eui64(addr.s6_addr + 8, dev) &&
				   ipv6_inherit_eui64(addr.s6_addr + 8, in6_dev)) {
				goto put;
			} else {
				dev_addr_generated = true;
			}
			goto ok;
		}
		net_dbg_ratelimited("IPv6 addrconf: prefix with wrong length %d\n",
				    pinfo->prefix_len);
		goto put;

ok:
		err = addrconf_prefix_rcv_add_addr(net, dev, pinfo, in6_dev,
						   &addr, addr_type,
						   addr_flags, sllao,
						   tokenized, valid_lft,
						   prefered_lft);
		if (err)
			goto put;

		/* Ignore error case here because previous prefix add addr was
		 * successful which will be notified.
		 */
		ndisc_ops_prefix_rcv_add_addr(net, dev, pinfo, in6_dev, &addr,
					      addr_type, addr_flags, sllao,
					      tokenized, valid_lft,
					      prefered_lft,
					      dev_addr_generated);
	}
	inet6_prefix_notify(RTM_NEWPREFIX, in6_dev, pinfo);
put:
	in6_dev_put(in6_dev);
}

static int addrconf_set_sit_dstaddr(struct net *net, struct net_device *dev,
		struct in6_ifreq *ireq)
{
	struct ip_tunnel_parm p = { };
	int err;

	if (!(ipv6_addr_type(&ireq->ifr6_addr) & IPV6_ADDR_COMPATv4))
		return -EADDRNOTAVAIL;

	p.iph.daddr = ireq->ifr6_addr.s6_addr32[3];
	p.iph.version = 4;
	p.iph.ihl = 5;
	p.iph.protocol = IPPROTO_IPV6;
	p.iph.ttl = 64;

	if (!dev->netdev_ops->ndo_tunnel_ctl)
		return -EOPNOTSUPP;
	err = dev->netdev_ops->ndo_tunnel_ctl(dev, &p, SIOCADDTUNNEL);
	if (err)
		return err;

	dev = __dev_get_by_name(net, p.name);
	if (!dev)
		return -ENOBUFS;
	return dev_open(dev, NULL);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Set destination address.
 *	Special case for SIT interfaces where we create a new "virtual"
 *	device.
 */
int addrconf_set_dstaddr(struct net *net, void __user *arg)
{
<<<<<<< HEAD
	struct in6_ifreq ireq;
	struct net_device *dev;
	int err = -EINVAL;

	rtnl_lock();

	err = -EFAULT;
	if (copy_from_user(&ireq, arg, sizeof(struct in6_ifreq)))
		goto err_exit;

	dev = __dev_get_by_index(net, ireq.ifr6_ifindex);

	err = -ENODEV;
	if (dev == NULL)
		goto err_exit;

#if defined(CONFIG_IPV6_SIT) || defined(CONFIG_IPV6_SIT_MODULE)
	if (dev->type == ARPHRD_SIT) {
		const struct net_device_ops *ops = dev->netdev_ops;
		struct ifreq ifr;
		struct ip_tunnel_parm p;

		err = -EADDRNOTAVAIL;
		if (!(ipv6_addr_type(&ireq.ifr6_addr) & IPV6_ADDR_COMPATv4))
			goto err_exit;

		memset(&p, 0, sizeof(p));
		p.iph.daddr = ireq.ifr6_addr.s6_addr32[3];
		p.iph.saddr = 0;
		p.iph.version = 4;
		p.iph.ihl = 5;
		p.iph.protocol = IPPROTO_IPV6;
		p.iph.ttl = 64;
		ifr.ifr_ifru.ifru_data = (__force void __user *)&p;

		if (ops->ndo_do_ioctl) {
			mm_segment_t oldfs = get_fs();

			set_fs(KERNEL_DS);
			err = ops->ndo_do_ioctl(dev, &ifr, SIOCADDTUNNEL);
			set_fs(oldfs);
		} else
			err = -EOPNOTSUPP;

		if (err == 0) {
			err = -ENOBUFS;
			dev = __dev_get_by_name(net, p.name);
			if (!dev)
				goto err_exit;
			err = dev_open(dev);
		}
	}
#endif

err_exit:
=======
	struct net_device *dev;
	struct in6_ifreq ireq;
	int err = -ENODEV;

	if (!IS_ENABLED(CONFIG_IPV6_SIT))
		return -ENODEV;
	if (copy_from_user(&ireq, arg, sizeof(struct in6_ifreq)))
		return -EFAULT;

	rtnl_lock();
	dev = __dev_get_by_index(net, ireq.ifr6_ifindex);
	if (dev && dev->type == ARPHRD_SIT)
		err = addrconf_set_sit_dstaddr(net, dev, &ireq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rtnl_unlock();
	return err;
}

<<<<<<< HEAD
/*
 *	Manual configuration of address on an interface
 */
static int inet6_addr_add(struct net *net, int ifindex, const struct in6_addr *pfx,
			  unsigned int plen, __u8 ifa_flags, __u32 prefered_lft,
			  __u32 valid_lft)
=======
static int ipv6_mc_config(struct sock *sk, bool join,
			  const struct in6_addr *addr, int ifindex)
{
	int ret;

	ASSERT_RTNL();

	lock_sock(sk);
	if (join)
		ret = ipv6_sock_mc_join(sk, ifindex, addr);
	else
		ret = ipv6_sock_mc_drop(sk, ifindex, addr);
	release_sock(sk);

	return ret;
}

/*
 *	Manual configuration of address on an interface
 */
static int inet6_addr_add(struct net *net, int ifindex,
			  struct ifa6_config *cfg,
			  struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inet6_ifaddr *ifp;
	struct inet6_dev *idev;
	struct net_device *dev;
<<<<<<< HEAD
	int scope;
	u32 flags;
	clock_t expires;
	unsigned long timeout;

	ASSERT_RTNL();

	if (plen > 128)
		return -EINVAL;

	/* check the lifetime */
	if (!valid_lft || prefered_lft > valid_lft)
		return -EINVAL;
=======
	unsigned long timeout;
	clock_t expires;
	u32 flags;

	ASSERT_RTNL();

	if (cfg->plen > 128) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid prefix length");
		return -EINVAL;
	}

	/* check the lifetime */
	if (!cfg->valid_lft || cfg->preferred_lft > cfg->valid_lft) {
		NL_SET_ERR_MSG_MOD(extack, "address lifetime invalid");
		return -EINVAL;
	}

	if (cfg->ifa_flags & IFA_F_MANAGETEMPADDR && cfg->plen != 64) {
		NL_SET_ERR_MSG_MOD(extack, "address with \"mngtmpaddr\" flag must have a prefix length of 64");
		return -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dev = __dev_get_by_index(net, ifindex);
	if (!dev)
		return -ENODEV;

	idev = addrconf_add_dev(dev);
<<<<<<< HEAD
	if (IS_ERR(idev))
		return PTR_ERR(idev);

	scope = ipv6_addr_scope(pfx);

	timeout = addrconf_timeout_fixup(valid_lft, HZ);
	if (addrconf_finite_timeout(timeout)) {
		expires = jiffies_to_clock_t(timeout * HZ);
		valid_lft = timeout;
=======
	if (IS_ERR(idev)) {
		NL_SET_ERR_MSG_MOD(extack, "IPv6 is disabled on this device");
		return PTR_ERR(idev);
	}

	if (cfg->ifa_flags & IFA_F_MCAUTOJOIN) {
		int ret = ipv6_mc_config(net->ipv6.mc_autojoin_sk,
					 true, cfg->pfx, ifindex);

		if (ret < 0) {
			NL_SET_ERR_MSG_MOD(extack, "Multicast auto join failed");
			return ret;
		}
	}

	cfg->scope = ipv6_addr_scope(cfg->pfx);

	timeout = addrconf_timeout_fixup(cfg->valid_lft, HZ);
	if (addrconf_finite_timeout(timeout)) {
		expires = jiffies_to_clock_t(timeout * HZ);
		cfg->valid_lft = timeout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		flags = RTF_EXPIRES;
	} else {
		expires = 0;
		flags = 0;
<<<<<<< HEAD
		ifa_flags |= IFA_F_PERMANENT;
	}

	timeout = addrconf_timeout_fixup(prefered_lft, HZ);
	if (addrconf_finite_timeout(timeout)) {
		if (timeout == 0)
			ifa_flags |= IFA_F_DEPRECATED;
		prefered_lft = timeout;
	}

	ifp = ipv6_add_addr(idev, pfx, plen, scope, ifa_flags);

	if (!IS_ERR(ifp)) {
		spin_lock_bh(&ifp->lock);
		ifp->valid_lft = valid_lft;
		ifp->prefered_lft = prefered_lft;
		ifp->tstamp = jiffies;
		spin_unlock_bh(&ifp->lock);

		addrconf_prefix_route(&ifp->addr, ifp->prefix_len, dev,
				      expires, flags);
=======
		cfg->ifa_flags |= IFA_F_PERMANENT;
	}

	timeout = addrconf_timeout_fixup(cfg->preferred_lft, HZ);
	if (addrconf_finite_timeout(timeout)) {
		if (timeout == 0)
			cfg->ifa_flags |= IFA_F_DEPRECATED;
		cfg->preferred_lft = timeout;
	}

	ifp = ipv6_add_addr(idev, cfg, true, extack);
	if (!IS_ERR(ifp)) {
		if (!(cfg->ifa_flags & IFA_F_NOPREFIXROUTE)) {
			addrconf_prefix_route(&ifp->addr, ifp->prefix_len,
					      ifp->rt_priority, dev, expires,
					      flags, GFP_KERNEL);
		}

		/* Send a netlink notification if DAD is enabled and
		 * optimistic flag is not set
		 */
		if (!(ifp->flags & (IFA_F_OPTIMISTIC | IFA_F_NODAD)))
			ipv6_ifa_notify(0, ifp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Note that section 3.1 of RFC 4429 indicates
		 * that the Optimistic flag should not be set for
		 * manually configured addresses
		 */
<<<<<<< HEAD
		addrconf_dad_start(ifp, 0);
		in6_ifa_put(ifp);
		addrconf_verify(0);
		return 0;
=======
		addrconf_dad_start(ifp);
		if (cfg->ifa_flags & IFA_F_MANAGETEMPADDR)
			manage_tempaddrs(idev, ifp, cfg->valid_lft,
					 cfg->preferred_lft, true, jiffies);
		in6_ifa_put(ifp);
		addrconf_verify_rtnl(net);
		return 0;
	} else if (cfg->ifa_flags & IFA_F_MCAUTOJOIN) {
		ipv6_mc_config(net->ipv6.mc_autojoin_sk, false,
			       cfg->pfx, ifindex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return PTR_ERR(ifp);
}

<<<<<<< HEAD
static int inet6_addr_del(struct net *net, int ifindex, const struct in6_addr *pfx,
			  unsigned int plen)
=======
static int inet6_addr_del(struct net *net, int ifindex, u32 ifa_flags,
			  const struct in6_addr *pfx, unsigned int plen,
			  struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inet6_ifaddr *ifp;
	struct inet6_dev *idev;
	struct net_device *dev;

<<<<<<< HEAD
	if (plen > 128)
		return -EINVAL;

	dev = __dev_get_by_index(net, ifindex);
	if (!dev)
		return -ENODEV;

	if ((idev = __in6_dev_get(dev)) == NULL)
		return -ENXIO;
=======
	if (plen > 128) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid prefix length");
		return -EINVAL;
	}

	dev = __dev_get_by_index(net, ifindex);
	if (!dev) {
		NL_SET_ERR_MSG_MOD(extack, "Unable to find the interface");
		return -ENODEV;
	}

	idev = __in6_dev_get(dev);
	if (!idev) {
		NL_SET_ERR_MSG_MOD(extack, "IPv6 is disabled on this device");
		return -ENXIO;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	read_lock_bh(&idev->lock);
	list_for_each_entry(ifp, &idev->addr_list, if_list) {
		if (ifp->prefix_len == plen &&
		    ipv6_addr_equal(pfx, &ifp->addr)) {
			in6_ifa_hold(ifp);
			read_unlock_bh(&idev->lock);

<<<<<<< HEAD
			ipv6_del_addr(ifp);

			/* If the last address is deleted administratively,
			   disable IPv6 on this interface.
			 */
			if (list_empty(&idev->addr_list))
				addrconf_ifdown(idev->dev, 1);
=======
			if (!(ifp->flags & IFA_F_TEMPORARY) &&
			    (ifa_flags & IFA_F_MANAGETEMPADDR))
				manage_tempaddrs(idev, ifp, 0, 0, false,
						 jiffies);
			ipv6_del_addr(ifp);
			addrconf_verify_rtnl(net);
			if (ipv6_addr_is_multicast(pfx)) {
				ipv6_mc_config(net->ipv6.mc_autojoin_sk,
					       false, pfx, dev->ifindex);
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 0;
		}
	}
	read_unlock_bh(&idev->lock);
<<<<<<< HEAD
=======

	NL_SET_ERR_MSG_MOD(extack, "address not found");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -EADDRNOTAVAIL;
}


int addrconf_add_ifaddr(struct net *net, void __user *arg)
{
<<<<<<< HEAD
	struct in6_ifreq ireq;
	int err;

	if (!capable(CAP_NET_ADMIN))
=======
	struct ifa6_config cfg = {
		.ifa_flags = IFA_F_PERMANENT,
		.preferred_lft = INFINITY_LIFE_TIME,
		.valid_lft = INFINITY_LIFE_TIME,
	};
	struct in6_ifreq ireq;
	int err;

	if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EPERM;

	if (copy_from_user(&ireq, arg, sizeof(struct in6_ifreq)))
		return -EFAULT;

<<<<<<< HEAD
	rtnl_lock();
	err = inet6_addr_add(net, ireq.ifr6_ifindex, &ireq.ifr6_addr,
			     ireq.ifr6_prefixlen, IFA_F_PERMANENT,
			     INFINITY_LIFE_TIME, INFINITY_LIFE_TIME);
=======
	cfg.pfx = &ireq.ifr6_addr;
	cfg.plen = ireq.ifr6_prefixlen;

	rtnl_lock();
	err = inet6_addr_add(net, ireq.ifr6_ifindex, &cfg, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rtnl_unlock();
	return err;
}

int addrconf_del_ifaddr(struct net *net, void __user *arg)
{
	struct in6_ifreq ireq;
	int err;

<<<<<<< HEAD
	if (!capable(CAP_NET_ADMIN))
=======
	if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EPERM;

	if (copy_from_user(&ireq, arg, sizeof(struct in6_ifreq)))
		return -EFAULT;

	rtnl_lock();
<<<<<<< HEAD
	err = inet6_addr_del(net, ireq.ifr6_ifindex, &ireq.ifr6_addr,
			     ireq.ifr6_prefixlen);
=======
	err = inet6_addr_del(net, ireq.ifr6_ifindex, 0, &ireq.ifr6_addr,
			     ireq.ifr6_prefixlen, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rtnl_unlock();
	return err;
}

static void add_addr(struct inet6_dev *idev, const struct in6_addr *addr,
<<<<<<< HEAD
		     int plen, int scope)
{
	struct inet6_ifaddr *ifp;

	ifp = ipv6_add_addr(idev, addr, plen, scope, IFA_F_PERMANENT);
=======
		     int plen, int scope, u8 proto)
{
	struct inet6_ifaddr *ifp;
	struct ifa6_config cfg = {
		.pfx = addr,
		.plen = plen,
		.ifa_flags = IFA_F_PERMANENT,
		.valid_lft = INFINITY_LIFE_TIME,
		.preferred_lft = INFINITY_LIFE_TIME,
		.scope = scope,
		.ifa_proto = proto
	};

	ifp = ipv6_add_addr(idev, &cfg, true, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!IS_ERR(ifp)) {
		spin_lock_bh(&ifp->lock);
		ifp->flags &= ~IFA_F_TENTATIVE;
		spin_unlock_bh(&ifp->lock);
<<<<<<< HEAD
=======
		rt_genid_bump_ipv6(dev_net(idev->dev));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ipv6_ifa_notify(RTM_NEWADDR, ifp);
		in6_ifa_put(ifp);
	}
}

<<<<<<< HEAD
#if defined(CONFIG_IPV6_SIT) || defined(CONFIG_IPV6_SIT_MODULE)
static void sit_add_v4_addrs(struct inet6_dev *idev)
=======
#if IS_ENABLED(CONFIG_IPV6_SIT) || IS_ENABLED(CONFIG_NET_IPGRE) || IS_ENABLED(CONFIG_IPV6_GRE)
static void add_v4_addrs(struct inet6_dev *idev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct in6_addr addr;
	struct net_device *dev;
	struct net *net = dev_net(idev->dev);
<<<<<<< HEAD
	int scope;
=======
	int scope, plen, offset = 0;
	u32 pflags = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ASSERT_RTNL();

	memset(&addr, 0, sizeof(struct in6_addr));
<<<<<<< HEAD
	memcpy(&addr.s6_addr32[3], idev->dev->dev_addr, 4);

	if (idev->dev->flags&IFF_POINTOPOINT) {
		addr.s6_addr32[0] = htonl(0xfe800000);
		scope = IFA_LINK;
	} else {
		scope = IPV6_ADDR_COMPATv4;
	}

	if (addr.s6_addr32[3]) {
		add_addr(idev, &addr, 128, scope);
=======
	/* in case of IP6GRE the dev_addr is an IPv6 and therefore we use only the last 4 bytes */
	if (idev->dev->addr_len == sizeof(struct in6_addr))
		offset = sizeof(struct in6_addr) - 4;
	memcpy(&addr.s6_addr32[3], idev->dev->dev_addr + offset, 4);

	if (!(idev->dev->flags & IFF_POINTOPOINT) && idev->dev->type == ARPHRD_SIT) {
		scope = IPV6_ADDR_COMPATv4;
		plen = 96;
		pflags |= RTF_NONEXTHOP;
	} else {
		if (idev->cnf.addr_gen_mode == IN6_ADDR_GEN_MODE_NONE)
			return;

		addr.s6_addr32[0] = htonl(0xfe800000);
		scope = IFA_LINK;
		plen = 64;
	}

	if (addr.s6_addr32[3]) {
		add_addr(idev, &addr, plen, scope, IFAPROT_UNSPEC);
		addrconf_prefix_route(&addr, plen, 0, idev->dev, 0, pflags,
				      GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	for_each_netdev(net, dev) {
<<<<<<< HEAD
		struct in_device * in_dev = __in_dev_get_rtnl(dev);
		if (in_dev && (dev->flags & IFF_UP)) {
			struct in_ifaddr * ifa;

			int flag = scope;

			for (ifa = in_dev->ifa_list; ifa; ifa = ifa->ifa_next) {
				int plen;

=======
		struct in_device *in_dev = __in_dev_get_rtnl(dev);
		if (in_dev && (dev->flags & IFF_UP)) {
			struct in_ifaddr *ifa;
			int flag = scope;

			in_dev_for_each_ifa_rtnl(ifa, in_dev) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				addr.s6_addr32[3] = ifa->ifa_local;

				if (ifa->ifa_scope == RT_SCOPE_LINK)
					continue;
				if (ifa->ifa_scope >= RT_SCOPE_HOST) {
					if (idev->dev->flags&IFF_POINTOPOINT)
						continue;
					flag |= IFA_HOST;
				}
<<<<<<< HEAD
				if (idev->dev->flags&IFF_POINTOPOINT)
					plen = 64;
				else
					plen = 96;

				add_addr(idev, &addr, plen, flag);
=======

				add_addr(idev, &addr, plen, flag,
					 IFAPROT_UNSPEC);
				addrconf_prefix_route(&addr, plen, 0, idev->dev,
						      0, pflags, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
	}
}
#endif

static void init_loopback(struct net_device *dev)
{
	struct inet6_dev  *idev;
<<<<<<< HEAD
	struct net_device *sp_dev;
	struct inet6_ifaddr *sp_ifa;
	struct rt6_info *sp_rt;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* ::1 */

	ASSERT_RTNL();

<<<<<<< HEAD
	if ((idev = ipv6_find_idev(dev)) == NULL) {
		printk(KERN_DEBUG "init loopback: add_dev failed\n");
		return;
	}

	add_addr(idev, &in6addr_loopback, 128, IFA_HOST);

	/* Add routes to other interface's IPv6 addresses */
	for_each_netdev(dev_net(dev), sp_dev) {
		if (!strcmp(sp_dev->name, dev->name))
			continue;

		idev = __in6_dev_get(sp_dev);
		if (!idev)
			continue;

		read_lock_bh(&idev->lock);
		list_for_each_entry(sp_ifa, &idev->addr_list, if_list) {

			if (sp_ifa->flags & (IFA_F_DADFAILED | IFA_F_TENTATIVE))
				continue;

			if (sp_ifa->rt) {
				/* This dst has been added to garbage list when
				 * lo device down, release this obsolete dst and
				 * reallocate a new router for ifa.
				 */
				if (sp_ifa->rt->dst.obsolete > 0) {
					dst_release(&sp_ifa->rt->dst);
					sp_ifa->rt = NULL;
				} else {
					continue;
				}
			}

			sp_rt = addrconf_dst_alloc(idev, &sp_ifa->addr, 0);

			/* Failure cases are ignored */
			if (!IS_ERR(sp_rt)) {
				sp_ifa->rt = sp_rt;
				ip6_ins_rt(sp_rt);
			}
		}
		read_unlock_bh(&idev->lock);
	}
}

static void addrconf_add_linklocal(struct inet6_dev *idev, const struct in6_addr *addr)
{
	struct inet6_ifaddr * ifp;
	u32 addr_flags = IFA_F_PERMANENT;

#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	if (idev->cnf.optimistic_dad &&
	    !dev_net(idev->dev)->ipv6.devconf_all->forwarding)
		addr_flags |= IFA_F_OPTIMISTIC;
#endif


	ifp = ipv6_add_addr(idev, addr, 64, IFA_LINK, addr_flags);
	if (!IS_ERR(ifp)) {
		addrconf_prefix_route(&ifp->addr, ifp->prefix_len, idev->dev, 0, 0);
		addrconf_dad_start(ifp, 0);
		in6_ifa_put(ifp);
	}
}

static void addrconf_dev_config(struct net_device *dev)
{
	struct in6_addr addr;
	struct inet6_dev    * idev;
=======
	idev = ipv6_find_idev(dev);
	if (IS_ERR(idev)) {
		pr_debug("%s: add_dev failed\n", __func__);
		return;
	}

	add_addr(idev, &in6addr_loopback, 128, IFA_HOST, IFAPROT_KERNEL_LO);
}

void addrconf_add_linklocal(struct inet6_dev *idev,
			    const struct in6_addr *addr, u32 flags)
{
	struct ifa6_config cfg = {
		.pfx = addr,
		.plen = 64,
		.ifa_flags = flags | IFA_F_PERMANENT,
		.valid_lft = INFINITY_LIFE_TIME,
		.preferred_lft = INFINITY_LIFE_TIME,
		.scope = IFA_LINK,
		.ifa_proto = IFAPROT_KERNEL_LL
	};
	struct inet6_ifaddr *ifp;

#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	if ((READ_ONCE(dev_net(idev->dev)->ipv6.devconf_all->optimistic_dad) ||
	     READ_ONCE(idev->cnf.optimistic_dad)) &&
	    !dev_net(idev->dev)->ipv6.devconf_all->forwarding)
		cfg.ifa_flags |= IFA_F_OPTIMISTIC;
#endif

	ifp = ipv6_add_addr(idev, &cfg, true, NULL);
	if (!IS_ERR(ifp)) {
		addrconf_prefix_route(&ifp->addr, ifp->prefix_len, 0, idev->dev,
				      0, 0, GFP_ATOMIC);
		addrconf_dad_start(ifp);
		in6_ifa_put(ifp);
	}
}
EXPORT_SYMBOL_GPL(addrconf_add_linklocal);

static bool ipv6_reserved_interfaceid(struct in6_addr address)
{
	if ((address.s6_addr32[2] | address.s6_addr32[3]) == 0)
		return true;

	if (address.s6_addr32[2] == htonl(0x02005eff) &&
	    ((address.s6_addr32[3] & htonl(0xfe000000)) == htonl(0xfe000000)))
		return true;

	if (address.s6_addr32[2] == htonl(0xfdffffff) &&
	    ((address.s6_addr32[3] & htonl(0xffffff80)) == htonl(0xffffff80)))
		return true;

	return false;
}

static int ipv6_generate_stable_address(struct in6_addr *address,
					u8 dad_count,
					const struct inet6_dev *idev)
{
	static DEFINE_SPINLOCK(lock);
	static __u32 digest[SHA1_DIGEST_WORDS];
	static __u32 workspace[SHA1_WORKSPACE_WORDS];

	static union {
		char __data[SHA1_BLOCK_SIZE];
		struct {
			struct in6_addr secret;
			__be32 prefix[2];
			unsigned char hwaddr[MAX_ADDR_LEN];
			u8 dad_count;
		} __packed;
	} data;

	struct in6_addr secret;
	struct in6_addr temp;
	struct net *net = dev_net(idev->dev);

	BUILD_BUG_ON(sizeof(data.__data) != sizeof(data));

	if (idev->cnf.stable_secret.initialized)
		secret = idev->cnf.stable_secret.secret;
	else if (net->ipv6.devconf_dflt->stable_secret.initialized)
		secret = net->ipv6.devconf_dflt->stable_secret.secret;
	else
		return -1;

retry:
	spin_lock_bh(&lock);

	sha1_init(digest);
	memset(&data, 0, sizeof(data));
	memset(workspace, 0, sizeof(workspace));
	memcpy(data.hwaddr, idev->dev->perm_addr, idev->dev->addr_len);
	data.prefix[0] = address->s6_addr32[0];
	data.prefix[1] = address->s6_addr32[1];
	data.secret = secret;
	data.dad_count = dad_count;

	sha1_transform(digest, data.__data, workspace);

	temp = *address;
	temp.s6_addr32[2] = (__force __be32)digest[0];
	temp.s6_addr32[3] = (__force __be32)digest[1];

	spin_unlock_bh(&lock);

	if (ipv6_reserved_interfaceid(temp)) {
		dad_count++;
		if (dad_count > dev_net(idev->dev)->ipv6.sysctl.idgen_retries)
			return -1;
		goto retry;
	}

	*address = temp;
	return 0;
}

static void ipv6_gen_mode_random_init(struct inet6_dev *idev)
{
	struct ipv6_stable_secret *s = &idev->cnf.stable_secret;

	if (s->initialized)
		return;
	s = &idev->cnf.stable_secret;
	get_random_bytes(&s->secret, sizeof(s->secret));
	s->initialized = true;
}

static void addrconf_addr_gen(struct inet6_dev *idev, bool prefix_route)
{
	struct in6_addr addr;

	/* no link local addresses on L3 master devices */
	if (netif_is_l3_master(idev->dev))
		return;

	/* no link local addresses on devices flagged as slaves */
	if (idev->dev->priv_flags & IFF_NO_ADDRCONF)
		return;

	ipv6_addr_set(&addr, htonl(0xFE800000), 0, 0, 0);

	switch (idev->cnf.addr_gen_mode) {
	case IN6_ADDR_GEN_MODE_RANDOM:
		ipv6_gen_mode_random_init(idev);
		fallthrough;
	case IN6_ADDR_GEN_MODE_STABLE_PRIVACY:
		if (!ipv6_generate_stable_address(&addr, 0, idev))
			addrconf_add_linklocal(idev, &addr,
					       IFA_F_STABLE_PRIVACY);
		else if (prefix_route)
			addrconf_prefix_route(&addr, 64, 0, idev->dev,
					      0, 0, GFP_KERNEL);
		break;
	case IN6_ADDR_GEN_MODE_EUI64:
		/* addrconf_add_linklocal also adds a prefix_route and we
		 * only need to care about prefix routes if ipv6_generate_eui64
		 * couldn't generate one.
		 */
		if (ipv6_generate_eui64(addr.s6_addr + 8, idev->dev) == 0)
			addrconf_add_linklocal(idev, &addr, 0);
		else if (prefix_route)
			addrconf_prefix_route(&addr, 64, 0, idev->dev,
					      0, 0, GFP_KERNEL);
		break;
	case IN6_ADDR_GEN_MODE_NONE:
	default:
		/* will not add any link local address */
		break;
	}
}

static void addrconf_dev_config(struct net_device *dev)
{
	struct inet6_dev *idev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ASSERT_RTNL();

	if ((dev->type != ARPHRD_ETHER) &&
	    (dev->type != ARPHRD_FDDI) &&
<<<<<<< HEAD
	    (dev->type != ARPHRD_IEEE802_TR) &&
	    (dev->type != ARPHRD_ARCNET) &&
	    (dev->type != ARPHRD_RAWIP) &&
	    (dev->type != ARPHRD_INFINIBAND)) {
		/* Alas, we support only Ethernet autoconfiguration. */
=======
	    (dev->type != ARPHRD_ARCNET) &&
	    (dev->type != ARPHRD_INFINIBAND) &&
	    (dev->type != ARPHRD_IEEE1394) &&
	    (dev->type != ARPHRD_TUNNEL6) &&
	    (dev->type != ARPHRD_6LOWPAN) &&
	    (dev->type != ARPHRD_TUNNEL) &&
	    (dev->type != ARPHRD_NONE) &&
	    (dev->type != ARPHRD_RAWIP)) {
		/* Alas, we support only Ethernet autoconfiguration. */
		idev = __in6_dev_get(dev);
		if (!IS_ERR_OR_NULL(idev) && dev->flags & IFF_UP &&
		    dev->flags & IFF_MULTICAST)
			ipv6_mc_up(idev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	idev = addrconf_add_dev(dev);
	if (IS_ERR(idev))
		return;

<<<<<<< HEAD
	memset(&addr, 0, sizeof(struct in6_addr));
	addr.s6_addr32[0] = htonl(0xFE800000);

	if (ipv6_generate_eui64(addr.s6_addr + 8, dev) == 0)
		addrconf_add_linklocal(idev, &addr);
}

#if defined(CONFIG_IPV6_SIT) || defined(CONFIG_IPV6_SIT_MODULE)
=======
	/* this device type has no EUI support */
	if (dev->type == ARPHRD_NONE &&
	    idev->cnf.addr_gen_mode == IN6_ADDR_GEN_MODE_EUI64)
		WRITE_ONCE(idev->cnf.addr_gen_mode,
			   IN6_ADDR_GEN_MODE_RANDOM);

	addrconf_addr_gen(idev, false);
}

#if IS_ENABLED(CONFIG_IPV6_SIT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void addrconf_sit_config(struct net_device *dev)
{
	struct inet6_dev *idev;

	ASSERT_RTNL();

	/*
	 * Configure the tunnel with one of our IPv4
	 * addresses... we should configure all of
	 * our v4 addrs in the tunnel
	 */

<<<<<<< HEAD
	if ((idev = ipv6_find_idev(dev)) == NULL) {
		printk(KERN_DEBUG "init sit: add_dev failed\n");
=======
	idev = ipv6_find_idev(dev);
	if (IS_ERR(idev)) {
		pr_debug("%s: add_dev failed\n", __func__);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	if (dev->priv_flags & IFF_ISATAP) {
<<<<<<< HEAD
		struct in6_addr addr;

		ipv6_addr_set(&addr,  htonl(0xFE800000), 0, 0, 0);
		addrconf_prefix_route(&addr, 64, dev, 0, 0);
		if (!ipv6_generate_eui64(addr.s6_addr + 8, dev))
			addrconf_add_linklocal(idev, &addr);
		return;
	}

	sit_add_v4_addrs(idev);

	if (dev->flags&IFF_POINTOPOINT) {
		addrconf_add_mroute(dev);
		addrconf_add_lroute(dev);
	} else
		sit_route_add(dev);
}
#endif

#if defined(CONFIG_NET_IPGRE) || defined(CONFIG_NET_IPGRE_MODULE)
static void addrconf_gre_config(struct net_device *dev)
{
	struct inet6_dev *idev;
	struct in6_addr addr;

	pr_info("ipv6: addrconf_gre_config(%s)\n", dev->name);

	ASSERT_RTNL();

	if ((idev = ipv6_find_idev(dev)) == NULL) {
		printk(KERN_DEBUG "init gre: add_dev failed\n");
		return;
	}

	ipv6_addr_set(&addr,  htonl(0xFE800000), 0, 0, 0);
	addrconf_prefix_route(&addr, 64, dev, 0, 0);

	if (!ipv6_generate_eui64(addr.s6_addr + 8, dev))
		addrconf_add_linklocal(idev, &addr);
}
#endif

static inline int
ipv6_inherit_linklocal(struct inet6_dev *idev, struct net_device *link_dev)
{
	struct in6_addr lladdr;

	if (!ipv6_get_lladdr(link_dev, &lladdr, IFA_F_TENTATIVE)) {
		addrconf_add_linklocal(idev, &lladdr);
		return 0;
	}
	return -1;
}

static void ip6_tnl_add_linklocal(struct inet6_dev *idev)
{
	struct net_device *link_dev;
	struct net *net = dev_net(idev->dev);

	/* first try to inherit the link-local address from the link device */
	if (idev->dev->iflink &&
	    (link_dev = __dev_get_by_index(net, idev->dev->iflink))) {
		if (!ipv6_inherit_linklocal(idev, link_dev))
			return;
	}
	/* then try to inherit it from any device */
	for_each_netdev(net, link_dev) {
		if (!ipv6_inherit_linklocal(idev, link_dev))
			return;
	}
	printk(KERN_DEBUG "init ip6-ip6: add_linklocal failed\n");
}

/*
 * Autoconfigure tunnel with a link-local address so routing protocols,
 * DHCPv6, MLD etc. can be run over the virtual link
 */

static void addrconf_ip6_tnl_config(struct net_device *dev)
{
	struct inet6_dev *idev;

	ASSERT_RTNL();

	idev = addrconf_add_dev(dev);
	if (IS_ERR(idev)) {
		printk(KERN_DEBUG "init ip6-ip6: add_dev failed\n");
		return;
	}
	ip6_tnl_add_linklocal(idev);
}

static int addrconf_notify(struct notifier_block *this, unsigned long event,
			   void * data)
{
	struct net_device *dev = (struct net_device *) data;
	struct inet6_dev *idev = __in6_dev_get(dev);
=======
		addrconf_addr_gen(idev, false);
		return;
	}

	add_v4_addrs(idev);

	if (dev->flags&IFF_POINTOPOINT)
		addrconf_add_mroute(dev);
}
#endif

#if IS_ENABLED(CONFIG_NET_IPGRE) || IS_ENABLED(CONFIG_IPV6_GRE)
static void addrconf_gre_config(struct net_device *dev)
{
	struct inet6_dev *idev;

	ASSERT_RTNL();

	idev = ipv6_find_idev(dev);
	if (IS_ERR(idev)) {
		pr_debug("%s: add_dev failed\n", __func__);
		return;
	}

	if (dev->type == ARPHRD_ETHER) {
		addrconf_addr_gen(idev, true);
		return;
	}

	add_v4_addrs(idev);

	if (dev->flags & IFF_POINTOPOINT)
		addrconf_add_mroute(dev);
}
#endif

static void addrconf_init_auto_addrs(struct net_device *dev)
{
	switch (dev->type) {
#if IS_ENABLED(CONFIG_IPV6_SIT)
	case ARPHRD_SIT:
		addrconf_sit_config(dev);
		break;
#endif
#if IS_ENABLED(CONFIG_NET_IPGRE) || IS_ENABLED(CONFIG_IPV6_GRE)
	case ARPHRD_IP6GRE:
	case ARPHRD_IPGRE:
		addrconf_gre_config(dev);
		break;
#endif
	case ARPHRD_LOOPBACK:
		init_loopback(dev);
		break;

	default:
		addrconf_dev_config(dev);
		break;
	}
}

static int fixup_permanent_addr(struct net *net,
				struct inet6_dev *idev,
				struct inet6_ifaddr *ifp)
{
	/* !fib6_node means the host route was removed from the
	 * FIB, for example, if 'lo' device is taken down. In that
	 * case regenerate the host route.
	 */
	if (!ifp->rt || !ifp->rt->fib6_node) {
		struct fib6_info *f6i, *prev;

		f6i = addrconf_f6i_alloc(net, idev, &ifp->addr, false,
					 GFP_ATOMIC, NULL);
		if (IS_ERR(f6i))
			return PTR_ERR(f6i);

		/* ifp->rt can be accessed outside of rtnl */
		spin_lock(&ifp->lock);
		prev = ifp->rt;
		ifp->rt = f6i;
		spin_unlock(&ifp->lock);

		fib6_info_release(prev);
	}

	if (!(ifp->flags & IFA_F_NOPREFIXROUTE)) {
		addrconf_prefix_route(&ifp->addr, ifp->prefix_len,
				      ifp->rt_priority, idev->dev, 0, 0,
				      GFP_ATOMIC);
	}

	if (ifp->state == INET6_IFADDR_STATE_PREDAD)
		addrconf_dad_start(ifp);

	return 0;
}

static void addrconf_permanent_addr(struct net *net, struct net_device *dev)
{
	struct inet6_ifaddr *ifp, *tmp;
	struct inet6_dev *idev;

	idev = __in6_dev_get(dev);
	if (!idev)
		return;

	write_lock_bh(&idev->lock);

	list_for_each_entry_safe(ifp, tmp, &idev->addr_list, if_list) {
		if ((ifp->flags & IFA_F_PERMANENT) &&
		    fixup_permanent_addr(net, idev, ifp) < 0) {
			write_unlock_bh(&idev->lock);
			in6_ifa_hold(ifp);
			ipv6_del_addr(ifp);
			write_lock_bh(&idev->lock);

			net_info_ratelimited("%s: Failed to add prefix route for address %pI6c; dropping\n",
					     idev->dev->name, &ifp->addr);
		}
	}

	write_unlock_bh(&idev->lock);
}

static int addrconf_notify(struct notifier_block *this, unsigned long event,
			   void *ptr)
{
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	struct netdev_notifier_change_info *change_info;
	struct netdev_notifier_changeupper_info *info;
	struct inet6_dev *idev = __in6_dev_get(dev);
	struct net *net = dev_net(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int run_pending = 0;
	int err;

	switch (event) {
	case NETDEV_REGISTER:
		if (!idev && dev->mtu >= IPV6_MIN_MTU) {
			idev = ipv6_add_dev(dev);
<<<<<<< HEAD
			if (!idev)
				return notifier_from_errno(-ENOMEM);
		}
		break;

	case NETDEV_UP:
	case NETDEV_CHANGE:
		if (dev->flags & IFF_SLAVE)
			break;

		if (event == NETDEV_UP) {
			if (!addrconf_qdisc_ok(dev)) {
				/* device is not ready yet. */
				printk(KERN_INFO
					"ADDRCONF(NETDEV_UP): %s: "
					"link is not ready\n",
					dev->name);
=======
			if (IS_ERR(idev))
				return notifier_from_errno(PTR_ERR(idev));
		}
		break;

	case NETDEV_CHANGEMTU:
		/* if MTU under IPV6_MIN_MTU stop IPv6 on this interface. */
		if (dev->mtu < IPV6_MIN_MTU) {
			addrconf_ifdown(dev, dev != net->loopback_dev);
			break;
		}

		if (idev) {
			rt6_mtu_change(dev, dev->mtu);
			WRITE_ONCE(idev->cnf.mtu6, dev->mtu);
			break;
		}

		/* allocate new idev */
		idev = ipv6_add_dev(dev);
		if (IS_ERR(idev))
			break;

		/* device is still not ready */
		if (!(idev->if_flags & IF_READY))
			break;

		run_pending = 1;
		fallthrough;
	case NETDEV_UP:
	case NETDEV_CHANGE:
		if (idev && idev->cnf.disable_ipv6)
			break;

		if (dev->priv_flags & IFF_NO_ADDRCONF) {
			if (event == NETDEV_UP && !IS_ERR_OR_NULL(idev) &&
			    dev->flags & IFF_UP && dev->flags & IFF_MULTICAST)
				ipv6_mc_up(idev);
			break;
		}

		if (event == NETDEV_UP) {
			/* restore routes for permanent addresses */
			addrconf_permanent_addr(net, dev);

			if (!addrconf_link_ready(dev)) {
				/* device is not ready yet. */
				pr_debug("ADDRCONF(NETDEV_UP): %s: link is not ready\n",
					 dev->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;
			}

			if (!idev && dev->mtu >= IPV6_MIN_MTU)
				idev = ipv6_add_dev(dev);

<<<<<<< HEAD
			if (idev) {
				idev->if_flags |= IF_READY;
				run_pending = 1;
			}
		} else {
			if (!addrconf_qdisc_ok(dev)) {
				/* device is still not ready. */
				break;
			}

			if (idev) {
				if (idev->if_flags & IF_READY)
					/* device is already configured. */
					break;
				idev->if_flags |= IF_READY;
			}

			printk(KERN_INFO
					"ADDRCONF(NETDEV_CHANGE): %s: "
					"link becomes ready\n",
					dev->name);
=======
			if (!IS_ERR_OR_NULL(idev)) {
				idev->if_flags |= IF_READY;
				run_pending = 1;
			}
		} else if (event == NETDEV_CHANGE) {
			if (!addrconf_link_ready(dev)) {
				/* device is still not ready. */
				rt6_sync_down_dev(dev, event);
				break;
			}

			if (!IS_ERR_OR_NULL(idev)) {
				if (idev->if_flags & IF_READY) {
					/* device is already configured -
					 * but resend MLD reports, we might
					 * have roamed and need to update
					 * multicast snooping switches
					 */
					ipv6_mc_up(idev);
					change_info = ptr;
					if (change_info->flags_changed & IFF_NOARP)
						addrconf_dad_run(idev, true);
					rt6_sync_up(dev, RTNH_F_LINKDOWN);
					break;
				}
				idev->if_flags |= IF_READY;
			}

			pr_debug("ADDRCONF(NETDEV_CHANGE): %s: link becomes ready\n",
				 dev->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			run_pending = 1;
		}

<<<<<<< HEAD
		switch (dev->type) {
#if defined(CONFIG_IPV6_SIT) || defined(CONFIG_IPV6_SIT_MODULE)
		case ARPHRD_SIT:
			addrconf_sit_config(dev);
			break;
#endif
#if defined(CONFIG_NET_IPGRE) || defined(CONFIG_NET_IPGRE_MODULE)
		case ARPHRD_IPGRE:
			addrconf_gre_config(dev);
			break;
#endif
		case ARPHRD_TUNNEL6:
			addrconf_ip6_tnl_config(dev);
			break;
		case ARPHRD_LOOPBACK:
			init_loopback(dev);
			break;

		default:
			addrconf_dev_config(dev);
			break;
		}

		if (idev) {
			if (run_pending)
				addrconf_dad_run(idev);
=======
		addrconf_init_auto_addrs(dev);

		if (!IS_ERR_OR_NULL(idev)) {
			if (run_pending)
				addrconf_dad_run(idev, false);

			/* Device has an address by now */
			rt6_sync_up(dev, RTNH_F_DEAD);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			/*
			 * If the MTU changed during the interface down,
			 * when the interface up, the changed MTU must be
			 * reflected in the idev as well as routers.
			 */
			if (idev->cnf.mtu6 != dev->mtu &&
			    dev->mtu >= IPV6_MIN_MTU) {
				rt6_mtu_change(dev, dev->mtu);
<<<<<<< HEAD
				idev->cnf.mtu6 = dev->mtu;
			}
			idev->tstamp = jiffies;
=======
				WRITE_ONCE(idev->cnf.mtu6, dev->mtu);
			}
			WRITE_ONCE(idev->tstamp, jiffies);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			inet6_ifinfo_notify(RTM_NEWLINK, idev);

			/*
			 * If the changed mtu during down is lower than
			 * IPV6_MIN_MTU stop IPv6 on this interface.
			 */
			if (dev->mtu < IPV6_MIN_MTU)
<<<<<<< HEAD
				addrconf_ifdown(dev, 1);
		}
		break;

	case NETDEV_CHANGEMTU:
		if (idev && dev->mtu >= IPV6_MIN_MTU) {
			rt6_mtu_change(dev, dev->mtu);
			idev->cnf.mtu6 = dev->mtu;
			break;
		}

		if (!idev && dev->mtu >= IPV6_MIN_MTU) {
			idev = ipv6_add_dev(dev);
			if (idev)
				break;
		}

		/*
		 * MTU falled under IPV6_MIN_MTU.
		 * Stop IPv6 on this interface.
		 */

=======
				addrconf_ifdown(dev, dev != net->loopback_dev);
		}
		break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case NETDEV_DOWN:
	case NETDEV_UNREGISTER:
		/*
		 *	Remove all addresses from this interface.
		 */
		addrconf_ifdown(dev, event != NETDEV_DOWN);
		break;

	case NETDEV_CHANGENAME:
		if (idev) {
			snmp6_unregister_dev(idev);
			addrconf_sysctl_unregister(idev);
<<<<<<< HEAD
			addrconf_sysctl_register(idev);
			err = snmp6_register_dev(idev);
			if (err)
				return notifier_from_errno(err);
=======
			err = addrconf_sysctl_register(idev);
			if (err)
				return notifier_from_errno(err);
			err = snmp6_register_dev(idev);
			if (err) {
				addrconf_sysctl_unregister(idev);
				return notifier_from_errno(err);
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		break;

	case NETDEV_PRE_TYPE_CHANGE:
	case NETDEV_POST_TYPE_CHANGE:
<<<<<<< HEAD
		addrconf_type_change(dev, event);
		break;
=======
		if (idev)
			addrconf_type_change(dev, event);
		break;

	case NETDEV_CHANGEUPPER:
		info = ptr;

		/* flush all routes if dev is linked to or unlinked from
		 * an L3 master device (e.g., VRF)
		 */
		if (info->upper_dev && netif_is_l3_master(info->upper_dev))
			addrconf_ifdown(dev, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return NOTIFY_OK;
}

/*
 *	addrconf module should be notified of a device going up
 */
static struct notifier_block ipv6_dev_notf = {
	.notifier_call = addrconf_notify,
<<<<<<< HEAD
=======
	.priority = ADDRCONF_NOTIFY_PRIORITY,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static void addrconf_type_change(struct net_device *dev, unsigned long event)
{
	struct inet6_dev *idev;
	ASSERT_RTNL();

	idev = __in6_dev_get(dev);

	if (event == NETDEV_POST_TYPE_CHANGE)
		ipv6_mc_remap(idev);
	else if (event == NETDEV_PRE_TYPE_CHANGE)
		ipv6_mc_unmap(idev);
}

<<<<<<< HEAD
static int addrconf_ifdown(struct net_device *dev, int how)
{
	struct net *net = dev_net(dev);
	struct inet6_dev *idev;
	struct inet6_ifaddr *ifa;
=======
static bool addr_is_local(const struct in6_addr *addr)
{
	return ipv6_addr_type(addr) &
		(IPV6_ADDR_LINKLOCAL | IPV6_ADDR_LOOPBACK);
}

static int addrconf_ifdown(struct net_device *dev, bool unregister)
{
	unsigned long event = unregister ? NETDEV_UNREGISTER : NETDEV_DOWN;
	struct net *net = dev_net(dev);
	struct inet6_dev *idev;
	struct inet6_ifaddr *ifa;
	LIST_HEAD(tmp_addr_list);
	bool keep_addr = false;
	bool was_ready;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int state, i;

	ASSERT_RTNL();

<<<<<<< HEAD
	rt6_ifdown(net, dev);
	neigh_ifdown(&nd_tbl, dev);

	idev = __in6_dev_get(dev);
	if (idev == NULL)
=======
	rt6_disable_ip(dev, event);

	idev = __in6_dev_get(dev);
	if (!idev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENODEV;

	/*
	 * Step 1: remove reference to ipv6 device from parent device.
	 *	   Do not dev_put!
	 */
<<<<<<< HEAD
	if (how) {
=======
	if (unregister) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		idev->dead = 1;

		/* protected by rtnl_lock */
		RCU_INIT_POINTER(dev->ip6_ptr, NULL);

		/* Step 1.5: remove snmp6 entry */
		snmp6_unregister_dev(idev);

	}

<<<<<<< HEAD
#ifdef CONFIG_LGP_DATA_BUGFIX_IPV6_ADDRCONF_KERNEL_CRASH
	spin_lock_bh(&addrconf_hash_lock);
#endif
	/* Step 2: clear hash table */
	for (i = 0; i < IN6_ADDR_HSIZE; i++) {
		struct hlist_head *h = &inet6_addr_lst[i];
		struct hlist_node *n;


#ifdef CONFIG_LGP_DATA_BUGFIX_IPV6_ADDRCONF_KERNEL_CRASH
	//spin_lock_bh(&addrconf_hash_lock);
#else
	spin_lock_bh(&addrconf_hash_lock);
#endif
	restart:
		hlist_for_each_entry_rcu(ifa, n, h, addr_lst) {
			if (ifa->idev == idev) {
				hlist_del_init_rcu(&ifa->addr_lst);
				addrconf_del_timer(ifa);
				goto restart;
			}
		}
#ifdef CONFIG_LGP_DATA_BUGFIX_IPV6_ADDRCONF_KERNEL_CRASH
	//spin_unlock_bh(&addrconf_hash_lock);
#else
	spin_unlock_bh(&addrconf_hash_lock);
#endif
=======
	/* combine the user config with event to determine if permanent
	 * addresses are to be removed from address hash table
	 */
	if (!unregister && !idev->cnf.disable_ipv6) {
		/* aggregate the system setting and interface setting */
		int _keep_addr = READ_ONCE(net->ipv6.devconf_all->keep_addr_on_down);

		if (!_keep_addr)
			_keep_addr = READ_ONCE(idev->cnf.keep_addr_on_down);

		keep_addr = (_keep_addr > 0);
	}

	/* Step 2: clear hash table */
	for (i = 0; i < IN6_ADDR_HSIZE; i++) {
		struct hlist_head *h = &net->ipv6.inet6_addr_lst[i];

		spin_lock_bh(&net->ipv6.addrconf_hash_lock);
restart:
		hlist_for_each_entry_rcu(ifa, h, addr_lst) {
			if (ifa->idev == idev) {
				addrconf_del_dad_work(ifa);
				/* combined flag + permanent flag decide if
				 * address is retained on a down event
				 */
				if (!keep_addr ||
				    !(ifa->flags & IFA_F_PERMANENT) ||
				    addr_is_local(&ifa->addr)) {
					hlist_del_init_rcu(&ifa->addr_lst);
					goto restart;
				}
			}
		}
		spin_unlock_bh(&net->ipv6.addrconf_hash_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	write_lock_bh(&idev->lock);

<<<<<<< HEAD
	/* Step 2: clear flags for stateless addrconf */
	if (!how)
		idev->if_flags &= ~(IF_RS_SENT|IF_RA_RCVD|IF_READY);

#ifdef CONFIG_IPV6_PRIVACY
	if (how && del_timer(&idev->regen_timer))
		in6_dev_put(idev);
=======
	addrconf_del_rs_timer(idev);

	/* Step 2: clear flags for stateless addrconf, repeated down
	 *         detection
	 */
	was_ready = idev->if_flags & IF_READY;
	if (!unregister)
		idev->if_flags &= ~(IF_RS_SENT|IF_RA_RCVD|IF_READY);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Step 3: clear tempaddr list */
	while (!list_empty(&idev->tempaddr_list)) {
		ifa = list_first_entry(&idev->tempaddr_list,
				       struct inet6_ifaddr, tmp_list);
		list_del(&ifa->tmp_list);
		write_unlock_bh(&idev->lock);
		spin_lock_bh(&ifa->lock);

		if (ifa->ifpub) {
			in6_ifa_put(ifa->ifpub);
			ifa->ifpub = NULL;
		}
		spin_unlock_bh(&ifa->lock);
		in6_ifa_put(ifa);
		write_lock_bh(&idev->lock);
	}
<<<<<<< HEAD
#endif

	while (!list_empty(&idev->addr_list)) {
		ifa = list_first_entry(&idev->addr_list,
				       struct inet6_ifaddr, if_list);
		addrconf_del_timer(ifa);

		list_del(&ifa->if_list);

		write_unlock_bh(&idev->lock);

		spin_lock_bh(&ifa->state_lock);
		state = ifa->state;
		ifa->state = INET6_IFADDR_STATE_DEAD;
		spin_unlock_bh(&ifa->state_lock);

		if (state != INET6_IFADDR_STATE_DEAD) {
			__ipv6_ifa_notify(RTM_DELADDR, ifa);
			atomic_notifier_call_chain(&inet6addr_chain, NETDEV_DOWN, ifa);
		}
		in6_ifa_put(ifa);

		write_lock_bh(&idev->lock);
	}

	write_unlock_bh(&idev->lock);

#ifdef CONFIG_LGP_DATA_BUGFIX_IPV6_ADDRCONF_KERNEL_CRASH
	spin_unlock_bh(&addrconf_hash_lock);
#endif

	/* Step 5: Discard multicast list */
	if (how)
		ipv6_mc_destroy_dev(idev);
	else
		ipv6_mc_down(idev);

	idev->tstamp = jiffies;

	/* Last: Shot the device (if unregistered) */
	if (how) {
=======

	list_for_each_entry(ifa, &idev->addr_list, if_list)
		list_add_tail(&ifa->if_list_aux, &tmp_addr_list);
	write_unlock_bh(&idev->lock);

	while (!list_empty(&tmp_addr_list)) {
		struct fib6_info *rt = NULL;
		bool keep;

		ifa = list_first_entry(&tmp_addr_list,
				       struct inet6_ifaddr, if_list_aux);
		list_del(&ifa->if_list_aux);

		addrconf_del_dad_work(ifa);

		keep = keep_addr && (ifa->flags & IFA_F_PERMANENT) &&
			!addr_is_local(&ifa->addr);

		spin_lock_bh(&ifa->lock);

		if (keep) {
			/* set state to skip the notifier below */
			state = INET6_IFADDR_STATE_DEAD;
			ifa->state = INET6_IFADDR_STATE_PREDAD;
			if (!(ifa->flags & IFA_F_NODAD))
				ifa->flags |= IFA_F_TENTATIVE;

			rt = ifa->rt;
			ifa->rt = NULL;
		} else {
			state = ifa->state;
			ifa->state = INET6_IFADDR_STATE_DEAD;
		}

		spin_unlock_bh(&ifa->lock);

		if (rt)
			ip6_del_rt(net, rt, false);

		if (state != INET6_IFADDR_STATE_DEAD) {
			__ipv6_ifa_notify(RTM_DELADDR, ifa);
			inet6addr_notifier_call_chain(NETDEV_DOWN, ifa);
		} else {
			if (idev->cnf.forwarding)
				addrconf_leave_anycast(ifa);
			addrconf_leave_solict(ifa->idev, &ifa->addr);
		}

		if (!keep) {
			write_lock_bh(&idev->lock);
			list_del_rcu(&ifa->if_list);
			write_unlock_bh(&idev->lock);
			in6_ifa_put(ifa);
		}
	}

	/* Step 5: Discard anycast and multicast list */
	if (unregister) {
		ipv6_ac_destroy_dev(idev);
		ipv6_mc_destroy_dev(idev);
	} else if (was_ready) {
		ipv6_mc_down(idev);
	}

	WRITE_ONCE(idev->tstamp, jiffies);
	idev->ra_mtu = 0;

	/* Last: Shot the device (if unregistered) */
	if (unregister) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		addrconf_sysctl_unregister(idev);
		neigh_parms_release(&nd_tbl, idev->nd_parms);
		neigh_ifdown(&nd_tbl, dev);
		in6_dev_put(idev);
	}
	return 0;
}

<<<<<<< HEAD
static void addrconf_rs_timer(unsigned long data)
{
	struct inet6_ifaddr *ifp = (struct inet6_ifaddr *) data;
	struct inet6_dev *idev = ifp->idev;

	read_lock(&idev->lock);
	if (idev->dead || !(idev->if_flags & IF_READY))
		goto out;

	if (idev->cnf.forwarding)
		goto out;

	/* Announcement received after solicitation was sent */
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
	if (idev->if_flags & IF_RA_RCVD){
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
	ADBG((KERN_DEBUG "[LGE_DATA][%s()] The RA msg had been received!", __func__));
#endif
		goto out;
    }
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21    
	spin_lock(&ifp->lock);
	if (ifp->probes++ < idev->cnf.rtr_solicits) {
		/* The wait after the last probe can be shorter */
		addrconf_mod_timer(ifp, AC_RS,
				   (ifp->probes == idev->cnf.rtr_solicits) ?
				   idev->cnf.rtr_solicit_delay :
				   idev->cnf.rtr_solicit_interval);
		spin_unlock(&ifp->lock);
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
	ADBG((KERN_DEBUG "[LGE_DATA][%s()][stage 2] rs is sent now!", __func__));
#endif
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
		ndisc_send_rs(idev->dev, &ifp->addr, &in6addr_linklocal_allrouters);
	} else {
		spin_unlock(&ifp->lock);
=======
static void addrconf_rs_timer(struct timer_list *t)
{
	struct inet6_dev *idev = from_timer(idev, t, rs_timer);
	struct net_device *dev = idev->dev;
	struct in6_addr lladdr;
	int rtr_solicits;

	write_lock(&idev->lock);
	if (idev->dead || !(idev->if_flags & IF_READY))
		goto out;

	if (!ipv6_accept_ra(idev))
		goto out;

	/* Announcement received after solicitation was sent */
	if (idev->if_flags & IF_RA_RCVD)
		goto out;

	rtr_solicits = READ_ONCE(idev->cnf.rtr_solicits);

	if (idev->rs_probes++ < rtr_solicits || rtr_solicits < 0) {
		write_unlock(&idev->lock);
		if (!ipv6_get_lladdr(dev, &lladdr, IFA_F_TENTATIVE))
			ndisc_send_rs(dev, &lladdr,
				      &in6addr_linklocal_allrouters);
		else
			goto put;

		write_lock(&idev->lock);
		idev->rs_interval = rfc3315_s14_backoff_update(
				idev->rs_interval,
				READ_ONCE(idev->cnf.rtr_solicit_max_interval));
		/* The wait after the last probe can be shorter */
		addrconf_mod_rs_timer(idev, (idev->rs_probes ==
					     READ_ONCE(idev->cnf.rtr_solicits)) ?
				      READ_ONCE(idev->cnf.rtr_solicit_delay) :
				      idev->rs_interval);
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Note: we do not support deprecated "all on-link"
		 * assumption any longer.
		 */
<<<<<<< HEAD
		printk(KERN_DEBUG "%s: no IPv6 routers present\n",
		       idev->dev->name);
	}

out:
	read_unlock(&idev->lock);
	in6_ifa_put(ifp);
=======
		pr_debug("%s: no IPv6 routers present\n", idev->dev->name);
	}

out:
	write_unlock(&idev->lock);
put:
	in6_dev_put(idev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Duplicate Address Detection
 */
static void addrconf_dad_kick(struct inet6_ifaddr *ifp)
{
<<<<<<< HEAD
	unsigned long rand_num;
	struct inet6_dev *idev = ifp->idev;
=======
	struct inet6_dev *idev = ifp->idev;
	unsigned long rand_num;
	u64 nonce;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ifp->flags & IFA_F_OPTIMISTIC)
		rand_num = 0;
	else
<<<<<<< HEAD
		rand_num = net_random() % (idev->cnf.rtr_solicit_delay ? : 1);

	ifp->probes = idev->cnf.dad_transmits;
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
	ADBG((KERN_DEBUG "[LGE_DATA][%s()] dad_transmits == %d, ramd_num == %lu", __func__, idev->cnf.dad_transmits, rand_num));
#endif
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
	addrconf_mod_timer(ifp, AC_DAD, rand_num);
}

static void addrconf_dad_start(struct inet6_ifaddr *ifp, u32 flags)
{
	struct inet6_dev *idev = ifp->idev;
	struct net_device *dev = idev->dev;
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-07-02
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
    int ipv6AddrType = 0; //initializing

    const char InterfaceNameToApply[6]="rmnet";
    char CurrentInterfaceName[6]={0};//initializing
    ipv6AddrType = ipv6_addr_type(&ifp->addr);

    ADBG((KERN_DEBUG "[LGE_DATA][%s()] dad_start! dev_name == %s", __func__, dev->name));
    ADBG((KERN_DEBUG "[LGE_DATA][%s()] ipv6_addr_type == %d", __func__, ipv6AddrType));

    strncpy(CurrentInterfaceName,dev->name,5);
    if(CurrentInterfaceName == NULL){
        ADBG((KERN_DEBUG "[LGE_DATA] CurrentInterfaceName is NULL !\n"));
        return;
    }
#endif
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-07-02
	addrconf_join_solict(dev, &ifp->addr);

	net_srandom(ifp->addr.s6_addr32[3]);

=======
		rand_num = get_random_u32_below(
				READ_ONCE(idev->cnf.rtr_solicit_delay) ? : 1);

	nonce = 0;
	if (READ_ONCE(idev->cnf.enhanced_dad) ||
	    READ_ONCE(dev_net(idev->dev)->ipv6.devconf_all->enhanced_dad)) {
		do
			get_random_bytes(&nonce, 6);
		while (nonce == 0);
	}
	ifp->dad_nonce = nonce;
	ifp->dad_probes = READ_ONCE(idev->cnf.dad_transmits);
	addrconf_mod_dad_work(ifp, rand_num);
}

static void addrconf_dad_begin(struct inet6_ifaddr *ifp)
{
	struct inet6_dev *idev = ifp->idev;
	struct net_device *dev = idev->dev;
	bool bump_id, notify = false;
	struct net *net;

	addrconf_join_solict(dev, &ifp->addr);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	read_lock_bh(&idev->lock);
	spin_lock(&ifp->lock);
	if (ifp->state == INET6_IFADDR_STATE_DEAD)
		goto out;

<<<<<<< HEAD
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-07-02
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
	if (((strcmp(InterfaceNameToApply, CurrentInterfaceName) == 0) && (ipv6AddrType == LGE_DATA_GLOBAL_SCOPE)) 
        || (dev->flags&(IFF_NOARP|IFF_LOOPBACK) ||
	    idev->cnf.accept_dad < 1 ||
	    !(ifp->flags&IFA_F_TENTATIVE) ||
	    ifp->flags & IFA_F_NODAD))
#else
// Kernel Original implemenatation START
	if (dev->flags&(IFF_NOARP|IFF_LOOPBACK) ||
	    idev->cnf.accept_dad < 1 ||
	    !(ifp->flags&IFA_F_TENTATIVE) ||
	    ifp->flags & IFA_F_NODAD)
// Kernel Original implemenatation END
#endif
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-07-02
    {
		ifp->flags &= ~(IFA_F_TENTATIVE|IFA_F_OPTIMISTIC|IFA_F_DADFAILED);
		spin_unlock(&ifp->lock);
		read_unlock_bh(&idev->lock);
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-07-02
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
	ADBG((KERN_DEBUG "[LGE_DATA][%s()] ipv6_addr_type == %d, Because the IPv6 type is Global Scope, we will immediately finish the DAD process for Global Scope.", __func__, ipv6AddrType));
#endif
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-07-02
		addrconf_dad_completed(ifp);
=======
	net = dev_net(dev);
	if (dev->flags&(IFF_NOARP|IFF_LOOPBACK) ||
	    (READ_ONCE(net->ipv6.devconf_all->accept_dad) < 1 &&
	     READ_ONCE(idev->cnf.accept_dad) < 1) ||
	    !(ifp->flags&IFA_F_TENTATIVE) ||
	    ifp->flags & IFA_F_NODAD) {
		bool send_na = false;

		if (ifp->flags & IFA_F_TENTATIVE &&
		    !(ifp->flags & IFA_F_OPTIMISTIC))
			send_na = true;
		bump_id = ifp->flags & IFA_F_TENTATIVE;
		ifp->flags &= ~(IFA_F_TENTATIVE|IFA_F_OPTIMISTIC|IFA_F_DADFAILED);
		spin_unlock(&ifp->lock);
		read_unlock_bh(&idev->lock);

		addrconf_dad_completed(ifp, bump_id, send_na);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	if (!(idev->if_flags & IF_READY)) {
		spin_unlock(&ifp->lock);
		read_unlock_bh(&idev->lock);
		/*
		 * If the device is not ready:
		 * - keep it tentative if it is a permanent address.
		 * - otherwise, kill it.
		 */
		in6_ifa_hold(ifp);
		addrconf_dad_stop(ifp, 0);
		return;
	}

	/*
	 * Optimistic nodes can start receiving
	 * Frames right away
	 */
	if (ifp->flags & IFA_F_OPTIMISTIC) {
<<<<<<< HEAD
		ip6_ins_rt(ifp->rt);
		if (ipv6_use_optimistic_addr(idev)) {
			/* Because optimistic nodes can use this address,
			 * notify listeners. If DAD fails, RTM_DELADDR is sent.
			 */
			ipv6_ifa_notify(RTM_NEWADDR, ifp);
=======
		ip6_ins_rt(net, ifp->rt);
		if (ipv6_use_optimistic_addr(net, idev)) {
			/* Because optimistic nodes can use this address,
			 * notify listeners. If DAD fails, RTM_DELADDR is sent.
			 */
			notify = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	addrconf_dad_kick(ifp);
out:
	spin_unlock(&ifp->lock);
	read_unlock_bh(&idev->lock);
<<<<<<< HEAD
}
static void addrconf_dad_timer(unsigned long data)
{
	struct inet6_ifaddr *ifp = (struct inet6_ifaddr *) data;
	struct inet6_dev *idev = ifp->idev;
	struct in6_addr mcaddr;
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
	struct net_device *dev = idev->dev;
    const char InterfaceNameToApply[6]="rmnet";
    char CurrentInterfaceName[6]={0};//initializing
#endif
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
	if (!ifp->probes && addrconf_dad_end(ifp))
		goto out;

	read_lock(&idev->lock);
	if (idev->dead || !(idev->if_flags & IF_READY)) {
		read_unlock(&idev->lock);
=======
	if (notify)
		ipv6_ifa_notify(RTM_NEWADDR, ifp);
}

static void addrconf_dad_start(struct inet6_ifaddr *ifp)
{
	bool begin_dad = false;

	spin_lock_bh(&ifp->lock);
	if (ifp->state != INET6_IFADDR_STATE_DEAD) {
		ifp->state = INET6_IFADDR_STATE_PREDAD;
		begin_dad = true;
	}
	spin_unlock_bh(&ifp->lock);

	if (begin_dad)
		addrconf_mod_dad_work(ifp, 0);
}

static void addrconf_dad_work(struct work_struct *w)
{
	struct inet6_ifaddr *ifp = container_of(to_delayed_work(w),
						struct inet6_ifaddr,
						dad_work);
	struct inet6_dev *idev = ifp->idev;
	bool bump_id, disable_ipv6 = false;
	struct in6_addr mcaddr;

	enum {
		DAD_PROCESS,
		DAD_BEGIN,
		DAD_ABORT,
	} action = DAD_PROCESS;

	rtnl_lock();

	spin_lock_bh(&ifp->lock);
	if (ifp->state == INET6_IFADDR_STATE_PREDAD) {
		action = DAD_BEGIN;
		ifp->state = INET6_IFADDR_STATE_DAD;
	} else if (ifp->state == INET6_IFADDR_STATE_ERRDAD) {
		action = DAD_ABORT;
		ifp->state = INET6_IFADDR_STATE_POSTDAD;

		if ((READ_ONCE(dev_net(idev->dev)->ipv6.devconf_all->accept_dad) > 1 ||
		     READ_ONCE(idev->cnf.accept_dad) > 1) &&
		    !idev->cnf.disable_ipv6 &&
		    !(ifp->flags & IFA_F_STABLE_PRIVACY)) {
			struct in6_addr addr;

			addr.s6_addr32[0] = htonl(0xfe800000);
			addr.s6_addr32[1] = 0;

			if (!ipv6_generate_eui64(addr.s6_addr + 8, idev->dev) &&
			    ipv6_addr_equal(&ifp->addr, &addr)) {
				/* DAD failed for link-local based on MAC */
				WRITE_ONCE(idev->cnf.disable_ipv6, 1);

				pr_info("%s: IPv6 being disabled!\n",
					ifp->idev->dev->name);
				disable_ipv6 = true;
			}
		}
	}
	spin_unlock_bh(&ifp->lock);

	if (action == DAD_BEGIN) {
		addrconf_dad_begin(ifp);
		goto out;
	} else if (action == DAD_ABORT) {
		in6_ifa_hold(ifp);
		addrconf_dad_stop(ifp, 1);
		if (disable_ipv6)
			addrconf_ifdown(idev->dev, false);
		goto out;
	}

	if (!ifp->dad_probes && addrconf_dad_end(ifp))
		goto out;

	write_lock_bh(&idev->lock);
	if (idev->dead || !(idev->if_flags & IF_READY)) {
		write_unlock_bh(&idev->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	spin_lock(&ifp->lock);
	if (ifp->state == INET6_IFADDR_STATE_DEAD) {
		spin_unlock(&ifp->lock);
<<<<<<< HEAD
		read_unlock(&idev->lock);
		goto out;
	}

	if (ifp->probes == 0) {
		/*
		 * DAD was successful
		 */
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
	ADBG((KERN_DEBUG "[LGE_DATA][%s()] DAD was successful!", __func__));
#endif
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
		ifp->flags &= ~(IFA_F_TENTATIVE|IFA_F_OPTIMISTIC|IFA_F_DADFAILED);
		spin_unlock(&ifp->lock);
		read_unlock(&idev->lock);

		addrconf_dad_completed(ifp);
=======
		write_unlock_bh(&idev->lock);
		goto out;
	}

	if (ifp->dad_probes == 0) {
		bool send_na = false;

		/*
		 * DAD was successful
		 */

		if (ifp->flags & IFA_F_TENTATIVE &&
		    !(ifp->flags & IFA_F_OPTIMISTIC))
			send_na = true;
		bump_id = ifp->flags & IFA_F_TENTATIVE;
		ifp->flags &= ~(IFA_F_TENTATIVE|IFA_F_OPTIMISTIC|IFA_F_DADFAILED);
		spin_unlock(&ifp->lock);
		write_unlock_bh(&idev->lock);

		addrconf_dad_completed(ifp, bump_id, send_na);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		goto out;
	}

<<<<<<< HEAD
	ifp->probes--;
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
	ADBG((KERN_DEBUG "[LGE_DATA][%s()], ifp->idev->nd_parms->retrans_time == %d", __func__, ifp->idev->nd_parms->retrans_time));
	ADBG((KERN_DEBUG "[LGE_DATA][%s()] dev_name == %s", __func__, dev->name));

    strncpy(CurrentInterfaceName,dev->name,5);
    if(CurrentInterfaceName == NULL){
        spin_unlock(&ifp->lock);
        read_unlock(&idev->lock);
        ADBG((KERN_DEBUG "[LGE_DATA] CurrentInterfaceName is NULL !\n"));
		goto out;
    }

	ADBG((KERN_DEBUG "[LGE_DATA][%s()] CopyInterfaceName == %s, CurrentInterfaceName == %s", __func__, InterfaceNameToApply, CurrentInterfaceName));    
    
    if(strcmp(InterfaceNameToApply, CurrentInterfaceName) == 0){//In case of rmnet, this patch will be applied bacause We should not impact to the Wi-Fi and so on.
	    addrconf_mod_timer(ifp, AC_DAD, LGE_DATA_WAITING_TIME_FOR_DAD_OF_LGU);
	ADBG((KERN_DEBUG "[LGE_DATA][%s()] The waiting time for link-local DAD is set as [%d] milli-seconds in case of only rmnet interface !", __func__, LGE_DATA_WAITING_TIME_FOR_DAD_OF_LGU*10));
    }else{
//kernel original code -- START
        addrconf_mod_timer(ifp, AC_DAD, ifp->idev->nd_parms->retrans_time);
//kernel original code -- END
    }
#else
    addrconf_mod_timer(ifp, AC_DAD, ifp->idev->nd_parms->retrans_time);
#endif
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
	spin_unlock(&ifp->lock);
	read_unlock(&idev->lock);

	/* send a neighbour solicitation for our addr */
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
	ADBG((KERN_DEBUG "[LGE_DATA][%s()] send a neighbour solicitation for our addr !", __func__));
#endif
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
	addrconf_addr_solict_mult(&ifp->addr, &mcaddr);
	ndisc_send_ns(ifp->idev->dev, NULL, &ifp->addr, &mcaddr, &in6addr_any);
out:
	in6_ifa_put(ifp);
}

static void addrconf_dad_completed(struct inet6_ifaddr *ifp)
{
	struct net_device *dev = ifp->idev->dev;
=======
	ifp->dad_probes--;
	addrconf_mod_dad_work(ifp,
			      max(NEIGH_VAR(ifp->idev->nd_parms, RETRANS_TIME),
				  HZ/100));
	spin_unlock(&ifp->lock);
	write_unlock_bh(&idev->lock);

	/* send a neighbour solicitation for our addr */
	addrconf_addr_solict_mult(&ifp->addr, &mcaddr);
	ndisc_send_ns(ifp->idev->dev, &ifp->addr, &mcaddr, &in6addr_any,
		      ifp->dad_nonce);
out:
	in6_ifa_put(ifp);
	rtnl_unlock();
}

/* ifp->idev must be at least read locked */
static bool ipv6_lonely_lladdr(struct inet6_ifaddr *ifp)
{
	struct inet6_ifaddr *ifpiter;
	struct inet6_dev *idev = ifp->idev;

	list_for_each_entry_reverse(ifpiter, &idev->addr_list, if_list) {
		if (ifpiter->scope > IFA_LINK)
			break;
		if (ifp != ifpiter && ifpiter->scope == IFA_LINK &&
		    (ifpiter->flags & (IFA_F_PERMANENT|IFA_F_TENTATIVE|
				       IFA_F_OPTIMISTIC|IFA_F_DADFAILED)) ==
		    IFA_F_PERMANENT)
			return false;
	}
	return true;
}

static void addrconf_dad_completed(struct inet6_ifaddr *ifp, bool bump_id,
				   bool send_na)
{
	struct net_device *dev = ifp->idev->dev;
	struct in6_addr lladdr;
	bool send_rs, send_mld;

	addrconf_del_dad_work(ifp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 *	Configure the address for reception. Now it is valid.
	 */

	ipv6_ifa_notify(RTM_NEWADDR, ifp);
<<<<<<< HEAD
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
	ADBG((KERN_DEBUG "[LGE_DATA][%s()] dad_is_completed!", __func__));
#endif
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* If added prefix is link local and we are prepared to process
	   router advertisements, start sending router solicitations.
	 */

<<<<<<< HEAD
	if (((ifp->idev->cnf.accept_ra == 1 && !ifp->idev->cnf.forwarding) ||
	     ifp->idev->cnf.accept_ra == 2) &&
	    ifp->idev->cnf.rtr_solicits > 0 &&
	    (dev->flags&IFF_LOOPBACK) == 0 &&
	    (ipv6_addr_type(&ifp->addr) & IPV6_ADDR_LINKLOCAL)) {
=======
	read_lock_bh(&ifp->idev->lock);
	send_mld = ifp->scope == IFA_LINK && ipv6_lonely_lladdr(ifp);
	send_rs = send_mld &&
		  ipv6_accept_ra(ifp->idev) &&
		  READ_ONCE(ifp->idev->cnf.rtr_solicits) != 0 &&
		  (dev->flags & IFF_LOOPBACK) == 0 &&
		  (dev->type != ARPHRD_TUNNEL) &&
		  !netif_is_team_port(dev);
	read_unlock_bh(&ifp->idev->lock);

	/* While dad is in progress mld report's source address is in6_addrany.
	 * Resend with proper ll now.
	 */
	if (send_mld)
		ipv6_mc_dad_complete(ifp->idev);

	/* send unsolicited NA if enabled */
	if (send_na &&
	    (READ_ONCE(ifp->idev->cnf.ndisc_notify) ||
	     READ_ONCE(dev_net(dev)->ipv6.devconf_all->ndisc_notify))) {
		ndisc_send_na(dev, &in6addr_linklocal_allnodes, &ifp->addr,
			      /*router=*/ !!ifp->idev->cnf.forwarding,
			      /*solicited=*/ false, /*override=*/ true,
			      /*inc_opt=*/ true);
	}

	if (send_rs) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 *	If a host as already performed a random delay
		 *	[...] as part of DAD [...] there is no need
		 *	to delay again before sending the first RS
		 */
<<<<<<< HEAD
// LGE_CHANGE_S, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
#ifdef CONFIG_LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER
	ADBG((KERN_DEBUG "[LGE_DATA][%s()][stage 1] rs is sent now!", __func__));
#endif
// LGE_CHANGE_E, [LGE_DATA][LGP_DATA_TCPIP_SLAAC_IPV6_ALLOCATION_BOOSTER], heeyeon.nah@lge.com, 2013-05-21
		ndisc_send_rs(ifp->idev->dev, &ifp->addr, &in6addr_linklocal_allrouters);

		spin_lock_bh(&ifp->lock);
		ifp->probes = 1;
		ifp->idev->if_flags |= IF_RS_SENT;
		addrconf_mod_timer(ifp, AC_RS, ifp->idev->cnf.rtr_solicit_interval);
		spin_unlock_bh(&ifp->lock);
	}
}

static void addrconf_dad_run(struct inet6_dev *idev)
=======
		if (ipv6_get_lladdr(dev, &lladdr, IFA_F_TENTATIVE))
			return;
		ndisc_send_rs(dev, &lladdr, &in6addr_linklocal_allrouters);

		write_lock_bh(&ifp->idev->lock);
		spin_lock(&ifp->lock);
		ifp->idev->rs_interval = rfc3315_s14_backoff_init(
			READ_ONCE(ifp->idev->cnf.rtr_solicit_interval));
		ifp->idev->rs_probes = 1;
		ifp->idev->if_flags |= IF_RS_SENT;
		addrconf_mod_rs_timer(ifp->idev, ifp->idev->rs_interval);
		spin_unlock(&ifp->lock);
		write_unlock_bh(&ifp->idev->lock);
	}

	if (bump_id)
		rt_genid_bump_ipv6(dev_net(dev));

	/* Make sure that a new temporary address will be created
	 * before this temporary address becomes deprecated.
	 */
	if (ifp->flags & IFA_F_TEMPORARY)
		addrconf_verify_rtnl(dev_net(dev));
}

static void addrconf_dad_run(struct inet6_dev *idev, bool restart)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inet6_ifaddr *ifp;

	read_lock_bh(&idev->lock);
	list_for_each_entry(ifp, &idev->addr_list, if_list) {
		spin_lock(&ifp->lock);
<<<<<<< HEAD
		if (ifp->flags & IFA_F_TENTATIVE &&
		    ifp->state == INET6_IFADDR_STATE_DAD)
			addrconf_dad_kick(ifp);
=======
		if ((ifp->flags & IFA_F_TENTATIVE &&
		     ifp->state == INET6_IFADDR_STATE_DAD) || restart) {
			if (restart)
				ifp->state = INET6_IFADDR_STATE_PREDAD;
			addrconf_dad_kick(ifp);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_unlock(&ifp->lock);
	}
	read_unlock_bh(&idev->lock);
}

#ifdef CONFIG_PROC_FS
struct if6_iter_state {
	struct seq_net_private p;
	int bucket;
	int offset;
};

static struct inet6_ifaddr *if6_get_first(struct seq_file *seq, loff_t pos)
{
<<<<<<< HEAD
	struct inet6_ifaddr *ifa = NULL;
	struct if6_iter_state *state = seq->private;
	struct net *net = seq_file_net(seq);
=======
	struct if6_iter_state *state = seq->private;
	struct net *net = seq_file_net(seq);
	struct inet6_ifaddr *ifa = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int p = 0;

	/* initial bucket if pos is 0 */
	if (pos == 0) {
		state->bucket = 0;
		state->offset = 0;
	}

	for (; state->bucket < IN6_ADDR_HSIZE; ++state->bucket) {
<<<<<<< HEAD
		struct hlist_node *n;
		hlist_for_each_entry_rcu_bh(ifa, n, &inet6_addr_lst[state->bucket],
					 addr_lst) {
			if (!net_eq(dev_net(ifa->idev->dev), net))
				continue;
=======
		hlist_for_each_entry_rcu(ifa, &net->ipv6.inet6_addr_lst[state->bucket],
					 addr_lst) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* sync with offset */
			if (p < state->offset) {
				p++;
				continue;
			}
<<<<<<< HEAD
			state->offset++;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return ifa;
		}

		/* prepare for next bucket */
		state->offset = 0;
		p = 0;
	}
	return NULL;
}

static struct inet6_ifaddr *if6_get_next(struct seq_file *seq,
					 struct inet6_ifaddr *ifa)
{
	struct if6_iter_state *state = seq->private;
	struct net *net = seq_file_net(seq);
<<<<<<< HEAD
	struct hlist_node *n = &ifa->addr_lst;

	hlist_for_each_entry_continue_rcu_bh(ifa, n, addr_lst) {
		if (!net_eq(dev_net(ifa->idev->dev), net))
			continue;
=======

	hlist_for_each_entry_continue_rcu(ifa, addr_lst) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		state->offset++;
		return ifa;
	}

<<<<<<< HEAD
	while (++state->bucket < IN6_ADDR_HSIZE) {
		state->offset = 0;
		hlist_for_each_entry_rcu_bh(ifa, n,
				     &inet6_addr_lst[state->bucket], addr_lst) {
			if (!net_eq(dev_net(ifa->idev->dev), net))
				continue;
			state->offset++;
=======
	state->offset = 0;
	while (++state->bucket < IN6_ADDR_HSIZE) {
		hlist_for_each_entry_rcu(ifa,
				     &net->ipv6.inet6_addr_lst[state->bucket], addr_lst) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return ifa;
		}
	}

	return NULL;
}

static void *if6_seq_start(struct seq_file *seq, loff_t *pos)
<<<<<<< HEAD
	__acquires(rcu_bh)
{
	rcu_read_lock_bh();
=======
	__acquires(rcu)
{
	rcu_read_lock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return if6_get_first(seq, *pos);
}

static void *if6_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct inet6_ifaddr *ifa;

	ifa = if6_get_next(seq, v);
	++*pos;
	return ifa;
}

static void if6_seq_stop(struct seq_file *seq, void *v)
<<<<<<< HEAD
	__releases(rcu_bh)
{
	rcu_read_unlock_bh();
=======
	__releases(rcu)
{
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int if6_seq_show(struct seq_file *seq, void *v)
{
	struct inet6_ifaddr *ifp = (struct inet6_ifaddr *)v;
	seq_printf(seq, "%pi6 %02x %02x %02x %02x %8s\n",
		   &ifp->addr,
		   ifp->idev->dev->ifindex,
		   ifp->prefix_len,
		   ifp->scope,
<<<<<<< HEAD
		   ifp->flags,
=======
		   (u8) ifp->flags,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		   ifp->idev->dev->name);
	return 0;
}

static const struct seq_operations if6_seq_ops = {
	.start	= if6_seq_start,
	.next	= if6_seq_next,
	.show	= if6_seq_show,
	.stop	= if6_seq_stop,
};

<<<<<<< HEAD
static int if6_seq_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &if6_seq_ops,
			    sizeof(struct if6_iter_state));
}

static const struct file_operations if6_fops = {
	.owner		= THIS_MODULE,
	.open		= if6_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release_net,
};

static int __net_init if6_proc_net_init(struct net *net)
{
	if (!proc_net_fops_create(net, "if_inet6", S_IRUGO, &if6_fops))
=======
static int __net_init if6_proc_net_init(struct net *net)
{
	if (!proc_create_net("if_inet6", 0444, net->proc_net, &if6_seq_ops,
			sizeof(struct if6_iter_state)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENOMEM;
	return 0;
}

static void __net_exit if6_proc_net_exit(struct net *net)
{
<<<<<<< HEAD
       proc_net_remove(net, "if_inet6");
}

static struct pernet_operations if6_proc_net_ops = {
       .init = if6_proc_net_init,
       .exit = if6_proc_net_exit,
=======
	remove_proc_entry("if_inet6", net->proc_net);
}

static struct pernet_operations if6_proc_net_ops = {
	.init = if6_proc_net_init,
	.exit = if6_proc_net_exit,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int __init if6_proc_init(void)
{
	return register_pernet_subsys(&if6_proc_net_ops);
}

void if6_proc_exit(void)
{
	unregister_pernet_subsys(&if6_proc_net_ops);
}
#endif	/* CONFIG_PROC_FS */

<<<<<<< HEAD
#if defined(CONFIG_IPV6_MIP6) || defined(CONFIG_IPV6_MIP6_MODULE)
/* Check if address is a home address configured on any interface. */
int ipv6_chk_home_addr(struct net *net, const struct in6_addr *addr)
{
	int ret = 0;
	struct inet6_ifaddr *ifp = NULL;
	struct hlist_node *n;
	unsigned int hash = ipv6_addr_hash(addr);

	rcu_read_lock_bh();
	hlist_for_each_entry_rcu_bh(ifp, n, &inet6_addr_lst[hash], addr_lst) {
		if (!net_eq(dev_net(ifp->idev->dev), net))
			continue;
=======
#if IS_ENABLED(CONFIG_IPV6_MIP6)
/* Check if address is a home address configured on any interface. */
int ipv6_chk_home_addr(struct net *net, const struct in6_addr *addr)
{
	unsigned int hash = inet6_addr_hash(net, addr);
	struct inet6_ifaddr *ifp = NULL;
	int ret = 0;

	rcu_read_lock();
	hlist_for_each_entry_rcu(ifp, &net->ipv6.inet6_addr_lst[hash], addr_lst) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ipv6_addr_equal(&ifp->addr, addr) &&
		    (ifp->flags & IFA_F_HOMEADDRESS)) {
			ret = 1;
			break;
		}
	}
<<<<<<< HEAD
	rcu_read_unlock_bh();
=======
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
#endif

<<<<<<< HEAD
=======
/* RFC6554 has some algorithm to avoid loops in segment routing by
 * checking if the segments contains any of a local interface address.
 *
 * Quote:
 *
 * To detect loops in the SRH, a router MUST determine if the SRH
 * includes multiple addresses assigned to any interface on that router.
 * If such addresses appear more than once and are separated by at least
 * one address not assigned to that router.
 */
int ipv6_chk_rpl_srh_loop(struct net *net, const struct in6_addr *segs,
			  unsigned char nsegs)
{
	const struct in6_addr *addr;
	int i, ret = 0, found = 0;
	struct inet6_ifaddr *ifp;
	bool separated = false;
	unsigned int hash;
	bool hash_found;

	rcu_read_lock();
	for (i = 0; i < nsegs; i++) {
		addr = &segs[i];
		hash = inet6_addr_hash(net, addr);

		hash_found = false;
		hlist_for_each_entry_rcu(ifp, &net->ipv6.inet6_addr_lst[hash], addr_lst) {

			if (ipv6_addr_equal(&ifp->addr, addr)) {
				hash_found = true;
				break;
			}
		}

		if (hash_found) {
			if (found > 1 && separated) {
				ret = 1;
				break;
			}

			separated = false;
			found++;
		} else {
			separated = true;
		}
	}
	rcu_read_unlock();

	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Periodic address status verification
 */

<<<<<<< HEAD
static void addrconf_verify(unsigned long foo)
{
	unsigned long now, next, next_sec, next_sched;
	struct inet6_ifaddr *ifp;
	struct hlist_node *node;
	int i;

	rcu_read_lock_bh();
	spin_lock(&addrconf_verify_lock);
	now = jiffies;
	next = round_jiffies_up(now + ADDR_CHECK_FREQUENCY);

	del_timer(&addr_chk_timer);

	for (i = 0; i < IN6_ADDR_HSIZE; i++) {
restart:
		hlist_for_each_entry_rcu_bh(ifp, node,
					 &inet6_addr_lst[i], addr_lst) {
			unsigned long age;

			if (ifp->flags & IFA_F_PERMANENT)
=======
static void addrconf_verify_rtnl(struct net *net)
{
	unsigned long now, next, next_sec, next_sched;
	struct inet6_ifaddr *ifp;
	int i;

	ASSERT_RTNL();

	rcu_read_lock_bh();
	now = jiffies;
	next = round_jiffies_up(now + ADDR_CHECK_FREQUENCY);

	cancel_delayed_work(&net->ipv6.addr_chk_work);

	for (i = 0; i < IN6_ADDR_HSIZE; i++) {
restart:
		hlist_for_each_entry_rcu_bh(ifp, &net->ipv6.inet6_addr_lst[i], addr_lst) {
			unsigned long age;

			/* When setting preferred_lft to a value not zero or
			 * infinity, while valid_lft is infinity
			 * IFA_F_PERMANENT has a non-infinity life time.
			 */
			if ((ifp->flags & IFA_F_PERMANENT) &&
			    (ifp->prefered_lft == INFINITY_LIFE_TIME))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				continue;

			spin_lock(&ifp->lock);
			/* We try to batch several events at once. */
			age = (now - ifp->tstamp + ADDRCONF_TIMER_FUZZ_MINUS) / HZ;

<<<<<<< HEAD
=======
			if ((ifp->flags&IFA_F_TEMPORARY) &&
			    !(ifp->flags&IFA_F_TENTATIVE) &&
			    ifp->prefered_lft != INFINITY_LIFE_TIME &&
			    !ifp->regen_count && ifp->ifpub) {
				/* This is a non-regenerated temporary addr. */

				unsigned long regen_advance = ipv6_get_regen_advance(ifp->idev);

				if (age + regen_advance >= ifp->prefered_lft) {
					struct inet6_ifaddr *ifpub = ifp->ifpub;
					if (time_before(ifp->tstamp + ifp->prefered_lft * HZ, next))
						next = ifp->tstamp + ifp->prefered_lft * HZ;

					ifp->regen_count++;
					in6_ifa_hold(ifp);
					in6_ifa_hold(ifpub);
					spin_unlock(&ifp->lock);

					spin_lock(&ifpub->lock);
					ifpub->regen_count = 0;
					spin_unlock(&ifpub->lock);
					rcu_read_unlock_bh();
					ipv6_create_tempaddr(ifpub, true);
					in6_ifa_put(ifpub);
					in6_ifa_put(ifp);
					rcu_read_lock_bh();
					goto restart;
				} else if (time_before(ifp->tstamp + ifp->prefered_lft * HZ - regen_advance * HZ, next))
					next = ifp->tstamp + ifp->prefered_lft * HZ - regen_advance * HZ;
			}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (ifp->valid_lft != INFINITY_LIFE_TIME &&
			    age >= ifp->valid_lft) {
				spin_unlock(&ifp->lock);
				in6_ifa_hold(ifp);
<<<<<<< HEAD
				ipv6_del_addr(ifp);
=======
				rcu_read_unlock_bh();
				ipv6_del_addr(ifp);
				rcu_read_lock_bh();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				goto restart;
			} else if (ifp->prefered_lft == INFINITY_LIFE_TIME) {
				spin_unlock(&ifp->lock);
				continue;
			} else if (age >= ifp->prefered_lft) {
				/* jiffies - ifp->tstamp > age >= ifp->prefered_lft */
				int deprecate = 0;

				if (!(ifp->flags&IFA_F_DEPRECATED)) {
					deprecate = 1;
					ifp->flags |= IFA_F_DEPRECATED;
				}

<<<<<<< HEAD
				if (time_before(ifp->tstamp + ifp->valid_lft * HZ, next))
=======
				if ((ifp->valid_lft != INFINITY_LIFE_TIME) &&
				    (time_before(ifp->tstamp + ifp->valid_lft * HZ, next)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					next = ifp->tstamp + ifp->valid_lft * HZ;

				spin_unlock(&ifp->lock);

				if (deprecate) {
					in6_ifa_hold(ifp);

					ipv6_ifa_notify(0, ifp);
					in6_ifa_put(ifp);
					goto restart;
				}
<<<<<<< HEAD
#ifdef CONFIG_IPV6_PRIVACY
			} else if ((ifp->flags&IFA_F_TEMPORARY) &&
				   !(ifp->flags&IFA_F_TENTATIVE)) {
				unsigned long regen_advance = ifp->idev->cnf.regen_max_retry *
					ifp->idev->cnf.dad_transmits *
					ifp->idev->nd_parms->retrans_time / HZ;

				if (age >= ifp->prefered_lft - regen_advance) {
					struct inet6_ifaddr *ifpub = ifp->ifpub;
					if (time_before(ifp->tstamp + ifp->prefered_lft * HZ, next))
						next = ifp->tstamp + ifp->prefered_lft * HZ;
					if (!ifp->regen_count && ifpub) {
						ifp->regen_count++;
						in6_ifa_hold(ifp);
						in6_ifa_hold(ifpub);
						spin_unlock(&ifp->lock);

						spin_lock(&ifpub->lock);
						ifpub->regen_count = 0;
						spin_unlock(&ifpub->lock);
						ipv6_create_tempaddr(ifpub, ifp);
						in6_ifa_put(ifpub);
						in6_ifa_put(ifp);
						goto restart;
					}
				} else if (time_before(ifp->tstamp + ifp->prefered_lft * HZ - regen_advance * HZ, next))
					next = ifp->tstamp + ifp->prefered_lft * HZ - regen_advance * HZ;
				spin_unlock(&ifp->lock);
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			} else {
				/* ifp->prefered_lft <= ifp->valid_lft */
				if (time_before(ifp->tstamp + ifp->prefered_lft * HZ, next))
					next = ifp->tstamp + ifp->prefered_lft * HZ;
				spin_unlock(&ifp->lock);
			}
		}
	}

	next_sec = round_jiffies_up(next);
	next_sched = next;

	/* If rounded timeout is accurate enough, accept it. */
	if (time_before(next_sec, next + ADDRCONF_TIMER_FUZZ))
		next_sched = next_sec;

	/* And minimum interval is ADDRCONF_TIMER_FUZZ_MAX. */
	if (time_before(next_sched, jiffies + ADDRCONF_TIMER_FUZZ_MAX))
		next_sched = jiffies + ADDRCONF_TIMER_FUZZ_MAX;

<<<<<<< HEAD
	ADBG((KERN_DEBUG "now = %lu, schedule = %lu, rounded schedule = %lu => %lu\n",
	      now, next, next_sec, next_sched));

	addr_chk_timer.expires = next_sched;
	add_timer(&addr_chk_timer);
	spin_unlock(&addrconf_verify_lock);
	rcu_read_unlock_bh();
}

static struct in6_addr *extract_addr(struct nlattr *addr, struct nlattr *local)
{
	struct in6_addr *pfx = NULL;

=======
	pr_debug("now = %lu, schedule = %lu, rounded schedule = %lu => %lu\n",
		 now, next, next_sec, next_sched);
	mod_delayed_work(addrconf_wq, &net->ipv6.addr_chk_work, next_sched - now);
	rcu_read_unlock_bh();
}

static void addrconf_verify_work(struct work_struct *w)
{
	struct net *net = container_of(to_delayed_work(w), struct net,
				       ipv6.addr_chk_work);

	rtnl_lock();
	addrconf_verify_rtnl(net);
	rtnl_unlock();
}

static void addrconf_verify(struct net *net)
{
	mod_delayed_work(addrconf_wq, &net->ipv6.addr_chk_work, 0);
}

static struct in6_addr *extract_addr(struct nlattr *addr, struct nlattr *local,
				     struct in6_addr **peer_pfx)
{
	struct in6_addr *pfx = NULL;

	*peer_pfx = NULL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (addr)
		pfx = nla_data(addr);

	if (local) {
		if (pfx && nla_memcmp(local, pfx, sizeof(*pfx)))
<<<<<<< HEAD
			pfx = NULL;
		else
			pfx = nla_data(local);
=======
			*peer_pfx = pfx;
		pfx = nla_data(local);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return pfx;
}

static const struct nla_policy ifa_ipv6_policy[IFA_MAX+1] = {
	[IFA_ADDRESS]		= { .len = sizeof(struct in6_addr) },
	[IFA_LOCAL]		= { .len = sizeof(struct in6_addr) },
	[IFA_CACHEINFO]		= { .len = sizeof(struct ifa_cacheinfo) },
<<<<<<< HEAD
};

static int
inet6_rtm_deladdr(struct sk_buff *skb, struct nlmsghdr *nlh, void *arg)
=======
	[IFA_FLAGS]		= { .len = sizeof(u32) },
	[IFA_RT_PRIORITY]	= { .len = sizeof(u32) },
	[IFA_TARGET_NETNSID]	= { .type = NLA_S32 },
	[IFA_PROTO]		= { .type = NLA_U8 },
};

static int
inet6_rtm_deladdr(struct sk_buff *skb, struct nlmsghdr *nlh,
		  struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net *net = sock_net(skb->sk);
	struct ifaddrmsg *ifm;
	struct nlattr *tb[IFA_MAX+1];
<<<<<<< HEAD
	struct in6_addr *pfx;
	int err;

	err = nlmsg_parse(nlh, sizeof(*ifm), tb, IFA_MAX, ifa_ipv6_policy);
=======
	struct in6_addr *pfx, *peer_pfx;
	u32 ifa_flags;
	int err;

	err = nlmsg_parse_deprecated(nlh, sizeof(*ifm), tb, IFA_MAX,
				     ifa_ipv6_policy, extack);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0)
		return err;

	ifm = nlmsg_data(nlh);
<<<<<<< HEAD
	pfx = extract_addr(tb[IFA_ADDRESS], tb[IFA_LOCAL]);
	if (pfx == NULL)
		return -EINVAL;

	return inet6_addr_del(net, ifm->ifa_index, pfx, ifm->ifa_prefixlen);
}

static int inet6_addr_modify(struct inet6_ifaddr *ifp, u8 ifa_flags,
			     u32 prefered_lft, u32 valid_lft)
=======
	pfx = extract_addr(tb[IFA_ADDRESS], tb[IFA_LOCAL], &peer_pfx);
	if (!pfx)
		return -EINVAL;

	ifa_flags = tb[IFA_FLAGS] ? nla_get_u32(tb[IFA_FLAGS]) : ifm->ifa_flags;

	/* We ignore other flags so far. */
	ifa_flags &= IFA_F_MANAGETEMPADDR;

	return inet6_addr_del(net, ifm->ifa_index, ifa_flags, pfx,
			      ifm->ifa_prefixlen, extack);
}

static int modify_prefix_route(struct inet6_ifaddr *ifp,
			       unsigned long expires, u32 flags,
			       bool modify_peer)
{
	struct fib6_table *table;
	struct fib6_info *f6i;
	u32 prio;

	f6i = addrconf_get_prefix_route(modify_peer ? &ifp->peer_addr : &ifp->addr,
					ifp->prefix_len,
					ifp->idev->dev, 0, RTF_DEFAULT, true);
	if (!f6i)
		return -ENOENT;

	prio = ifp->rt_priority ? : IP6_RT_PRIO_ADDRCONF;
	if (f6i->fib6_metric != prio) {
		/* delete old one */
		ip6_del_rt(dev_net(ifp->idev->dev), f6i, false);

		/* add new one */
		addrconf_prefix_route(modify_peer ? &ifp->peer_addr : &ifp->addr,
				      ifp->prefix_len,
				      ifp->rt_priority, ifp->idev->dev,
				      expires, flags, GFP_KERNEL);
	} else {
		table = f6i->fib6_table;
		spin_lock_bh(&table->tb6_lock);

		if (!(flags & RTF_EXPIRES)) {
			fib6_clean_expires(f6i);
			fib6_remove_gc_list(f6i);
		} else {
			fib6_set_expires(f6i, expires);
			fib6_add_gc_list(f6i);
		}

		spin_unlock_bh(&table->tb6_lock);

		fib6_info_release(f6i);
	}

	return 0;
}

static int inet6_addr_modify(struct net *net, struct inet6_ifaddr *ifp,
			     struct ifa6_config *cfg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 flags;
	clock_t expires;
	unsigned long timeout;
<<<<<<< HEAD

	if (!valid_lft || (prefered_lft > valid_lft))
		return -EINVAL;

	timeout = addrconf_timeout_fixup(valid_lft, HZ);
	if (addrconf_finite_timeout(timeout)) {
		expires = jiffies_to_clock_t(timeout * HZ);
		valid_lft = timeout;
=======
	bool was_managetempaddr;
	bool had_prefixroute;
	bool new_peer = false;

	ASSERT_RTNL();

	if (!cfg->valid_lft || cfg->preferred_lft > cfg->valid_lft)
		return -EINVAL;

	if (cfg->ifa_flags & IFA_F_MANAGETEMPADDR &&
	    (ifp->flags & IFA_F_TEMPORARY || ifp->prefix_len != 64))
		return -EINVAL;

	if (!(ifp->flags & IFA_F_TENTATIVE) || ifp->flags & IFA_F_DADFAILED)
		cfg->ifa_flags &= ~IFA_F_OPTIMISTIC;

	timeout = addrconf_timeout_fixup(cfg->valid_lft, HZ);
	if (addrconf_finite_timeout(timeout)) {
		expires = jiffies_to_clock_t(timeout * HZ);
		cfg->valid_lft = timeout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		flags = RTF_EXPIRES;
	} else {
		expires = 0;
		flags = 0;
<<<<<<< HEAD
		ifa_flags |= IFA_F_PERMANENT;
	}

	timeout = addrconf_timeout_fixup(prefered_lft, HZ);
	if (addrconf_finite_timeout(timeout)) {
		if (timeout == 0)
			ifa_flags |= IFA_F_DEPRECATED;
		prefered_lft = timeout;
	}

	spin_lock_bh(&ifp->lock);
	ifp->flags = (ifp->flags & ~(IFA_F_DEPRECATED | IFA_F_PERMANENT | IFA_F_NODAD | IFA_F_HOMEADDRESS)) | ifa_flags;
	ifp->tstamp = jiffies;
	ifp->valid_lft = valid_lft;
	ifp->prefered_lft = prefered_lft;
=======
		cfg->ifa_flags |= IFA_F_PERMANENT;
	}

	timeout = addrconf_timeout_fixup(cfg->preferred_lft, HZ);
	if (addrconf_finite_timeout(timeout)) {
		if (timeout == 0)
			cfg->ifa_flags |= IFA_F_DEPRECATED;
		cfg->preferred_lft = timeout;
	}

	if (cfg->peer_pfx &&
	    memcmp(&ifp->peer_addr, cfg->peer_pfx, sizeof(struct in6_addr))) {
		if (!ipv6_addr_any(&ifp->peer_addr))
			cleanup_prefix_route(ifp, expires, true, true);
		new_peer = true;
	}

	spin_lock_bh(&ifp->lock);
	was_managetempaddr = ifp->flags & IFA_F_MANAGETEMPADDR;
	had_prefixroute = ifp->flags & IFA_F_PERMANENT &&
			  !(ifp->flags & IFA_F_NOPREFIXROUTE);
	ifp->flags &= ~(IFA_F_DEPRECATED | IFA_F_PERMANENT | IFA_F_NODAD |
			IFA_F_HOMEADDRESS | IFA_F_MANAGETEMPADDR |
			IFA_F_NOPREFIXROUTE);
	ifp->flags |= cfg->ifa_flags;
	WRITE_ONCE(ifp->tstamp, jiffies);
	WRITE_ONCE(ifp->valid_lft, cfg->valid_lft);
	WRITE_ONCE(ifp->prefered_lft, cfg->preferred_lft);
	WRITE_ONCE(ifp->ifa_proto, cfg->ifa_proto);

	if (cfg->rt_priority && cfg->rt_priority != ifp->rt_priority)
		WRITE_ONCE(ifp->rt_priority, cfg->rt_priority);

	if (new_peer)
		ifp->peer_addr = *cfg->peer_pfx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_unlock_bh(&ifp->lock);
	if (!(ifp->flags&IFA_F_TENTATIVE))
		ipv6_ifa_notify(0, ifp);

<<<<<<< HEAD
	addrconf_prefix_route(&ifp->addr, ifp->prefix_len, ifp->idev->dev,
			      expires, flags);
	addrconf_verify(0);
=======
	if (!(cfg->ifa_flags & IFA_F_NOPREFIXROUTE)) {
		int rc = -ENOENT;

		if (had_prefixroute)
			rc = modify_prefix_route(ifp, expires, flags, false);

		/* prefix route could have been deleted; if so restore it */
		if (rc == -ENOENT) {
			addrconf_prefix_route(&ifp->addr, ifp->prefix_len,
					      ifp->rt_priority, ifp->idev->dev,
					      expires, flags, GFP_KERNEL);
		}

		if (had_prefixroute && !ipv6_addr_any(&ifp->peer_addr))
			rc = modify_prefix_route(ifp, expires, flags, true);

		if (rc == -ENOENT && !ipv6_addr_any(&ifp->peer_addr)) {
			addrconf_prefix_route(&ifp->peer_addr, ifp->prefix_len,
					      ifp->rt_priority, ifp->idev->dev,
					      expires, flags, GFP_KERNEL);
		}
	} else if (had_prefixroute) {
		enum cleanup_prefix_rt_t action;
		unsigned long rt_expires;

		write_lock_bh(&ifp->idev->lock);
		action = check_cleanup_prefix_route(ifp, &rt_expires);
		write_unlock_bh(&ifp->idev->lock);

		if (action != CLEANUP_PREFIX_RT_NOP) {
			cleanup_prefix_route(ifp, rt_expires,
				action == CLEANUP_PREFIX_RT_DEL, false);
		}
	}

	if (was_managetempaddr || ifp->flags & IFA_F_MANAGETEMPADDR) {
		if (was_managetempaddr &&
		    !(ifp->flags & IFA_F_MANAGETEMPADDR)) {
			cfg->valid_lft = 0;
			cfg->preferred_lft = 0;
		}
		manage_tempaddrs(ifp->idev, ifp, cfg->valid_lft,
				 cfg->preferred_lft, !was_managetempaddr,
				 jiffies);
	}

	addrconf_verify_rtnl(net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int
<<<<<<< HEAD
inet6_rtm_newaddr(struct sk_buff *skb, struct nlmsghdr *nlh, void *arg)
=======
inet6_rtm_newaddr(struct sk_buff *skb, struct nlmsghdr *nlh,
		  struct netlink_ext_ack *extack)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net *net = sock_net(skb->sk);
	struct ifaddrmsg *ifm;
	struct nlattr *tb[IFA_MAX+1];
<<<<<<< HEAD
	struct in6_addr *pfx;
	struct inet6_ifaddr *ifa;
	struct net_device *dev;
	u32 valid_lft = INFINITY_LIFE_TIME, preferred_lft = INFINITY_LIFE_TIME;
	u8 ifa_flags;
	int err;

	err = nlmsg_parse(nlh, sizeof(*ifm), tb, IFA_MAX, ifa_ipv6_policy);
	if (err < 0)
		return err;

	ifm = nlmsg_data(nlh);
	pfx = extract_addr(tb[IFA_ADDRESS], tb[IFA_LOCAL]);
	if (pfx == NULL)
		return -EINVAL;

=======
	struct in6_addr *peer_pfx;
	struct inet6_ifaddr *ifa;
	struct net_device *dev;
	struct inet6_dev *idev;
	struct ifa6_config cfg;
	int err;

	err = nlmsg_parse_deprecated(nlh, sizeof(*ifm), tb, IFA_MAX,
				     ifa_ipv6_policy, extack);
	if (err < 0)
		return err;

	memset(&cfg, 0, sizeof(cfg));

	ifm = nlmsg_data(nlh);
	cfg.pfx = extract_addr(tb[IFA_ADDRESS], tb[IFA_LOCAL], &peer_pfx);
	if (!cfg.pfx)
		return -EINVAL;

	cfg.peer_pfx = peer_pfx;
	cfg.plen = ifm->ifa_prefixlen;
	if (tb[IFA_RT_PRIORITY])
		cfg.rt_priority = nla_get_u32(tb[IFA_RT_PRIORITY]);

	if (tb[IFA_PROTO])
		cfg.ifa_proto = nla_get_u8(tb[IFA_PROTO]);

	cfg.valid_lft = INFINITY_LIFE_TIME;
	cfg.preferred_lft = INFINITY_LIFE_TIME;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (tb[IFA_CACHEINFO]) {
		struct ifa_cacheinfo *ci;

		ci = nla_data(tb[IFA_CACHEINFO]);
<<<<<<< HEAD
		valid_lft = ci->ifa_valid;
		preferred_lft = ci->ifa_prefered;
	} else {
		preferred_lft = INFINITY_LIFE_TIME;
		valid_lft = INFINITY_LIFE_TIME;
	}

	dev =  __dev_get_by_index(net, ifm->ifa_index);
	if (dev == NULL)
		return -ENODEV;

	/* We ignore other flags so far. */
	ifa_flags = ifm->ifa_flags & (IFA_F_NODAD | IFA_F_HOMEADDRESS);

	ifa = ipv6_get_ifaddr(net, pfx, dev, 1);
	if (ifa == NULL) {
		/*
		 * It would be best to check for !NLM_F_CREATE here but
		 * userspace alreay relies on not having to provide this.
		 */
		return inet6_addr_add(net, ifm->ifa_index, pfx,
				      ifm->ifa_prefixlen, ifa_flags,
				      preferred_lft, valid_lft);
	}

	if (nlh->nlmsg_flags & NLM_F_EXCL ||
	    !(nlh->nlmsg_flags & NLM_F_REPLACE))
		err = -EEXIST;
	else
		err = inet6_addr_modify(ifa, ifa_flags, preferred_lft, valid_lft);
=======
		cfg.valid_lft = ci->ifa_valid;
		cfg.preferred_lft = ci->ifa_prefered;
	}

	dev =  __dev_get_by_index(net, ifm->ifa_index);
	if (!dev) {
		NL_SET_ERR_MSG_MOD(extack, "Unable to find the interface");
		return -ENODEV;
	}

	if (tb[IFA_FLAGS])
		cfg.ifa_flags = nla_get_u32(tb[IFA_FLAGS]);
	else
		cfg.ifa_flags = ifm->ifa_flags;

	/* We ignore other flags so far. */
	cfg.ifa_flags &= IFA_F_NODAD | IFA_F_HOMEADDRESS |
			 IFA_F_MANAGETEMPADDR | IFA_F_NOPREFIXROUTE |
			 IFA_F_MCAUTOJOIN | IFA_F_OPTIMISTIC;

	idev = ipv6_find_idev(dev);
	if (IS_ERR(idev))
		return PTR_ERR(idev);

	if (!ipv6_allow_optimistic_dad(net, idev))
		cfg.ifa_flags &= ~IFA_F_OPTIMISTIC;

	if (cfg.ifa_flags & IFA_F_NODAD &&
	    cfg.ifa_flags & IFA_F_OPTIMISTIC) {
		NL_SET_ERR_MSG(extack, "IFA_F_NODAD and IFA_F_OPTIMISTIC are mutually exclusive");
		return -EINVAL;
	}

	ifa = ipv6_get_ifaddr(net, cfg.pfx, dev, 1);
	if (!ifa) {
		/*
		 * It would be best to check for !NLM_F_CREATE here but
		 * userspace already relies on not having to provide this.
		 */
		return inet6_addr_add(net, ifm->ifa_index, &cfg, extack);
	}

	if (nlh->nlmsg_flags & NLM_F_EXCL ||
	    !(nlh->nlmsg_flags & NLM_F_REPLACE)) {
		NL_SET_ERR_MSG_MOD(extack, "address already assigned");
		err = -EEXIST;
	} else {
		err = inet6_addr_modify(net, ifa, &cfg);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	in6_ifa_put(ifa);

	return err;
}

<<<<<<< HEAD
static void put_ifaddrmsg(struct nlmsghdr *nlh, u8 prefixlen, u8 flags,
=======
static void put_ifaddrmsg(struct nlmsghdr *nlh, u8 prefixlen, u32 flags,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  u8 scope, int ifindex)
{
	struct ifaddrmsg *ifm;

	ifm = nlmsg_data(nlh);
	ifm->ifa_family = AF_INET6;
	ifm->ifa_prefixlen = prefixlen;
	ifm->ifa_flags = flags;
	ifm->ifa_scope = scope;
	ifm->ifa_index = ifindex;
}

static int put_cacheinfo(struct sk_buff *skb, unsigned long cstamp,
			 unsigned long tstamp, u32 preferred, u32 valid)
{
	struct ifa_cacheinfo ci;

	ci.cstamp = cstamp_delta(cstamp);
	ci.tstamp = cstamp_delta(tstamp);
	ci.ifa_prefered = preferred;
	ci.ifa_valid = valid;

	return nla_put(skb, IFA_CACHEINFO, sizeof(ci), &ci);
}

static inline int rt_scope(int ifa_scope)
{
	if (ifa_scope & IFA_HOST)
		return RT_SCOPE_HOST;
	else if (ifa_scope & IFA_LINK)
		return RT_SCOPE_LINK;
	else if (ifa_scope & IFA_SITE)
		return RT_SCOPE_SITE;
	else
		return RT_SCOPE_UNIVERSE;
}

static inline int inet6_ifaddr_msgsize(void)
{
	return NLMSG_ALIGN(sizeof(struct ifaddrmsg))
<<<<<<< HEAD
	       + nla_total_size(16) /* IFA_ADDRESS */
	       + nla_total_size(sizeof(struct ifa_cacheinfo));
}

static int inet6_fill_ifaddr(struct sk_buff *skb, struct inet6_ifaddr *ifa,
			     u32 pid, u32 seq, int event, unsigned int flags)
{
	struct nlmsghdr  *nlh;
	u32 preferred, valid;

	nlh = nlmsg_put(skb, pid, seq, event, sizeof(struct ifaddrmsg), flags);
	if (nlh == NULL)
		return -EMSGSIZE;

	put_ifaddrmsg(nlh, ifa->prefix_len, ifa->flags, rt_scope(ifa->scope),
		      ifa->idev->dev->ifindex);

	if (!(ifa->flags&IFA_F_PERMANENT)) {
		preferred = ifa->prefered_lft;
		valid = ifa->valid_lft;
		if (preferred != INFINITY_LIFE_TIME) {
			long tval = (jiffies - ifa->tstamp)/HZ;
=======
	       + nla_total_size(16) /* IFA_LOCAL */
	       + nla_total_size(16) /* IFA_ADDRESS */
	       + nla_total_size(sizeof(struct ifa_cacheinfo))
	       + nla_total_size(4)  /* IFA_FLAGS */
	       + nla_total_size(1)  /* IFA_PROTO */
	       + nla_total_size(4)  /* IFA_RT_PRIORITY */;
}

enum addr_type_t {
	UNICAST_ADDR,
	MULTICAST_ADDR,
	ANYCAST_ADDR,
};

struct inet6_fill_args {
	u32 portid;
	u32 seq;
	int event;
	unsigned int flags;
	int netnsid;
	int ifindex;
	enum addr_type_t type;
};

static int inet6_fill_ifaddr(struct sk_buff *skb,
			     const struct inet6_ifaddr *ifa,
			     struct inet6_fill_args *args)
{
	struct nlmsghdr *nlh;
	u32 preferred, valid;
	u32 flags, priority;
	u8 proto;

	nlh = nlmsg_put(skb, args->portid, args->seq, args->event,
			sizeof(struct ifaddrmsg), args->flags);
	if (!nlh)
		return -EMSGSIZE;

	flags = READ_ONCE(ifa->flags);
	put_ifaddrmsg(nlh, ifa->prefix_len, ifa->flags, rt_scope(ifa->scope),
		      ifa->idev->dev->ifindex);

	if (args->netnsid >= 0 &&
	    nla_put_s32(skb, IFA_TARGET_NETNSID, args->netnsid))
		goto error;

	preferred = READ_ONCE(ifa->prefered_lft);
	valid = READ_ONCE(ifa->valid_lft);

	if (!((flags & IFA_F_PERMANENT) &&
	      (preferred == INFINITY_LIFE_TIME))) {
		if (preferred != INFINITY_LIFE_TIME) {
			long tval = (jiffies - READ_ONCE(ifa->tstamp)) / HZ;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (preferred > tval)
				preferred -= tval;
			else
				preferred = 0;
			if (valid != INFINITY_LIFE_TIME) {
				if (valid > tval)
					valid -= tval;
				else
					valid = 0;
			}
		}
	} else {
		preferred = INFINITY_LIFE_TIME;
		valid = INFINITY_LIFE_TIME;
	}

<<<<<<< HEAD
	if (nla_put(skb, IFA_ADDRESS, 16, &ifa->addr) < 0 ||
	    put_cacheinfo(skb, ifa->cstamp, ifa->tstamp, preferred, valid) < 0) {
		nlmsg_cancel(skb, nlh);
		return -EMSGSIZE;
	}

	return nlmsg_end(skb, nlh);
}

static int inet6_fill_ifmcaddr(struct sk_buff *skb, struct ifmcaddr6 *ifmca,
				u32 pid, u32 seq, int event, u16 flags)
{
	struct nlmsghdr  *nlh;
	u8 scope = RT_SCOPE_UNIVERSE;
	int ifindex = ifmca->idev->dev->ifindex;
=======
	if (!ipv6_addr_any(&ifa->peer_addr)) {
		if (nla_put_in6_addr(skb, IFA_LOCAL, &ifa->addr) < 0 ||
		    nla_put_in6_addr(skb, IFA_ADDRESS, &ifa->peer_addr) < 0)
			goto error;
	} else {
		if (nla_put_in6_addr(skb, IFA_ADDRESS, &ifa->addr) < 0)
			goto error;
	}

	priority = READ_ONCE(ifa->rt_priority);
	if (priority && nla_put_u32(skb, IFA_RT_PRIORITY, priority))
		goto error;

	if (put_cacheinfo(skb, ifa->cstamp, READ_ONCE(ifa->tstamp),
			  preferred, valid) < 0)
		goto error;

	if (nla_put_u32(skb, IFA_FLAGS, flags) < 0)
		goto error;

	proto = READ_ONCE(ifa->ifa_proto);
	if (proto && nla_put_u8(skb, IFA_PROTO, proto))
		goto error;

	nlmsg_end(skb, nlh);
	return 0;

error:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}

static int inet6_fill_ifmcaddr(struct sk_buff *skb,
			       const struct ifmcaddr6 *ifmca,
			       struct inet6_fill_args *args)
{
	int ifindex = ifmca->idev->dev->ifindex;
	u8 scope = RT_SCOPE_UNIVERSE;
	struct nlmsghdr *nlh;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ipv6_addr_scope(&ifmca->mca_addr) & IFA_SITE)
		scope = RT_SCOPE_SITE;

<<<<<<< HEAD
	nlh = nlmsg_put(skb, pid, seq, event, sizeof(struct ifaddrmsg), flags);
	if (nlh == NULL)
		return -EMSGSIZE;

	put_ifaddrmsg(nlh, 128, IFA_F_PERMANENT, scope, ifindex);
	if (nla_put(skb, IFA_MULTICAST, 16, &ifmca->mca_addr) < 0 ||
	    put_cacheinfo(skb, ifmca->mca_cstamp, ifmca->mca_tstamp,
=======
	nlh = nlmsg_put(skb, args->portid, args->seq, args->event,
			sizeof(struct ifaddrmsg), args->flags);
	if (!nlh)
		return -EMSGSIZE;

	if (args->netnsid >= 0 &&
	    nla_put_s32(skb, IFA_TARGET_NETNSID, args->netnsid)) {
		nlmsg_cancel(skb, nlh);
		return -EMSGSIZE;
	}

	put_ifaddrmsg(nlh, 128, IFA_F_PERMANENT, scope, ifindex);
	if (nla_put_in6_addr(skb, IFA_MULTICAST, &ifmca->mca_addr) < 0 ||
	    put_cacheinfo(skb, ifmca->mca_cstamp, READ_ONCE(ifmca->mca_tstamp),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  INFINITY_LIFE_TIME, INFINITY_LIFE_TIME) < 0) {
		nlmsg_cancel(skb, nlh);
		return -EMSGSIZE;
	}

<<<<<<< HEAD
	return nlmsg_end(skb, nlh);
}

static int inet6_fill_ifacaddr(struct sk_buff *skb, struct ifacaddr6 *ifaca,
				u32 pid, u32 seq, int event, unsigned int flags)
{
	struct nlmsghdr  *nlh;
	u8 scope = RT_SCOPE_UNIVERSE;
	int ifindex = ifaca->aca_idev->dev->ifindex;
=======
	nlmsg_end(skb, nlh);
	return 0;
}

static int inet6_fill_ifacaddr(struct sk_buff *skb,
			       const struct ifacaddr6 *ifaca,
			       struct inet6_fill_args *args)
{
	struct net_device *dev = fib6_info_nh_dev(ifaca->aca_rt);
	int ifindex = dev ? dev->ifindex : 1;
	u8 scope = RT_SCOPE_UNIVERSE;
	struct nlmsghdr *nlh;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ipv6_addr_scope(&ifaca->aca_addr) & IFA_SITE)
		scope = RT_SCOPE_SITE;

<<<<<<< HEAD
	nlh = nlmsg_put(skb, pid, seq, event, sizeof(struct ifaddrmsg), flags);
	if (nlh == NULL)
		return -EMSGSIZE;

	put_ifaddrmsg(nlh, 128, IFA_F_PERMANENT, scope, ifindex);
	if (nla_put(skb, IFA_ANYCAST, 16, &ifaca->aca_addr) < 0 ||
	    put_cacheinfo(skb, ifaca->aca_cstamp, ifaca->aca_tstamp,
=======
	nlh = nlmsg_put(skb, args->portid, args->seq, args->event,
			sizeof(struct ifaddrmsg), args->flags);
	if (!nlh)
		return -EMSGSIZE;

	if (args->netnsid >= 0 &&
	    nla_put_s32(skb, IFA_TARGET_NETNSID, args->netnsid)) {
		nlmsg_cancel(skb, nlh);
		return -EMSGSIZE;
	}

	put_ifaddrmsg(nlh, 128, IFA_F_PERMANENT, scope, ifindex);
	if (nla_put_in6_addr(skb, IFA_ANYCAST, &ifaca->aca_addr) < 0 ||
	    put_cacheinfo(skb, ifaca->aca_cstamp, READ_ONCE(ifaca->aca_tstamp),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  INFINITY_LIFE_TIME, INFINITY_LIFE_TIME) < 0) {
		nlmsg_cancel(skb, nlh);
		return -EMSGSIZE;
	}

<<<<<<< HEAD
	return nlmsg_end(skb, nlh);
}

enum addr_type_t {
	UNICAST_ADDR,
	MULTICAST_ADDR,
	ANYCAST_ADDR,
};

/* called with rcu_read_lock() */
static int in6_dump_addrs(struct inet6_dev *idev, struct sk_buff *skb,
			  struct netlink_callback *cb, enum addr_type_t type,
			  int s_ip_idx, int *p_ip_idx)
{
	struct ifmcaddr6 *ifmca;
	struct ifacaddr6 *ifaca;
	int err = 1;
	int ip_idx = *p_ip_idx;

	read_lock_bh(&idev->lock);
	switch (type) {
	case UNICAST_ADDR: {
		struct inet6_ifaddr *ifa;

		/* unicast address incl. temp addr */
		list_for_each_entry(ifa, &idev->addr_list, if_list) {
			if (++ip_idx < s_ip_idx)
				continue;
			err = inet6_fill_ifaddr(skb, ifa,
						NETLINK_CB(cb->skb).pid,
						cb->nlh->nlmsg_seq,
						RTM_NEWADDR,
						NLM_F_MULTI);
			if (err <= 0)
				break;
=======
	nlmsg_end(skb, nlh);
	return 0;
}

/* called with rcu_read_lock() */
static int in6_dump_addrs(const struct inet6_dev *idev, struct sk_buff *skb,
			  struct netlink_callback *cb, int *s_ip_idx,
			  struct inet6_fill_args *fillargs)
{
	const struct ifmcaddr6 *ifmca;
	const struct ifacaddr6 *ifaca;
	int ip_idx = 0;
	int err = 0;

	switch (fillargs->type) {
	case UNICAST_ADDR: {
		const struct inet6_ifaddr *ifa;
		fillargs->event = RTM_NEWADDR;

		/* unicast address incl. temp addr */
		list_for_each_entry_rcu(ifa, &idev->addr_list, if_list) {
			if (ip_idx < *s_ip_idx)
				goto next;
			err = inet6_fill_ifaddr(skb, ifa, fillargs);
			if (err < 0)
				break;
			nl_dump_check_consistent(cb, nlmsg_hdr(skb));
next:
			ip_idx++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		break;
	}
	case MULTICAST_ADDR:
<<<<<<< HEAD
		/* multicast address */
		for (ifmca = idev->mc_list; ifmca;
		     ifmca = ifmca->next, ip_idx++) {
			if (ip_idx < s_ip_idx)
				continue;
			err = inet6_fill_ifmcaddr(skb, ifmca,
						  NETLINK_CB(cb->skb).pid,
						  cb->nlh->nlmsg_seq,
						  RTM_GETMULTICAST,
						  NLM_F_MULTI);
			if (err <= 0)
=======
		fillargs->event = RTM_GETMULTICAST;

		/* multicast address */
		for (ifmca = rcu_dereference(idev->mc_list);
		     ifmca;
		     ifmca = rcu_dereference(ifmca->next), ip_idx++) {
			if (ip_idx < *s_ip_idx)
				continue;
			err = inet6_fill_ifmcaddr(skb, ifmca, fillargs);
			if (err < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;
		}
		break;
	case ANYCAST_ADDR:
<<<<<<< HEAD
		/* anycast address */
		for (ifaca = idev->ac_list; ifaca;
		     ifaca = ifaca->aca_next, ip_idx++) {
			if (ip_idx < s_ip_idx)
				continue;
			err = inet6_fill_ifacaddr(skb, ifaca,
						  NETLINK_CB(cb->skb).pid,
						  cb->nlh->nlmsg_seq,
						  RTM_GETANYCAST,
						  NLM_F_MULTI);
			if (err <= 0)
=======
		fillargs->event = RTM_GETANYCAST;
		/* anycast address */
		for (ifaca = rcu_dereference(idev->ac_list); ifaca;
		     ifaca = rcu_dereference(ifaca->aca_next), ip_idx++) {
			if (ip_idx < *s_ip_idx)
				continue;
			err = inet6_fill_ifacaddr(skb, ifaca, fillargs);
			if (err < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;
		}
		break;
	default:
		break;
	}
<<<<<<< HEAD
	read_unlock_bh(&idev->lock);
	*p_ip_idx = ip_idx;
	return err;
}

static int inet6_dump_addr(struct sk_buff *skb, struct netlink_callback *cb,
			   enum addr_type_t type)
{
	struct net *net = sock_net(skb->sk);
	int h, s_h;
	int idx, ip_idx;
	int s_idx, s_ip_idx;
	struct net_device *dev;
	struct inet6_dev *idev;
	struct hlist_head *head;
	struct hlist_node *node;

	s_h = cb->args[0];
	s_idx = idx = cb->args[1];
	s_ip_idx = ip_idx = cb->args[2];

	rcu_read_lock();
	for (h = s_h; h < NETDEV_HASHENTRIES; h++, s_idx = 0) {
		idx = 0;
		head = &net->dev_index_head[h];
		hlist_for_each_entry_rcu(dev, node, head, index_hlist) {
			if (idx < s_idx)
				goto cont;
			if (h > s_h || idx > s_idx)
				s_ip_idx = 0;
			ip_idx = 0;
			idev = __in6_dev_get(dev);
			if (!idev)
				goto cont;

			if (in6_dump_addrs(idev, skb, cb, type,
					   s_ip_idx, &ip_idx) <= 0)
				goto done;
cont:
			idx++;
		}
	}
done:
	rcu_read_unlock();
	cb->args[0] = h;
	cb->args[1] = idx;
	cb->args[2] = ip_idx;

	return skb->len;
=======
	*s_ip_idx = err ? ip_idx : 0;
	return err;
}

static int inet6_valid_dump_ifaddr_req(const struct nlmsghdr *nlh,
				       struct inet6_fill_args *fillargs,
				       struct net **tgt_net, struct sock *sk,
				       struct netlink_callback *cb)
{
	struct netlink_ext_ack *extack = cb->extack;
	struct nlattr *tb[IFA_MAX+1];
	struct ifaddrmsg *ifm;
	int err, i;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*ifm))) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid header for address dump request");
		return -EINVAL;
	}

	ifm = nlmsg_data(nlh);
	if (ifm->ifa_prefixlen || ifm->ifa_flags || ifm->ifa_scope) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid values in header for address dump request");
		return -EINVAL;
	}

	fillargs->ifindex = ifm->ifa_index;
	if (fillargs->ifindex) {
		cb->answer_flags |= NLM_F_DUMP_FILTERED;
		fillargs->flags |= NLM_F_DUMP_FILTERED;
	}

	err = nlmsg_parse_deprecated_strict(nlh, sizeof(*ifm), tb, IFA_MAX,
					    ifa_ipv6_policy, extack);
	if (err < 0)
		return err;

	for (i = 0; i <= IFA_MAX; ++i) {
		if (!tb[i])
			continue;

		if (i == IFA_TARGET_NETNSID) {
			struct net *net;

			fillargs->netnsid = nla_get_s32(tb[i]);
			net = rtnl_get_net_ns_capable(sk, fillargs->netnsid);
			if (IS_ERR(net)) {
				fillargs->netnsid = -1;
				NL_SET_ERR_MSG_MOD(extack, "Invalid target network namespace id");
				return PTR_ERR(net);
			}
			*tgt_net = net;
		} else {
			NL_SET_ERR_MSG_MOD(extack, "Unsupported attribute in dump request");
			return -EINVAL;
		}
	}

	return 0;
}

static int inet6_dump_addr(struct sk_buff *skb, struct netlink_callback *cb,
			   enum addr_type_t type)
{
	struct net *tgt_net = sock_net(skb->sk);
	const struct nlmsghdr *nlh = cb->nlh;
	struct inet6_fill_args fillargs = {
		.portid = NETLINK_CB(cb->skb).portid,
		.seq = cb->nlh->nlmsg_seq,
		.flags = NLM_F_MULTI,
		.netnsid = -1,
		.type = type,
	};
	struct {
		unsigned long ifindex;
		int ip_idx;
	} *ctx = (void *)cb->ctx;
	struct net_device *dev;
	struct inet6_dev *idev;
	int err = 0;

	rcu_read_lock();
	if (cb->strict_check) {
		err = inet6_valid_dump_ifaddr_req(nlh, &fillargs, &tgt_net,
						  skb->sk, cb);
		if (err < 0)
			goto done;

		err = 0;
		if (fillargs.ifindex) {
			dev = dev_get_by_index_rcu(tgt_net, fillargs.ifindex);
			if (!dev) {
				err = -ENODEV;
				goto done;
			}
			idev = __in6_dev_get(dev);
			if (idev)
				err = in6_dump_addrs(idev, skb, cb,
						     &ctx->ip_idx,
						     &fillargs);
			goto done;
		}
	}

	cb->seq = inet6_base_seq(tgt_net);
	for_each_netdev_dump(tgt_net, dev, ctx->ifindex) {
		idev = __in6_dev_get(dev);
		if (!idev)
			continue;
		err = in6_dump_addrs(idev, skb, cb, &ctx->ip_idx,
				     &fillargs);
		if (err < 0)
			goto done;
	}
done:
	rcu_read_unlock();
	if (fillargs.netnsid >= 0)
		put_net(tgt_net);

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int inet6_dump_ifaddr(struct sk_buff *skb, struct netlink_callback *cb)
{
	enum addr_type_t type = UNICAST_ADDR;

	return inet6_dump_addr(skb, cb, type);
}

static int inet6_dump_ifmcaddr(struct sk_buff *skb, struct netlink_callback *cb)
{
	enum addr_type_t type = MULTICAST_ADDR;

	return inet6_dump_addr(skb, cb, type);
}


static int inet6_dump_ifacaddr(struct sk_buff *skb, struct netlink_callback *cb)
{
	enum addr_type_t type = ANYCAST_ADDR;

	return inet6_dump_addr(skb, cb, type);
}

<<<<<<< HEAD
static int inet6_rtm_getaddr(struct sk_buff *in_skb, struct nlmsghdr* nlh,
			     void *arg)
{
	struct net *net = sock_net(in_skb->sk);
	struct ifaddrmsg *ifm;
	struct nlattr *tb[IFA_MAX+1];
	struct in6_addr *addr = NULL;
=======
static int inet6_rtm_valid_getaddr_req(struct sk_buff *skb,
				       const struct nlmsghdr *nlh,
				       struct nlattr **tb,
				       struct netlink_ext_ack *extack)
{
	struct ifaddrmsg *ifm;
	int i, err;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*ifm))) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid header for get address request");
		return -EINVAL;
	}

	if (!netlink_strict_get_check(skb))
		return nlmsg_parse_deprecated(nlh, sizeof(*ifm), tb, IFA_MAX,
					      ifa_ipv6_policy, extack);

	ifm = nlmsg_data(nlh);
	if (ifm->ifa_prefixlen || ifm->ifa_flags || ifm->ifa_scope) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid values in header for get address request");
		return -EINVAL;
	}

	err = nlmsg_parse_deprecated_strict(nlh, sizeof(*ifm), tb, IFA_MAX,
					    ifa_ipv6_policy, extack);
	if (err)
		return err;

	for (i = 0; i <= IFA_MAX; i++) {
		if (!tb[i])
			continue;

		switch (i) {
		case IFA_TARGET_NETNSID:
		case IFA_ADDRESS:
		case IFA_LOCAL:
			break;
		default:
			NL_SET_ERR_MSG_MOD(extack, "Unsupported attribute in get address request");
			return -EINVAL;
		}
	}

	return 0;
}

static int inet6_rtm_getaddr(struct sk_buff *in_skb, struct nlmsghdr *nlh,
			     struct netlink_ext_ack *extack)
{
	struct net *tgt_net = sock_net(in_skb->sk);
	struct inet6_fill_args fillargs = {
		.portid = NETLINK_CB(in_skb).portid,
		.seq = nlh->nlmsg_seq,
		.event = RTM_NEWADDR,
		.flags = 0,
		.netnsid = -1,
	};
	struct ifaddrmsg *ifm;
	struct nlattr *tb[IFA_MAX+1];
	struct in6_addr *addr = NULL, *peer;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct net_device *dev = NULL;
	struct inet6_ifaddr *ifa;
	struct sk_buff *skb;
	int err;

<<<<<<< HEAD
	err = nlmsg_parse(nlh, sizeof(*ifm), tb, IFA_MAX, ifa_ipv6_policy);
	if (err < 0)
		goto errout;

	addr = extract_addr(tb[IFA_ADDRESS], tb[IFA_LOCAL]);
	if (addr == NULL) {
		err = -EINVAL;
		goto errout;
	}

	ifm = nlmsg_data(nlh);
	if (ifm->ifa_index)
		dev = __dev_get_by_index(net, ifm->ifa_index);

	ifa = ipv6_get_ifaddr(net, addr, dev, 1);
=======
	err = inet6_rtm_valid_getaddr_req(in_skb, nlh, tb, extack);
	if (err < 0)
		return err;

	if (tb[IFA_TARGET_NETNSID]) {
		fillargs.netnsid = nla_get_s32(tb[IFA_TARGET_NETNSID]);

		tgt_net = rtnl_get_net_ns_capable(NETLINK_CB(in_skb).sk,
						  fillargs.netnsid);
		if (IS_ERR(tgt_net))
			return PTR_ERR(tgt_net);
	}

	addr = extract_addr(tb[IFA_ADDRESS], tb[IFA_LOCAL], &peer);
	if (!addr) {
		err = -EINVAL;
		goto errout;
	}
	ifm = nlmsg_data(nlh);
	if (ifm->ifa_index)
		dev = dev_get_by_index(tgt_net, ifm->ifa_index);

	ifa = ipv6_get_ifaddr(tgt_net, addr, dev, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ifa) {
		err = -EADDRNOTAVAIL;
		goto errout;
	}

	skb = nlmsg_new(inet6_ifaddr_msgsize(), GFP_KERNEL);
	if (!skb) {
		err = -ENOBUFS;
		goto errout_ifa;
	}

<<<<<<< HEAD
	err = inet6_fill_ifaddr(skb, ifa, NETLINK_CB(in_skb).pid,
				nlh->nlmsg_seq, RTM_NEWADDR, 0);
=======
	err = inet6_fill_ifaddr(skb, ifa, &fillargs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0) {
		/* -EMSGSIZE implies BUG in inet6_ifaddr_msgsize() */
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(skb);
		goto errout_ifa;
	}
<<<<<<< HEAD
	err = rtnl_unicast(skb, net, NETLINK_CB(in_skb).pid);
errout_ifa:
	in6_ifa_put(ifa);
errout:
=======
	err = rtnl_unicast(skb, tgt_net, NETLINK_CB(in_skb).portid);
errout_ifa:
	in6_ifa_put(ifa);
errout:
	dev_put(dev);
	if (fillargs.netnsid >= 0)
		put_net(tgt_net);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static void inet6_ifa_notify(int event, struct inet6_ifaddr *ifa)
{
	struct sk_buff *skb;
	struct net *net = dev_net(ifa->idev->dev);
<<<<<<< HEAD
	int err = -ENOBUFS;

	skb = nlmsg_new(inet6_ifaddr_msgsize(), GFP_ATOMIC);
	if (skb == NULL)
		goto errout;

	err = inet6_fill_ifaddr(skb, ifa, 0, 0, event, 0);
=======
	struct inet6_fill_args fillargs = {
		.portid = 0,
		.seq = 0,
		.event = event,
		.flags = 0,
		.netnsid = -1,
	};
	int err = -ENOBUFS;

	skb = nlmsg_new(inet6_ifaddr_msgsize(), GFP_ATOMIC);
	if (!skb)
		goto errout;

	err = inet6_fill_ifaddr(skb, ifa, &fillargs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err < 0) {
		/* -EMSGSIZE implies BUG in inet6_ifaddr_msgsize() */
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(skb);
		goto errout;
	}
	rtnl_notify(skb, net, 0, RTNLGRP_IPV6_IFADDR, NULL, GFP_ATOMIC);
	return;
errout:
	if (err < 0)
		rtnl_set_sk_err(net, RTNLGRP_IPV6_IFADDR, err);
}

<<<<<<< HEAD
static inline void ipv6_store_devconf(struct ipv6_devconf *cnf,
				__s32 *array, int bytes)
=======
static void ipv6_store_devconf(const struct ipv6_devconf *cnf,
			       __s32 *array, int bytes)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	BUG_ON(bytes < (DEVCONF_MAX * 4));

	memset(array, 0, bytes);
<<<<<<< HEAD
	array[DEVCONF_FORWARDING] = cnf->forwarding;
	array[DEVCONF_HOPLIMIT] = cnf->hop_limit;
	array[DEVCONF_MTU6] = cnf->mtu6;
	array[DEVCONF_ACCEPT_RA] = cnf->accept_ra;
	array[DEVCONF_ACCEPT_REDIRECTS] = cnf->accept_redirects;
	array[DEVCONF_AUTOCONF] = cnf->autoconf;
	array[DEVCONF_DAD_TRANSMITS] = cnf->dad_transmits;
	array[DEVCONF_RTR_SOLICITS] = cnf->rtr_solicits;
	array[DEVCONF_RTR_SOLICIT_INTERVAL] =
		jiffies_to_msecs(cnf->rtr_solicit_interval);
	array[DEVCONF_RTR_SOLICIT_DELAY] =
		jiffies_to_msecs(cnf->rtr_solicit_delay);
	array[DEVCONF_FORCE_MLD_VERSION] = cnf->force_mld_version;
#ifdef CONFIG_IPV6_PRIVACY
	array[DEVCONF_USE_TEMPADDR] = cnf->use_tempaddr;
	array[DEVCONF_TEMP_VALID_LFT] = cnf->temp_valid_lft;
	array[DEVCONF_TEMP_PREFERED_LFT] = cnf->temp_prefered_lft;
	array[DEVCONF_REGEN_MAX_RETRY] = cnf->regen_max_retry;
	array[DEVCONF_MAX_DESYNC_FACTOR] = cnf->max_desync_factor;
#endif
	array[DEVCONF_MAX_ADDRESSES] = cnf->max_addresses;
	array[DEVCONF_ACCEPT_RA_DEFRTR] = cnf->accept_ra_defrtr;
	array[DEVCONF_ACCEPT_RA_PINFO] = cnf->accept_ra_pinfo;
#ifdef CONFIG_IPV6_ROUTER_PREF
	array[DEVCONF_ACCEPT_RA_RTR_PREF] = cnf->accept_ra_rtr_pref;
	array[DEVCONF_RTR_PROBE_INTERVAL] =
		jiffies_to_msecs(cnf->rtr_probe_interval);
#ifdef CONFIG_IPV6_ROUTE_INFO
	array[DEVCONF_ACCEPT_RA_RT_INFO_MAX_PLEN] = cnf->accept_ra_rt_info_max_plen;
#endif
#endif
	array[DEVCONF_ACCEPT_RA_RT_TABLE] = cnf->accept_ra_rt_table;
	array[DEVCONF_PROXY_NDP] = cnf->proxy_ndp;
	array[DEVCONF_ACCEPT_SOURCE_ROUTE] = cnf->accept_source_route;
#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	array[DEVCONF_OPTIMISTIC_DAD] = cnf->optimistic_dad;
	array[DEVCONF_USE_OPTIMISTIC] = cnf->use_optimistic;
#endif
#ifdef CONFIG_IPV6_MROUTE
	array[DEVCONF_MC_FORWARDING] = cnf->mc_forwarding;
#endif
	array[DEVCONF_DISABLE_IPV6] = cnf->disable_ipv6;
	array[DEVCONF_ACCEPT_DAD] = cnf->accept_dad;
	array[DEVCONF_FORCE_TLLAO] = cnf->force_tllao;
#ifdef CONFIG_LGE_DHCPV6_WIFI
	array[DEVCONF_RA_INFO_FLAG] = cnf->ra_info_flag;
#endif	
	array[DEVCONF_ACCEPT_RA_MTU] = cnf->accept_ra_mtu;
=======
	array[DEVCONF_FORWARDING] = READ_ONCE(cnf->forwarding);
	array[DEVCONF_HOPLIMIT] = READ_ONCE(cnf->hop_limit);
	array[DEVCONF_MTU6] = READ_ONCE(cnf->mtu6);
	array[DEVCONF_ACCEPT_RA] = READ_ONCE(cnf->accept_ra);
	array[DEVCONF_ACCEPT_REDIRECTS] = READ_ONCE(cnf->accept_redirects);
	array[DEVCONF_AUTOCONF] = READ_ONCE(cnf->autoconf);
	array[DEVCONF_DAD_TRANSMITS] = READ_ONCE(cnf->dad_transmits);
	array[DEVCONF_RTR_SOLICITS] = READ_ONCE(cnf->rtr_solicits);
	array[DEVCONF_RTR_SOLICIT_INTERVAL] =
		jiffies_to_msecs(READ_ONCE(cnf->rtr_solicit_interval));
	array[DEVCONF_RTR_SOLICIT_MAX_INTERVAL] =
		jiffies_to_msecs(READ_ONCE(cnf->rtr_solicit_max_interval));
	array[DEVCONF_RTR_SOLICIT_DELAY] =
		jiffies_to_msecs(READ_ONCE(cnf->rtr_solicit_delay));
	array[DEVCONF_FORCE_MLD_VERSION] = READ_ONCE(cnf->force_mld_version);
	array[DEVCONF_MLDV1_UNSOLICITED_REPORT_INTERVAL] =
		jiffies_to_msecs(READ_ONCE(cnf->mldv1_unsolicited_report_interval));
	array[DEVCONF_MLDV2_UNSOLICITED_REPORT_INTERVAL] =
		jiffies_to_msecs(READ_ONCE(cnf->mldv2_unsolicited_report_interval));
	array[DEVCONF_USE_TEMPADDR] = READ_ONCE(cnf->use_tempaddr);
	array[DEVCONF_TEMP_VALID_LFT] = READ_ONCE(cnf->temp_valid_lft);
	array[DEVCONF_TEMP_PREFERED_LFT] = READ_ONCE(cnf->temp_prefered_lft);
	array[DEVCONF_REGEN_MAX_RETRY] = READ_ONCE(cnf->regen_max_retry);
	array[DEVCONF_MAX_DESYNC_FACTOR] = READ_ONCE(cnf->max_desync_factor);
	array[DEVCONF_MAX_ADDRESSES] = READ_ONCE(cnf->max_addresses);
	array[DEVCONF_ACCEPT_RA_DEFRTR] = READ_ONCE(cnf->accept_ra_defrtr);
	array[DEVCONF_RA_DEFRTR_METRIC] = READ_ONCE(cnf->ra_defrtr_metric);
	array[DEVCONF_ACCEPT_RA_MIN_HOP_LIMIT] =
		READ_ONCE(cnf->accept_ra_min_hop_limit);
	array[DEVCONF_ACCEPT_RA_PINFO] = READ_ONCE(cnf->accept_ra_pinfo);
#ifdef CONFIG_IPV6_ROUTER_PREF
	array[DEVCONF_ACCEPT_RA_RTR_PREF] = READ_ONCE(cnf->accept_ra_rtr_pref);
	array[DEVCONF_RTR_PROBE_INTERVAL] =
		jiffies_to_msecs(READ_ONCE(cnf->rtr_probe_interval));
#ifdef CONFIG_IPV6_ROUTE_INFO
	array[DEVCONF_ACCEPT_RA_RT_INFO_MIN_PLEN] =
		READ_ONCE(cnf->accept_ra_rt_info_min_plen);
	array[DEVCONF_ACCEPT_RA_RT_INFO_MAX_PLEN] =
		READ_ONCE(cnf->accept_ra_rt_info_max_plen);
#endif
#endif
	array[DEVCONF_PROXY_NDP] = READ_ONCE(cnf->proxy_ndp);
	array[DEVCONF_ACCEPT_SOURCE_ROUTE] =
		READ_ONCE(cnf->accept_source_route);
#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	array[DEVCONF_OPTIMISTIC_DAD] = READ_ONCE(cnf->optimistic_dad);
	array[DEVCONF_USE_OPTIMISTIC] = READ_ONCE(cnf->use_optimistic);
#endif
#ifdef CONFIG_IPV6_MROUTE
	array[DEVCONF_MC_FORWARDING] = atomic_read(&cnf->mc_forwarding);
#endif
	array[DEVCONF_DISABLE_IPV6] = READ_ONCE(cnf->disable_ipv6);
	array[DEVCONF_ACCEPT_DAD] = READ_ONCE(cnf->accept_dad);
	array[DEVCONF_FORCE_TLLAO] = READ_ONCE(cnf->force_tllao);
	array[DEVCONF_NDISC_NOTIFY] = READ_ONCE(cnf->ndisc_notify);
	array[DEVCONF_SUPPRESS_FRAG_NDISC] =
		READ_ONCE(cnf->suppress_frag_ndisc);
	array[DEVCONF_ACCEPT_RA_FROM_LOCAL] =
		READ_ONCE(cnf->accept_ra_from_local);
	array[DEVCONF_ACCEPT_RA_MTU] = READ_ONCE(cnf->accept_ra_mtu);
	array[DEVCONF_IGNORE_ROUTES_WITH_LINKDOWN] =
		READ_ONCE(cnf->ignore_routes_with_linkdown);
	/* we omit DEVCONF_STABLE_SECRET for now */
	array[DEVCONF_USE_OIF_ADDRS_ONLY] = READ_ONCE(cnf->use_oif_addrs_only);
	array[DEVCONF_DROP_UNICAST_IN_L2_MULTICAST] =
		READ_ONCE(cnf->drop_unicast_in_l2_multicast);
	array[DEVCONF_DROP_UNSOLICITED_NA] = READ_ONCE(cnf->drop_unsolicited_na);
	array[DEVCONF_KEEP_ADDR_ON_DOWN] = READ_ONCE(cnf->keep_addr_on_down);
	array[DEVCONF_SEG6_ENABLED] = READ_ONCE(cnf->seg6_enabled);
#ifdef CONFIG_IPV6_SEG6_HMAC
	array[DEVCONF_SEG6_REQUIRE_HMAC] = READ_ONCE(cnf->seg6_require_hmac);
#endif
	array[DEVCONF_ENHANCED_DAD] = READ_ONCE(cnf->enhanced_dad);
	array[DEVCONF_ADDR_GEN_MODE] = READ_ONCE(cnf->addr_gen_mode);
	array[DEVCONF_DISABLE_POLICY] = READ_ONCE(cnf->disable_policy);
	array[DEVCONF_NDISC_TCLASS] = READ_ONCE(cnf->ndisc_tclass);
	array[DEVCONF_RPL_SEG_ENABLED] = READ_ONCE(cnf->rpl_seg_enabled);
	array[DEVCONF_IOAM6_ENABLED] = READ_ONCE(cnf->ioam6_enabled);
	array[DEVCONF_IOAM6_ID] = READ_ONCE(cnf->ioam6_id);
	array[DEVCONF_IOAM6_ID_WIDE] = READ_ONCE(cnf->ioam6_id_wide);
	array[DEVCONF_NDISC_EVICT_NOCARRIER] =
		READ_ONCE(cnf->ndisc_evict_nocarrier);
	array[DEVCONF_ACCEPT_UNTRACKED_NA] =
		READ_ONCE(cnf->accept_untracked_na);
	array[DEVCONF_ACCEPT_RA_MIN_LFT] = READ_ONCE(cnf->accept_ra_min_lft);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline size_t inet6_ifla6_size(void)
{
	return nla_total_size(4) /* IFLA_INET6_FLAGS */
	     + nla_total_size(sizeof(struct ifla_cacheinfo))
	     + nla_total_size(DEVCONF_MAX * 4) /* IFLA_INET6_CONF */
	     + nla_total_size(IPSTATS_MIB_MAX * 8) /* IFLA_INET6_STATS */
<<<<<<< HEAD
	     + nla_total_size(ICMP6_MIB_MAX * 8); /* IFLA_INET6_ICMP6STATS */
=======
	     + nla_total_size(ICMP6_MIB_MAX * 8) /* IFLA_INET6_ICMP6STATS */
	     + nla_total_size(sizeof(struct in6_addr)) /* IFLA_INET6_TOKEN */
	     + nla_total_size(1) /* IFLA_INET6_ADDR_GEN_MODE */
	     + nla_total_size(4) /* IFLA_INET6_RA_MTU */
	     + 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline size_t inet6_if_nlmsg_size(void)
{
	return NLMSG_ALIGN(sizeof(struct ifinfomsg))
	       + nla_total_size(IFNAMSIZ) /* IFLA_IFNAME */
	       + nla_total_size(MAX_ADDR_LEN) /* IFLA_ADDRESS */
	       + nla_total_size(4) /* IFLA_MTU */
	       + nla_total_size(4) /* IFLA_LINK */
<<<<<<< HEAD
=======
	       + nla_total_size(1) /* IFLA_OPERSTATE */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       + nla_total_size(inet6_ifla6_size()); /* IFLA_PROTINFO */
}

static inline void __snmp6_fill_statsdev(u64 *stats, atomic_long_t *mib,
<<<<<<< HEAD
				      int items, int bytes)
{
	int i;
	int pad = bytes - sizeof(u64) * items;
	BUG_ON(pad < 0);

	/* Use put_unaligned() because stats may not be aligned for u64. */
	put_unaligned(items, &stats[0]);
	for (i = 1; i < items; i++)
		put_unaligned(atomic_long_read(&mib[i]), &stats[i]);

	memset(&stats[items], 0, pad);
}

static inline void __snmp6_fill_stats64(u64 *stats, void __percpu **mib,
				      int items, int bytes, size_t syncpoff)
{
	int i;
	int pad = bytes - sizeof(u64) * items;
	BUG_ON(pad < 0);

	/* Use put_unaligned() because stats may not be aligned for u64. */
	put_unaligned(items, &stats[0]);
	for (i = 1; i < items; i++)
		put_unaligned(snmp_fold_field64(mib, i, syncpoff), &stats[i]);

	memset(&stats[items], 0, pad);
=======
					int bytes)
{
	int i;
	int pad = bytes - sizeof(u64) * ICMP6_MIB_MAX;
	BUG_ON(pad < 0);

	/* Use put_unaligned() because stats may not be aligned for u64. */
	put_unaligned(ICMP6_MIB_MAX, &stats[0]);
	for (i = 1; i < ICMP6_MIB_MAX; i++)
		put_unaligned(atomic_long_read(&mib[i]), &stats[i]);

	memset(&stats[ICMP6_MIB_MAX], 0, pad);
}

static inline void __snmp6_fill_stats64(u64 *stats, void __percpu *mib,
					int bytes, size_t syncpoff)
{
	int i, c;
	u64 buff[IPSTATS_MIB_MAX];
	int pad = bytes - sizeof(u64) * IPSTATS_MIB_MAX;

	BUG_ON(pad < 0);

	memset(buff, 0, sizeof(buff));
	buff[0] = IPSTATS_MIB_MAX;

	for_each_possible_cpu(c) {
		for (i = 1; i < IPSTATS_MIB_MAX; i++)
			buff[i] += snmp_get_cpu_field64(mib, c, i, syncpoff);
	}

	memcpy(stats, buff, IPSTATS_MIB_MAX * sizeof(u64));
	memset(&stats[IPSTATS_MIB_MAX], 0, pad);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void snmp6_fill_stats(u64 *stats, struct inet6_dev *idev, int attrtype,
			     int bytes)
{
	switch (attrtype) {
	case IFLA_INET6_STATS:
<<<<<<< HEAD
		__snmp6_fill_stats64(stats, (void __percpu **)idev->stats.ipv6,
				     IPSTATS_MIB_MAX, bytes, offsetof(struct ipstats_mib, syncp));
		break;
	case IFLA_INET6_ICMP6STATS:
		__snmp6_fill_statsdev(stats, idev->stats.icmpv6dev->mibs, ICMP6_MIB_MAX, bytes);
=======
		__snmp6_fill_stats64(stats, idev->stats.ipv6, bytes,
				     offsetof(struct ipstats_mib, syncp));
		break;
	case IFLA_INET6_ICMP6STATS:
		__snmp6_fill_statsdev(stats, idev->stats.icmpv6dev->mibs, bytes);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
}

<<<<<<< HEAD
static int inet6_fill_ifla6_attrs(struct sk_buff *skb, struct inet6_dev *idev)
{
	struct nlattr *nla;
	struct ifla_cacheinfo ci;

	NLA_PUT_U32(skb, IFLA_INET6_FLAGS, idev->if_flags);

	ci.max_reasm_len = IPV6_MAXPLEN;
	ci.tstamp = cstamp_delta(idev->tstamp);
	ci.reachable_time = jiffies_to_msecs(idev->nd_parms->reachable_time);
	ci.retrans_time = jiffies_to_msecs(idev->nd_parms->retrans_time);
	NLA_PUT(skb, IFLA_INET6_CACHEINFO, sizeof(ci), &ci);

	nla = nla_reserve(skb, IFLA_INET6_CONF, DEVCONF_MAX * sizeof(s32));
	if (nla == NULL)
=======
static int inet6_fill_ifla6_attrs(struct sk_buff *skb, struct inet6_dev *idev,
				  u32 ext_filter_mask)
{
	struct ifla_cacheinfo ci;
	struct nlattr *nla;
	u32 ra_mtu;

	if (nla_put_u32(skb, IFLA_INET6_FLAGS, READ_ONCE(idev->if_flags)))
		goto nla_put_failure;
	ci.max_reasm_len = IPV6_MAXPLEN;
	ci.tstamp = cstamp_delta(READ_ONCE(idev->tstamp));
	ci.reachable_time = jiffies_to_msecs(idev->nd_parms->reachable_time);
	ci.retrans_time = jiffies_to_msecs(NEIGH_VAR(idev->nd_parms, RETRANS_TIME));
	if (nla_put(skb, IFLA_INET6_CACHEINFO, sizeof(ci), &ci))
		goto nla_put_failure;
	nla = nla_reserve(skb, IFLA_INET6_CONF, DEVCONF_MAX * sizeof(s32));
	if (!nla)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto nla_put_failure;
	ipv6_store_devconf(&idev->cnf, nla_data(nla), nla_len(nla));

	/* XXX - MC not implemented */

<<<<<<< HEAD
	nla = nla_reserve(skb, IFLA_INET6_STATS, IPSTATS_MIB_MAX * sizeof(u64));
	if (nla == NULL)
=======
	if (ext_filter_mask & RTEXT_FILTER_SKIP_STATS)
		return 0;

	nla = nla_reserve(skb, IFLA_INET6_STATS, IPSTATS_MIB_MAX * sizeof(u64));
	if (!nla)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto nla_put_failure;
	snmp6_fill_stats(nla_data(nla), idev, IFLA_INET6_STATS, nla_len(nla));

	nla = nla_reserve(skb, IFLA_INET6_ICMP6STATS, ICMP6_MIB_MAX * sizeof(u64));
<<<<<<< HEAD
	if (nla == NULL)
		goto nla_put_failure;
	snmp6_fill_stats(nla_data(nla), idev, IFLA_INET6_ICMP6STATS, nla_len(nla));

=======
	if (!nla)
		goto nla_put_failure;
	snmp6_fill_stats(nla_data(nla), idev, IFLA_INET6_ICMP6STATS, nla_len(nla));

	nla = nla_reserve(skb, IFLA_INET6_TOKEN, sizeof(struct in6_addr));
	if (!nla)
		goto nla_put_failure;
	read_lock_bh(&idev->lock);
	memcpy(nla_data(nla), idev->token.s6_addr, nla_len(nla));
	read_unlock_bh(&idev->lock);

	if (nla_put_u8(skb, IFLA_INET6_ADDR_GEN_MODE,
		       READ_ONCE(idev->cnf.addr_gen_mode)))
		goto nla_put_failure;

	ra_mtu = READ_ONCE(idev->ra_mtu);
	if (ra_mtu && nla_put_u32(skb, IFLA_INET6_RA_MTU, ra_mtu))
		goto nla_put_failure;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

nla_put_failure:
	return -EMSGSIZE;
}

<<<<<<< HEAD
static size_t inet6_get_link_af_size(const struct net_device *dev)
=======
static size_t inet6_get_link_af_size(const struct net_device *dev,
				     u32 ext_filter_mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (!__in6_dev_get(dev))
		return 0;

	return inet6_ifla6_size();
}

<<<<<<< HEAD
static int inet6_fill_link_af(struct sk_buff *skb, const struct net_device *dev)
=======
static int inet6_fill_link_af(struct sk_buff *skb, const struct net_device *dev,
			      u32 ext_filter_mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inet6_dev *idev = __in6_dev_get(dev);

	if (!idev)
		return -ENODATA;

<<<<<<< HEAD
	if (inet6_fill_ifla6_attrs(skb, idev) < 0)
=======
	if (inet6_fill_ifla6_attrs(skb, idev, ext_filter_mask) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EMSGSIZE;

	return 0;
}

<<<<<<< HEAD
static int inet6_fill_ifinfo(struct sk_buff *skb, struct inet6_dev *idev,
			     u32 pid, u32 seq, int event, unsigned int flags)
=======
static int inet6_set_iftoken(struct inet6_dev *idev, struct in6_addr *token,
			     struct netlink_ext_ack *extack)
{
	struct inet6_ifaddr *ifp;
	struct net_device *dev = idev->dev;
	bool clear_token, update_rs = false;
	struct in6_addr ll_addr;

	ASSERT_RTNL();

	if (!token)
		return -EINVAL;

	if (dev->flags & IFF_LOOPBACK) {
		NL_SET_ERR_MSG_MOD(extack, "Device is loopback");
		return -EINVAL;
	}

	if (dev->flags & IFF_NOARP) {
		NL_SET_ERR_MSG_MOD(extack,
				   "Device does not do neighbour discovery");
		return -EINVAL;
	}

	if (!ipv6_accept_ra(idev)) {
		NL_SET_ERR_MSG_MOD(extack,
				   "Router advertisement is disabled on device");
		return -EINVAL;
	}

	if (READ_ONCE(idev->cnf.rtr_solicits) == 0) {
		NL_SET_ERR_MSG(extack,
			       "Router solicitation is disabled on device");
		return -EINVAL;
	}

	write_lock_bh(&idev->lock);

	BUILD_BUG_ON(sizeof(token->s6_addr) != 16);
	memcpy(idev->token.s6_addr + 8, token->s6_addr + 8, 8);

	write_unlock_bh(&idev->lock);

	clear_token = ipv6_addr_any(token);
	if (clear_token)
		goto update_lft;

	if (!idev->dead && (idev->if_flags & IF_READY) &&
	    !ipv6_get_lladdr(dev, &ll_addr, IFA_F_TENTATIVE |
			     IFA_F_OPTIMISTIC)) {
		/* If we're not ready, then normal ifup will take care
		 * of this. Otherwise, we need to request our rs here.
		 */
		ndisc_send_rs(dev, &ll_addr, &in6addr_linklocal_allrouters);
		update_rs = true;
	}

update_lft:
	write_lock_bh(&idev->lock);

	if (update_rs) {
		idev->if_flags |= IF_RS_SENT;
		idev->rs_interval = rfc3315_s14_backoff_init(
			READ_ONCE(idev->cnf.rtr_solicit_interval));
		idev->rs_probes = 1;
		addrconf_mod_rs_timer(idev, idev->rs_interval);
	}

	/* Well, that's kinda nasty ... */
	list_for_each_entry(ifp, &idev->addr_list, if_list) {
		spin_lock(&ifp->lock);
		if (ifp->tokenized) {
			ifp->valid_lft = 0;
			ifp->prefered_lft = 0;
		}
		spin_unlock(&ifp->lock);
	}

	write_unlock_bh(&idev->lock);
	inet6_ifinfo_notify(RTM_NEWLINK, idev);
	addrconf_verify_rtnl(dev_net(dev));
	return 0;
}

static const struct nla_policy inet6_af_policy[IFLA_INET6_MAX + 1] = {
	[IFLA_INET6_ADDR_GEN_MODE]	= { .type = NLA_U8 },
	[IFLA_INET6_TOKEN]		= { .len = sizeof(struct in6_addr) },
	[IFLA_INET6_RA_MTU]		= { .type = NLA_REJECT,
					    .reject_message =
						"IFLA_INET6_RA_MTU can not be set" },
};

static int check_addr_gen_mode(int mode)
{
	if (mode != IN6_ADDR_GEN_MODE_EUI64 &&
	    mode != IN6_ADDR_GEN_MODE_NONE &&
	    mode != IN6_ADDR_GEN_MODE_STABLE_PRIVACY &&
	    mode != IN6_ADDR_GEN_MODE_RANDOM)
		return -EINVAL;
	return 1;
}

static int check_stable_privacy(struct inet6_dev *idev, struct net *net,
				int mode)
{
	if (mode == IN6_ADDR_GEN_MODE_STABLE_PRIVACY &&
	    !idev->cnf.stable_secret.initialized &&
	    !net->ipv6.devconf_dflt->stable_secret.initialized)
		return -EINVAL;
	return 1;
}

static int inet6_validate_link_af(const struct net_device *dev,
				  const struct nlattr *nla,
				  struct netlink_ext_ack *extack)
{
	struct nlattr *tb[IFLA_INET6_MAX + 1];
	struct inet6_dev *idev = NULL;
	int err;

	if (dev) {
		idev = __in6_dev_get(dev);
		if (!idev)
			return -EAFNOSUPPORT;
	}

	err = nla_parse_nested_deprecated(tb, IFLA_INET6_MAX, nla,
					  inet6_af_policy, extack);
	if (err)
		return err;

	if (!tb[IFLA_INET6_TOKEN] && !tb[IFLA_INET6_ADDR_GEN_MODE])
		return -EINVAL;

	if (tb[IFLA_INET6_ADDR_GEN_MODE]) {
		u8 mode = nla_get_u8(tb[IFLA_INET6_ADDR_GEN_MODE]);

		if (check_addr_gen_mode(mode) < 0)
			return -EINVAL;
		if (dev && check_stable_privacy(idev, dev_net(dev), mode) < 0)
			return -EINVAL;
	}

	return 0;
}

static int inet6_set_link_af(struct net_device *dev, const struct nlattr *nla,
			     struct netlink_ext_ack *extack)
{
	struct inet6_dev *idev = __in6_dev_get(dev);
	struct nlattr *tb[IFLA_INET6_MAX + 1];
	int err;

	if (!idev)
		return -EAFNOSUPPORT;

	if (nla_parse_nested_deprecated(tb, IFLA_INET6_MAX, nla, NULL, NULL) < 0)
		return -EINVAL;

	if (tb[IFLA_INET6_TOKEN]) {
		err = inet6_set_iftoken(idev, nla_data(tb[IFLA_INET6_TOKEN]),
					extack);
		if (err)
			return err;
	}

	if (tb[IFLA_INET6_ADDR_GEN_MODE]) {
		u8 mode = nla_get_u8(tb[IFLA_INET6_ADDR_GEN_MODE]);

		WRITE_ONCE(idev->cnf.addr_gen_mode, mode);
	}

	return 0;
}

static int inet6_fill_ifinfo(struct sk_buff *skb, struct inet6_dev *idev,
			     u32 portid, u32 seq, int event, unsigned int flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct net_device *dev = idev->dev;
	struct ifinfomsg *hdr;
	struct nlmsghdr *nlh;
<<<<<<< HEAD
	void *protoinfo;

	nlh = nlmsg_put(skb, pid, seq, event, sizeof(*hdr), flags);
	if (nlh == NULL)
=======
	int ifindex, iflink;
	void *protoinfo;

	nlh = nlmsg_put(skb, portid, seq, event, sizeof(*hdr), flags);
	if (!nlh)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EMSGSIZE;

	hdr = nlmsg_data(nlh);
	hdr->ifi_family = AF_INET6;
	hdr->__ifi_pad = 0;
	hdr->ifi_type = dev->type;
<<<<<<< HEAD
	hdr->ifi_index = dev->ifindex;
	hdr->ifi_flags = dev_get_flags(dev);
	hdr->ifi_change = 0;

	NLA_PUT_STRING(skb, IFLA_IFNAME, dev->name);

	if (dev->addr_len)
		NLA_PUT(skb, IFLA_ADDRESS, dev->addr_len, dev->dev_addr);

	NLA_PUT_U32(skb, IFLA_MTU, dev->mtu);
	if (dev->ifindex != dev->iflink)
		NLA_PUT_U32(skb, IFLA_LINK, dev->iflink);

	protoinfo = nla_nest_start(skb, IFLA_PROTINFO);
	if (protoinfo == NULL)
		goto nla_put_failure;

	if (inet6_fill_ifla6_attrs(skb, idev) < 0)
		goto nla_put_failure;

	nla_nest_end(skb, protoinfo);
	return nlmsg_end(skb, nlh);
=======
	ifindex = READ_ONCE(dev->ifindex);
	hdr->ifi_index = ifindex;
	hdr->ifi_flags = dev_get_flags(dev);
	hdr->ifi_change = 0;

	iflink = dev_get_iflink(dev);
	if (nla_put_string(skb, IFLA_IFNAME, dev->name) ||
	    (dev->addr_len &&
	     nla_put(skb, IFLA_ADDRESS, dev->addr_len, dev->dev_addr)) ||
	    nla_put_u32(skb, IFLA_MTU, READ_ONCE(dev->mtu)) ||
	    (ifindex != iflink &&
	     nla_put_u32(skb, IFLA_LINK, iflink)) ||
	    nla_put_u8(skb, IFLA_OPERSTATE,
		       netif_running(dev) ? READ_ONCE(dev->operstate) : IF_OPER_DOWN))
		goto nla_put_failure;
	protoinfo = nla_nest_start_noflag(skb, IFLA_PROTINFO);
	if (!protoinfo)
		goto nla_put_failure;

	if (inet6_fill_ifla6_attrs(skb, idev, 0) < 0)
		goto nla_put_failure;

	nla_nest_end(skb, protoinfo);
	nlmsg_end(skb, nlh);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}

<<<<<<< HEAD
static int inet6_dump_ifinfo(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct net *net = sock_net(skb->sk);
	int h, s_h;
	int idx = 0, s_idx;
	struct net_device *dev;
	struct inet6_dev *idev;
	struct hlist_head *head;
	struct hlist_node *node;

	s_h = cb->args[0];
	s_idx = cb->args[1];

	rcu_read_lock();
	for (h = s_h; h < NETDEV_HASHENTRIES; h++, s_idx = 0) {
		idx = 0;
		head = &net->dev_index_head[h];
		hlist_for_each_entry_rcu(dev, node, head, index_hlist) {
			if (idx < s_idx)
				goto cont;
			idev = __in6_dev_get(dev);
			if (!idev)
				goto cont;
			if (inet6_fill_ifinfo(skb, idev,
					      NETLINK_CB(cb->skb).pid,
					      cb->nlh->nlmsg_seq,
					      RTM_NEWLINK, NLM_F_MULTI) <= 0)
				goto out;
cont:
			idx++;
		}
	}
out:
	rcu_read_unlock();
	cb->args[1] = idx;
	cb->args[0] = h;

	return skb->len;
=======
static int inet6_valid_dump_ifinfo(const struct nlmsghdr *nlh,
				   struct netlink_ext_ack *extack)
{
	struct ifinfomsg *ifm;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*ifm))) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid header for link dump request");
		return -EINVAL;
	}

	if (nlmsg_attrlen(nlh, sizeof(*ifm))) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid data after header");
		return -EINVAL;
	}

	ifm = nlmsg_data(nlh);
	if (ifm->__ifi_pad || ifm->ifi_type || ifm->ifi_flags ||
	    ifm->ifi_change || ifm->ifi_index) {
		NL_SET_ERR_MSG_MOD(extack, "Invalid values in header for dump request");
		return -EINVAL;
	}

	return 0;
}

static int inet6_dump_ifinfo(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct net *net = sock_net(skb->sk);
	struct {
		unsigned long ifindex;
	} *ctx = (void *)cb->ctx;
	struct net_device *dev;
	struct inet6_dev *idev;
	int err;

	/* only requests using strict checking can pass data to
	 * influence the dump
	 */
	if (cb->strict_check) {
		err = inet6_valid_dump_ifinfo(cb->nlh, cb->extack);

		if (err < 0)
			return err;
	}

	err = 0;
	rcu_read_lock();
	for_each_netdev_dump(net, dev, ctx->ifindex) {
		idev = __in6_dev_get(dev);
		if (!idev)
			continue;
		err = inet6_fill_ifinfo(skb, idev,
					NETLINK_CB(cb->skb).portid,
					cb->nlh->nlmsg_seq,
					RTM_NEWLINK, NLM_F_MULTI);
		if (err < 0)
			break;
	}
	rcu_read_unlock();

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void inet6_ifinfo_notify(int event, struct inet6_dev *idev)
{
	struct sk_buff *skb;
	struct net *net = dev_net(idev->dev);
	int err = -ENOBUFS;

	skb = nlmsg_new(inet6_if_nlmsg_size(), GFP_ATOMIC);
<<<<<<< HEAD
	if (skb == NULL)
=======
	if (!skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto errout;

	err = inet6_fill_ifinfo(skb, idev, 0, 0, event, 0);
	if (err < 0) {
		/* -EMSGSIZE implies BUG in inet6_if_nlmsg_size() */
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(skb);
		goto errout;
	}
	rtnl_notify(skb, net, 0, RTNLGRP_IPV6_IFINFO, NULL, GFP_ATOMIC);
	return;
errout:
	if (err < 0)
		rtnl_set_sk_err(net, RTNLGRP_IPV6_IFINFO, err);
}

static inline size_t inet6_prefix_nlmsg_size(void)
{
	return NLMSG_ALIGN(sizeof(struct prefixmsg))
	       + nla_total_size(sizeof(struct in6_addr))
	       + nla_total_size(sizeof(struct prefix_cacheinfo));
}

static int inet6_fill_prefix(struct sk_buff *skb, struct inet6_dev *idev,
<<<<<<< HEAD
			     struct prefix_info *pinfo, u32 pid, u32 seq,
=======
			     struct prefix_info *pinfo, u32 portid, u32 seq,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			     int event, unsigned int flags)
{
	struct prefixmsg *pmsg;
	struct nlmsghdr *nlh;
	struct prefix_cacheinfo	ci;

<<<<<<< HEAD
	nlh = nlmsg_put(skb, pid, seq, event, sizeof(*pmsg), flags);
	if (nlh == NULL)
=======
	nlh = nlmsg_put(skb, portid, seq, event, sizeof(*pmsg), flags);
	if (!nlh)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EMSGSIZE;

	pmsg = nlmsg_data(nlh);
	pmsg->prefix_family = AF_INET6;
	pmsg->prefix_pad1 = 0;
	pmsg->prefix_pad2 = 0;
	pmsg->prefix_ifindex = idev->dev->ifindex;
	pmsg->prefix_len = pinfo->prefix_len;
	pmsg->prefix_type = pinfo->type;
	pmsg->prefix_pad3 = 0;
<<<<<<< HEAD
	pmsg->prefix_flags = 0;
	if (pinfo->onlink)
		pmsg->prefix_flags |= IF_PREFIX_ONLINK;
	if (pinfo->autoconf)
		pmsg->prefix_flags |= IF_PREFIX_AUTOCONF;

	NLA_PUT(skb, PREFIX_ADDRESS, sizeof(pinfo->prefix), &pinfo->prefix);

	ci.preferred_time = ntohl(pinfo->prefered);
	ci.valid_time = ntohl(pinfo->valid);
	NLA_PUT(skb, PREFIX_CACHEINFO, sizeof(ci), &ci);

	return nlmsg_end(skb, nlh);
=======
	pmsg->prefix_flags = pinfo->flags;

	if (nla_put(skb, PREFIX_ADDRESS, sizeof(pinfo->prefix), &pinfo->prefix))
		goto nla_put_failure;
	ci.preferred_time = ntohl(pinfo->prefered);
	ci.valid_time = ntohl(pinfo->valid);
	if (nla_put(skb, PREFIX_CACHEINFO, sizeof(ci), &ci))
		goto nla_put_failure;
	nlmsg_end(skb, nlh);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}

static void inet6_prefix_notify(int event, struct inet6_dev *idev,
			 struct prefix_info *pinfo)
{
	struct sk_buff *skb;
	struct net *net = dev_net(idev->dev);
	int err = -ENOBUFS;

	skb = nlmsg_new(inet6_prefix_nlmsg_size(), GFP_ATOMIC);
<<<<<<< HEAD
	if (skb == NULL)
=======
	if (!skb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto errout;

	err = inet6_fill_prefix(skb, idev, pinfo, 0, 0, event, 0);
	if (err < 0) {
		/* -EMSGSIZE implies BUG in inet6_prefix_nlmsg_size() */
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(skb);
		goto errout;
	}
	rtnl_notify(skb, net, 0, RTNLGRP_IPV6_PREFIX, NULL, GFP_ATOMIC);
	return;
errout:
	if (err < 0)
		rtnl_set_sk_err(net, RTNLGRP_IPV6_PREFIX, err);
}

static void __ipv6_ifa_notify(int event, struct inet6_ifaddr *ifp)
{
<<<<<<< HEAD
=======
	struct net *net = dev_net(ifp->idev->dev);

	if (event)
		ASSERT_RTNL();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inet6_ifa_notify(event ? : RTM_NEWADDR, ifp);

	switch (event) {
	case RTM_NEWADDR:
		/*
<<<<<<< HEAD
		 * If the address was optimistic
		 * we inserted the route at the start of
		 * our DAD process, so we don't need
		 * to do it again
		 */
		if (!(ifp->rt->rt6i_node))
			ip6_ins_rt(ifp->rt);
		if (ifp->idev->cnf.forwarding)
			addrconf_join_anycast(ifp);
=======
		 * If the address was optimistic we inserted the route at the
		 * start of our DAD process, so we don't need to do it again.
		 * If the device was taken down in the middle of the DAD
		 * cycle there is a race where we could get here without a
		 * host route, so nothing to insert. That will be fixed when
		 * the device is brought up.
		 */
		if (ifp->rt && !rcu_access_pointer(ifp->rt->fib6_node)) {
			ip6_ins_rt(net, ifp->rt);
		} else if (!ifp->rt && (ifp->idev->dev->flags & IFF_UP)) {
			pr_warn("BUG: Address %pI6c on device %s is missing its host route.\n",
				&ifp->addr, ifp->idev->dev->name);
		}

		if (ifp->idev->cnf.forwarding)
			addrconf_join_anycast(ifp);
		if (!ipv6_addr_any(&ifp->peer_addr))
			addrconf_prefix_route(&ifp->peer_addr, 128,
					      ifp->rt_priority, ifp->idev->dev,
					      0, 0, GFP_ATOMIC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case RTM_DELADDR:
		if (ifp->idev->cnf.forwarding)
			addrconf_leave_anycast(ifp);
		addrconf_leave_solict(ifp->idev, &ifp->addr);
<<<<<<< HEAD
		dst_hold(&ifp->rt->dst);

		if (ip6_del_rt(ifp->rt))
			dst_free(&ifp->rt->dst);
		break;
	}
=======
		if (!ipv6_addr_any(&ifp->peer_addr)) {
			struct fib6_info *rt;

			rt = addrconf_get_prefix_route(&ifp->peer_addr, 128,
						       ifp->idev->dev, 0, 0,
						       false);
			if (rt)
				ip6_del_rt(net, rt, false);
		}
		if (ifp->rt) {
			ip6_del_rt(net, ifp->rt, false);
			ifp->rt = NULL;
		}
		rt_genid_bump_ipv6(net);
		break;
	}
	atomic_inc(&net->ipv6.dev_addr_genid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ipv6_ifa_notify(int event, struct inet6_ifaddr *ifp)
{
<<<<<<< HEAD
	rcu_read_lock_bh();
	if (likely(ifp->idev->dead == 0))
		__ipv6_ifa_notify(event, ifp);
	rcu_read_unlock_bh();
=======
	if (likely(ifp->idev->dead == 0))
		__ipv6_ifa_notify(event, ifp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_SYSCTL

<<<<<<< HEAD
static
int addrconf_sysctl_forward(ctl_table *ctl, int write,
			   void __user *buffer, size_t *lenp, loff_t *ppos)
=======
static int addrconf_sysctl_forward(struct ctl_table *ctl, int write,
		void *buffer, size_t *lenp, loff_t *ppos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int *valp = ctl->data;
	int val = *valp;
	loff_t pos = *ppos;
<<<<<<< HEAD
	ctl_table lctl;
=======
	struct ctl_table lctl;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	/*
	 * ctl->data points to idev->cnf.forwarding, we should
	 * not modify it until we get the rtnl lock.
	 */
	lctl = *ctl;
	lctl.data = &val;

	ret = proc_dointvec(&lctl, write, buffer, lenp, ppos);

	if (write)
		ret = addrconf_fixup_forwarding(ctl, valp, val);
	if (ret)
		*ppos = pos;
	return ret;
}

<<<<<<< HEAD
static
int addrconf_sysctl_mtu(struct ctl_table *ctl, int write,
			void __user *buffer, size_t *lenp, loff_t *ppos)
=======
static int addrconf_sysctl_mtu(struct ctl_table *ctl, int write,
		void *buffer, size_t *lenp, loff_t *ppos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inet6_dev *idev = ctl->extra1;
	int min_mtu = IPV6_MIN_MTU;
	struct ctl_table lctl;

	lctl = *ctl;
	lctl.extra1 = &min_mtu;
	lctl.extra2 = idev ? &idev->dev->mtu : NULL;

	return proc_dointvec_minmax(&lctl, write, buffer, lenp, ppos);
}

static void dev_disable_change(struct inet6_dev *idev)
{
<<<<<<< HEAD
	if (!idev || !idev->dev)
		return;

	if (idev->cnf.disable_ipv6)
		addrconf_notify(NULL, NETDEV_DOWN, idev->dev);
	else
		addrconf_notify(NULL, NETDEV_UP, idev->dev);
=======
	struct netdev_notifier_info info;

	if (!idev || !idev->dev)
		return;

	netdev_notifier_info_init(&info, idev->dev);
	if (idev->cnf.disable_ipv6)
		addrconf_notify(NULL, NETDEV_DOWN, &info);
	else
		addrconf_notify(NULL, NETDEV_UP, &info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void addrconf_disable_change(struct net *net, __s32 newf)
{
	struct net_device *dev;
	struct inet6_dev *idev;

<<<<<<< HEAD
	rcu_read_lock();
	for_each_netdev_rcu(net, dev) {
		idev = __in6_dev_get(dev);
		if (idev) {
			int changed = (!idev->cnf.disable_ipv6) ^ (!newf);
			idev->cnf.disable_ipv6 = newf;
=======
	for_each_netdev(net, dev) {
		idev = __in6_dev_get(dev);
		if (idev) {
			int changed = (!idev->cnf.disable_ipv6) ^ (!newf);

			WRITE_ONCE(idev->cnf.disable_ipv6, newf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (changed)
				dev_disable_change(idev);
		}
	}
<<<<<<< HEAD
	rcu_read_unlock();
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int addrconf_disable_ipv6(struct ctl_table *table, int *p, int newf)
{
<<<<<<< HEAD
	struct net *net;
	int old;

	if (!rtnl_trylock())
		return restart_syscall();

	net = (struct net *)table->extra2;
	old = *p;
	*p = newf;

	if (p == &net->ipv6.devconf_dflt->disable_ipv6) {
		rtnl_unlock();
		return 0;
	}

	if (p == &net->ipv6.devconf_all->disable_ipv6) {
		net->ipv6.devconf_dflt->disable_ipv6 = newf;
=======
	struct net *net = (struct net *)table->extra2;
	int old;

	if (p == &net->ipv6.devconf_dflt->disable_ipv6) {
		WRITE_ONCE(*p, newf);
		return 0;
	}

	if (!rtnl_trylock())
		return restart_syscall();

	old = *p;
	WRITE_ONCE(*p, newf);

	if (p == &net->ipv6.devconf_all->disable_ipv6) {
		WRITE_ONCE(net->ipv6.devconf_dflt->disable_ipv6, newf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		addrconf_disable_change(net, newf);
	} else if ((!newf) ^ (!old))
		dev_disable_change((struct inet6_dev *)table->extra1);

	rtnl_unlock();
	return 0;
}

<<<<<<< HEAD
static
int addrconf_sysctl_disable(ctl_table *ctl, int write,
			    void __user *buffer, size_t *lenp, loff_t *ppos)
=======
static int addrconf_sysctl_disable(struct ctl_table *ctl, int write,
		void *buffer, size_t *lenp, loff_t *ppos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int *valp = ctl->data;
	int val = *valp;
	loff_t pos = *ppos;
<<<<<<< HEAD
	ctl_table lctl;
=======
	struct ctl_table lctl;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	/*
	 * ctl->data points to idev->cnf.disable_ipv6, we should
	 * not modify it until we get the rtnl lock.
	 */
	lctl = *ctl;
	lctl.data = &val;

	ret = proc_dointvec(&lctl, write, buffer, lenp, ppos);

	if (write)
		ret = addrconf_disable_ipv6(ctl, valp, val);
	if (ret)
		*ppos = pos;
	return ret;
}

<<<<<<< HEAD
static struct addrconf_sysctl_table
{
	struct ctl_table_header *sysctl_header;
	ctl_table addrconf_vars[DEVCONF_MAX+1];
	char *dev_name;
} addrconf_sysctl __read_mostly = {
	.sysctl_header = NULL,
	.addrconf_vars = {
		{
			.procname	= "forwarding",
			.data		= &ipv6_devconf.forwarding,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= addrconf_sysctl_forward,
		},
		{
			.procname	= "hop_limit",
			.data		= &ipv6_devconf.hop_limit,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "mtu",
			.data		= &ipv6_devconf.mtu6,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= addrconf_sysctl_mtu,
		},
		{
			.procname	= "accept_ra",
			.data		= &ipv6_devconf.accept_ra,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "accept_redirects",
			.data		= &ipv6_devconf.accept_redirects,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "autoconf",
			.data		= &ipv6_devconf.autoconf,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "dad_transmits",
			.data		= &ipv6_devconf.dad_transmits,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "router_solicitations",
			.data		= &ipv6_devconf.rtr_solicits,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "router_solicitation_interval",
			.data		= &ipv6_devconf.rtr_solicit_interval,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec_jiffies,
		},
		{
			.procname	= "router_solicitation_delay",
			.data		= &ipv6_devconf.rtr_solicit_delay,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec_jiffies,
		},
		{
			.procname	= "force_mld_version",
			.data		= &ipv6_devconf.force_mld_version,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
#ifdef CONFIG_IPV6_PRIVACY
		{
			.procname	= "use_tempaddr",
			.data		= &ipv6_devconf.use_tempaddr,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "temp_valid_lft",
			.data		= &ipv6_devconf.temp_valid_lft,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "temp_prefered_lft",
			.data		= &ipv6_devconf.temp_prefered_lft,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "regen_max_retry",
			.data		= &ipv6_devconf.regen_max_retry,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "max_desync_factor",
			.data		= &ipv6_devconf.max_desync_factor,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
#endif
		{
			.procname	= "max_addresses",
			.data		= &ipv6_devconf.max_addresses,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "accept_ra_defrtr",
			.data		= &ipv6_devconf.accept_ra_defrtr,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "accept_ra_pinfo",
			.data		= &ipv6_devconf.accept_ra_pinfo,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
#ifdef CONFIG_IPV6_ROUTER_PREF
		{
			.procname	= "accept_ra_rtr_pref",
			.data		= &ipv6_devconf.accept_ra_rtr_pref,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "router_probe_interval",
			.data		= &ipv6_devconf.rtr_probe_interval,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec_jiffies,
		},
#ifdef CONFIG_IPV6_ROUTE_INFO
		{
			.procname	= "accept_ra_rt_info_max_plen",
			.data		= &ipv6_devconf.accept_ra_rt_info_max_plen,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
#endif
#endif
		{
			.procname	= "accept_ra_rt_table",
			.data		= &ipv6_devconf.accept_ra_rt_table,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "proxy_ndp",
			.data		= &ipv6_devconf.proxy_ndp,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname	= "accept_source_route",
			.data		= &ipv6_devconf.accept_source_route,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
		{
			.procname       = "optimistic_dad",
			.data           = &ipv6_devconf.optimistic_dad,
			.maxlen         = sizeof(int),
			.mode           = 0644,
			.proc_handler   = proc_dointvec,

		},
		{
			.procname       = "use_optimistic",
			.data           = &ipv6_devconf.use_optimistic,
			.maxlen         = sizeof(int),
			.mode           = 0644,
			.proc_handler   = proc_dointvec,

		},
#endif
#ifdef CONFIG_IPV6_MROUTE
		{
			.procname	= "mc_forwarding",
			.data		= &ipv6_devconf.mc_forwarding,
			.maxlen		= sizeof(int),
			.mode		= 0444,
			.proc_handler	= proc_dointvec,
		},
#endif
		{
			.procname	= "disable_ipv6",
			.data		= &ipv6_devconf.disable_ipv6,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= addrconf_sysctl_disable,
		},
		{
			.procname	= "accept_dad",
			.data		= &ipv6_devconf.accept_dad,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			.procname       = "force_tllao",
			.data           = &ipv6_devconf.force_tllao,
			.maxlen         = sizeof(int),
			.mode           = 0644,
			.proc_handler   = proc_dointvec
		},
		{
			.procname	= "accept_ra_prefix_route",
			.data		= &ipv6_devconf.accept_ra_prefix_route,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
#ifdef CONFIG_LGE_DHCPV6_WIFI
		{
			.procname		= "ra_info_flag",
			.data			= &ipv6_devconf.ra_info_flag,
			.maxlen 		= sizeof(int),
			.mode			= 0644,
			.proc_handler	= proc_dointvec
		},
#endif
		{
			.procname	= "accept_ra_mtu",
			.data		= &ipv6_devconf.accept_ra_mtu,
			.maxlen		= sizeof(int),
			.mode		= 0644,
			.proc_handler	= proc_dointvec,
		},
		{
			/* sentinel */
		}
	},
=======
static int addrconf_sysctl_proxy_ndp(struct ctl_table *ctl, int write,
		void *buffer, size_t *lenp, loff_t *ppos)
{
	int *valp = ctl->data;
	int ret;
	int old, new;

	old = *valp;
	ret = proc_dointvec(ctl, write, buffer, lenp, ppos);
	new = *valp;

	if (write && old != new) {
		struct net *net = ctl->extra2;

		if (!rtnl_trylock())
			return restart_syscall();

		if (valp == &net->ipv6.devconf_dflt->proxy_ndp)
			inet6_netconf_notify_devconf(net, RTM_NEWNETCONF,
						     NETCONFA_PROXY_NEIGH,
						     NETCONFA_IFINDEX_DEFAULT,
						     net->ipv6.devconf_dflt);
		else if (valp == &net->ipv6.devconf_all->proxy_ndp)
			inet6_netconf_notify_devconf(net, RTM_NEWNETCONF,
						     NETCONFA_PROXY_NEIGH,
						     NETCONFA_IFINDEX_ALL,
						     net->ipv6.devconf_all);
		else {
			struct inet6_dev *idev = ctl->extra1;

			inet6_netconf_notify_devconf(net, RTM_NEWNETCONF,
						     NETCONFA_PROXY_NEIGH,
						     idev->dev->ifindex,
						     &idev->cnf);
		}
		rtnl_unlock();
	}

	return ret;
}

static int addrconf_sysctl_addr_gen_mode(struct ctl_table *ctl, int write,
					 void *buffer, size_t *lenp,
					 loff_t *ppos)
{
	int ret = 0;
	u32 new_val;
	struct inet6_dev *idev = (struct inet6_dev *)ctl->extra1;
	struct net *net = (struct net *)ctl->extra2;
	struct ctl_table tmp = {
		.data = &new_val,
		.maxlen = sizeof(new_val),
		.mode = ctl->mode,
	};

	if (!rtnl_trylock())
		return restart_syscall();

	new_val = *((u32 *)ctl->data);

	ret = proc_douintvec(&tmp, write, buffer, lenp, ppos);
	if (ret != 0)
		goto out;

	if (write) {
		if (check_addr_gen_mode(new_val) < 0) {
			ret = -EINVAL;
			goto out;
		}

		if (idev) {
			if (check_stable_privacy(idev, net, new_val) < 0) {
				ret = -EINVAL;
				goto out;
			}

			if (idev->cnf.addr_gen_mode != new_val) {
				WRITE_ONCE(idev->cnf.addr_gen_mode, new_val);
				addrconf_init_auto_addrs(idev->dev);
			}
		} else if (&net->ipv6.devconf_all->addr_gen_mode == ctl->data) {
			struct net_device *dev;

			WRITE_ONCE(net->ipv6.devconf_dflt->addr_gen_mode, new_val);
			for_each_netdev(net, dev) {
				idev = __in6_dev_get(dev);
				if (idev &&
				    idev->cnf.addr_gen_mode != new_val) {
					WRITE_ONCE(idev->cnf.addr_gen_mode,
						  new_val);
					addrconf_init_auto_addrs(idev->dev);
				}
			}
		}

		WRITE_ONCE(*((u32 *)ctl->data), new_val);
	}

out:
	rtnl_unlock();

	return ret;
}

static int addrconf_sysctl_stable_secret(struct ctl_table *ctl, int write,
					 void *buffer, size_t *lenp,
					 loff_t *ppos)
{
	int err;
	struct in6_addr addr;
	char str[IPV6_MAX_STRLEN];
	struct ctl_table lctl = *ctl;
	struct net *net = ctl->extra2;
	struct ipv6_stable_secret *secret = ctl->data;

	if (&net->ipv6.devconf_all->stable_secret == ctl->data)
		return -EIO;

	lctl.maxlen = IPV6_MAX_STRLEN;
	lctl.data = str;

	if (!rtnl_trylock())
		return restart_syscall();

	if (!write && !secret->initialized) {
		err = -EIO;
		goto out;
	}

	err = snprintf(str, sizeof(str), "%pI6", &secret->secret);
	if (err >= sizeof(str)) {
		err = -EIO;
		goto out;
	}

	err = proc_dostring(&lctl, write, buffer, lenp, ppos);
	if (err || !write)
		goto out;

	if (in6_pton(str, -1, addr.in6_u.u6_addr8, -1, NULL) != 1) {
		err = -EIO;
		goto out;
	}

	secret->initialized = true;
	secret->secret = addr;

	if (&net->ipv6.devconf_dflt->stable_secret == ctl->data) {
		struct net_device *dev;

		for_each_netdev(net, dev) {
			struct inet6_dev *idev = __in6_dev_get(dev);

			if (idev) {
				WRITE_ONCE(idev->cnf.addr_gen_mode,
					   IN6_ADDR_GEN_MODE_STABLE_PRIVACY);
			}
		}
	} else {
		struct inet6_dev *idev = ctl->extra1;

		WRITE_ONCE(idev->cnf.addr_gen_mode,
			   IN6_ADDR_GEN_MODE_STABLE_PRIVACY);
	}

out:
	rtnl_unlock();

	return err;
}

static
int addrconf_sysctl_ignore_routes_with_linkdown(struct ctl_table *ctl,
						int write, void *buffer,
						size_t *lenp,
						loff_t *ppos)
{
	int *valp = ctl->data;
	int val = *valp;
	loff_t pos = *ppos;
	struct ctl_table lctl;
	int ret;

	/* ctl->data points to idev->cnf.ignore_routes_when_linkdown
	 * we should not modify it until we get the rtnl lock.
	 */
	lctl = *ctl;
	lctl.data = &val;

	ret = proc_dointvec(&lctl, write, buffer, lenp, ppos);

	if (write)
		ret = addrconf_fixup_linkdown(ctl, valp, val);
	if (ret)
		*ppos = pos;
	return ret;
}

static
void addrconf_set_nopolicy(struct rt6_info *rt, int action)
{
	if (rt) {
		if (action)
			rt->dst.flags |= DST_NOPOLICY;
		else
			rt->dst.flags &= ~DST_NOPOLICY;
	}
}

static
void addrconf_disable_policy_idev(struct inet6_dev *idev, int val)
{
	struct inet6_ifaddr *ifa;

	read_lock_bh(&idev->lock);
	list_for_each_entry(ifa, &idev->addr_list, if_list) {
		spin_lock(&ifa->lock);
		if (ifa->rt) {
			/* host routes only use builtin fib6_nh */
			struct fib6_nh *nh = ifa->rt->fib6_nh;
			int cpu;

			rcu_read_lock();
			ifa->rt->dst_nopolicy = val ? true : false;
			if (nh->rt6i_pcpu) {
				for_each_possible_cpu(cpu) {
					struct rt6_info **rtp;

					rtp = per_cpu_ptr(nh->rt6i_pcpu, cpu);
					addrconf_set_nopolicy(*rtp, val);
				}
			}
			rcu_read_unlock();
		}
		spin_unlock(&ifa->lock);
	}
	read_unlock_bh(&idev->lock);
}

static
int addrconf_disable_policy(struct ctl_table *ctl, int *valp, int val)
{
	struct net *net = (struct net *)ctl->extra2;
	struct inet6_dev *idev;

	if (valp == &net->ipv6.devconf_dflt->disable_policy) {
		WRITE_ONCE(*valp, val);
		return 0;
	}

	if (!rtnl_trylock())
		return restart_syscall();

	WRITE_ONCE(*valp, val);

	if (valp == &net->ipv6.devconf_all->disable_policy)  {
		struct net_device *dev;

		for_each_netdev(net, dev) {
			idev = __in6_dev_get(dev);
			if (idev)
				addrconf_disable_policy_idev(idev, val);
		}
	} else {
		idev = (struct inet6_dev *)ctl->extra1;
		addrconf_disable_policy_idev(idev, val);
	}

	rtnl_unlock();
	return 0;
}

static int addrconf_sysctl_disable_policy(struct ctl_table *ctl, int write,
				   void *buffer, size_t *lenp, loff_t *ppos)
{
	int *valp = ctl->data;
	int val = *valp;
	loff_t pos = *ppos;
	struct ctl_table lctl;
	int ret;

	lctl = *ctl;
	lctl.data = &val;
	ret = proc_dointvec(&lctl, write, buffer, lenp, ppos);

	if (write && (*valp != val))
		ret = addrconf_disable_policy(ctl, valp, val);

	if (ret)
		*ppos = pos;

	return ret;
}

static int minus_one = -1;
static const int two_five_five = 255;
static u32 ioam6_if_id_max = U16_MAX;

static const struct ctl_table addrconf_sysctl[] = {
	{
		.procname	= "forwarding",
		.data		= &ipv6_devconf.forwarding,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= addrconf_sysctl_forward,
	},
	{
		.procname	= "hop_limit",
		.data		= &ipv6_devconf.hop_limit,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= (void *)SYSCTL_ONE,
		.extra2		= (void *)&two_five_five,
	},
	{
		.procname	= "mtu",
		.data		= &ipv6_devconf.mtu6,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= addrconf_sysctl_mtu,
	},
	{
		.procname	= "accept_ra",
		.data		= &ipv6_devconf.accept_ra,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "accept_redirects",
		.data		= &ipv6_devconf.accept_redirects,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "autoconf",
		.data		= &ipv6_devconf.autoconf,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "dad_transmits",
		.data		= &ipv6_devconf.dad_transmits,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "router_solicitations",
		.data		= &ipv6_devconf.rtr_solicits,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &minus_one,
	},
	{
		.procname	= "router_solicitation_interval",
		.data		= &ipv6_devconf.rtr_solicit_interval,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "router_solicitation_max_interval",
		.data		= &ipv6_devconf.rtr_solicit_max_interval,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "router_solicitation_delay",
		.data		= &ipv6_devconf.rtr_solicit_delay,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	{
		.procname	= "force_mld_version",
		.data		= &ipv6_devconf.force_mld_version,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "mldv1_unsolicited_report_interval",
		.data		=
			&ipv6_devconf.mldv1_unsolicited_report_interval,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_ms_jiffies,
	},
	{
		.procname	= "mldv2_unsolicited_report_interval",
		.data		=
			&ipv6_devconf.mldv2_unsolicited_report_interval,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_ms_jiffies,
	},
	{
		.procname	= "use_tempaddr",
		.data		= &ipv6_devconf.use_tempaddr,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "temp_valid_lft",
		.data		= &ipv6_devconf.temp_valid_lft,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "temp_prefered_lft",
		.data		= &ipv6_devconf.temp_prefered_lft,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname       = "regen_min_advance",
		.data           = &ipv6_devconf.regen_min_advance,
		.maxlen         = sizeof(int),
		.mode           = 0644,
		.proc_handler   = proc_dointvec,
	},
	{
		.procname	= "regen_max_retry",
		.data		= &ipv6_devconf.regen_max_retry,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "max_desync_factor",
		.data		= &ipv6_devconf.max_desync_factor,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "max_addresses",
		.data		= &ipv6_devconf.max_addresses,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "accept_ra_defrtr",
		.data		= &ipv6_devconf.accept_ra_defrtr,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "ra_defrtr_metric",
		.data		= &ipv6_devconf.ra_defrtr_metric,
		.maxlen		= sizeof(u32),
		.mode		= 0644,
		.proc_handler	= proc_douintvec_minmax,
		.extra1		= (void *)SYSCTL_ONE,
	},
	{
		.procname	= "accept_ra_min_hop_limit",
		.data		= &ipv6_devconf.accept_ra_min_hop_limit,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "accept_ra_min_lft",
		.data		= &ipv6_devconf.accept_ra_min_lft,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "accept_ra_pinfo",
		.data		= &ipv6_devconf.accept_ra_pinfo,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "ra_honor_pio_life",
		.data		= &ipv6_devconf.ra_honor_pio_life,
		.maxlen		= sizeof(u8),
		.mode		= 0644,
		.proc_handler	= proc_dou8vec_minmax,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_ONE,
	},
#ifdef CONFIG_IPV6_ROUTER_PREF
	{
		.procname	= "accept_ra_rtr_pref",
		.data		= &ipv6_devconf.accept_ra_rtr_pref,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "router_probe_interval",
		.data		= &ipv6_devconf.rtr_probe_interval,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
#ifdef CONFIG_IPV6_ROUTE_INFO
	{
		.procname	= "accept_ra_rt_info_min_plen",
		.data		= &ipv6_devconf.accept_ra_rt_info_min_plen,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "accept_ra_rt_info_max_plen",
		.data		= &ipv6_devconf.accept_ra_rt_info_max_plen,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
#endif
#endif
	{
		.procname	= "proxy_ndp",
		.data		= &ipv6_devconf.proxy_ndp,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= addrconf_sysctl_proxy_ndp,
	},
	{
		.procname	= "accept_source_route",
		.data		= &ipv6_devconf.accept_source_route,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	{
		.procname	= "optimistic_dad",
		.data		= &ipv6_devconf.optimistic_dad,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler   = proc_dointvec,
	},
	{
		.procname	= "use_optimistic",
		.data		= &ipv6_devconf.use_optimistic,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
#endif
#ifdef CONFIG_IPV6_MROUTE
	{
		.procname	= "mc_forwarding",
		.data		= &ipv6_devconf.mc_forwarding,
		.maxlen		= sizeof(int),
		.mode		= 0444,
		.proc_handler	= proc_dointvec,
	},
#endif
	{
		.procname	= "disable_ipv6",
		.data		= &ipv6_devconf.disable_ipv6,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= addrconf_sysctl_disable,
	},
	{
		.procname	= "accept_dad",
		.data		= &ipv6_devconf.accept_dad,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "force_tllao",
		.data		= &ipv6_devconf.force_tllao,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "ndisc_notify",
		.data		= &ipv6_devconf.ndisc_notify,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "suppress_frag_ndisc",
		.data		= &ipv6_devconf.suppress_frag_ndisc,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec
	},
	{
		.procname	= "accept_ra_from_local",
		.data		= &ipv6_devconf.accept_ra_from_local,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "accept_ra_mtu",
		.data		= &ipv6_devconf.accept_ra_mtu,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "stable_secret",
		.data		= &ipv6_devconf.stable_secret,
		.maxlen		= IPV6_MAX_STRLEN,
		.mode		= 0600,
		.proc_handler	= addrconf_sysctl_stable_secret,
	},
	{
		.procname	= "use_oif_addrs_only",
		.data		= &ipv6_devconf.use_oif_addrs_only,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "ignore_routes_with_linkdown",
		.data		= &ipv6_devconf.ignore_routes_with_linkdown,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= addrconf_sysctl_ignore_routes_with_linkdown,
	},
	{
		.procname	= "drop_unicast_in_l2_multicast",
		.data		= &ipv6_devconf.drop_unicast_in_l2_multicast,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "drop_unsolicited_na",
		.data		= &ipv6_devconf.drop_unsolicited_na,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "keep_addr_on_down",
		.data		= &ipv6_devconf.keep_addr_on_down,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,

	},
	{
		.procname	= "seg6_enabled",
		.data		= &ipv6_devconf.seg6_enabled,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
#ifdef CONFIG_IPV6_SEG6_HMAC
	{
		.procname	= "seg6_require_hmac",
		.data		= &ipv6_devconf.seg6_require_hmac,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
#endif
	{
		.procname       = "enhanced_dad",
		.data           = &ipv6_devconf.enhanced_dad,
		.maxlen         = sizeof(int),
		.mode           = 0644,
		.proc_handler   = proc_dointvec,
	},
	{
		.procname	= "addr_gen_mode",
		.data		= &ipv6_devconf.addr_gen_mode,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= addrconf_sysctl_addr_gen_mode,
	},
	{
		.procname       = "disable_policy",
		.data           = &ipv6_devconf.disable_policy,
		.maxlen         = sizeof(int),
		.mode           = 0644,
		.proc_handler   = addrconf_sysctl_disable_policy,
	},
	{
		.procname	= "ndisc_tclass",
		.data		= &ipv6_devconf.ndisc_tclass,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= (void *)SYSCTL_ZERO,
		.extra2		= (void *)&two_five_five,
	},
	{
		.procname	= "rpl_seg_enabled",
		.data		= &ipv6_devconf.rpl_seg_enabled,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "ioam6_enabled",
		.data		= &ipv6_devconf.ioam6_enabled,
		.maxlen		= sizeof(u8),
		.mode		= 0644,
		.proc_handler	= proc_dou8vec_minmax,
		.extra1		= (void *)SYSCTL_ZERO,
		.extra2		= (void *)SYSCTL_ONE,
	},
	{
		.procname	= "ioam6_id",
		.data		= &ipv6_devconf.ioam6_id,
		.maxlen		= sizeof(u32),
		.mode		= 0644,
		.proc_handler	= proc_douintvec_minmax,
		.extra1		= (void *)SYSCTL_ZERO,
		.extra2		= (void *)&ioam6_if_id_max,
	},
	{
		.procname	= "ioam6_id_wide",
		.data		= &ipv6_devconf.ioam6_id_wide,
		.maxlen		= sizeof(u32),
		.mode		= 0644,
		.proc_handler	= proc_douintvec,
	},
	{
		.procname	= "ndisc_evict_nocarrier",
		.data		= &ipv6_devconf.ndisc_evict_nocarrier,
		.maxlen		= sizeof(u8),
		.mode		= 0644,
		.proc_handler	= proc_dou8vec_minmax,
		.extra1		= (void *)SYSCTL_ZERO,
		.extra2		= (void *)SYSCTL_ONE,
	},
	{
		.procname	= "accept_untracked_na",
		.data		= &ipv6_devconf.accept_untracked_na,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_TWO,
	},
	{
		/* sentinel */
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __addrconf_sysctl_register(struct net *net, char *dev_name,
		struct inet6_dev *idev, struct ipv6_devconf *p)
{
<<<<<<< HEAD
	int i;
	struct addrconf_sysctl_table *t;

#define ADDRCONF_CTL_PATH_DEV	3

	struct ctl_path addrconf_ctl_path[] = {
		{ .procname = "net", },
		{ .procname = "ipv6", },
		{ .procname = "conf", },
		{ /* to be set */ },
		{ },
	};


	t = kmemdup(&addrconf_sysctl, sizeof(*t), GFP_KERNEL);
	if (t == NULL)
		goto out;

	for (i = 0; t->addrconf_vars[i].data; i++) {
		t->addrconf_vars[i].data += (char *)p - (char *)&ipv6_devconf;
		t->addrconf_vars[i].extra1 = idev; /* embedded; no ref */
		t->addrconf_vars[i].extra2 = net;
	}

	/*
	 * Make a copy of dev_name, because '.procname' is regarded as const
	 * by sysctl and we wouldn't want anyone to change it under our feet
	 * (see SIOCSIFNAME).
	 */
	t->dev_name = kstrdup(dev_name, GFP_KERNEL);
	if (!t->dev_name)
		goto free;

	addrconf_ctl_path[ADDRCONF_CTL_PATH_DEV].procname = t->dev_name;

	t->sysctl_header = register_net_sysctl_table(net, addrconf_ctl_path,
			t->addrconf_vars);
	if (t->sysctl_header == NULL)
		goto free_procname;

	p->sysctl = t;
	return 0;

free_procname:
	kfree(t->dev_name);
free:
	kfree(t);
=======
	int i, ifindex;
	struct ctl_table *table;
	char path[sizeof("net/ipv6/conf/") + IFNAMSIZ];

	table = kmemdup(addrconf_sysctl, sizeof(addrconf_sysctl), GFP_KERNEL_ACCOUNT);
	if (!table)
		goto out;

	for (i = 0; table[i].data; i++) {
		table[i].data += (char *)p - (char *)&ipv6_devconf;
		/* If one of these is already set, then it is not safe to
		 * overwrite either of them: this makes proc_dointvec_minmax
		 * usable.
		 */
		if (!table[i].extra1 && !table[i].extra2) {
			table[i].extra1 = idev; /* embedded; no ref */
			table[i].extra2 = net;
		}
	}

	snprintf(path, sizeof(path), "net/ipv6/conf/%s", dev_name);

	p->sysctl_header = register_net_sysctl_sz(net, path, table,
						  ARRAY_SIZE(addrconf_sysctl));
	if (!p->sysctl_header)
		goto free;

	if (!strcmp(dev_name, "all"))
		ifindex = NETCONFA_IFINDEX_ALL;
	else if (!strcmp(dev_name, "default"))
		ifindex = NETCONFA_IFINDEX_DEFAULT;
	else
		ifindex = idev->dev->ifindex;
	inet6_netconf_notify_devconf(net, RTM_NEWNETCONF, NETCONFA_ALL,
				     ifindex, p);
	return 0;

free:
	kfree(table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return -ENOBUFS;
}

<<<<<<< HEAD
static void __addrconf_sysctl_unregister(struct ipv6_devconf *p)
{
	struct addrconf_sysctl_table *t;

	if (p->sysctl == NULL)
		return;

	t = p->sysctl;
	p->sysctl = NULL;
	unregister_net_sysctl_table(t->sysctl_header);
	kfree(t->dev_name);
	kfree(t);
}

static void addrconf_sysctl_register(struct inet6_dev *idev)
{
	neigh_sysctl_register(idev->dev, idev->nd_parms, "ipv6",
			      &ndisc_ifinfo_sysctl_change);
	__addrconf_sysctl_register(dev_net(idev->dev), idev->dev->name,
					idev, &idev->cnf);
=======
static void __addrconf_sysctl_unregister(struct net *net,
					 struct ipv6_devconf *p, int ifindex)
{
	struct ctl_table *table;

	if (!p->sysctl_header)
		return;

	table = p->sysctl_header->ctl_table_arg;
	unregister_net_sysctl_table(p->sysctl_header);
	p->sysctl_header = NULL;
	kfree(table);

	inet6_netconf_notify_devconf(net, RTM_DELNETCONF, 0, ifindex, NULL);
}

static int addrconf_sysctl_register(struct inet6_dev *idev)
{
	int err;

	if (!sysctl_dev_name_is_allowed(idev->dev->name))
		return -EINVAL;

	err = neigh_sysctl_register(idev->dev, idev->nd_parms,
				    &ndisc_ifinfo_sysctl_change);
	if (err)
		return err;
	err = __addrconf_sysctl_register(dev_net(idev->dev), idev->dev->name,
					 idev, &idev->cnf);
	if (err)
		neigh_sysctl_unregister(idev->nd_parms);

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void addrconf_sysctl_unregister(struct inet6_dev *idev)
{
<<<<<<< HEAD
	__addrconf_sysctl_unregister(&idev->cnf);
=======
	__addrconf_sysctl_unregister(dev_net(idev->dev), &idev->cnf,
				     idev->dev->ifindex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	neigh_sysctl_unregister(idev->nd_parms);
}


#endif

static int __net_init addrconf_init_net(struct net *net)
{
	int err = -ENOMEM;
	struct ipv6_devconf *all, *dflt;

<<<<<<< HEAD
	all = kmemdup(&ipv6_devconf, sizeof(ipv6_devconf), GFP_KERNEL);
	if (all == NULL)
		goto err_alloc_all;

	dflt = kmemdup(&ipv6_devconf_dflt, sizeof(ipv6_devconf_dflt), GFP_KERNEL);
	if (dflt == NULL)
		goto err_alloc_dflt;

=======
	spin_lock_init(&net->ipv6.addrconf_hash_lock);
	INIT_DEFERRABLE_WORK(&net->ipv6.addr_chk_work, addrconf_verify_work);
	net->ipv6.inet6_addr_lst = kcalloc(IN6_ADDR_HSIZE,
					   sizeof(struct hlist_head),
					   GFP_KERNEL);
	if (!net->ipv6.inet6_addr_lst)
		goto err_alloc_addr;

	all = kmemdup(&ipv6_devconf, sizeof(ipv6_devconf), GFP_KERNEL);
	if (!all)
		goto err_alloc_all;

	dflt = kmemdup(&ipv6_devconf_dflt, sizeof(ipv6_devconf_dflt), GFP_KERNEL);
	if (!dflt)
		goto err_alloc_dflt;

	if (!net_eq(net, &init_net)) {
		switch (net_inherit_devconf()) {
		case 1:  /* copy from init_net */
			memcpy(all, init_net.ipv6.devconf_all,
			       sizeof(ipv6_devconf));
			memcpy(dflt, init_net.ipv6.devconf_dflt,
			       sizeof(ipv6_devconf_dflt));
			break;
		case 3: /* copy from the current netns */
			memcpy(all, current->nsproxy->net_ns->ipv6.devconf_all,
			       sizeof(ipv6_devconf));
			memcpy(dflt,
			       current->nsproxy->net_ns->ipv6.devconf_dflt,
			       sizeof(ipv6_devconf_dflt));
			break;
		case 0:
		case 2:
			/* use compiled values */
			break;
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* these will be inherited by all namespaces */
	dflt->autoconf = ipv6_defaults.autoconf;
	dflt->disable_ipv6 = ipv6_defaults.disable_ipv6;

<<<<<<< HEAD
=======
	dflt->stable_secret.initialized = false;
	all->stable_secret.initialized = false;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	net->ipv6.devconf_all = all;
	net->ipv6.devconf_dflt = dflt;

#ifdef CONFIG_SYSCTL
	err = __addrconf_sysctl_register(net, "all", NULL, all);
	if (err < 0)
		goto err_reg_all;

	err = __addrconf_sysctl_register(net, "default", NULL, dflt);
	if (err < 0)
		goto err_reg_dflt;
#endif
	return 0;

#ifdef CONFIG_SYSCTL
err_reg_dflt:
<<<<<<< HEAD
	__addrconf_sysctl_unregister(all);
err_reg_all:
	kfree(dflt);
#endif
err_alloc_dflt:
	kfree(all);
err_alloc_all:
=======
	__addrconf_sysctl_unregister(net, all, NETCONFA_IFINDEX_ALL);
err_reg_all:
	kfree(dflt);
	net->ipv6.devconf_dflt = NULL;
#endif
err_alloc_dflt:
	kfree(all);
	net->ipv6.devconf_all = NULL;
err_alloc_all:
	kfree(net->ipv6.inet6_addr_lst);
err_alloc_addr:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static void __net_exit addrconf_exit_net(struct net *net)
{
<<<<<<< HEAD
#ifdef CONFIG_SYSCTL
	__addrconf_sysctl_unregister(net->ipv6.devconf_dflt);
	__addrconf_sysctl_unregister(net->ipv6.devconf_all);
#endif
	if (!net_eq(net, &init_net)) {
		kfree(net->ipv6.devconf_dflt);
		kfree(net->ipv6.devconf_all);
	}
=======
	int i;

#ifdef CONFIG_SYSCTL
	__addrconf_sysctl_unregister(net, net->ipv6.devconf_dflt,
				     NETCONFA_IFINDEX_DEFAULT);
	__addrconf_sysctl_unregister(net, net->ipv6.devconf_all,
				     NETCONFA_IFINDEX_ALL);
#endif
	kfree(net->ipv6.devconf_dflt);
	net->ipv6.devconf_dflt = NULL;
	kfree(net->ipv6.devconf_all);
	net->ipv6.devconf_all = NULL;

	cancel_delayed_work_sync(&net->ipv6.addr_chk_work);
	/*
	 *	Check hash table, then free it.
	 */
	for (i = 0; i < IN6_ADDR_HSIZE; i++)
		WARN_ON_ONCE(!hlist_empty(&net->ipv6.inet6_addr_lst[i]));

	kfree(net->ipv6.inet6_addr_lst);
	net->ipv6.inet6_addr_lst = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct pernet_operations addrconf_ops = {
	.init = addrconf_init_net,
	.exit = addrconf_exit_net,
};

<<<<<<< HEAD
/*
 *      Device notifier
 */

int register_inet6addr_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_register(&inet6addr_chain, nb);
}
EXPORT_SYMBOL(register_inet6addr_notifier);

int unregister_inet6addr_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_unregister(&inet6addr_chain, nb);
}
EXPORT_SYMBOL(unregister_inet6addr_notifier);

static struct rtnl_af_ops inet6_ops = {
	.family		  = AF_INET6,
	.fill_link_af	  = inet6_fill_link_af,
	.get_link_af_size = inet6_get_link_af_size,
=======
static struct rtnl_af_ops inet6_ops __read_mostly = {
	.family		  = AF_INET6,
	.fill_link_af	  = inet6_fill_link_af,
	.get_link_af_size = inet6_get_link_af_size,
	.validate_link_af = inet6_validate_link_af,
	.set_link_af	  = inet6_set_link_af,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 *	Init / cleanup code
 */

int __init addrconf_init(void)
{
<<<<<<< HEAD
	int i, err;

	err = ipv6_addr_label_init();
	if (err < 0) {
		printk(KERN_CRIT "IPv6 Addrconf:"
		       " cannot initialize default policy table: %d.\n", err);
=======
	struct inet6_dev *idev;
	int err;

	err = ipv6_addr_label_init();
	if (err < 0) {
		pr_crit("%s: cannot initialize default policy table: %d\n",
			__func__, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	err = register_pernet_subsys(&addrconf_ops);
	if (err < 0)
		goto out_addrlabel;

<<<<<<< HEAD
	/* The addrconf netdev notifier requires that loopback_dev
	 * has it's ipv6 private information allocated and setup
	 * before it can bring up and give link-local addresses
	 * to other devices which are up.
	 *
	 * Unfortunately, loopback_dev is not necessarily the first
	 * entry in the global dev_base list of net devices.  In fact,
	 * it is likely to be the very last entry on that list.
	 * So this causes the notifier registry below to try and
	 * give link-local addresses to all devices besides loopback_dev
	 * first, then loopback_dev, which cases all the non-loopback_dev
	 * devices to fail to get a link-local address.
	 *
	 * So, as a temporary fix, allocate the ipv6 structure for
	 * loopback_dev first by hand.
	 * Longer term, all of the dependencies ipv6 has upon the loopback
	 * device and it being up should be removed.
	 */
	rtnl_lock();
	if (!ipv6_add_dev(init_net.loopback_dev))
		err = -ENOMEM;
	rtnl_unlock();
	if (err)
		goto errlo;

	for (i = 0; i < IN6_ADDR_HSIZE; i++)
		INIT_HLIST_HEAD(&inet6_addr_lst[i]);

	register_netdevice_notifier(&ipv6_dev_notf);

	addrconf_verify(0);

	err = rtnl_af_register(&inet6_ops);
	if (err < 0)
		goto errout_af;

	err = __rtnl_register(PF_INET6, RTM_GETLINK, NULL, inet6_dump_ifinfo,
			      NULL);
	if (err < 0)
		goto errout;

	/* Only the first call to __rtnl_register can fail */
	__rtnl_register(PF_INET6, RTM_NEWADDR, inet6_rtm_newaddr, NULL, NULL);
	__rtnl_register(PF_INET6, RTM_DELADDR, inet6_rtm_deladdr, NULL, NULL);
	__rtnl_register(PF_INET6, RTM_GETADDR, inet6_rtm_getaddr,
			inet6_dump_ifaddr, NULL);
	__rtnl_register(PF_INET6, RTM_GETMULTICAST, NULL,
			inet6_dump_ifmcaddr, NULL);
	__rtnl_register(PF_INET6, RTM_GETANYCAST, NULL,
			inet6_dump_ifacaddr, NULL);

	ipv6_addr_label_rtnl_register();

	return 0;
errout:
	rtnl_af_unregister(&inet6_ops);
errout_af:
	unregister_netdevice_notifier(&ipv6_dev_notf);
errlo:
=======
	/* All works using addrconf_wq need to lock rtnl. */
	addrconf_wq = create_singlethread_workqueue("ipv6_addrconf");
	if (!addrconf_wq) {
		err = -ENOMEM;
		goto out_nowq;
	}

	rtnl_lock();
	idev = ipv6_add_dev(blackhole_netdev);
	rtnl_unlock();
	if (IS_ERR(idev)) {
		err = PTR_ERR(idev);
		goto errlo;
	}

	ip6_route_init_special_entries();

	register_netdevice_notifier(&ipv6_dev_notf);

	addrconf_verify(&init_net);

	rtnl_af_register(&inet6_ops);

	err = rtnl_register_module(THIS_MODULE, PF_INET6, RTM_GETLINK,
				   NULL, inet6_dump_ifinfo, RTNL_FLAG_DUMP_UNLOCKED);
	if (err < 0)
		goto errout;

	err = rtnl_register_module(THIS_MODULE, PF_INET6, RTM_NEWADDR,
				   inet6_rtm_newaddr, NULL, 0);
	if (err < 0)
		goto errout;
	err = rtnl_register_module(THIS_MODULE, PF_INET6, RTM_DELADDR,
				   inet6_rtm_deladdr, NULL, 0);
	if (err < 0)
		goto errout;
	err = rtnl_register_module(THIS_MODULE, PF_INET6, RTM_GETADDR,
				   inet6_rtm_getaddr, inet6_dump_ifaddr,
				   RTNL_FLAG_DOIT_UNLOCKED |
				   RTNL_FLAG_DUMP_UNLOCKED);
	if (err < 0)
		goto errout;
	err = rtnl_register_module(THIS_MODULE, PF_INET6, RTM_GETMULTICAST,
				   NULL, inet6_dump_ifmcaddr,
				   RTNL_FLAG_DUMP_UNLOCKED);
	if (err < 0)
		goto errout;
	err = rtnl_register_module(THIS_MODULE, PF_INET6, RTM_GETANYCAST,
				   NULL, inet6_dump_ifacaddr,
				   RTNL_FLAG_DUMP_UNLOCKED);
	if (err < 0)
		goto errout;
	err = rtnl_register_module(THIS_MODULE, PF_INET6, RTM_GETNETCONF,
				   inet6_netconf_get_devconf,
				   inet6_netconf_dump_devconf,
				   RTNL_FLAG_DOIT_UNLOCKED |
				   RTNL_FLAG_DUMP_UNLOCKED);
	if (err < 0)
		goto errout;
	err = ipv6_addr_label_rtnl_register();
	if (err < 0)
		goto errout;

	return 0;
errout:
	rtnl_unregister_all(PF_INET6);
	rtnl_af_unregister(&inet6_ops);
	unregister_netdevice_notifier(&ipv6_dev_notf);
errlo:
	destroy_workqueue(addrconf_wq);
out_nowq:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unregister_pernet_subsys(&addrconf_ops);
out_addrlabel:
	ipv6_addr_label_cleanup();
out:
	return err;
}

void addrconf_cleanup(void)
{
	struct net_device *dev;
<<<<<<< HEAD
	int i;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	unregister_netdevice_notifier(&ipv6_dev_notf);
	unregister_pernet_subsys(&addrconf_ops);
	ipv6_addr_label_cleanup();

<<<<<<< HEAD
	rtnl_lock();

	__rtnl_af_unregister(&inet6_ops);
=======
	rtnl_af_unregister(&inet6_ops);

	rtnl_lock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* clean dev list */
	for_each_netdev(&init_net, dev) {
		if (__in6_dev_get(dev) == NULL)
			continue;
<<<<<<< HEAD
		addrconf_ifdown(dev, 1);
	}
	addrconf_ifdown(init_net.loopback_dev, 2);

	/*
	 *	Check hash table.
	 */
	spin_lock_bh(&addrconf_hash_lock);
	for (i = 0; i < IN6_ADDR_HSIZE; i++)
		WARN_ON(!hlist_empty(&inet6_addr_lst[i]));
	spin_unlock_bh(&addrconf_hash_lock);

	del_timer(&addr_chk_timer);
	rtnl_unlock();
=======
		addrconf_ifdown(dev, true);
	}
	addrconf_ifdown(init_net.loopback_dev, true);

	rtnl_unlock();

	destroy_workqueue(addrconf_wq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
