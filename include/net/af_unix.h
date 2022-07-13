<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __LINUX_NET_AFUNIX_H
#define __LINUX_NET_AFUNIX_H

#include <linux/socket.h>
#include <linux/un.h>
#include <linux/mutex.h>
<<<<<<< HEAD
#include <net/sock.h>

extern void unix_inflight(struct user_struct *user, struct file *fp);
extern void unix_notinflight(struct user_struct *user, struct file *fp);
extern void unix_gc(void);
extern void wait_for_unix_gc(void);
extern struct sock *unix_get_socket(struct file *filp);
extern struct sock *unix_peer_get(struct sock *);

#define UNIX_HASH_SIZE	256
#define UNIX_HASH_BITS	8

extern unsigned int unix_tot_inflight;
extern spinlock_t unix_table_lock;
extern struct hlist_head unix_socket_table[2 * UNIX_HASH_SIZE];

struct unix_address {
	atomic_t	refcnt;
	int		len;
	unsigned	hash;
	struct sockaddr_un name[0];
=======
#include <linux/refcount.h>
#include <net/sock.h>

#if IS_ENABLED(CONFIG_UNIX)
struct unix_sock *unix_get_socket(struct file *filp);
#else
static inline struct unix_sock *unix_get_socket(struct file *filp)
{
	return NULL;
}
#endif

extern spinlock_t unix_gc_lock;
extern unsigned int unix_tot_inflight;

void unix_inflight(struct user_struct *user, struct file *fp);
void unix_notinflight(struct user_struct *user, struct file *fp);
void unix_gc(void);
void wait_for_unix_gc(struct scm_fp_list *fpl);

struct sock *unix_peer_get(struct sock *sk);

#define UNIX_HASH_MOD	(256 - 1)
#define UNIX_HASH_SIZE	(256 * 2)
#define UNIX_HASH_BITS	8

struct unix_address {
	refcount_t	refcnt;
	int		len;
	struct sockaddr_un name[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct unix_skb_parms {
	struct pid		*pid;		/* Skb credentials	*/
<<<<<<< HEAD
	const struct cred	*cred;
=======
	kuid_t			uid;
	kgid_t			gid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct scm_fp_list	*fp;		/* Passed files		*/
#ifdef CONFIG_SECURITY_NETWORK
	u32			secid;		/* Security ID		*/
#endif
<<<<<<< HEAD
};

#define UNIXCB(skb) 	(*(struct unix_skb_parms *)&((skb)->cb))
#define UNIXSID(skb)	(&UNIXCB((skb)).secid)

#define unix_state_lock(s)	spin_lock(&unix_sk(s)->lock)
#define unix_state_unlock(s)	spin_unlock(&unix_sk(s)->lock)
#define unix_state_lock_nested(s) \
				spin_lock_nested(&unix_sk(s)->lock, \
				SINGLE_DEPTH_NESTING)
=======
	u32			consumed;
} __randomize_layout;

struct scm_stat {
	atomic_t nr_fds;
};

#define UNIXCB(skb)	(*(struct unix_skb_parms *)&((skb)->cb))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* The AF_UNIX socket */
struct unix_sock {
	/* WARNING: sk has to be the first member */
	struct sock		sk;
<<<<<<< HEAD
	struct unix_address     *addr;
	struct path		path;
	struct mutex		readlock;
	struct sock		*peer;
	struct sock		*other;
	struct list_head	link;
	atomic_long_t		inflight;
	spinlock_t		lock;
	unsigned char		recursion_level;
=======
	struct unix_address	*addr;
	struct path		path;
	struct mutex		iolock, bindlock;
	struct sock		*peer;
	struct list_head	link;
	unsigned long		inflight;
	spinlock_t		lock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long		gc_flags;
#define UNIX_GC_CANDIDATE	0
#define UNIX_GC_MAYBE_CYCLE	1
	struct socket_wq	peer_wq;
<<<<<<< HEAD
	wait_queue_t		peer_wake;
};
#define unix_sk(__sk) ((struct unix_sock *)__sk)
=======
	wait_queue_entry_t	peer_wake;
	struct scm_stat		scm_stat;
#if IS_ENABLED(CONFIG_AF_UNIX_OOB)
	struct sk_buff		*oob_skb;
#endif
};

#define unix_sk(ptr) container_of_const(ptr, struct unix_sock, sk)
#define unix_peer(sk) (unix_sk(sk)->peer)

#define unix_state_lock(s)	spin_lock(&unix_sk(s)->lock)
#define unix_state_unlock(s)	spin_unlock(&unix_sk(s)->lock)
enum unix_socket_lock_class {
	U_LOCK_NORMAL,
	U_LOCK_SECOND,	/* for double locking, see unix_state_double_lock(). */
	U_LOCK_DIAG, /* used while dumping icons, see sk_diag_dump_icons(). */
	U_LOCK_GC_LISTENER, /* used for listening socket while determining gc
			     * candidates to close a small race window.
			     */
};

static inline void unix_state_lock_nested(struct sock *sk,
				   enum unix_socket_lock_class subclass)
{
	spin_lock_nested(&unix_sk(sk)->lock, subclass);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define peer_wait peer_wq.wait

long unix_inq_len(struct sock *sk);
long unix_outq_len(struct sock *sk);

<<<<<<< HEAD
#ifdef CONFIG_SYSCTL
extern int unix_sysctl_register(struct net *net);
extern void unix_sysctl_unregister(struct net *net);
=======
int __unix_dgram_recvmsg(struct sock *sk, struct msghdr *msg, size_t size,
			 int flags);
int __unix_stream_recvmsg(struct sock *sk, struct msghdr *msg, size_t size,
			  int flags);
#ifdef CONFIG_SYSCTL
int unix_sysctl_register(struct net *net);
void unix_sysctl_unregister(struct net *net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
static inline int unix_sysctl_register(struct net *net) { return 0; }
static inline void unix_sysctl_unregister(struct net *net) {}
#endif
<<<<<<< HEAD
=======

#ifdef CONFIG_BPF_SYSCALL
extern struct proto unix_dgram_proto;
extern struct proto unix_stream_proto;

int unix_dgram_bpf_update_proto(struct sock *sk, struct sk_psock *psock, bool restore);
int unix_stream_bpf_update_proto(struct sock *sk, struct sk_psock *psock, bool restore);
void __init unix_bpf_build_proto(void);
#else
static inline void __init unix_bpf_build_proto(void)
{}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
