<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/include/linux/sunrpc/stats.h
 *
 * Client statistics collection for SUN RPC
 *
 * Copyright (C) 1996 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINUX_SUNRPC_STATS_H
#define _LINUX_SUNRPC_STATS_H

#include <linux/proc_fs.h>

struct rpc_stat {
	const struct rpc_program *program;

	unsigned int		netcnt,
				netudpcnt,
				nettcpcnt,
				nettcpconn,
				netreconn;
	unsigned int		rpccnt,
				rpcretrans,
				rpcauthrefresh,
				rpcgarbage;
};

struct svc_stat {
	struct svc_program *	program;

	unsigned int		netcnt,
				netudpcnt,
				nettcpcnt,
				nettcpconn;
	unsigned int		rpccnt,
				rpcbadfmt,
				rpcbadauth,
				rpcbadclnt;
};

struct net;
#ifdef CONFIG_PROC_FS
int			rpc_proc_init(struct net *);
void			rpc_proc_exit(struct net *);
<<<<<<< HEAD
=======
struct proc_dir_entry *	rpc_proc_register(struct net *,struct rpc_stat *);
void			rpc_proc_unregister(struct net *,const char *);
void			rpc_proc_zero(const struct rpc_program *);
struct proc_dir_entry *	svc_proc_register(struct net *, struct svc_stat *,
					  const struct proc_ops *);
void			svc_proc_unregister(struct net *, const char *);

void			svc_seq_show(struct seq_file *,
				     const struct svc_stat *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
static inline int rpc_proc_init(struct net *net)
{
	return 0;
}

static inline void rpc_proc_exit(struct net *net)
{
}
<<<<<<< HEAD
#endif

#ifdef MODULE
void			rpc_modcount(struct inode *, int);
#endif

#ifdef CONFIG_PROC_FS
struct proc_dir_entry *	rpc_proc_register(struct net *,struct rpc_stat *);
void			rpc_proc_unregister(struct net *,const char *);
void			rpc_proc_zero(const struct rpc_program *);
struct proc_dir_entry *	svc_proc_register(struct net *, struct svc_stat *,
					  const struct file_operations *);
void			svc_proc_unregister(struct net *, const char *);

void			svc_seq_show(struct seq_file *,
				     const struct svc_stat *);
#else

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline struct proc_dir_entry *rpc_proc_register(struct net *net, struct rpc_stat *s) { return NULL; }
static inline void rpc_proc_unregister(struct net *net, const char *p) {}
static inline void rpc_proc_zero(const struct rpc_program *p) {}

static inline struct proc_dir_entry *svc_proc_register(struct net *net, struct svc_stat *s,
<<<<<<< HEAD
						       const struct file_operations *f) { return NULL; }
=======
						       const struct proc_ops *proc_ops) { return NULL; }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void svc_proc_unregister(struct net *net, const char *p) {}

static inline void svc_seq_show(struct seq_file *seq,
				const struct svc_stat *st) {}
#endif

#endif /* _LINUX_SUNRPC_STATS_H */
