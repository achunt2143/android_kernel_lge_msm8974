<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Request reply cache. This was heavily inspired by the
 * implementation in 4.3BSD/4.4BSD.
 *
 * Copyright (C) 1995, 1996 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef NFSCACHE_H
#define NFSCACHE_H

#include <linux/sunrpc/svc.h>
<<<<<<< HEAD

/*
 * Representation of a reply cache entry.
 */
struct svc_cacherep {
	struct hlist_node	c_hash;
	struct list_head	c_lru;

	unsigned char		c_state,	/* unused, inprog, done */
				c_type,		/* status, buffer */
				c_secure : 1;	/* req came from port < 1024 */
	struct sockaddr_in	c_addr;
	__be32			c_xid;
	u32			c_prot;
	u32			c_proc;
	u32			c_vers;
=======
#include "netns.h"

/*
 * Representation of a reply cache entry.
 *
 * Note that we use a sockaddr_in6 to hold the address instead of the more
 * typical sockaddr_storage. This is for space reasons, since sockaddr_storage
 * is much larger than a sockaddr_in6.
 */
struct nfsd_cacherep {
	struct {
		/* Keep often-read xid, csum in the same cache line: */
		__be32			k_xid;
		__wsum			k_csum;
		u32			k_proc;
		u32			k_prot;
		u32			k_vers;
		unsigned int		k_len;
		struct sockaddr_in6	k_addr;
	} c_key;

	struct rb_node		c_node;
	struct list_head	c_lru;
	unsigned char		c_state,	/* unused, inprog, done */
				c_type,		/* status, buffer */
				c_secure : 1;	/* req came from port < 1024 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long		c_timestamp;
	union {
		struct kvec	u_vec;
		__be32		u_status;
	}			c_u;
};

#define c_replvec		c_u.u_vec
#define c_replstat		c_u.u_status

/* cache entry states */
enum {
	RC_UNUSED,
	RC_INPROG,
	RC_DONE
};

/* return values */
enum {
	RC_DROPIT,
	RC_REPLY,
<<<<<<< HEAD
	RC_DOIT,
	RC_INTR
=======
	RC_DOIT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Cache types.
 * We may want to add more types one day, e.g. for diropres and
 * attrstat replies. Using cache entries with fixed length instead
 * of buffer pointers may be more efficient.
 */
enum {
	RC_NOCACHE,
	RC_REPLSTAT,
	RC_REPLBUFF,
};

<<<<<<< HEAD
/*
 * If requests are retransmitted within this interval, they're dropped.
 */
#define RC_DELAY		(HZ/5)

int	nfsd_reply_cache_init(void);
void	nfsd_reply_cache_shutdown(void);
int	nfsd_cache_lookup(struct svc_rqst *);
void	nfsd_cache_update(struct svc_rqst *, int, __be32 *);

#ifdef CONFIG_NFSD_V4
void	nfsd4_set_statp(struct svc_rqst *rqstp, __be32 *statp);
#else  /* CONFIG_NFSD_V4 */
static inline void nfsd4_set_statp(struct svc_rqst *rqstp, __be32 *statp)
{
}
#endif /* CONFIG_NFSD_V4 */
=======
/* Cache entries expire after this time period */
#define RC_EXPIRE		(120 * HZ)

/* Checksum this amount of the request */
#define RC_CSUMLEN		(256U)

int	nfsd_drc_slab_create(void);
void	nfsd_drc_slab_free(void);
int	nfsd_reply_cache_init(struct nfsd_net *);
void	nfsd_reply_cache_shutdown(struct nfsd_net *);
int	nfsd_cache_lookup(struct svc_rqst *rqstp, unsigned int start,
			  unsigned int len, struct nfsd_cacherep **cacherep);
void	nfsd_cache_update(struct svc_rqst *rqstp, struct nfsd_cacherep *rp,
			  int cachetype, __be32 *statp);
int	nfsd_reply_cache_stats_show(struct seq_file *m, void *v);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* NFSCACHE_H */
