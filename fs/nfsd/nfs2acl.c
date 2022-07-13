<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Process version 2 NFSACL requests.
 *
 * Copyright (C) 2002-2003 Andreas Gruenbacher <agruen@suse.de>
 */

#include "nfsd.h"
/* FIXME: nfsacl.h is a broken header */
#include <linux/nfsacl.h>
#include <linux/gfp.h>
#include "cache.h"
#include "xdr3.h"
#include "vfs.h"

#define NFSDDBG_FACILITY		NFSDDBG_PROC
<<<<<<< HEAD
#define RETURN_STATUS(st)	{ resp->status = (st); return (st); }
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * NULL call.
 */
static __be32
<<<<<<< HEAD
nfsacld_proc_null(struct svc_rqst *rqstp, void *argp, void *resp)
{
	return nfs_ok;
=======
nfsacld_proc_null(struct svc_rqst *rqstp)
{
	return rpc_success;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Get the Access and/or Default ACL of a file.
 */
<<<<<<< HEAD
static __be32 nfsacld_proc_getacl(struct svc_rqst * rqstp,
		struct nfsd3_getaclargs *argp, struct nfsd3_getaclres *resp)
{
	svc_fh *fh;
	struct posix_acl *acl;
	__be32 nfserr = 0;
=======
static __be32 nfsacld_proc_getacl(struct svc_rqst *rqstp)
{
	struct nfsd3_getaclargs *argp = rqstp->rq_argp;
	struct nfsd3_getaclres *resp = rqstp->rq_resp;
	struct posix_acl *acl;
	struct inode *inode;
	svc_fh *fh;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dprintk("nfsd: GETACL(2acl)   %s\n", SVCFH_fmt(&argp->fh));

	fh = fh_copy(&resp->fh, &argp->fh);
<<<<<<< HEAD
	nfserr = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_NOP);
	if (nfserr)
		RETURN_STATUS(nfserr);

	if (argp->mask & ~(NFS_ACL|NFS_ACLCNT|NFS_DFACL|NFS_DFACLCNT))
		RETURN_STATUS(nfserr_inval);
	resp->mask = argp->mask;

	if (resp->mask & (NFS_ACL|NFS_ACLCNT)) {
		acl = nfsd_get_posix_acl(fh, ACL_TYPE_ACCESS);
		if (IS_ERR(acl)) {
			int err = PTR_ERR(acl);

			if (err == -ENODATA || err == -EOPNOTSUPP)
				acl = NULL;
			else {
				nfserr = nfserrno(err);
				goto fail;
			}
		}
		if (acl == NULL) {
			/* Solaris returns the inode's minimum ACL. */

			struct inode *inode = fh->fh_dentry->d_inode;
			acl = posix_acl_from_mode(inode->i_mode, GFP_KERNEL);
		}
=======
	resp->status = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_NOP);
	if (resp->status != nfs_ok)
		goto out;

	inode = d_inode(fh->fh_dentry);

	if (argp->mask & ~NFS_ACL_MASK) {
		resp->status = nfserr_inval;
		goto out;
	}
	resp->mask = argp->mask;

	resp->status = fh_getattr(fh, &resp->stat);
	if (resp->status != nfs_ok)
		goto out;

	if (resp->mask & (NFS_ACL|NFS_ACLCNT)) {
		acl = get_inode_acl(inode, ACL_TYPE_ACCESS);
		if (acl == NULL) {
			/* Solaris returns the inode's minimum ACL. */
			acl = posix_acl_from_mode(inode->i_mode, GFP_KERNEL);
		}
		if (IS_ERR(acl)) {
			resp->status = nfserrno(PTR_ERR(acl));
			goto fail;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		resp->acl_access = acl;
	}
	if (resp->mask & (NFS_DFACL|NFS_DFACLCNT)) {
		/* Check how Solaris handles requests for the Default ACL
		   of a non-directory! */
<<<<<<< HEAD

		acl = nfsd_get_posix_acl(fh, ACL_TYPE_DEFAULT);
		if (IS_ERR(acl)) {
			int err = PTR_ERR(acl);

			if (err == -ENODATA || err == -EOPNOTSUPP)
				acl = NULL;
			else {
				nfserr = nfserrno(err);
				goto fail;
			}
=======
		acl = get_inode_acl(inode, ACL_TYPE_DEFAULT);
		if (IS_ERR(acl)) {
			resp->status = nfserrno(PTR_ERR(acl));
			goto fail;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		resp->acl_default = acl;
	}

	/* resp->acl_{access,default} are released in nfssvc_release_getacl. */
<<<<<<< HEAD
	RETURN_STATUS(0);
=======
out:
	return rpc_success;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

fail:
	posix_acl_release(resp->acl_access);
	posix_acl_release(resp->acl_default);
<<<<<<< HEAD
	RETURN_STATUS(nfserr);
=======
	goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Set the Access and/or Default ACL of a file.
 */
<<<<<<< HEAD
static __be32 nfsacld_proc_setacl(struct svc_rqst * rqstp,
		struct nfsd3_setaclargs *argp,
		struct nfsd_attrstat *resp)
{
	svc_fh *fh;
	__be32 nfserr = 0;
=======
static __be32 nfsacld_proc_setacl(struct svc_rqst *rqstp)
{
	struct nfsd3_setaclargs *argp = rqstp->rq_argp;
	struct nfsd_attrstat *resp = rqstp->rq_resp;
	struct inode *inode;
	svc_fh *fh;
	int error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dprintk("nfsd: SETACL(2acl)   %s\n", SVCFH_fmt(&argp->fh));

	fh = fh_copy(&resp->fh, &argp->fh);
<<<<<<< HEAD
	nfserr = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_SATTR);

	if (!nfserr) {
		nfserr = nfserrno( nfsd_set_posix_acl(
			fh, ACL_TYPE_ACCESS, argp->acl_access) );
	}
	if (!nfserr) {
		nfserr = nfserrno( nfsd_set_posix_acl(
			fh, ACL_TYPE_DEFAULT, argp->acl_default) );
	}

=======
	resp->status = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_SATTR);
	if (resp->status != nfs_ok)
		goto out;

	inode = d_inode(fh->fh_dentry);

	error = fh_want_write(fh);
	if (error)
		goto out_errno;

	inode_lock(inode);

	error = set_posix_acl(&nop_mnt_idmap, fh->fh_dentry, ACL_TYPE_ACCESS,
			      argp->acl_access);
	if (error)
		goto out_drop_lock;
	error = set_posix_acl(&nop_mnt_idmap, fh->fh_dentry, ACL_TYPE_DEFAULT,
			      argp->acl_default);
	if (error)
		goto out_drop_lock;

	inode_unlock(inode);

	fh_drop_write(fh);

	resp->status = fh_getattr(fh, &resp->stat);

out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* argp->acl_{access,default} may have been allocated in
	   nfssvc_decode_setaclargs. */
	posix_acl_release(argp->acl_access);
	posix_acl_release(argp->acl_default);
<<<<<<< HEAD
	return nfserr;
=======
	return rpc_success;

out_drop_lock:
	inode_unlock(inode);
	fh_drop_write(fh);
out_errno:
	resp->status = nfserrno(error);
	goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Check file attributes
 */
<<<<<<< HEAD
static __be32 nfsacld_proc_getattr(struct svc_rqst * rqstp,
		struct nfsd_fhandle *argp, struct nfsd_attrstat *resp)
{
	dprintk("nfsd: GETATTR  %s\n", SVCFH_fmt(&argp->fh));

	fh_copy(&resp->fh, &argp->fh);
	return fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_NOP);
=======
static __be32 nfsacld_proc_getattr(struct svc_rqst *rqstp)
{
	struct nfsd_fhandle *argp = rqstp->rq_argp;
	struct nfsd_attrstat *resp = rqstp->rq_resp;

	dprintk("nfsd: GETATTR  %s\n", SVCFH_fmt(&argp->fh));

	fh_copy(&resp->fh, &argp->fh);
	resp->status = fh_verify(rqstp, &resp->fh, 0, NFSD_MAY_NOP);
	if (resp->status != nfs_ok)
		goto out;
	resp->status = fh_getattr(&resp->fh, &resp->stat);
out:
	return rpc_success;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Check file access
 */
<<<<<<< HEAD
static __be32 nfsacld_proc_access(struct svc_rqst *rqstp, struct nfsd3_accessargs *argp,
		struct nfsd3_accessres *resp)
{
	__be32 nfserr;
=======
static __be32 nfsacld_proc_access(struct svc_rqst *rqstp)
{
	struct nfsd3_accessargs *argp = rqstp->rq_argp;
	struct nfsd3_accessres *resp = rqstp->rq_resp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dprintk("nfsd: ACCESS(2acl)   %s 0x%x\n",
			SVCFH_fmt(&argp->fh),
			argp->access);

	fh_copy(&resp->fh, &argp->fh);
	resp->access = argp->access;
<<<<<<< HEAD
	nfserr = nfsd_access(rqstp, &resp->fh, &resp->access, NULL);
	return nfserr;
=======
	resp->status = nfsd_access(rqstp, &resp->fh, &resp->access, NULL);
	if (resp->status != nfs_ok)
		goto out;
	resp->status = fh_getattr(&resp->fh, &resp->stat);
out:
	return rpc_success;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * XDR decode functions
 */
<<<<<<< HEAD
static int nfsaclsvc_decode_getaclargs(struct svc_rqst *rqstp, __be32 *p,
		struct nfsd3_getaclargs *argp)
{
	if (!(p = nfs2svc_decode_fh(p, &argp->fh)))
		return 0;
	argp->mask = ntohl(*p); p++;

	return xdr_argsize_check(rqstp, p);
}


static int nfsaclsvc_decode_setaclargs(struct svc_rqst *rqstp, __be32 *p,
		struct nfsd3_setaclargs *argp)
{
	struct kvec *head = rqstp->rq_arg.head;
	unsigned int base;
	int n;

	if (!(p = nfs2svc_decode_fh(p, &argp->fh)))
		return 0;
	argp->mask = ntohl(*p++);
	if (argp->mask & ~(NFS_ACL|NFS_ACLCNT|NFS_DFACL|NFS_DFACLCNT) ||
	    !xdr_argsize_check(rqstp, p))
		return 0;

	base = (char *)p - (char *)head->iov_base;
	n = nfsacl_decode(&rqstp->rq_arg, base, NULL,
			  (argp->mask & NFS_ACL) ?
			  &argp->acl_access : NULL);
	if (n > 0)
		n = nfsacl_decode(&rqstp->rq_arg, base + n, NULL,
				  (argp->mask & NFS_DFACL) ?
				  &argp->acl_default : NULL);
	return (n > 0);
}

static int nfsaclsvc_decode_fhandleargs(struct svc_rqst *rqstp, __be32 *p,
		struct nfsd_fhandle *argp)
{
	if (!(p = nfs2svc_decode_fh(p, &argp->fh)))
		return 0;
	return xdr_argsize_check(rqstp, p);
}

static int nfsaclsvc_decode_accessargs(struct svc_rqst *rqstp, __be32 *p,
		struct nfsd3_accessargs *argp)
{
	if (!(p = nfs2svc_decode_fh(p, &argp->fh)))
		return 0;
	argp->access = ntohl(*p++);

	return xdr_argsize_check(rqstp, p);
=======

static bool
nfsaclsvc_decode_getaclargs(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nfsd3_getaclargs *argp = rqstp->rq_argp;

	if (!svcxdr_decode_fhandle(xdr, &argp->fh))
		return false;
	if (xdr_stream_decode_u32(xdr, &argp->mask) < 0)
		return false;

	return true;
}

static bool
nfsaclsvc_decode_setaclargs(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nfsd3_setaclargs *argp = rqstp->rq_argp;

	if (!svcxdr_decode_fhandle(xdr, &argp->fh))
		return false;
	if (xdr_stream_decode_u32(xdr, &argp->mask) < 0)
		return false;
	if (argp->mask & ~NFS_ACL_MASK)
		return false;
	if (!nfs_stream_decode_acl(xdr, NULL, (argp->mask & NFS_ACL) ?
				   &argp->acl_access : NULL))
		return false;
	if (!nfs_stream_decode_acl(xdr, NULL, (argp->mask & NFS_DFACL) ?
				   &argp->acl_default : NULL))
		return false;

	return true;
}

static bool
nfsaclsvc_decode_accessargs(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nfsd3_accessargs *args = rqstp->rq_argp;

	if (!svcxdr_decode_fhandle(xdr, &args->fh))
		return false;
	if (xdr_stream_decode_u32(xdr, &args->access) < 0)
		return false;

	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * XDR encode functions
 */

<<<<<<< HEAD
/*
 * There must be an encoding function for void results so svc_process
 * will work properly.
 */
int
nfsaclsvc_encode_voidres(struct svc_rqst *rqstp, __be32 *p, void *dummy)
{
	return xdr_ressize_check(rqstp, p);
}

/* GETACL */
static int nfsaclsvc_encode_getaclres(struct svc_rqst *rqstp, __be32 *p,
		struct nfsd3_getaclres *resp)
{
	struct dentry *dentry = resp->fh.fh_dentry;
	struct inode *inode;
	struct kvec *head = rqstp->rq_res.head;
	unsigned int base;
	int n;
	int w;

	/*
	 * Since this is version 2, the check for nfserr in
	 * nfsd_dispatch actually ensures the following cannot happen.
	 * However, it seems fragile to depend on that.
	 */
	if (dentry == NULL || dentry->d_inode == NULL)
		return 0;
	inode = dentry->d_inode;

	p = nfs2svc_encode_fattr(rqstp, p, &resp->fh);
	*p++ = htonl(resp->mask);
	if (!xdr_ressize_check(rqstp, p))
		return 0;
	base = (char *)p - (char *)head->iov_base;

	rqstp->rq_res.page_len = w = nfsacl_size(
		(resp->mask & NFS_ACL)   ? resp->acl_access  : NULL,
		(resp->mask & NFS_DFACL) ? resp->acl_default : NULL);
	while (w > 0) {
		if (!rqstp->rq_respages[rqstp->rq_resused++])
			return 0;
		w -= PAGE_SIZE;
	}

	n = nfsacl_encode(&rqstp->rq_res, base, inode,
			  resp->acl_access,
			  resp->mask & NFS_ACL, 0);
	if (n > 0)
		n = nfsacl_encode(&rqstp->rq_res, base + n, inode,
				  resp->acl_default,
				  resp->mask & NFS_DFACL,
				  NFS_ACL_DEFAULT);
	if (n <= 0)
		return 0;
	return 1;
}

static int nfsaclsvc_encode_attrstatres(struct svc_rqst *rqstp, __be32 *p,
		struct nfsd_attrstat *resp)
{
	p = nfs2svc_encode_fattr(rqstp, p, &resp->fh);
	return xdr_ressize_check(rqstp, p);
}

/* ACCESS */
static int nfsaclsvc_encode_accessres(struct svc_rqst *rqstp, __be32 *p,
		struct nfsd3_accessres *resp)
{
	p = nfs2svc_encode_fattr(rqstp, p, &resp->fh);
	*p++ = htonl(resp->access);
	return xdr_ressize_check(rqstp, p);
=======
/* GETACL */
static bool
nfsaclsvc_encode_getaclres(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nfsd3_getaclres *resp = rqstp->rq_resp;
	struct dentry *dentry = resp->fh.fh_dentry;
	struct inode *inode;

	if (!svcxdr_encode_stat(xdr, resp->status))
		return false;

	if (dentry == NULL || d_really_is_negative(dentry))
		return true;
	inode = d_inode(dentry);

	if (!svcxdr_encode_fattr(rqstp, xdr, &resp->fh, &resp->stat))
		return false;
	if (xdr_stream_encode_u32(xdr, resp->mask) < 0)
		return false;

	if (!nfs_stream_encode_acl(xdr, inode, resp->acl_access,
				   resp->mask & NFS_ACL, 0))
		return false;
	if (!nfs_stream_encode_acl(xdr, inode, resp->acl_default,
				   resp->mask & NFS_DFACL, NFS_ACL_DEFAULT))
		return false;

	return true;
}

/* ACCESS */
static bool
nfsaclsvc_encode_accessres(struct svc_rqst *rqstp, struct xdr_stream *xdr)
{
	struct nfsd3_accessres *resp = rqstp->rq_resp;

	if (!svcxdr_encode_stat(xdr, resp->status))
		return false;
	switch (resp->status) {
	case nfs_ok:
		if (!svcxdr_encode_fattr(rqstp, xdr, &resp->fh, &resp->stat))
			return false;
		if (xdr_stream_encode_u32(xdr, resp->access) < 0)
			return false;
		break;
	}

	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * XDR release functions
 */
<<<<<<< HEAD
static int nfsaclsvc_release_getacl(struct svc_rqst *rqstp, __be32 *p,
		struct nfsd3_getaclres *resp)
{
	fh_put(&resp->fh);
	posix_acl_release(resp->acl_access);
	posix_acl_release(resp->acl_default);
	return 1;
}

static int nfsaclsvc_release_attrstat(struct svc_rqst *rqstp, __be32 *p,
		struct nfsd_attrstat *resp)
{
	fh_put(&resp->fh);
	return 1;
}

static int nfsaclsvc_release_access(struct svc_rqst *rqstp, __be32 *p,
               struct nfsd3_accessres *resp)
{
       fh_put(&resp->fh);
       return 1;
}

#define nfsaclsvc_decode_voidargs	NULL
#define nfsaclsvc_release_void		NULL
#define nfsd3_fhandleargs	nfsd_fhandle
#define nfsd3_attrstatres	nfsd_attrstat
#define nfsd3_voidres		nfsd3_voidargs
struct nfsd3_voidargs { int dummy; };

#define PROC(name, argt, rest, relt, cache, respsize)	\
 { (svc_procfunc) nfsacld_proc_##name,		\
   (kxdrproc_t) nfsaclsvc_decode_##argt##args,	\
   (kxdrproc_t) nfsaclsvc_encode_##rest##res,	\
   (kxdrproc_t) nfsaclsvc_release_##relt,		\
   sizeof(struct nfsd3_##argt##args),		\
   sizeof(struct nfsd3_##rest##res),		\
   0,						\
   cache,					\
   respsize,					\
 }

=======
static void nfsaclsvc_release_getacl(struct svc_rqst *rqstp)
{
	struct nfsd3_getaclres *resp = rqstp->rq_resp;

	fh_put(&resp->fh);
	posix_acl_release(resp->acl_access);
	posix_acl_release(resp->acl_default);
}

static void nfsaclsvc_release_access(struct svc_rqst *rqstp)
{
	struct nfsd3_accessres *resp = rqstp->rq_resp;

	fh_put(&resp->fh);
}

struct nfsd3_voidargs { int dummy; };

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define ST 1		/* status*/
#define AT 21		/* attributes */
#define pAT (1+AT)	/* post attributes - conditional */
#define ACL (1+NFS_ACL_MAX_ENTRIES*3)  /* Access Control List */

<<<<<<< HEAD
static struct svc_procedure		nfsd_acl_procedures2[] = {
  PROC(null,	void,		void,		void,	  RC_NOCACHE, ST),
  PROC(getacl,	getacl,		getacl,		getacl,	  RC_NOCACHE, ST+1+2*(1+ACL)),
  PROC(setacl,	setacl,		attrstat,	attrstat, RC_NOCACHE, ST+AT),
  PROC(getattr, fhandle,	attrstat,	attrstat, RC_NOCACHE, ST+AT),
  PROC(access,	access,		access,		access,   RC_NOCACHE, ST+AT+1),
};

struct svc_version	nfsd_acl_version2 = {
		.vs_vers	= 2,
		.vs_nproc	= 5,
		.vs_proc	= nfsd_acl_procedures2,
		.vs_dispatch	= nfsd_dispatch,
		.vs_xdrsize	= NFS3_SVC_XDRSIZE,
		.vs_hidden	= 0,
=======
static const struct svc_procedure nfsd_acl_procedures2[5] = {
	[ACLPROC2_NULL] = {
		.pc_func = nfsacld_proc_null,
		.pc_decode = nfssvc_decode_voidarg,
		.pc_encode = nfssvc_encode_voidres,
		.pc_argsize = sizeof(struct nfsd_voidargs),
		.pc_argzero = sizeof(struct nfsd_voidargs),
		.pc_ressize = sizeof(struct nfsd_voidres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST,
		.pc_name = "NULL",
	},
	[ACLPROC2_GETACL] = {
		.pc_func = nfsacld_proc_getacl,
		.pc_decode = nfsaclsvc_decode_getaclargs,
		.pc_encode = nfsaclsvc_encode_getaclres,
		.pc_release = nfsaclsvc_release_getacl,
		.pc_argsize = sizeof(struct nfsd3_getaclargs),
		.pc_argzero = sizeof(struct nfsd3_getaclargs),
		.pc_ressize = sizeof(struct nfsd3_getaclres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+1+2*(1+ACL),
		.pc_name = "GETACL",
	},
	[ACLPROC2_SETACL] = {
		.pc_func = nfsacld_proc_setacl,
		.pc_decode = nfsaclsvc_decode_setaclargs,
		.pc_encode = nfssvc_encode_attrstatres,
		.pc_release = nfssvc_release_attrstat,
		.pc_argsize = sizeof(struct nfsd3_setaclargs),
		.pc_argzero = sizeof(struct nfsd3_setaclargs),
		.pc_ressize = sizeof(struct nfsd_attrstat),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+AT,
		.pc_name = "SETACL",
	},
	[ACLPROC2_GETATTR] = {
		.pc_func = nfsacld_proc_getattr,
		.pc_decode = nfssvc_decode_fhandleargs,
		.pc_encode = nfssvc_encode_attrstatres,
		.pc_release = nfssvc_release_attrstat,
		.pc_argsize = sizeof(struct nfsd_fhandle),
		.pc_argzero = sizeof(struct nfsd_fhandle),
		.pc_ressize = sizeof(struct nfsd_attrstat),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+AT,
		.pc_name = "GETATTR",
	},
	[ACLPROC2_ACCESS] = {
		.pc_func = nfsacld_proc_access,
		.pc_decode = nfsaclsvc_decode_accessargs,
		.pc_encode = nfsaclsvc_encode_accessres,
		.pc_release = nfsaclsvc_release_access,
		.pc_argsize = sizeof(struct nfsd3_accessargs),
		.pc_argzero = sizeof(struct nfsd3_accessargs),
		.pc_ressize = sizeof(struct nfsd3_accessres),
		.pc_cachetype = RC_NOCACHE,
		.pc_xdrressize = ST+AT+1,
		.pc_name = "SETATTR",
	},
};

static DEFINE_PER_CPU_ALIGNED(unsigned long,
			      nfsd_acl_count2[ARRAY_SIZE(nfsd_acl_procedures2)]);
const struct svc_version nfsd_acl_version2 = {
	.vs_vers	= 2,
	.vs_nproc	= ARRAY_SIZE(nfsd_acl_procedures2),
	.vs_proc	= nfsd_acl_procedures2,
	.vs_count	= nfsd_acl_count2,
	.vs_dispatch	= nfsd_dispatch,
	.vs_xdrsize	= NFS3_SVC_XDRSIZE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
