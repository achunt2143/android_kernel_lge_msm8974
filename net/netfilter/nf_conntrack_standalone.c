<<<<<<< HEAD
/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2004 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/types.h>
#include <linux/netfilter.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/percpu.h>
#include <linux/netdevice.h>
#include <linux/security.h>
#include <net/net_namespace.h>
#ifdef CONFIG_SYSCTL
#include <linux/sysctl.h>
#endif

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
<<<<<<< HEAD
#include <net/netfilter/nf_conntrack_l3proto.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_acct.h>
#include <net/netfilter/nf_conntrack_zones.h>
#include <net/netfilter/nf_conntrack_timestamp.h>
<<<<<<< HEAD
#include <linux/rculist_nulls.h>

MODULE_LICENSE("GPL");

#ifdef CONFIG_NF_CONNTRACK_PROCFS
int
print_tuple(struct seq_file *s, const struct nf_conntrack_tuple *tuple,
            const struct nf_conntrack_l3proto *l3proto,
            const struct nf_conntrack_l4proto *l4proto)
{
	return l3proto->print_tuple(s, tuple) || l4proto->print_tuple(s, tuple);
=======
#ifdef CONFIG_LWTUNNEL
#include <net/netfilter/nf_hooks_lwtunnel.h>
#endif
#include <linux/rculist_nulls.h>

static bool enable_hooks __read_mostly;
MODULE_PARM_DESC(enable_hooks, "Always enable conntrack hooks");
module_param(enable_hooks, bool, 0000);

unsigned int nf_conntrack_net_id __read_mostly;

#ifdef CONFIG_NF_CONNTRACK_PROCFS
void
print_tuple(struct seq_file *s, const struct nf_conntrack_tuple *tuple,
            const struct nf_conntrack_l4proto *l4proto)
{
	switch (tuple->src.l3num) {
	case NFPROTO_IPV4:
		seq_printf(s, "src=%pI4 dst=%pI4 ",
			   &tuple->src.u3.ip, &tuple->dst.u3.ip);
		break;
	case NFPROTO_IPV6:
		seq_printf(s, "src=%pI6 dst=%pI6 ",
			   tuple->src.u3.ip6, tuple->dst.u3.ip6);
		break;
	default:
		break;
	}

	switch (l4proto->l4proto) {
	case IPPROTO_ICMP:
		seq_printf(s, "type=%u code=%u id=%u ",
			   tuple->dst.u.icmp.type,
			   tuple->dst.u.icmp.code,
			   ntohs(tuple->src.u.icmp.id));
		break;
	case IPPROTO_TCP:
		seq_printf(s, "sport=%hu dport=%hu ",
			   ntohs(tuple->src.u.tcp.port),
			   ntohs(tuple->dst.u.tcp.port));
		break;
	case IPPROTO_UDPLITE:
	case IPPROTO_UDP:
		seq_printf(s, "sport=%hu dport=%hu ",
			   ntohs(tuple->src.u.udp.port),
			   ntohs(tuple->dst.u.udp.port));

		break;
	case IPPROTO_DCCP:
		seq_printf(s, "sport=%hu dport=%hu ",
			   ntohs(tuple->src.u.dccp.port),
			   ntohs(tuple->dst.u.dccp.port));
		break;
	case IPPROTO_SCTP:
		seq_printf(s, "sport=%hu dport=%hu ",
			   ntohs(tuple->src.u.sctp.port),
			   ntohs(tuple->dst.u.sctp.port));
		break;
	case IPPROTO_ICMPV6:
		seq_printf(s, "type=%u code=%u id=%u ",
			   tuple->dst.u.icmp.type,
			   tuple->dst.u.icmp.code,
			   ntohs(tuple->src.u.icmp.id));
		break;
	case IPPROTO_GRE:
		seq_printf(s, "srckey=0x%x dstkey=0x%x ",
			   ntohs(tuple->src.u.gre.key),
			   ntohs(tuple->dst.u.gre.key));
		break;
	default:
		break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(print_tuple);

struct ct_iter_state {
	struct seq_net_private p;
<<<<<<< HEAD
=======
	struct hlist_nulls_head *hash;
	unsigned int htable_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int bucket;
	u_int64_t time_now;
};

static struct hlist_nulls_node *ct_get_first(struct seq_file *seq)
{
<<<<<<< HEAD
	struct net *net = seq_file_net(seq);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ct_iter_state *st = seq->private;
	struct hlist_nulls_node *n;

	for (st->bucket = 0;
<<<<<<< HEAD
	     st->bucket < net->ct.htable_size;
	     st->bucket++) {
		n = rcu_dereference(hlist_nulls_first_rcu(&net->ct.hash[st->bucket]));
=======
	     st->bucket < st->htable_size;
	     st->bucket++) {
		n = rcu_dereference(
			hlist_nulls_first_rcu(&st->hash[st->bucket]));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!is_a_nulls(n))
			return n;
	}
	return NULL;
}

static struct hlist_nulls_node *ct_get_next(struct seq_file *seq,
				      struct hlist_nulls_node *head)
{
<<<<<<< HEAD
	struct net *net = seq_file_net(seq);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ct_iter_state *st = seq->private;

	head = rcu_dereference(hlist_nulls_next_rcu(head));
	while (is_a_nulls(head)) {
		if (likely(get_nulls_value(head) == st->bucket)) {
<<<<<<< HEAD
			if (++st->bucket >= net->ct.htable_size)
				return NULL;
		}
		head = rcu_dereference(
				hlist_nulls_first_rcu(
					&net->ct.hash[st->bucket]));
=======
			if (++st->bucket >= st->htable_size)
				return NULL;
		}
		head = rcu_dereference(
			hlist_nulls_first_rcu(&st->hash[st->bucket]));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return head;
}

static struct hlist_nulls_node *ct_get_idx(struct seq_file *seq, loff_t pos)
{
	struct hlist_nulls_node *head = ct_get_first(seq);

	if (head)
		while (pos && (head = ct_get_next(seq, head)))
			pos--;
	return pos ? NULL : head;
}

static void *ct_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(RCU)
{
	struct ct_iter_state *st = seq->private;

<<<<<<< HEAD
	st->time_now = ktime_to_ns(ktime_get_real());
	rcu_read_lock();
=======
	st->time_now = ktime_get_real_ns();
	rcu_read_lock();

	nf_conntrack_get_ht(&st->hash, &st->htable_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ct_get_idx(seq, *pos);
}

static void *ct_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	(*pos)++;
	return ct_get_next(s, v);
}

static void ct_seq_stop(struct seq_file *s, void *v)
	__releases(RCU)
{
	rcu_read_unlock();
}

#ifdef CONFIG_NF_CONNTRACK_SECMARK
<<<<<<< HEAD
static int ct_show_secctx(struct seq_file *s, const struct nf_conn *ct)
=======
static void ct_show_secctx(struct seq_file *s, const struct nf_conn *ct)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret;
	u32 len;
	char *secctx;

	ret = security_secid_to_secctx(ct->secmark, &secctx, &len);
	if (ret)
<<<<<<< HEAD
		return 0;

	ret = seq_printf(s, "secctx=%s ", secctx);

	security_release_secctx(secctx, len);
	return ret;
}
#else
static inline int ct_show_secctx(struct seq_file *s, const struct nf_conn *ct)
{
	return 0;
=======
		return;

	seq_printf(s, "secctx=%s ", secctx);

	security_release_secctx(secctx, len);
}
#else
static inline void ct_show_secctx(struct seq_file *s, const struct nf_conn *ct)
{
}
#endif

#ifdef CONFIG_NF_CONNTRACK_ZONES
static void ct_show_zone(struct seq_file *s, const struct nf_conn *ct,
			 int dir)
{
	const struct nf_conntrack_zone *zone = nf_ct_zone(ct);

	if (zone->dir != dir)
		return;
	switch (zone->dir) {
	case NF_CT_DEFAULT_ZONE_DIR:
		seq_printf(s, "zone=%u ", zone->id);
		break;
	case NF_CT_ZONE_DIR_ORIG:
		seq_printf(s, "zone-orig=%u ", zone->id);
		break;
	case NF_CT_ZONE_DIR_REPL:
		seq_printf(s, "zone-reply=%u ", zone->id);
		break;
	default:
		break;
	}
}
#else
static inline void ct_show_zone(struct seq_file *s, const struct nf_conn *ct,
				int dir)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

#ifdef CONFIG_NF_CONNTRACK_TIMESTAMP
<<<<<<< HEAD
static int ct_show_delta_time(struct seq_file *s, const struct nf_conn *ct)
=======
static void ct_show_delta_time(struct seq_file *s, const struct nf_conn *ct)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ct_iter_state *st = s->private;
	struct nf_conn_tstamp *tstamp;
	s64 delta_time;

	tstamp = nf_conn_tstamp_find(ct);
	if (tstamp) {
		delta_time = st->time_now - tstamp->start;
		if (delta_time > 0)
			delta_time = div_s64(delta_time, NSEC_PER_SEC);
		else
			delta_time = 0;

<<<<<<< HEAD
		return seq_printf(s, "delta-time=%llu ",
				  (unsigned long long)delta_time);
	}
	return 0;
}
#else
static inline int
ct_show_delta_time(struct seq_file *s, const struct nf_conn *ct)
{
	return 0;
}
#endif

=======
		seq_printf(s, "delta-time=%llu ",
			   (unsigned long long)delta_time);
	}
	return;
}
#else
static inline void
ct_show_delta_time(struct seq_file *s, const struct nf_conn *ct)
{
}
#endif

static const char* l3proto_name(u16 proto)
{
	switch (proto) {
	case AF_INET: return "ipv4";
	case AF_INET6: return "ipv6";
	}

	return "unknown";
}

static const char* l4proto_name(u16 proto)
{
	switch (proto) {
	case IPPROTO_ICMP: return "icmp";
	case IPPROTO_TCP: return "tcp";
	case IPPROTO_UDP: return "udp";
	case IPPROTO_DCCP: return "dccp";
	case IPPROTO_GRE: return "gre";
	case IPPROTO_SCTP: return "sctp";
	case IPPROTO_UDPLITE: return "udplite";
	case IPPROTO_ICMPV6: return "icmpv6";
	}

	return "unknown";
}

static void
seq_print_acct(struct seq_file *s, const struct nf_conn *ct, int dir)
{
	struct nf_conn_acct *acct;
	struct nf_conn_counter *counter;

	acct = nf_conn_acct_find(ct);
	if (!acct)
		return;

	counter = acct->counter;
	seq_printf(s, "packets=%llu bytes=%llu ",
		   (unsigned long long)atomic64_read(&counter[dir].packets),
		   (unsigned long long)atomic64_read(&counter[dir].bytes));
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* return 0 on success, 1 in case of error */
static int ct_seq_show(struct seq_file *s, void *v)
{
	struct nf_conntrack_tuple_hash *hash = v;
	struct nf_conn *ct = nf_ct_tuplehash_to_ctrack(hash);
<<<<<<< HEAD
	const struct nf_conntrack_l3proto *l3proto;
	const struct nf_conntrack_l4proto *l4proto;
	int ret = 0;

	NF_CT_ASSERT(ct);
	if (unlikely(!atomic_inc_not_zero(&ct->ct_general.use)))
		return 0;

=======
	const struct nf_conntrack_l4proto *l4proto;
	struct net *net = seq_file_net(s);
	int ret = 0;

	WARN_ON(!ct);
	if (unlikely(!refcount_inc_not_zero(&ct->ct_general.use)))
		return 0;

	/* load ->status after refcount increase */
	smp_acquire__after_ctrl_dep();

	if (nf_ct_should_gc(ct)) {
		nf_ct_kill(ct);
		goto release;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* we only want to print DIR_ORIGINAL */
	if (NF_CT_DIRECTION(hash))
		goto release;

<<<<<<< HEAD
	l3proto = __nf_ct_l3proto_find(nf_ct_l3num(ct));
	NF_CT_ASSERT(l3proto);
	l4proto = __nf_ct_l4proto_find(nf_ct_l3num(ct), nf_ct_protonum(ct));
	NF_CT_ASSERT(l4proto);

	ret = -ENOSPC;
	if (seq_printf(s, "%-8s %u %-8s %u %ld ",
		       l3proto->name, nf_ct_l3num(ct),
		       l4proto->name, nf_ct_protonum(ct),
		       timer_pending(&ct->timeout)
		       ? (long)(ct->timeout.expires - jiffies)/HZ : 0) != 0)
		goto release;

	if (l4proto->print_conntrack && l4proto->print_conntrack(s, ct))
		goto release;

	if (print_tuple(s, &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple,
			l3proto, l4proto))
		goto release;

	if (seq_print_acct(s, ct, IP_CT_DIR_ORIGINAL))
		goto release;

	if (!(test_bit(IPS_SEEN_REPLY_BIT, &ct->status)))
		if (seq_printf(s, "[UNREPLIED] "))
			goto release;

	if (print_tuple(s, &ct->tuplehash[IP_CT_DIR_REPLY].tuple,
			l3proto, l4proto))
		goto release;

	if (seq_print_acct(s, ct, IP_CT_DIR_REPLY))
		goto release;

	if (test_bit(IPS_ASSURED_BIT, &ct->status))
		if (seq_printf(s, "[ASSURED] "))
			goto release;

#if defined(CONFIG_NF_CONNTRACK_MARK)
	if (seq_printf(s, "mark=%u ", ct->mark))
		goto release;
#endif

	if (ct_show_secctx(s, ct))
		goto release;

#ifdef CONFIG_NF_CONNTRACK_ZONES
	if (seq_printf(s, "zone=%u ", nf_ct_zone(ct)))
		goto release;
#endif

	if (ct_show_delta_time(s, ct))
		goto release;

	if (seq_printf(s, "use=%u\n", atomic_read(&ct->ct_general.use)))
=======
	if (!net_eq(nf_ct_net(ct), net))
		goto release;

	l4proto = nf_ct_l4proto_find(nf_ct_protonum(ct));

	ret = -ENOSPC;
	seq_printf(s, "%-8s %u %-8s %u ",
		   l3proto_name(nf_ct_l3num(ct)), nf_ct_l3num(ct),
		   l4proto_name(l4proto->l4proto), nf_ct_protonum(ct));

	if (!test_bit(IPS_OFFLOAD_BIT, &ct->status))
		seq_printf(s, "%ld ", nf_ct_expires(ct)  / HZ);

	if (l4proto->print_conntrack)
		l4proto->print_conntrack(s, ct);

	print_tuple(s, &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple,
		    l4proto);

	ct_show_zone(s, ct, NF_CT_ZONE_DIR_ORIG);

	if (seq_has_overflowed(s))
		goto release;

	seq_print_acct(s, ct, IP_CT_DIR_ORIGINAL);

	if (!(test_bit(IPS_SEEN_REPLY_BIT, &ct->status)))
		seq_puts(s, "[UNREPLIED] ");

	print_tuple(s, &ct->tuplehash[IP_CT_DIR_REPLY].tuple, l4proto);

	ct_show_zone(s, ct, NF_CT_ZONE_DIR_REPL);

	seq_print_acct(s, ct, IP_CT_DIR_REPLY);

	if (test_bit(IPS_HW_OFFLOAD_BIT, &ct->status))
		seq_puts(s, "[HW_OFFLOAD] ");
	else if (test_bit(IPS_OFFLOAD_BIT, &ct->status))
		seq_puts(s, "[OFFLOAD] ");
	else if (test_bit(IPS_ASSURED_BIT, &ct->status))
		seq_puts(s, "[ASSURED] ");

	if (seq_has_overflowed(s))
		goto release;

#if defined(CONFIG_NF_CONNTRACK_MARK)
	seq_printf(s, "mark=%u ", READ_ONCE(ct->mark));
#endif

	ct_show_secctx(s, ct);
	ct_show_zone(s, ct, NF_CT_DEFAULT_ZONE_DIR);
	ct_show_delta_time(s, ct);

	seq_printf(s, "use=%u\n", refcount_read(&ct->ct_general.use));

	if (seq_has_overflowed(s))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto release;

	ret = 0;
release:
	nf_ct_put(ct);
	return ret;
}

static const struct seq_operations ct_seq_ops = {
	.start = ct_seq_start,
	.next  = ct_seq_next,
	.stop  = ct_seq_stop,
	.show  = ct_seq_show
};

<<<<<<< HEAD
static int ct_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &ct_seq_ops,
			sizeof(struct ct_iter_state));
}

static const struct file_operations ct_file_ops = {
	.owner   = THIS_MODULE,
	.open    = ct_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_net,
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void *ct_cpu_seq_start(struct seq_file *seq, loff_t *pos)
{
	struct net *net = seq_file_net(seq);
	int cpu;

	if (*pos == 0)
		return SEQ_START_TOKEN;

	for (cpu = *pos-1; cpu < nr_cpu_ids; ++cpu) {
		if (!cpu_possible(cpu))
			continue;
		*pos = cpu + 1;
		return per_cpu_ptr(net->ct.stat, cpu);
	}

	return NULL;
}

static void *ct_cpu_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct net *net = seq_file_net(seq);
	int cpu;

	for (cpu = *pos; cpu < nr_cpu_ids; ++cpu) {
		if (!cpu_possible(cpu))
			continue;
		*pos = cpu + 1;
		return per_cpu_ptr(net->ct.stat, cpu);
	}
<<<<<<< HEAD

=======
	(*pos)++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NULL;
}

static void ct_cpu_seq_stop(struct seq_file *seq, void *v)
{
}

static int ct_cpu_seq_show(struct seq_file *seq, void *v)
{
	struct net *net = seq_file_net(seq);
<<<<<<< HEAD
	unsigned int nr_conntracks = atomic_read(&net->ct.count);
	const struct ip_conntrack_stat *st = v;

	if (v == SEQ_START_TOKEN) {
		seq_printf(seq, "entries  searched found new invalid ignore delete delete_list insert insert_failed drop early_drop icmp_error  expect_new expect_create expect_delete search_restart\n");
		return 0;
	}

	seq_printf(seq, "%08x  %08x %08x %08x %08x %08x %08x %08x "
			"%08x %08x %08x %08x %08x  %08x %08x %08x %08x\n",
		   nr_conntracks,
		   st->searched,
		   st->found,
		   st->new,
		   st->invalid,
		   st->ignore,
		   st->delete,
		   st->delete_list,
=======
	const struct ip_conntrack_stat *st = v;
	unsigned int nr_conntracks;

	if (v == SEQ_START_TOKEN) {
		seq_puts(seq, "entries  clashres found new invalid ignore delete chainlength insert insert_failed drop early_drop icmp_error  expect_new expect_create expect_delete search_restart\n");
		return 0;
	}

	nr_conntracks = nf_conntrack_count(net);

	seq_printf(seq, "%08x  %08x %08x %08x %08x %08x %08x %08x "
			"%08x %08x %08x %08x %08x  %08x %08x %08x %08x\n",
		   nr_conntracks,
		   st->clash_resolve,
		   st->found,
		   0,
		   st->invalid,
		   0,
		   0,
		   st->chaintoolong,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		   st->insert,
		   st->insert_failed,
		   st->drop,
		   st->early_drop,
		   st->error,

		   st->expect_new,
		   st->expect_create,
		   st->expect_delete,
		   st->search_restart
		);
	return 0;
}

static const struct seq_operations ct_cpu_seq_ops = {
	.start	= ct_cpu_seq_start,
	.next	= ct_cpu_seq_next,
	.stop	= ct_cpu_seq_stop,
	.show	= ct_cpu_seq_show,
};

<<<<<<< HEAD
static int ct_cpu_seq_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &ct_cpu_seq_ops,
			    sizeof(struct seq_net_private));
}

static const struct file_operations ct_cpu_seq_fops = {
	.owner	 = THIS_MODULE,
	.open	 = ct_cpu_seq_open,
	.read	 = seq_read,
	.llseek	 = seq_lseek,
	.release = seq_release_net,
};

static int nf_conntrack_standalone_init_proc(struct net *net)
{
	struct proc_dir_entry *pde;

	pde = proc_net_fops_create(net, "nf_conntrack", 0440, &ct_file_ops);
	if (!pde)
		goto out_nf_conntrack;

	pde = proc_create("nf_conntrack", S_IRUGO, net->proc_net_stat,
			  &ct_cpu_seq_fops);
=======
static int nf_conntrack_standalone_init_proc(struct net *net)
{
	struct proc_dir_entry *pde;
	kuid_t root_uid;
	kgid_t root_gid;

	pde = proc_create_net("nf_conntrack", 0440, net->proc_net, &ct_seq_ops,
			sizeof(struct ct_iter_state));
	if (!pde)
		goto out_nf_conntrack;

	root_uid = make_kuid(net->user_ns, 0);
	root_gid = make_kgid(net->user_ns, 0);
	if (uid_valid(root_uid) && gid_valid(root_gid))
		proc_set_user(pde, root_uid, root_gid);

	pde = proc_create_net("nf_conntrack", 0444, net->proc_net_stat,
			&ct_cpu_seq_ops, sizeof(struct seq_net_private));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!pde)
		goto out_stat_nf_conntrack;
	return 0;

out_stat_nf_conntrack:
<<<<<<< HEAD
	proc_net_remove(net, "nf_conntrack");
=======
	remove_proc_entry("nf_conntrack", net->proc_net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_nf_conntrack:
	return -ENOMEM;
}

static void nf_conntrack_standalone_fini_proc(struct net *net)
{
	remove_proc_entry("nf_conntrack", net->proc_net_stat);
<<<<<<< HEAD
	proc_net_remove(net, "nf_conntrack");
=======
	remove_proc_entry("nf_conntrack", net->proc_net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#else
static int nf_conntrack_standalone_init_proc(struct net *net)
{
	return 0;
}

static void nf_conntrack_standalone_fini_proc(struct net *net)
{
}
#endif /* CONFIG_NF_CONNTRACK_PROCFS */

<<<<<<< HEAD
/* Sysctl support */

#ifdef CONFIG_SYSCTL
/* Log invalid packets of a given protocol */
static int log_invalid_proto_min = 0;
static int log_invalid_proto_max = 255;

static struct ctl_table_header *nf_ct_netfilter_header;

static ctl_table nf_ct_sysctl_table[] = {
	{
=======
u32 nf_conntrack_count(const struct net *net)
{
	const struct nf_conntrack_net *cnet = nf_ct_pernet(net);

	return atomic_read(&cnet->count);
}
EXPORT_SYMBOL_GPL(nf_conntrack_count);

/* Sysctl support */

#ifdef CONFIG_SYSCTL
/* size the user *wants to set */
static unsigned int nf_conntrack_htable_size_user __read_mostly;

static int
nf_conntrack_hash_sysctl(struct ctl_table *table, int write,
			 void *buffer, size_t *lenp, loff_t *ppos)
{
	int ret;

	/* module_param hashsize could have changed value */
	nf_conntrack_htable_size_user = nf_conntrack_htable_size;

	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	if (ret < 0 || !write)
		return ret;

	/* update ret, we might not be able to satisfy request */
	ret = nf_conntrack_hash_resize(nf_conntrack_htable_size_user);

	/* update it to the actual value used by conntrack */
	nf_conntrack_htable_size_user = nf_conntrack_htable_size;
	return ret;
}

static struct ctl_table_header *nf_ct_netfilter_header;

enum nf_ct_sysctl_index {
	NF_SYSCTL_CT_MAX,
	NF_SYSCTL_CT_COUNT,
	NF_SYSCTL_CT_BUCKETS,
	NF_SYSCTL_CT_CHECKSUM,
	NF_SYSCTL_CT_LOG_INVALID,
	NF_SYSCTL_CT_EXPECT_MAX,
	NF_SYSCTL_CT_ACCT,
#ifdef CONFIG_NF_CONNTRACK_EVENTS
	NF_SYSCTL_CT_EVENTS,
#endif
#ifdef CONFIG_NF_CONNTRACK_TIMESTAMP
	NF_SYSCTL_CT_TIMESTAMP,
#endif
	NF_SYSCTL_CT_PROTO_TIMEOUT_GENERIC,
	NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_SYN_SENT,
	NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_SYN_RECV,
	NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_ESTABLISHED,
	NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_FIN_WAIT,
	NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_CLOSE_WAIT,
	NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_LAST_ACK,
	NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_TIME_WAIT,
	NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_CLOSE,
	NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_RETRANS,
	NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_UNACK,
#if IS_ENABLED(CONFIG_NF_FLOW_TABLE)
	NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_OFFLOAD,
#endif
	NF_SYSCTL_CT_PROTO_TCP_LOOSE,
	NF_SYSCTL_CT_PROTO_TCP_LIBERAL,
	NF_SYSCTL_CT_PROTO_TCP_IGNORE_INVALID_RST,
	NF_SYSCTL_CT_PROTO_TCP_MAX_RETRANS,
	NF_SYSCTL_CT_PROTO_TIMEOUT_UDP,
	NF_SYSCTL_CT_PROTO_TIMEOUT_UDP_STREAM,
#if IS_ENABLED(CONFIG_NF_FLOW_TABLE)
	NF_SYSCTL_CT_PROTO_TIMEOUT_UDP_OFFLOAD,
#endif
	NF_SYSCTL_CT_PROTO_TIMEOUT_ICMP,
	NF_SYSCTL_CT_PROTO_TIMEOUT_ICMPV6,
#ifdef CONFIG_NF_CT_PROTO_SCTP
	NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_CLOSED,
	NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_COOKIE_WAIT,
	NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_COOKIE_ECHOED,
	NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_ESTABLISHED,
	NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_SHUTDOWN_SENT,
	NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_SHUTDOWN_RECD,
	NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_SHUTDOWN_ACK_SENT,
	NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_HEARTBEAT_SENT,
#endif
#ifdef CONFIG_NF_CT_PROTO_DCCP
	NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_REQUEST,
	NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_RESPOND,
	NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_PARTOPEN,
	NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_OPEN,
	NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_CLOSEREQ,
	NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_CLOSING,
	NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_TIMEWAIT,
	NF_SYSCTL_CT_PROTO_DCCP_LOOSE,
#endif
#ifdef CONFIG_NF_CT_PROTO_GRE
	NF_SYSCTL_CT_PROTO_TIMEOUT_GRE,
	NF_SYSCTL_CT_PROTO_TIMEOUT_GRE_STREAM,
#endif
#ifdef CONFIG_LWTUNNEL
	NF_SYSCTL_CT_LWTUNNEL,
#endif

	__NF_SYSCTL_CT_LAST_SYSCTL,
};

#define NF_SYSCTL_CT_LAST_SYSCTL (__NF_SYSCTL_CT_LAST_SYSCTL + 1)

static struct ctl_table nf_ct_sysctl_table[] = {
	[NF_SYSCTL_CT_MAX] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.procname	= "nf_conntrack_max",
		.data		= &nf_conntrack_max,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
<<<<<<< HEAD
	{
		.procname	= "nf_conntrack_count",
		.data		= &init_net.ct.count,
=======
	[NF_SYSCTL_CT_COUNT] = {
		.procname	= "nf_conntrack_count",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.maxlen		= sizeof(int),
		.mode		= 0444,
		.proc_handler	= proc_dointvec,
	},
<<<<<<< HEAD
	{
		.procname       = "nf_conntrack_buckets",
		.data           = &init_net.ct.htable_size,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0444,
		.proc_handler   = proc_dointvec,
	},
	{
		.procname	= "nf_conntrack_checksum",
		.data		= &init_net.ct.sysctl_checksum,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "nf_conntrack_log_invalid",
		.data		= &init_net.ct.sysctl_log_invalid,
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &log_invalid_proto_min,
		.extra2		= &log_invalid_proto_max,
	},
	{
=======
	[NF_SYSCTL_CT_BUCKETS] = {
		.procname       = "nf_conntrack_buckets",
		.data           = &nf_conntrack_htable_size_user,
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = nf_conntrack_hash_sysctl,
	},
	[NF_SYSCTL_CT_CHECKSUM] = {
		.procname	= "nf_conntrack_checksum",
		.data		= &init_net.ct.sysctl_checksum,
		.maxlen		= sizeof(u8),
		.mode		= 0644,
		.proc_handler	= proc_dou8vec_minmax,
		.extra1 	= SYSCTL_ZERO,
		.extra2 	= SYSCTL_ONE,
	},
	[NF_SYSCTL_CT_LOG_INVALID] = {
		.procname	= "nf_conntrack_log_invalid",
		.data		= &init_net.ct.sysctl_log_invalid,
		.maxlen		= sizeof(u8),
		.mode		= 0644,
		.proc_handler	= proc_dou8vec_minmax,
	},
	[NF_SYSCTL_CT_EXPECT_MAX] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.procname	= "nf_conntrack_expect_max",
		.data		= &nf_ct_expect_max,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
<<<<<<< HEAD
	{ }
};

#define NET_NF_CONNTRACK_MAX 2089

static ctl_table nf_ct_netfilter_table[] = {
=======
	[NF_SYSCTL_CT_ACCT] = {
		.procname	= "nf_conntrack_acct",
		.data		= &init_net.ct.sysctl_acct,
		.maxlen		= sizeof(u8),
		.mode		= 0644,
		.proc_handler	= proc_dou8vec_minmax,
		.extra1 	= SYSCTL_ZERO,
		.extra2 	= SYSCTL_ONE,
	},
#ifdef CONFIG_NF_CONNTRACK_EVENTS
	[NF_SYSCTL_CT_EVENTS] = {
		.procname	= "nf_conntrack_events",
		.data		= &init_net.ct.sysctl_events,
		.maxlen		= sizeof(u8),
		.mode		= 0644,
		.proc_handler	= proc_dou8vec_minmax,
		.extra1 	= SYSCTL_ZERO,
		.extra2		= SYSCTL_TWO,
	},
#endif
#ifdef CONFIG_NF_CONNTRACK_TIMESTAMP
	[NF_SYSCTL_CT_TIMESTAMP] = {
		.procname	= "nf_conntrack_timestamp",
		.data		= &init_net.ct.sysctl_tstamp,
		.maxlen		= sizeof(u8),
		.mode		= 0644,
		.proc_handler	= proc_dou8vec_minmax,
		.extra1 	= SYSCTL_ZERO,
		.extra2 	= SYSCTL_ONE,
	},
#endif
	[NF_SYSCTL_CT_PROTO_TIMEOUT_GENERIC] = {
		.procname	= "nf_conntrack_generic_timeout",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_SYN_SENT] = {
		.procname	= "nf_conntrack_tcp_timeout_syn_sent",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_SYN_RECV] = {
		.procname	= "nf_conntrack_tcp_timeout_syn_recv",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_ESTABLISHED] = {
		.procname	= "nf_conntrack_tcp_timeout_established",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_FIN_WAIT] = {
		.procname	= "nf_conntrack_tcp_timeout_fin_wait",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_CLOSE_WAIT] = {
		.procname	= "nf_conntrack_tcp_timeout_close_wait",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_LAST_ACK] = {
		.procname	= "nf_conntrack_tcp_timeout_last_ack",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_TIME_WAIT] = {
		.procname	= "nf_conntrack_tcp_timeout_time_wait",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_CLOSE] = {
		.procname	= "nf_conntrack_tcp_timeout_close",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_RETRANS] = {
		.procname	= "nf_conntrack_tcp_timeout_max_retrans",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_UNACK] = {
		.procname	= "nf_conntrack_tcp_timeout_unacknowledged",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
#if IS_ENABLED(CONFIG_NF_FLOW_TABLE)
	[NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_OFFLOAD] = {
		.procname	= "nf_flowtable_tcp_timeout",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
#endif
	[NF_SYSCTL_CT_PROTO_TCP_LOOSE] = {
		.procname	= "nf_conntrack_tcp_loose",
		.maxlen		= sizeof(u8),
		.mode		= 0644,
		.proc_handler	= proc_dou8vec_minmax,
		.extra1 	= SYSCTL_ZERO,
		.extra2 	= SYSCTL_ONE,
	},
	[NF_SYSCTL_CT_PROTO_TCP_LIBERAL] = {
		.procname       = "nf_conntrack_tcp_be_liberal",
		.maxlen		= sizeof(u8),
		.mode           = 0644,
		.proc_handler	= proc_dou8vec_minmax,
		.extra1 	= SYSCTL_ZERO,
		.extra2 	= SYSCTL_ONE,
	},
	[NF_SYSCTL_CT_PROTO_TCP_IGNORE_INVALID_RST] = {
		.procname	= "nf_conntrack_tcp_ignore_invalid_rst",
		.maxlen		= sizeof(u8),
		.mode		= 0644,
		.proc_handler	= proc_dou8vec_minmax,
		.extra1		= SYSCTL_ZERO,
		.extra2		= SYSCTL_ONE,
	},
	[NF_SYSCTL_CT_PROTO_TCP_MAX_RETRANS] = {
		.procname	= "nf_conntrack_tcp_max_retrans",
		.maxlen		= sizeof(u8),
		.mode		= 0644,
		.proc_handler	= proc_dou8vec_minmax,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_UDP] = {
		.procname	= "nf_conntrack_udp_timeout",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_UDP_STREAM] = {
		.procname	= "nf_conntrack_udp_timeout_stream",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
#if IS_ENABLED(CONFIG_NF_FLOW_TABLE)
	[NF_SYSCTL_CT_PROTO_TIMEOUT_UDP_OFFLOAD] = {
		.procname	= "nf_flowtable_udp_timeout",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
#endif
	[NF_SYSCTL_CT_PROTO_TIMEOUT_ICMP] = {
		.procname	= "nf_conntrack_icmp_timeout",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_ICMPV6] = {
		.procname	= "nf_conntrack_icmpv6_timeout",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
#ifdef CONFIG_NF_CT_PROTO_SCTP
	[NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_CLOSED] = {
		.procname	= "nf_conntrack_sctp_timeout_closed",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_COOKIE_WAIT] = {
		.procname	= "nf_conntrack_sctp_timeout_cookie_wait",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_COOKIE_ECHOED] = {
		.procname	= "nf_conntrack_sctp_timeout_cookie_echoed",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_ESTABLISHED] = {
		.procname	= "nf_conntrack_sctp_timeout_established",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_SHUTDOWN_SENT] = {
		.procname	= "nf_conntrack_sctp_timeout_shutdown_sent",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_SHUTDOWN_RECD] = {
		.procname	= "nf_conntrack_sctp_timeout_shutdown_recd",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_SHUTDOWN_ACK_SENT] = {
		.procname	= "nf_conntrack_sctp_timeout_shutdown_ack_sent",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_HEARTBEAT_SENT] = {
		.procname	= "nf_conntrack_sctp_timeout_heartbeat_sent",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
#endif
#ifdef CONFIG_NF_CT_PROTO_DCCP
	[NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_REQUEST] = {
		.procname	= "nf_conntrack_dccp_timeout_request",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_RESPOND] = {
		.procname	= "nf_conntrack_dccp_timeout_respond",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_PARTOPEN] = {
		.procname	= "nf_conntrack_dccp_timeout_partopen",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_OPEN] = {
		.procname	= "nf_conntrack_dccp_timeout_open",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_CLOSEREQ] = {
		.procname	= "nf_conntrack_dccp_timeout_closereq",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_CLOSING] = {
		.procname	= "nf_conntrack_dccp_timeout_closing",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_TIMEWAIT] = {
		.procname	= "nf_conntrack_dccp_timeout_timewait",
		.maxlen		= sizeof(unsigned int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_DCCP_LOOSE] = {
		.procname	= "nf_conntrack_dccp_loose",
		.maxlen		= sizeof(u8),
		.mode		= 0644,
		.proc_handler	= proc_dou8vec_minmax,
		.extra1 	= SYSCTL_ZERO,
		.extra2 	= SYSCTL_ONE,
	},
#endif
#ifdef CONFIG_NF_CT_PROTO_GRE
	[NF_SYSCTL_CT_PROTO_TIMEOUT_GRE] = {
		.procname       = "nf_conntrack_gre_timeout",
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_jiffies,
	},
	[NF_SYSCTL_CT_PROTO_TIMEOUT_GRE_STREAM] = {
		.procname       = "nf_conntrack_gre_timeout_stream",
		.maxlen         = sizeof(unsigned int),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_jiffies,
	},
#endif
#ifdef CONFIG_LWTUNNEL
	[NF_SYSCTL_CT_LWTUNNEL] = {
		.procname	= "nf_hooks_lwtunnel",
		.data		= NULL,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= nf_hooks_lwtunnel_sysctl_handler,
	},
#endif
	{}
};

static struct ctl_table nf_ct_netfilter_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.procname	= "nf_conntrack_max",
		.data		= &nf_conntrack_max,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{ }
};

<<<<<<< HEAD
static struct ctl_path nf_ct_path[] = {
	{ .procname = "net", },
	{ }
};

static int nf_conntrack_standalone_init_sysctl(struct net *net)
{
	struct ctl_table *table;

	if (net_eq(net, &init_net)) {
		nf_ct_netfilter_header =
		       register_sysctl_paths(nf_ct_path, nf_ct_netfilter_table);
		if (!nf_ct_netfilter_header)
			goto out;
	}
=======
static void nf_conntrack_standalone_init_tcp_sysctl(struct net *net,
						    struct ctl_table *table)
{
	struct nf_tcp_net *tn = nf_tcp_pernet(net);

#define XASSIGN(XNAME, tn) \
	table[NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_ ## XNAME].data = \
			&(tn)->timeouts[TCP_CONNTRACK_ ## XNAME]

	XASSIGN(SYN_SENT, tn);
	XASSIGN(SYN_RECV, tn);
	XASSIGN(ESTABLISHED, tn);
	XASSIGN(FIN_WAIT, tn);
	XASSIGN(CLOSE_WAIT, tn);
	XASSIGN(LAST_ACK, tn);
	XASSIGN(TIME_WAIT, tn);
	XASSIGN(CLOSE, tn);
	XASSIGN(RETRANS, tn);
	XASSIGN(UNACK, tn);
#undef XASSIGN
#define XASSIGN(XNAME, rval) \
	table[NF_SYSCTL_CT_PROTO_TCP_ ## XNAME].data = (rval)

	XASSIGN(LOOSE, &tn->tcp_loose);
	XASSIGN(LIBERAL, &tn->tcp_be_liberal);
	XASSIGN(MAX_RETRANS, &tn->tcp_max_retrans);
	XASSIGN(IGNORE_INVALID_RST, &tn->tcp_ignore_invalid_rst);
#undef XASSIGN

#if IS_ENABLED(CONFIG_NF_FLOW_TABLE)
	table[NF_SYSCTL_CT_PROTO_TIMEOUT_TCP_OFFLOAD].data = &tn->offload_timeout;
#endif

}

static void nf_conntrack_standalone_init_sctp_sysctl(struct net *net,
						     struct ctl_table *table)
{
#ifdef CONFIG_NF_CT_PROTO_SCTP
	struct nf_sctp_net *sn = nf_sctp_pernet(net);

#define XASSIGN(XNAME, sn) \
	table[NF_SYSCTL_CT_PROTO_TIMEOUT_SCTP_ ## XNAME].data = \
			&(sn)->timeouts[SCTP_CONNTRACK_ ## XNAME]

	XASSIGN(CLOSED, sn);
	XASSIGN(COOKIE_WAIT, sn);
	XASSIGN(COOKIE_ECHOED, sn);
	XASSIGN(ESTABLISHED, sn);
	XASSIGN(SHUTDOWN_SENT, sn);
	XASSIGN(SHUTDOWN_RECD, sn);
	XASSIGN(SHUTDOWN_ACK_SENT, sn);
	XASSIGN(HEARTBEAT_SENT, sn);
#undef XASSIGN
#endif
}

static void nf_conntrack_standalone_init_dccp_sysctl(struct net *net,
						     struct ctl_table *table)
{
#ifdef CONFIG_NF_CT_PROTO_DCCP
	struct nf_dccp_net *dn = nf_dccp_pernet(net);

#define XASSIGN(XNAME, dn) \
	table[NF_SYSCTL_CT_PROTO_TIMEOUT_DCCP_ ## XNAME].data = \
			&(dn)->dccp_timeout[CT_DCCP_ ## XNAME]

	XASSIGN(REQUEST, dn);
	XASSIGN(RESPOND, dn);
	XASSIGN(PARTOPEN, dn);
	XASSIGN(OPEN, dn);
	XASSIGN(CLOSEREQ, dn);
	XASSIGN(CLOSING, dn);
	XASSIGN(TIMEWAIT, dn);
#undef XASSIGN

	table[NF_SYSCTL_CT_PROTO_DCCP_LOOSE].data = &dn->dccp_loose;
#endif
}

static void nf_conntrack_standalone_init_gre_sysctl(struct net *net,
						    struct ctl_table *table)
{
#ifdef CONFIG_NF_CT_PROTO_GRE
	struct nf_gre_net *gn = nf_gre_pernet(net);

	table[NF_SYSCTL_CT_PROTO_TIMEOUT_GRE].data = &gn->timeouts[GRE_CT_UNREPLIED];
	table[NF_SYSCTL_CT_PROTO_TIMEOUT_GRE_STREAM].data = &gn->timeouts[GRE_CT_REPLIED];
#endif
}

static int nf_conntrack_standalone_init_sysctl(struct net *net)
{
	struct nf_conntrack_net *cnet = nf_ct_pernet(net);
	struct nf_udp_net *un = nf_udp_pernet(net);
	struct ctl_table *table;

	BUILD_BUG_ON(ARRAY_SIZE(nf_ct_sysctl_table) != NF_SYSCTL_CT_LAST_SYSCTL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	table = kmemdup(nf_ct_sysctl_table, sizeof(nf_ct_sysctl_table),
			GFP_KERNEL);
	if (!table)
<<<<<<< HEAD
		goto out_kmemdup;

	table[1].data = &net->ct.count;
	table[2].data = &net->ct.htable_size;
	table[3].data = &net->ct.sysctl_checksum;
	table[4].data = &net->ct.sysctl_log_invalid;

	net->ct.sysctl_header = register_net_sysctl_table(net,
					nf_net_netfilter_sysctl_path, table);
	if (!net->ct.sysctl_header)
=======
		return -ENOMEM;

	table[NF_SYSCTL_CT_COUNT].data = &cnet->count;
	table[NF_SYSCTL_CT_CHECKSUM].data = &net->ct.sysctl_checksum;
	table[NF_SYSCTL_CT_LOG_INVALID].data = &net->ct.sysctl_log_invalid;
	table[NF_SYSCTL_CT_ACCT].data = &net->ct.sysctl_acct;
#ifdef CONFIG_NF_CONNTRACK_EVENTS
	table[NF_SYSCTL_CT_EVENTS].data = &net->ct.sysctl_events;
#endif
#ifdef CONFIG_NF_CONNTRACK_TIMESTAMP
	table[NF_SYSCTL_CT_TIMESTAMP].data = &net->ct.sysctl_tstamp;
#endif
	table[NF_SYSCTL_CT_PROTO_TIMEOUT_GENERIC].data = &nf_generic_pernet(net)->timeout;
	table[NF_SYSCTL_CT_PROTO_TIMEOUT_ICMP].data = &nf_icmp_pernet(net)->timeout;
	table[NF_SYSCTL_CT_PROTO_TIMEOUT_ICMPV6].data = &nf_icmpv6_pernet(net)->timeout;
	table[NF_SYSCTL_CT_PROTO_TIMEOUT_UDP].data = &un->timeouts[UDP_CT_UNREPLIED];
	table[NF_SYSCTL_CT_PROTO_TIMEOUT_UDP_STREAM].data = &un->timeouts[UDP_CT_REPLIED];
#if IS_ENABLED(CONFIG_NF_FLOW_TABLE)
	table[NF_SYSCTL_CT_PROTO_TIMEOUT_UDP_OFFLOAD].data = &un->offload_timeout;
#endif

	nf_conntrack_standalone_init_tcp_sysctl(net, table);
	nf_conntrack_standalone_init_sctp_sysctl(net, table);
	nf_conntrack_standalone_init_dccp_sysctl(net, table);
	nf_conntrack_standalone_init_gre_sysctl(net, table);

	/* Don't allow non-init_net ns to alter global sysctls */
	if (!net_eq(&init_net, net)) {
		table[NF_SYSCTL_CT_MAX].mode = 0444;
		table[NF_SYSCTL_CT_EXPECT_MAX].mode = 0444;
		table[NF_SYSCTL_CT_BUCKETS].mode = 0444;
	}

	cnet->sysctl_header = register_net_sysctl_sz(net, "net/netfilter",
						     table,
						     ARRAY_SIZE(nf_ct_sysctl_table));
	if (!cnet->sysctl_header)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_unregister_netfilter;

	return 0;

out_unregister_netfilter:
	kfree(table);
<<<<<<< HEAD
out_kmemdup:
	if (net_eq(net, &init_net))
		unregister_sysctl_table(nf_ct_netfilter_header);
out:
	printk(KERN_ERR "nf_conntrack: can't register to sysctl.\n");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -ENOMEM;
}

static void nf_conntrack_standalone_fini_sysctl(struct net *net)
{
<<<<<<< HEAD
	struct ctl_table *table;

	if (net_eq(net, &init_net))
		unregister_sysctl_table(nf_ct_netfilter_header);
	table = net->ct.sysctl_header->ctl_table_arg;
	unregister_net_sysctl_table(net->ct.sysctl_header);
=======
	struct nf_conntrack_net *cnet = nf_ct_pernet(net);
	struct ctl_table *table;

	table = cnet->sysctl_header->ctl_table_arg;
	unregister_net_sysctl_table(cnet->sysctl_header);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(table);
}
#else
static int nf_conntrack_standalone_init_sysctl(struct net *net)
{
	return 0;
}

static void nf_conntrack_standalone_fini_sysctl(struct net *net)
{
}
#endif /* CONFIG_SYSCTL */

<<<<<<< HEAD
static int nf_conntrack_net_init(struct net *net)
{
	int ret;

	ret = nf_conntrack_init(net);
	if (ret < 0)
		goto out_init;
	ret = nf_conntrack_standalone_init_proc(net);
	if (ret < 0)
		goto out_proc;
	net->ct.sysctl_checksum = 1;
	net->ct.sysctl_log_invalid = 0;
	ret = nf_conntrack_standalone_init_sysctl(net);
	if (ret < 0)
		goto out_sysctl;
	return 0;

out_sysctl:
	nf_conntrack_standalone_fini_proc(net);
out_proc:
	nf_conntrack_cleanup(net);
out_init:
	return ret;
}

static void nf_conntrack_net_exit(struct net *net)
{
	nf_conntrack_standalone_fini_sysctl(net);
	nf_conntrack_standalone_fini_proc(net);
	nf_conntrack_cleanup(net);
}

static struct pernet_operations nf_conntrack_net_ops = {
	.init = nf_conntrack_net_init,
	.exit = nf_conntrack_net_exit,
=======
static void nf_conntrack_fini_net(struct net *net)
{
	if (enable_hooks)
		nf_ct_netns_put(net, NFPROTO_INET);

	nf_conntrack_standalone_fini_proc(net);
	nf_conntrack_standalone_fini_sysctl(net);
}

static int nf_conntrack_pernet_init(struct net *net)
{
	int ret;

	net->ct.sysctl_checksum = 1;

	ret = nf_conntrack_standalone_init_sysctl(net);
	if (ret < 0)
		return ret;

	ret = nf_conntrack_standalone_init_proc(net);
	if (ret < 0)
		goto out_proc;

	ret = nf_conntrack_init_net(net);
	if (ret < 0)
		goto out_init_net;

	if (enable_hooks) {
		ret = nf_ct_netns_get(net, NFPROTO_INET);
		if (ret < 0)
			goto out_hooks;
	}

	return 0;

out_hooks:
	nf_conntrack_cleanup_net(net);
out_init_net:
	nf_conntrack_standalone_fini_proc(net);
out_proc:
	nf_conntrack_standalone_fini_sysctl(net);
	return ret;
}

static void nf_conntrack_pernet_exit(struct list_head *net_exit_list)
{
	struct net *net;

	list_for_each_entry(net, net_exit_list, exit_list)
		nf_conntrack_fini_net(net);

	nf_conntrack_cleanup_net_list(net_exit_list);
}

static struct pernet_operations nf_conntrack_net_ops = {
	.init		= nf_conntrack_pernet_init,
	.exit_batch	= nf_conntrack_pernet_exit,
	.id		= &nf_conntrack_net_id,
	.size = sizeof(struct nf_conntrack_net),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init nf_conntrack_standalone_init(void)
{
<<<<<<< HEAD
	return register_pernet_subsys(&nf_conntrack_net_ops);
=======
	int ret = nf_conntrack_init_start();
	if (ret < 0)
		goto out_start;

	BUILD_BUG_ON(NFCT_INFOMASK <= IP_CT_NUMBER);

#ifdef CONFIG_SYSCTL
	nf_ct_netfilter_header =
		register_net_sysctl(&init_net, "net", nf_ct_netfilter_table);
	if (!nf_ct_netfilter_header) {
		pr_err("nf_conntrack: can't register to sysctl.\n");
		ret = -ENOMEM;
		goto out_sysctl;
	}

	nf_conntrack_htable_size_user = nf_conntrack_htable_size;
#endif

	nf_conntrack_init_end();

	ret = register_pernet_subsys(&nf_conntrack_net_ops);
	if (ret < 0)
		goto out_pernet;

	return 0;

out_pernet:
#ifdef CONFIG_SYSCTL
	unregister_net_sysctl_table(nf_ct_netfilter_header);
out_sysctl:
#endif
	nf_conntrack_cleanup_end();
out_start:
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit nf_conntrack_standalone_fini(void)
{
<<<<<<< HEAD
	unregister_pernet_subsys(&nf_conntrack_net_ops);
=======
	nf_conntrack_cleanup_start();
	unregister_pernet_subsys(&nf_conntrack_net_ops);
#ifdef CONFIG_SYSCTL
	unregister_net_sysctl_table(nf_ct_netfilter_header);
#endif
	nf_conntrack_cleanup_end();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(nf_conntrack_standalone_init);
module_exit(nf_conntrack_standalone_fini);
<<<<<<< HEAD

/* Some modules need us, but don't depend directly on any symbol.
   They should call this. */
void need_conntrack(void)
{
}
EXPORT_SYMBOL_GPL(need_conntrack);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
