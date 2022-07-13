<<<<<<< HEAD
=======
/*
 * Rusty Russell (C)2000 -- This code is GPL.
 * Patrick McHardy (c) 2006-2012
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/skbuff.h>
#include <linux/netfilter.h>
<<<<<<< HEAD
=======
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <linux/netfilter_bridge.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/seq_file.h>
#include <linux/rcupdate.h>
#include <net/protocol.h>
#include <net/netfilter/nf_queue.h>
#include <net/dst.h>

#include "nf_internals.h"

<<<<<<< HEAD
/*
 * A queue handler may be registered for each protocol.  Each is protected by
 * long term mutex.  The handler must provide an an outfn() to accept packets
 * for queueing and must reinject all packets it receives, no matter what.
 */
static const struct nf_queue_handler __rcu *queue_handler[NFPROTO_NUMPROTO] __read_mostly;

static DEFINE_MUTEX(queue_handler_mutex);

/* return EBUSY when somebody else is registered, return EEXIST if the
 * same handler is registered, return 0 in case of success. */
int nf_register_queue_handler(u_int8_t pf, const struct nf_queue_handler *qh)
{
	int ret;
	const struct nf_queue_handler *old;

	if (pf >= ARRAY_SIZE(queue_handler))
		return -EINVAL;

	mutex_lock(&queue_handler_mutex);
	old = rcu_dereference_protected(queue_handler[pf],
					lockdep_is_held(&queue_handler_mutex));
	if (old == qh)
		ret = -EEXIST;
	else if (old)
		ret = -EBUSY;
	else {
		rcu_assign_pointer(queue_handler[pf], qh);
		ret = 0;
	}
	mutex_unlock(&queue_handler_mutex);

	return ret;
=======
static const struct nf_queue_handler __rcu *nf_queue_handler;

/*
 * Hook for nfnetlink_queue to register its queue handler.
 * We do this so that most of the NFQUEUE code can be modular.
 *
 * Once the queue is registered it must reinject all packets it
 * receives, no matter what.
 */

void nf_register_queue_handler(const struct nf_queue_handler *qh)
{
	/* should never happen, we only have one queueing backend in kernel */
	WARN_ON(rcu_access_pointer(nf_queue_handler));
	rcu_assign_pointer(nf_queue_handler, qh);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(nf_register_queue_handler);

/* The caller must flush their queue before this */
<<<<<<< HEAD
int nf_unregister_queue_handler(u_int8_t pf, const struct nf_queue_handler *qh)
{
	const struct nf_queue_handler *old;

	if (pf >= ARRAY_SIZE(queue_handler))
		return -EINVAL;

	mutex_lock(&queue_handler_mutex);
	old = rcu_dereference_protected(queue_handler[pf],
					lockdep_is_held(&queue_handler_mutex));
	if (old && old != qh) {
		mutex_unlock(&queue_handler_mutex);
		return -EINVAL;
	}

	RCU_INIT_POINTER(queue_handler[pf], NULL);
	mutex_unlock(&queue_handler_mutex);

	synchronize_rcu();

	return 0;
}
EXPORT_SYMBOL(nf_unregister_queue_handler);

void nf_unregister_queue_handlers(const struct nf_queue_handler *qh)
{
	u_int8_t pf;

	mutex_lock(&queue_handler_mutex);
	for (pf = 0; pf < ARRAY_SIZE(queue_handler); pf++)  {
		if (rcu_dereference_protected(
				queue_handler[pf],
				lockdep_is_held(&queue_handler_mutex)
				) == qh)
			RCU_INIT_POINTER(queue_handler[pf], NULL);
	}
	mutex_unlock(&queue_handler_mutex);

	synchronize_rcu();
}
EXPORT_SYMBOL_GPL(nf_unregister_queue_handlers);

static void nf_queue_entry_release_refs(struct nf_queue_entry *entry)
{
	/* Release those devices we held, or Alexey will kill me. */
	if (entry->indev)
		dev_put(entry->indev);
	if (entry->outdev)
		dev_put(entry->outdev);
#ifdef CONFIG_BRIDGE_NETFILTER
	if (entry->skb->nf_bridge) {
		struct nf_bridge_info *nf_bridge = entry->skb->nf_bridge;

		if (nf_bridge->physindev)
			dev_put(nf_bridge->physindev);
		if (nf_bridge->physoutdev)
			dev_put(nf_bridge->physoutdev);
	}
#endif
	/* Drop reference to owner of hook which queued us. */
	module_put(entry->elem->owner);
}

/*
 * Any packet that leaves via this function must come back
 * through nf_reinject().
 */
static int __nf_queue(struct sk_buff *skb,
		      struct list_head *elem,
		      u_int8_t pf, unsigned int hook,
		      struct net_device *indev,
		      struct net_device *outdev,
		      int (*okfn)(struct sk_buff *),
		      unsigned int queuenum)
{
	int status = -ENOENT;
	struct nf_queue_entry *entry = NULL;
#ifdef CONFIG_BRIDGE_NETFILTER
	struct net_device *physindev;
	struct net_device *physoutdev;
#endif
	const struct nf_afinfo *afinfo;
	const struct nf_queue_handler *qh;

	/* QUEUE == DROP if no one is waiting, to be safe. */
	rcu_read_lock();

	qh = rcu_dereference(queue_handler[pf]);
	if (!qh) {
		status = -ESRCH;
		goto err_unlock;
	}

	afinfo = nf_get_afinfo(pf);
	if (!afinfo)
		goto err_unlock;

	entry = kmalloc(sizeof(*entry) + afinfo->route_key_size, GFP_ATOMIC);
	if (!entry) {
		status = -ENOMEM;
		goto err_unlock;
=======
void nf_unregister_queue_handler(void)
{
	RCU_INIT_POINTER(nf_queue_handler, NULL);
}
EXPORT_SYMBOL(nf_unregister_queue_handler);

static void nf_queue_sock_put(struct sock *sk)
{
#ifdef CONFIG_INET
	sock_gen_put(sk);
#else
	sock_put(sk);
#endif
}

static void nf_queue_entry_release_refs(struct nf_queue_entry *entry)
{
	struct nf_hook_state *state = &entry->state;

	/* Release those devices we held, or Alexey will kill me. */
	dev_put(state->in);
	dev_put(state->out);
	if (state->sk)
		nf_queue_sock_put(state->sk);

#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
	dev_put(entry->physin);
	dev_put(entry->physout);
#endif
}

void nf_queue_entry_free(struct nf_queue_entry *entry)
{
	nf_queue_entry_release_refs(entry);
	kfree(entry);
}
EXPORT_SYMBOL_GPL(nf_queue_entry_free);

static void __nf_queue_entry_init_physdevs(struct nf_queue_entry *entry)
{
#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
	const struct sk_buff *skb = entry->skb;

	if (nf_bridge_info_exists(skb)) {
		entry->physin = nf_bridge_get_physindev(skb, entry->state.net);
		entry->physout = nf_bridge_get_physoutdev(skb);
	} else {
		entry->physin = NULL;
		entry->physout = NULL;
	}
#endif
}

/* Bump dev refs so they don't vanish while packet is out */
bool nf_queue_entry_get_refs(struct nf_queue_entry *entry)
{
	struct nf_hook_state *state = &entry->state;

	if (state->sk && !refcount_inc_not_zero(&state->sk->sk_refcnt))
		return false;

	dev_hold(state->in);
	dev_hold(state->out);

#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
	dev_hold(entry->physin);
	dev_hold(entry->physout);
#endif
	return true;
}
EXPORT_SYMBOL_GPL(nf_queue_entry_get_refs);

void nf_queue_nf_hook_drop(struct net *net)
{
	const struct nf_queue_handler *qh;

	rcu_read_lock();
	qh = rcu_dereference(nf_queue_handler);
	if (qh)
		qh->nf_hook_drop(net);
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(nf_queue_nf_hook_drop);

static void nf_ip_saveroute(const struct sk_buff *skb,
			    struct nf_queue_entry *entry)
{
	struct ip_rt_info *rt_info = nf_queue_entry_reroute(entry);

	if (entry->state.hook == NF_INET_LOCAL_OUT) {
		const struct iphdr *iph = ip_hdr(skb);

		rt_info->tos = iph->tos;
		rt_info->daddr = iph->daddr;
		rt_info->saddr = iph->saddr;
		rt_info->mark = skb->mark;
	}
}

static void nf_ip6_saveroute(const struct sk_buff *skb,
			     struct nf_queue_entry *entry)
{
	struct ip6_rt_info *rt_info = nf_queue_entry_reroute(entry);

	if (entry->state.hook == NF_INET_LOCAL_OUT) {
		const struct ipv6hdr *iph = ipv6_hdr(skb);

		rt_info->daddr = iph->daddr;
		rt_info->saddr = iph->saddr;
		rt_info->mark = skb->mark;
	}
}

static int __nf_queue(struct sk_buff *skb, const struct nf_hook_state *state,
		      unsigned int index, unsigned int queuenum)
{
	struct nf_queue_entry *entry = NULL;
	const struct nf_queue_handler *qh;
	unsigned int route_key_size;
	int status;

	/* QUEUE == DROP if no one is waiting, to be safe. */
	qh = rcu_dereference(nf_queue_handler);
	if (!qh)
		return -ESRCH;

	switch (state->pf) {
	case AF_INET:
		route_key_size = sizeof(struct ip_rt_info);
		break;
	case AF_INET6:
		route_key_size = sizeof(struct ip6_rt_info);
		break;
	default:
		route_key_size = 0;
		break;
	}

	if (skb_sk_is_prefetched(skb)) {
		struct sock *sk = skb->sk;

		if (!sk_is_refcounted(sk)) {
			if (!refcount_inc_not_zero(&sk->sk_refcnt))
				return -ENOTCONN;

			/* drop refcount on skb_orphan */
			skb->destructor = sock_edemux;
		}
	}

	entry = kmalloc(sizeof(*entry) + route_key_size, GFP_ATOMIC);
	if (!entry)
		return -ENOMEM;

	if (skb_dst(skb) && !skb_dst_force(skb)) {
		kfree(entry);
		return -ENETDOWN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	*entry = (struct nf_queue_entry) {
		.skb	= skb,
<<<<<<< HEAD
		.elem	= list_entry(elem, struct nf_hook_ops, list),
		.pf	= pf,
		.hook	= hook,
		.indev	= indev,
		.outdev	= outdev,
		.okfn	= okfn,
	};

	/* If it's going away, ignore hook. */
	if (!try_module_get(entry->elem->owner)) {
		status = -ECANCELED;
		goto err_unlock;
	}
	/* Bump dev refs so they don't vanish while packet is out */
	if (indev)
		dev_hold(indev);
	if (outdev)
		dev_hold(outdev);
#ifdef CONFIG_BRIDGE_NETFILTER
	if (skb->nf_bridge) {
		physindev = skb->nf_bridge->physindev;
		if (physindev)
			dev_hold(physindev);
		physoutdev = skb->nf_bridge->physoutdev;
		if (physoutdev)
			dev_hold(physoutdev);
	}
#endif
	skb_dst_force(skb);
	afinfo->saveroute(skb, entry);
	status = qh->outfn(entry, queuenum);

	rcu_read_unlock();

	if (status < 0) {
		nf_queue_entry_release_refs(entry);
		goto err;
	}

	return 0;

err_unlock:
	rcu_read_unlock();
err:
	kfree(entry);
	return status;
}

#ifdef CONFIG_BRIDGE_NETFILTER
/* When called from bridge netfilter, skb->data must point to MAC header
 * before calling skb_gso_segment(). Else, original MAC header is lost
 * and segmented skbs will be sent to wrong destination.
 */
static void nf_bridge_adjust_skb_data(struct sk_buff *skb)
{
	if (skb->nf_bridge)
		__skb_push(skb, skb->network_header - skb->mac_header);
}

static void nf_bridge_adjust_segmented_data(struct sk_buff *skb)
{
	if (skb->nf_bridge)
		__skb_pull(skb, skb->network_header - skb->mac_header);
}
#else
#define nf_bridge_adjust_skb_data(s) do {} while (0)
#define nf_bridge_adjust_segmented_data(s) do {} while (0)
#endif

int nf_queue(struct sk_buff *skb,
	     struct list_head *elem,
	     u_int8_t pf, unsigned int hook,
	     struct net_device *indev,
	     struct net_device *outdev,
	     int (*okfn)(struct sk_buff *),
	     unsigned int queuenum)
{
	struct sk_buff *segs;
	int err = -EINVAL;
	unsigned int queued;

	if (!skb_is_gso(skb))
		return __nf_queue(skb, elem, pf, hook, indev, outdev, okfn,
				  queuenum);

	switch (pf) {
	case NFPROTO_IPV4:
		skb->protocol = htons(ETH_P_IP);
		break;
	case NFPROTO_IPV6:
		skb->protocol = htons(ETH_P_IPV6);
		break;
	}

	nf_bridge_adjust_skb_data(skb);
	segs = skb_gso_segment(skb, 0);
	/* Does not use PTR_ERR to limit the number of error codes that can be
	 * returned by nf_queue.  For instance, callers rely on -ECANCELED to mean
	 * 'ignore this hook'.
	 */
	if (IS_ERR_OR_NULL(segs))
		goto out_err;
	queued = 0;
	err = 0;
	do {
		struct sk_buff *nskb = segs->next;

		segs->next = NULL;
		if (err == 0) {
			nf_bridge_adjust_segmented_data(segs);
			err = __nf_queue(segs, elem, pf, hook, indev,
					   outdev, okfn, queuenum);
		}
		if (err == 0)
			queued++;
		else
			kfree_skb(segs);
		segs = nskb;
	} while (segs);

	if (queued) {
		kfree_skb(skb);
		return 0;
	}
  out_err:
	nf_bridge_adjust_segmented_data(skb);
	return err;
}

void nf_reinject(struct nf_queue_entry *entry, unsigned int verdict)
{
	struct sk_buff *skb = entry->skb;
	struct list_head *elem = &entry->elem->list;
	const struct nf_afinfo *afinfo;
	int err;

	rcu_read_lock();

	nf_queue_entry_release_refs(entry);

	/* Continue traversal iff userspace said ok... */
	if (verdict == NF_REPEAT) {
		elem = elem->prev;
		verdict = NF_ACCEPT;
	}

	if (verdict == NF_ACCEPT) {
		afinfo = nf_get_afinfo(entry->pf);
		if (!afinfo || afinfo->reroute(skb, entry) < 0)
			verdict = NF_DROP;
	}

	if (verdict == NF_ACCEPT) {
	next_hook:
		verdict = nf_iterate(&nf_hooks[entry->pf][entry->hook],
				     skb, entry->hook,
				     entry->indev, entry->outdev, &elem,
				     entry->okfn, INT_MIN);
	}

	switch (verdict & NF_VERDICT_MASK) {
	case NF_ACCEPT:
	case NF_STOP:
		local_bh_disable();
		entry->okfn(skb);
		local_bh_enable();
		break;
	case NF_QUEUE:
		err = __nf_queue(skb, elem, entry->pf, entry->hook,
				 entry->indev, entry->outdev, entry->okfn,
				 verdict >> NF_VERDICT_QBITS);
		if (err < 0) {
			if (err == -ECANCELED)
				goto next_hook;
			if (err == -ESRCH &&
			   (verdict & NF_VERDICT_FLAG_QUEUE_BYPASS))
				goto next_hook;
			kfree_skb(skb);
		}
		break;
	case NF_STOLEN:
		break;
	default:
		kfree_skb(skb);
	}
	rcu_read_unlock();
	kfree(entry);
}
EXPORT_SYMBOL(nf_reinject);

#ifdef CONFIG_PROC_FS
static void *seq_start(struct seq_file *seq, loff_t *pos)
{
	if (*pos >= ARRAY_SIZE(queue_handler))
		return NULL;

	return pos;
}

static void *seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	(*pos)++;

	if (*pos >= ARRAY_SIZE(queue_handler))
		return NULL;

	return pos;
}

static void seq_stop(struct seq_file *s, void *v)
{

}

static int seq_show(struct seq_file *s, void *v)
{
	int ret;
	loff_t *pos = v;
	const struct nf_queue_handler *qh;

	rcu_read_lock();
	qh = rcu_dereference(queue_handler[*pos]);
	if (!qh)
		ret = seq_printf(s, "%2lld NONE\n", *pos);
	else
		ret = seq_printf(s, "%2lld %s\n", *pos, qh->name);
	rcu_read_unlock();

	return ret;
}

static const struct seq_operations nfqueue_seq_ops = {
	.start	= seq_start,
	.next	= seq_next,
	.stop	= seq_stop,
	.show	= seq_show,
};

static int nfqueue_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &nfqueue_seq_ops);
}

static const struct file_operations nfqueue_file_ops = {
	.owner	 = THIS_MODULE,
	.open	 = nfqueue_open,
	.read	 = seq_read,
	.llseek	 = seq_lseek,
	.release = seq_release,
};
#endif /* PROC_FS */


int __init netfilter_queue_init(void)
{
#ifdef CONFIG_PROC_FS
	if (!proc_create("nf_queue", S_IRUGO,
			 proc_net_netfilter, &nfqueue_file_ops))
		return -1;
#endif
	return 0;
}

=======
		.state	= *state,
		.hook_index = index,
		.size	= sizeof(*entry) + route_key_size,
	};

	__nf_queue_entry_init_physdevs(entry);

	if (!nf_queue_entry_get_refs(entry)) {
		kfree(entry);
		return -ENOTCONN;
	}

	switch (entry->state.pf) {
	case AF_INET:
		nf_ip_saveroute(skb, entry);
		break;
	case AF_INET6:
		nf_ip6_saveroute(skb, entry);
		break;
	}

	status = qh->outfn(entry, queuenum);
	if (status < 0) {
		nf_queue_entry_free(entry);
		return status;
	}

	return 0;
}

/* Packets leaving via this function must come back through nf_reinject(). */
int nf_queue(struct sk_buff *skb, struct nf_hook_state *state,
	     unsigned int index, unsigned int verdict)
{
	int ret;

	ret = __nf_queue(skb, state, index, verdict >> NF_VERDICT_QBITS);
	if (ret < 0) {
		if (ret == -ESRCH &&
		    (verdict & NF_VERDICT_FLAG_QUEUE_BYPASS))
			return 1;
		kfree_skb(skb);
	}

	return 0;
}
EXPORT_SYMBOL_GPL(nf_queue);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
