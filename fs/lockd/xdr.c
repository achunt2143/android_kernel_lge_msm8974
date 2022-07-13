<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/fs/lockd/xdr.c
 *
 * XDR support for lockd and the lock client.
 *
 * Copyright (C) 1995, 1996 Olaf Kirch <okir@monad.swb.de>
 */

#include <linux/types.h>
#include <linux/sched.h>
#include <linux/nfs.h>

#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/clnt.h>
#include <linux/sunrpc/svc.h>
#include <linux/sunrpc/stats.h>
#include <linux/lockd/lockd.h>

<<<<<<< HEAD
#define NLMDBG_FACILITY		NLMDBG_XDR
=======
#include <uapi/linux/nfs2.h>

#include "svcxdr.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


static inline loff_t
s32_to_loff_t(__s32 offset)
{
	return (loff_t)offset;
}

static inline __s32
loff_t_to_s32(loff_t offset)
{
	__s32 res;
	if (offset >= NLM_OFFSET_MAX)
		res = NLM_OFFSET_MAX;
	else if (offset <= -NLM_OFFSET_MAX)
		res = -NLM_OFFSET_MAX;
	else
		res = offset;
	return res;
}

/*
<<<<<<< HEAD
 * XDR functions for basic NLM types
 */
static __be32 *nlm_decode_cookie(__be32 *p, struct nlm_cookie *c)
{
	unsigned int	len;

	len = ntohl(*p++);
	
	if(len==0)
	{
		c->len=4;
		memset(c->data, 0, 4);	/* hockeypux brain damage */
	}
	else if(len<=NLM_MAXCOOKIELEN)
	{
		c->len=len;
		memcpy(c->data, p, len);
		p+=XDR_QUADLEN(len);
	}
	else 
	{
		dprintk("lockd: bad cookie size %d (only cookies under "
			"%d bytes are supported.)\n",
				len, NLM_MAXCOOKIELEN);
		return NULL;
	}
	return p;
}

static inline __be32 *
nlm_encode_cookie(__be32 *p, struct nlm_cookie *c)
{
	*p++ = htonl(c->len);
	memcpy(p, c->data, c->len);
	p+=XDR_QUADLEN(c->len);
	return p;
}

static __be32 *
nlm_decode_fh(__be32 *p, struct nfs_fh *f)
{
	unsigned int	len;

	if ((len = ntohl(*p++)) != NFS2_FHSIZE) {
		dprintk("lockd: bad fhandle size %d (should be %d)\n",
			len, NFS2_FHSIZE);
		return NULL;
	}
	f->size = NFS2_FHSIZE;
	memset(f->data, 0, sizeof(f->data));
	memcpy(f->data, p, NFS2_FHSIZE);
	return p + XDR_QUADLEN(NFS2_FHSIZE);
}

static inline __be32 *
nlm_encode_fh(__be32 *p, struct nfs_fh *f)
{
	*p++ = htonl(NFS2_FHSIZE);
	memcpy(p, f->data, NFS2_FHSIZE);
	return p + XDR_QUADLEN(NFS2_FHSIZE);
}

/*
 * Encode and decode owner handle
 */
static inline __be32 *
nlm_decode_oh(__be32 *p, struct xdr_netobj *oh)
{
	return xdr_decode_netobj(p, oh);
}

static inline __be32 *
nlm_encode_oh(__be32 *p, struct xdr_netobj *oh)
{
	return xdr_encode_netobj(p, oh);
}

static __be32 *
nlm_decode_lock(__be32 *p, struct nlm_lock *lock)
{
	struct file_lock	*fl = &lock->fl;
	s32			start, len, end;

	if (!(p = xdr_decode_string_inplace(p, &lock->caller,
					    &lock->len,
					    NLM_MAXSTRLEN))
	 || !(p = nlm_decode_fh(p, &lock->fh))
	 || !(p = nlm_decode_oh(p, &lock->oh)))
		return NULL;
	lock->svid  = ntohl(*p++);

	locks_init_lock(fl);
	fl->fl_owner = current->files;
	fl->fl_pid   = (pid_t)lock->svid;
	fl->fl_flags = FL_POSIX;
	fl->fl_type  = F_RDLCK;		/* as good as anything else */
	start = ntohl(*p++);
	len = ntohl(*p++);
	end = start + len - 1;

	fl->fl_start = s32_to_loff_t(start);

=======
 * NLM file handles are defined by specification to be a variable-length
 * XDR opaque no longer than 1024 bytes. However, this implementation
 * constrains their length to exactly the length of an NFSv2 file
 * handle.
 */
static bool
svcxdr_decode_fhandle(struct xdr_stream *xdr, struct nfs_fh *fh)
{
	__be32 *p;
	u32 len;

	if (xdr_stream_decode_u32(xdr, &len) < 0)
		return false;
	if (len != NFS2_FHSIZE)
		return false;

	p = xdr_inline_decode(xdr, len);
	if (!p)
		return false;
	fh->size = NFS2_FHSIZE;
	memcpy(fh->data, p, len);
	memset(fh->data + NFS2_FHSIZE, 0, sizeof(fh->data) - NFS2_FHSIZE);

	return true;
}

static bool
svcxdr_decode_lock(struct xdr_stream *xdr, struct nlm_lock *lock)
{
	struct file_lock *fl = &lock->fl;
	s32 start, len, end;

	if (!svcxdr_decode_string(xdr, &lock->caller, &lock->len))
		return false;
	if (!svcxdr_decode_fhandle(xdr, &lock->fh))
		return false;
	if (!svcxdr_decode_owner(xdr, &lock->oh))
		return false;
	if (xdr_stream_decode_u32(xdr, &lock->svid) < 0)
		return false;
	if (xdr_stream_decode_u32(xdr, &start) < 0)
		return false;
	if (xdr_stream_decode_u32(xdr, &len) < 0)
		return false;

	locks_init_lock(fl);
	fl->c.flc_flags = FL_POSIX;
	fl->c.flc_type  = F_RDLCK;
	end = start + len - 1;
	fl->fl_start = s32_to_loff_t(start);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (len == 0 || end < 0)
		fl->fl_end = OFFSET_MAX;
	else
		fl->fl_end = s32_to_loff_t(end);
<<<<<<< HEAD
	return p;
}

/*
 * Encode result of a TEST/TEST_MSG call
 */
static __be32 *
nlm_encode_testres(__be32 *p, struct nlm_res *resp)
{
	s32		start, len;

	if (!(p = nlm_encode_cookie(p, &resp->cookie)))
		return NULL;
	*p++ = resp->status;

	if (resp->status == nlm_lck_denied) {
		struct file_lock	*fl = &resp->lock.fl;

		*p++ = (fl->fl_type == F_RDLCK)? xdr_zero : xdr_one;
		*p++ = htonl(resp->lock.svid);

		/* Encode owner handle. */
		if (!(p = xdr_encode_netobj(p, &resp->lock.oh)))
			return NULL;

		start = loff_t_to_s32(fl->fl_start);
		if (fl->fl_end == OFFSET_MAX)
			len = 0;
		else
			len = loff_t_to_s32(fl->fl_end - fl->fl_start + 1);

		*p++ = htonl(start);
		*p++ = htonl(len);
	}

	return p;
=======

	return true;
}

static bool
svcxdr_encode_holder(struct xdr_stream *xdr, const struct nlm_lock *lock)
{
	const struct file_lock *fl = &lock->fl;
	s32 start, len;

	/* exclusive */
	if (xdr_stream_encode_bool(xdr, fl->c.flc_type != F_RDLCK) < 0)
		return false;
	if (xdr_stream_encode_u32(xdr, lock->svid) < 0)
		return false;
	if (!svcxdr_encode_owner(xdr, &lock->oh))
		return false;
	start = loff_t_to_s32(fl->fl_start);
	if (fl->fl_end == OFFSET_MAX)
		len = 0;
	else
		len = loff_t_to_s32(fl->fl_end - fl->fl_start + 1);
	if (xdr_stream_encode_u32(xdr, start) < 0)
		return false;
	if (xdr_stream_encode_u32(xdr, len) < 0)
		return false;

	return true;
}

static bool
svcxdr_encode_testrply(struct xdr_stream *xdr, const struct nlm_res *resp)
{
	if (!svcxdr_encode_stats(xdr, resp->status))
		return false;
	switch (resp->status) {
	case nlm_lck_denied:
		if (!svcxdr_encode_holder(xdr, &resp->lock))
			return false;
	}

	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/*
<<<<<<< HEAD
 * First, the server side XDR functions
 */
int
nlmsvc_decode_testargs(struct svc_rqst *rqstp, __be32 *p, nlm_args *argp)
{
	u32	exclusive;

	if (!(p = nlm_decode_cookie(p, &argp->cookie)))
		return 0;

	exclusive = ntohl(*p++);
	if (!(p = nlm_decode_lock(p, &argp->lock)))
		return 0;
	if (exclusive)
		argp->lock.fl.fl_type = F_WRLCK;

	return xdr_argsize_check(rqstp, p);
}

int
nlmsvc_encode_testres(struct svc_rqst *rqstp, __be32 *p, struct nlm_res *resp)
{
	if (!(p = nlm_encode_testres(p, resp)))
		return 0;
	return xdr_ressize_check(rqstp, p);
}

int
nlmsvc_decode_lockargs(struct svc_rqst *rqstp, __be32 *p, nlm_args *argp)
{
	u32	exclusive;

	if (!(p = nlm_decode_cookie(p, &argp->cookie)))
		return 0;
	argp->block  = ntohl(*p++);
	exclusive    = ntohl(*p++);
	if (!(p = nlm_decode_lock(p, &argp->lock)))
		return 0;
	if (exclusive)
		argp->lock.fl.fl_type = F_WRLCK;
	argp->reclaim = ntohl(*p++);
	argp->state   = ntohl(*p++);
	argp->monitor = 1;		/* monitor client by default */

	return xdr_argsize_check(rqstp, p);
}

int
nlmsvc_decode_cancargs(struct svc_rqst *rqstp, __be32 *p, nlm_args *argp)
{
	u32	exclusive;

	if (!(p = nlm_decode_cookie(p, &argp->cookie)))
		return 0;
	argp->block = ntohl(*p++);
	exclusive = ntohl(*p++);
	if (!(p = nlm_decode_lock(p, &argp->lock)))
		return 0;
	if (exclusive)
		argp->lock.fl.fl_type = F_WRLCK;
	return xdr_argsize_check(rqstp, p);
}

int
nlmsvc_decode_unlockargs(struct svc_rqst *rqstp, __be32 *p, nlm_args *argp)
{
	if (!(p = nlm_decode_cookie(p, &argp->cookie))
	 || !(p = nlm_decode_lock(p, &argp->lock)))
		return 0;
	argp->lock.fl.fl_type = F_UNLCK;
	return xdr_argsize_check(rqstp, p);
}

int
nlmsvc_decode_shareargs(struct svc_rqst *rqstp, __be32 *p, nlm_args *argp)
{
=======
 * Decode Call arguments
 */

bool
nlmsvc_decode_void(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	return true;
}

bool
nlmsvc_decode_testargs(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nlm_args *argp = rqstp->rq_argp;
	u32 exclusive;

	if (!svcxdr_decode_cookie(xdr, &argp->cookie))
		return false;
	if (xdr_stream_decode_bool(xdr, &exclusive) < 0)
		return false;
	if (!svcxdr_decode_lock(xdr, &argp->lock))
		return false;
	if (exclusive)
		argp->lock.fl.c.flc_type = F_WRLCK;

	return true;
}

bool
nlmsvc_decode_lockargs(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nlm_args *argp = rqstp->rq_argp;
	u32 exclusive;

	if (!svcxdr_decode_cookie(xdr, &argp->cookie))
		return false;
	if (xdr_stream_decode_bool(xdr, &argp->block) < 0)
		return false;
	if (xdr_stream_decode_bool(xdr, &exclusive) < 0)
		return false;
	if (!svcxdr_decode_lock(xdr, &argp->lock))
		return false;
	if (exclusive)
		argp->lock.fl.c.flc_type = F_WRLCK;
	if (xdr_stream_decode_bool(xdr, &argp->reclaim) < 0)
		return false;
	if (xdr_stream_decode_u32(xdr, &argp->state) < 0)
		return false;
	argp->monitor = 1;		/* monitor client by default */

	return true;
}

bool
nlmsvc_decode_cancargs(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nlm_args *argp = rqstp->rq_argp;
	u32 exclusive;

	if (!svcxdr_decode_cookie(xdr, &argp->cookie))
		return false;
	if (xdr_stream_decode_bool(xdr, &argp->block) < 0)
		return false;
	if (xdr_stream_decode_bool(xdr, &exclusive) < 0)
		return false;
	if (!svcxdr_decode_lock(xdr, &argp->lock))
		return false;
	if (exclusive)
		argp->lock.fl.c.flc_type = F_WRLCK;

	return true;
}

bool
nlmsvc_decode_unlockargs(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nlm_args *argp = rqstp->rq_argp;

	if (!svcxdr_decode_cookie(xdr, &argp->cookie))
		return false;
	if (!svcxdr_decode_lock(xdr, &argp->lock))
		return false;
	argp->lock.fl.c.flc_type = F_UNLCK;

	return true;
}

bool
nlmsvc_decode_res(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nlm_res *resp = rqstp->rq_argp;

	if (!svcxdr_decode_cookie(xdr, &resp->cookie))
		return false;
	if (!svcxdr_decode_stats(xdr, &resp->status))
		return false;

	return true;
}

bool
nlmsvc_decode_reboot(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nlm_reboot *argp = rqstp->rq_argp;
	__be32 *p;
	u32 len;

	if (xdr_stream_decode_u32(xdr, &len) < 0)
		return false;
	if (len > SM_MAXSTRLEN)
		return false;
	p = xdr_inline_decode(xdr, len);
	if (!p)
		return false;
	argp->len = len;
	argp->mon = (char *)p;
	if (xdr_stream_decode_u32(xdr, &argp->state) < 0)
		return false;
	p = xdr_inline_decode(xdr, SM_PRIV_SIZE);
	if (!p)
		return false;
	memcpy(&argp->priv.data, p, sizeof(argp->priv.data));

	return true;
}

bool
nlmsvc_decode_shareargs(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nlm_args *argp = rqstp->rq_argp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nlm_lock	*lock = &argp->lock;

	memset(lock, 0, sizeof(*lock));
	locks_init_lock(&lock->fl);
<<<<<<< HEAD
	lock->svid = ~(u32) 0;
	lock->fl.fl_pid = (pid_t)lock->svid;

	if (!(p = nlm_decode_cookie(p, &argp->cookie))
	 || !(p = xdr_decode_string_inplace(p, &lock->caller,
					    &lock->len, NLM_MAXSTRLEN))
	 || !(p = nlm_decode_fh(p, &lock->fh))
	 || !(p = nlm_decode_oh(p, &lock->oh)))
		return 0;
	argp->fsm_mode = ntohl(*p++);
	argp->fsm_access = ntohl(*p++);
	return xdr_argsize_check(rqstp, p);
}

int
nlmsvc_encode_shareres(struct svc_rqst *rqstp, __be32 *p, struct nlm_res *resp)
{
	if (!(p = nlm_encode_cookie(p, &resp->cookie)))
		return 0;
	*p++ = resp->status;
	*p++ = xdr_zero;		/* sequence argument */
	return xdr_ressize_check(rqstp, p);
}

int
nlmsvc_encode_res(struct svc_rqst *rqstp, __be32 *p, struct nlm_res *resp)
{
	if (!(p = nlm_encode_cookie(p, &resp->cookie)))
		return 0;
	*p++ = resp->status;
	return xdr_ressize_check(rqstp, p);
}

int
nlmsvc_decode_notify(struct svc_rqst *rqstp, __be32 *p, struct nlm_args *argp)
{
	struct nlm_lock	*lock = &argp->lock;

	if (!(p = xdr_decode_string_inplace(p, &lock->caller,
					    &lock->len, NLM_MAXSTRLEN)))
		return 0;
	argp->state = ntohl(*p++);
	return xdr_argsize_check(rqstp, p);
}

int
nlmsvc_decode_reboot(struct svc_rqst *rqstp, __be32 *p, struct nlm_reboot *argp)
{
	if (!(p = xdr_decode_string_inplace(p, &argp->mon, &argp->len, SM_MAXSTRLEN)))
		return 0;
	argp->state = ntohl(*p++);
	memcpy(&argp->priv.data, p, sizeof(argp->priv.data));
	p += XDR_QUADLEN(SM_PRIV_SIZE);
	return xdr_argsize_check(rqstp, p);
}

int
nlmsvc_decode_res(struct svc_rqst *rqstp, __be32 *p, struct nlm_res *resp)
{
	if (!(p = nlm_decode_cookie(p, &resp->cookie)))
		return 0;
	resp->status = *p++;
	return xdr_argsize_check(rqstp, p);
}

int
nlmsvc_decode_void(struct svc_rqst *rqstp, __be32 *p, void *dummy)
{
	return xdr_argsize_check(rqstp, p);
}

int
nlmsvc_encode_void(struct svc_rqst *rqstp, __be32 *p, void *dummy)
{
	return xdr_ressize_check(rqstp, p);
=======
	lock->svid = ~(u32)0;

	if (!svcxdr_decode_cookie(xdr, &argp->cookie))
		return false;
	if (!svcxdr_decode_string(xdr, &lock->caller, &lock->len))
		return false;
	if (!svcxdr_decode_fhandle(xdr, &lock->fh))
		return false;
	if (!svcxdr_decode_owner(xdr, &lock->oh))
		return false;
	/* XXX: Range checks are missing in the original code */
	if (xdr_stream_decode_u32(xdr, &argp->fsm_mode) < 0)
		return false;
	if (xdr_stream_decode_u32(xdr, &argp->fsm_access) < 0)
		return false;

	return true;
}

bool
nlmsvc_decode_notify(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nlm_args *argp = rqstp->rq_argp;
	struct nlm_lock	*lock = &argp->lock;

	if (!svcxdr_decode_string(xdr, &lock->caller, &lock->len))
		return false;
	if (xdr_stream_decode_u32(xdr, &argp->state) < 0)
		return false;

	return true;
}


/*
 * Encode Reply results
 */

bool
nlmsvc_encode_void(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	return true;
}

bool
nlmsvc_encode_testres(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nlm_res *resp = rqstp->rq_resp;

	return svcxdr_encode_cookie(xdr, &resp->cookie) &&
		svcxdr_encode_testrply(xdr, resp);
}

bool
nlmsvc_encode_res(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nlm_res *resp = rqstp->rq_resp;

	return svcxdr_encode_cookie(xdr, &resp->cookie) &&
		svcxdr_encode_stats(xdr, resp->status);
}

bool
nlmsvc_encode_shareres(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nlm_res *resp = rqstp->rq_resp;

	if (!svcxdr_encode_cookie(xdr, &resp->cookie))
		return false;
	if (!svcxdr_encode_stats(xdr, resp->status))
		return false;
	/* sequence */
	if (xdr_stream_encode_u32(xdr, 0) < 0)
		return false;

	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
