<<<<<<< HEAD
=======
/* License: GPL */

#include <linux/filter.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mutex.h>
#include <linux/socket.h>
#include <linux/skbuff.h>
#include <net/netlink.h>
#include <net/net_namespace.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/rtnetlink.h>
#include <net/sock.h>

#include <linux/inet_diag.h>
#include <linux/sock_diag.h>

static struct sock_diag_handler *sock_diag_handlers[AF_MAX];
static int (*inet_rcv_compat)(struct sk_buff *skb, struct nlmsghdr *nlh);
static DEFINE_MUTEX(sock_diag_table_mutex);

int sock_diag_check_cookie(void *sk, __u32 *cookie)
{
	if ((cookie[0] != INET_DIAG_NOCOOKIE ||
	     cookie[1] != INET_DIAG_NOCOOKIE) &&
	    ((u32)(unsigned long)sk != cookie[0] ||
	     (u32)((((unsigned long)sk) >> 31) >> 1) != cookie[1]))
		return -ESTALE;
	else
		return 0;
}
EXPORT_SYMBOL_GPL(sock_diag_check_cookie);

void sock_diag_save_cookie(void *sk, __u32 *cookie)
{
	cookie[0] = (u32)(unsigned long)sk;
	cookie[1] = (u32)(((unsigned long)sk >> 31) >> 1);
=======
#include <net/sock.h>
#include <linux/kernel.h>
#include <linux/tcp.h>
#include <linux/workqueue.h>
#include <linux/nospec.h>
#include <linux/cookie.h>
#include <linux/inet_diag.h>
#include <linux/sock_diag.h>

static const struct sock_diag_handler __rcu *sock_diag_handlers[AF_MAX];

static struct sock_diag_inet_compat __rcu *inet_rcv_compat;

static struct workqueue_struct *broadcast_wq;

DEFINE_COOKIE(sock_cookie);

u64 __sock_gen_cookie(struct sock *sk)
{
	u64 res = atomic64_read(&sk->sk_cookie);

	if (!res) {
		u64 new = gen_cookie_next(&sock_cookie);

		atomic64_cmpxchg(&sk->sk_cookie, res, new);

		/* Another thread might have changed sk_cookie before us. */
		res = atomic64_read(&sk->sk_cookie);
	}
	return res;
}

int sock_diag_check_cookie(struct sock *sk, const __u32 *cookie)
{
	u64 res;

	if (cookie[0] == INET_DIAG_NOCOOKIE && cookie[1] == INET_DIAG_NOCOOKIE)
		return 0;

	res = sock_gen_cookie(sk);
	if ((u32)res != cookie[0] || (u32)(res >> 32) != cookie[1])
		return -ESTALE;

	return 0;
}
EXPORT_SYMBOL_GPL(sock_diag_check_cookie);

void sock_diag_save_cookie(struct sock *sk, __u32 *cookie)
{
	u64 res = sock_gen_cookie(sk);

	cookie[0] = (u32)res;
	cookie[1] = (u32)(res >> 32);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(sock_diag_save_cookie);

int sock_diag_put_meminfo(struct sock *sk, struct sk_buff *skb, int attrtype)
{
<<<<<<< HEAD
	__u32 *mem;

	mem = RTA_DATA(__RTA_PUT(skb, attrtype, SK_MEMINFO_VARS * sizeof(__u32)));

	mem[SK_MEMINFO_RMEM_ALLOC] = sk_rmem_alloc_get(sk);
	mem[SK_MEMINFO_RCVBUF] = sk->sk_rcvbuf;
	mem[SK_MEMINFO_WMEM_ALLOC] = sk_wmem_alloc_get(sk);
	mem[SK_MEMINFO_SNDBUF] = sk->sk_sndbuf;
	mem[SK_MEMINFO_FWD_ALLOC] = sk->sk_forward_alloc;
	mem[SK_MEMINFO_WMEM_QUEUED] = sk->sk_wmem_queued;
	mem[SK_MEMINFO_OPTMEM] = atomic_read(&sk->sk_omem_alloc);

	return 0;

rtattr_failure:
	return -EMSGSIZE;
}
EXPORT_SYMBOL_GPL(sock_diag_put_meminfo);

void sock_diag_register_inet_compat(int (*fn)(struct sk_buff *skb, struct nlmsghdr *nlh))
{
	mutex_lock(&sock_diag_table_mutex);
	inet_rcv_compat = fn;
	mutex_unlock(&sock_diag_table_mutex);
}
EXPORT_SYMBOL_GPL(sock_diag_register_inet_compat);

void sock_diag_unregister_inet_compat(int (*fn)(struct sk_buff *skb, struct nlmsghdr *nlh))
{
	mutex_lock(&sock_diag_table_mutex);
	inet_rcv_compat = NULL;
	mutex_unlock(&sock_diag_table_mutex);
}
EXPORT_SYMBOL_GPL(sock_diag_unregister_inet_compat);

int sock_diag_register(struct sock_diag_handler *hndl)
{
	int err = 0;

	if (hndl->family >= AF_MAX)
		return -EINVAL;

	mutex_lock(&sock_diag_table_mutex);
	if (sock_diag_handlers[hndl->family])
		err = -EBUSY;
	else
		sock_diag_handlers[hndl->family] = hndl;
	mutex_unlock(&sock_diag_table_mutex);

	return err;
}
EXPORT_SYMBOL_GPL(sock_diag_register);

void sock_diag_unregister(struct sock_diag_handler *hnld)
{
	int family = hnld->family;
=======
	u32 mem[SK_MEMINFO_VARS];

	sk_get_meminfo(sk, mem);

	return nla_put(skb, attrtype, sizeof(mem), &mem);
}
EXPORT_SYMBOL_GPL(sock_diag_put_meminfo);

int sock_diag_put_filterinfo(bool may_report_filterinfo, struct sock *sk,
			     struct sk_buff *skb, int attrtype)
{
	struct sock_fprog_kern *fprog;
	struct sk_filter *filter;
	struct nlattr *attr;
	unsigned int flen;
	int err = 0;

	if (!may_report_filterinfo) {
		nla_reserve(skb, attrtype, 0);
		return 0;
	}

	rcu_read_lock();
	filter = rcu_dereference(sk->sk_filter);
	if (!filter)
		goto out;

	fprog = filter->prog->orig_prog;
	if (!fprog)
		goto out;

	flen = bpf_classic_proglen(fprog);

	attr = nla_reserve(skb, attrtype, flen);
	if (attr == NULL) {
		err = -EMSGSIZE;
		goto out;
	}

	memcpy(nla_data(attr), fprog->filter, flen);
out:
	rcu_read_unlock();
	return err;
}
EXPORT_SYMBOL(sock_diag_put_filterinfo);

struct broadcast_sk {
	struct sock *sk;
	struct work_struct work;
};

static size_t sock_diag_nlmsg_size(void)
{
	return NLMSG_ALIGN(sizeof(struct inet_diag_msg)
	       + nla_total_size(sizeof(u8)) /* INET_DIAG_PROTOCOL */
	       + nla_total_size_64bit(sizeof(struct tcp_info))); /* INET_DIAG_INFO */
}

static const struct sock_diag_handler *sock_diag_lock_handler(int family)
{
	const struct sock_diag_handler *handler;

	rcu_read_lock();
	handler = rcu_dereference(sock_diag_handlers[family]);
	if (handler && !try_module_get(handler->owner))
		handler = NULL;
	rcu_read_unlock();

	return handler;
}

static void sock_diag_unlock_handler(const struct sock_diag_handler *handler)
{
	module_put(handler->owner);
}

static void sock_diag_broadcast_destroy_work(struct work_struct *work)
{
	struct broadcast_sk *bsk =
		container_of(work, struct broadcast_sk, work);
	struct sock *sk = bsk->sk;
	const struct sock_diag_handler *hndl;
	struct sk_buff *skb;
	const enum sknetlink_groups group = sock_diag_destroy_group(sk);
	int err = -1;

	WARN_ON(group == SKNLGRP_NONE);

	skb = nlmsg_new(sock_diag_nlmsg_size(), GFP_KERNEL);
	if (!skb)
		goto out;

	hndl = sock_diag_lock_handler(sk->sk_family);
	if (hndl) {
		if (hndl->get_info)
			err = hndl->get_info(skb, sk);
		sock_diag_unlock_handler(hndl);
	}
	if (!err)
		nlmsg_multicast(sock_net(sk)->diag_nlsk, skb, 0, group,
				GFP_KERNEL);
	else
		kfree_skb(skb);
out:
	sk_destruct(sk);
	kfree(bsk);
}

void sock_diag_broadcast_destroy(struct sock *sk)
{
	/* Note, this function is often called from an interrupt context. */
	struct broadcast_sk *bsk =
		kmalloc(sizeof(struct broadcast_sk), GFP_ATOMIC);
	if (!bsk)
		return sk_destruct(sk);
	bsk->sk = sk;
	INIT_WORK(&bsk->work, sock_diag_broadcast_destroy_work);
	queue_work(broadcast_wq, &bsk->work);
}

void sock_diag_register_inet_compat(const struct sock_diag_inet_compat *ptr)
{
	xchg((__force const struct sock_diag_inet_compat **)&inet_rcv_compat,
	     ptr);
}
EXPORT_SYMBOL_GPL(sock_diag_register_inet_compat);

void sock_diag_unregister_inet_compat(const struct sock_diag_inet_compat *ptr)
{
	const struct sock_diag_inet_compat *old;

	old = xchg((__force const struct sock_diag_inet_compat **)&inet_rcv_compat,
		   NULL);
	WARN_ON_ONCE(old != ptr);
}
EXPORT_SYMBOL_GPL(sock_diag_unregister_inet_compat);

int sock_diag_register(const struct sock_diag_handler *hndl)
{
	int family = hndl->family;

	if (family >= AF_MAX)
		return -EINVAL;

	return !cmpxchg((const struct sock_diag_handler **)
				&sock_diag_handlers[family],
			NULL, hndl) ? 0 : -EBUSY;
}
EXPORT_SYMBOL_GPL(sock_diag_register);

void sock_diag_unregister(const struct sock_diag_handler *hndl)
{
	int family = hndl->family;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (family >= AF_MAX)
		return;

<<<<<<< HEAD
	mutex_lock(&sock_diag_table_mutex);
	BUG_ON(sock_diag_handlers[family] != hnld);
	sock_diag_handlers[family] = NULL;
	mutex_unlock(&sock_diag_table_mutex);
}
EXPORT_SYMBOL_GPL(sock_diag_unregister);

static inline struct sock_diag_handler *sock_diag_lock_handler(int family)
{
	if (sock_diag_handlers[family] == NULL)
		request_module("net-pf-%d-proto-%d-type-%d", PF_NETLINK,
				NETLINK_SOCK_DIAG, family);

	mutex_lock(&sock_diag_table_mutex);
	return sock_diag_handlers[family];
}

static inline void sock_diag_unlock_handler(struct sock_diag_handler *h)
{
	mutex_unlock(&sock_diag_table_mutex);
}

static int __sock_diag_cmd(struct sk_buff *skb, struct nlmsghdr *nlh)
{
	int err;
	struct sock_diag_req *req = NLMSG_DATA(nlh);
	struct sock_diag_handler *hndl;
=======
	xchg((const struct sock_diag_handler **)&sock_diag_handlers[family],
	     NULL);
}
EXPORT_SYMBOL_GPL(sock_diag_unregister);

static int __sock_diag_cmd(struct sk_buff *skb, struct nlmsghdr *nlh)
{
	int err;
	struct sock_diag_req *req = nlmsg_data(nlh);
	const struct sock_diag_handler *hndl;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (nlmsg_len(nlh) < sizeof(*req))
		return -EINVAL;

	if (req->sdiag_family >= AF_MAX)
		return -EINVAL;
<<<<<<< HEAD

	hndl = sock_diag_lock_handler(req->sdiag_family);
	if (hndl == NULL)
		err = -ENOENT;
	else if (nlh->nlmsg_type == SOCK_DIAG_BY_FAMILY)
		err = hndl->dump(skb, nlh);
	else if (nlh->nlmsg_type == SOCK_DESTROY_BACKPORT && hndl->destroy)
=======
	req->sdiag_family = array_index_nospec(req->sdiag_family, AF_MAX);

	if (!rcu_access_pointer(sock_diag_handlers[req->sdiag_family]))
		sock_load_diag_module(req->sdiag_family, 0);

	hndl = sock_diag_lock_handler(req->sdiag_family);
	if (hndl == NULL)
		return -ENOENT;

	if (nlh->nlmsg_type == SOCK_DIAG_BY_FAMILY)
		err = hndl->dump(skb, nlh);
	else if (nlh->nlmsg_type == SOCK_DESTROY && hndl->destroy)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = hndl->destroy(skb, nlh);
	else
		err = -EOPNOTSUPP;
	sock_diag_unlock_handler(hndl);

	return err;
}

<<<<<<< HEAD
static int sock_diag_rcv_msg(struct sk_buff *skb, struct nlmsghdr *nlh)
{
=======
static int sock_diag_rcv_msg(struct sk_buff *skb, struct nlmsghdr *nlh,
			     struct netlink_ext_ack *extack)
{
	const struct sock_diag_inet_compat *ptr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	switch (nlh->nlmsg_type) {
	case TCPDIAG_GETSOCK:
	case DCCPDIAG_GETSOCK:
<<<<<<< HEAD
		if (inet_rcv_compat == NULL)
			request_module("net-pf-%d-proto-%d-type-%d", PF_NETLINK,
					NETLINK_SOCK_DIAG, AF_INET);

		mutex_lock(&sock_diag_table_mutex);
		if (inet_rcv_compat != NULL)
			ret = inet_rcv_compat(skb, nlh);
		else
			ret = -EOPNOTSUPP;
		mutex_unlock(&sock_diag_table_mutex);

		return ret;
	case SOCK_DIAG_BY_FAMILY:
	case SOCK_DESTROY_BACKPORT:
=======

		if (!rcu_access_pointer(inet_rcv_compat))
			sock_load_diag_module(AF_INET, 0);

		rcu_read_lock();
		ptr = rcu_dereference(inet_rcv_compat);
		if (ptr && !try_module_get(ptr->owner))
			ptr = NULL;
		rcu_read_unlock();

		ret = -EOPNOTSUPP;
		if (ptr) {
			ret = ptr->fn(skb, nlh);
			module_put(ptr->owner);
		}

		return ret;
	case SOCK_DIAG_BY_FAMILY:
	case SOCK_DESTROY:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return __sock_diag_cmd(skb, nlh);
	default:
		return -EINVAL;
	}
}

<<<<<<< HEAD
static DEFINE_MUTEX(sock_diag_mutex);

static void sock_diag_rcv(struct sk_buff *skb)
{
	mutex_lock(&sock_diag_mutex);
	netlink_rcv_skb(skb, &sock_diag_rcv_msg);
	mutex_unlock(&sock_diag_mutex);
=======
static void sock_diag_rcv(struct sk_buff *skb)
{
	netlink_rcv_skb(skb, &sock_diag_rcv_msg);
}

static int sock_diag_bind(struct net *net, int group)
{
	switch (group) {
	case SKNLGRP_INET_TCP_DESTROY:
	case SKNLGRP_INET_UDP_DESTROY:
		if (!rcu_access_pointer(sock_diag_handlers[AF_INET]))
			sock_load_diag_module(AF_INET, 0);
		break;
	case SKNLGRP_INET6_TCP_DESTROY:
	case SKNLGRP_INET6_UDP_DESTROY:
		if (!rcu_access_pointer(sock_diag_handlers[AF_INET6]))
			sock_load_diag_module(AF_INET6, 0);
		break;
	}
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int sock_diag_destroy(struct sock *sk, int err)
{
<<<<<<< HEAD
	if (!capable(CAP_NET_ADMIN))
=======
	if (!ns_capable(sock_net(sk)->user_ns, CAP_NET_ADMIN))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EPERM;

	if (!sk->sk_prot->diag_destroy)
		return -EOPNOTSUPP;

	return sk->sk_prot->diag_destroy(sk, err);
}
EXPORT_SYMBOL_GPL(sock_diag_destroy);

<<<<<<< HEAD
struct sock *sock_diag_nlsk;
EXPORT_SYMBOL_GPL(sock_diag_nlsk);

static int __init sock_diag_init(void)
{
	sock_diag_nlsk = netlink_kernel_create(&init_net, NETLINK_SOCK_DIAG, 0,
					sock_diag_rcv, NULL, THIS_MODULE);
	return sock_diag_nlsk == NULL ? -ENOMEM : 0;
}

static void __exit sock_diag_exit(void)
{
	netlink_kernel_release(sock_diag_nlsk);
}

module_init(sock_diag_init);
module_exit(sock_diag_exit);
MODULE_LICENSE("GPL");
MODULE_ALIAS_NET_PF_PROTO(PF_NETLINK, NETLINK_SOCK_DIAG);
=======
static int __net_init diag_net_init(struct net *net)
{
	struct netlink_kernel_cfg cfg = {
		.groups	= SKNLGRP_MAX,
		.input	= sock_diag_rcv,
		.bind	= sock_diag_bind,
		.flags	= NL_CFG_F_NONROOT_RECV,
	};

	net->diag_nlsk = netlink_kernel_create(net, NETLINK_SOCK_DIAG, &cfg);
	return net->diag_nlsk == NULL ? -ENOMEM : 0;
}

static void __net_exit diag_net_exit(struct net *net)
{
	netlink_kernel_release(net->diag_nlsk);
	net->diag_nlsk = NULL;
}

static struct pernet_operations diag_net_ops = {
	.init = diag_net_init,
	.exit = diag_net_exit,
};

static int __init sock_diag_init(void)
{
	broadcast_wq = alloc_workqueue("sock_diag_events", 0, 0);
	BUG_ON(!broadcast_wq);
	return register_pernet_subsys(&diag_net_ops);
}
device_initcall(sock_diag_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
