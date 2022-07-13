<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* XDR types for nfsd. This is mainly a typing exercise. */

#ifndef LINUX_NFSD_H
#define LINUX_NFSD_H

#include <linux/vfs.h>
#include "nfsd.h"
#include "nfsfh.h"

struct nfsd_fhandle {
	struct svc_fh		fh;
};

struct nfsd_sattrargs {
	struct svc_fh		fh;
	struct iattr		attrs;
};

struct nfsd_diropargs {
	struct svc_fh		fh;
	char *			name;
	unsigned int		len;
};

struct nfsd_readargs {
	struct svc_fh		fh;
	__u32			offset;
	__u32			count;
<<<<<<< HEAD
	int			vlen;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct nfsd_writeargs {
	svc_fh			fh;
	__u32			offset;
<<<<<<< HEAD
	int			len;
	int			vlen;
=======
	__u32			len;
	struct xdr_buf		payload;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct nfsd_createargs {
	struct svc_fh		fh;
	char *			name;
	unsigned int		len;
	struct iattr		attrs;
};

struct nfsd_renameargs {
	struct svc_fh		ffh;
	char *			fname;
	unsigned int		flen;
	struct svc_fh		tfh;
	char *			tname;
	unsigned int		tlen;
};

<<<<<<< HEAD
struct nfsd_readlinkargs {
	struct svc_fh		fh;
	char *			buffer;
};
	
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct nfsd_linkargs {
	struct svc_fh		ffh;
	struct svc_fh		tfh;
	char *			tname;
	unsigned int		tlen;
};

struct nfsd_symlinkargs {
	struct svc_fh		ffh;
	char *			fname;
	unsigned int		flen;
	char *			tname;
	unsigned int		tlen;
	struct iattr		attrs;
<<<<<<< HEAD
=======
	struct kvec		first;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct nfsd_readdirargs {
	struct svc_fh		fh;
	__u32			cookie;
	__u32			count;
<<<<<<< HEAD
	__be32 *		buffer;
};

struct nfsd_attrstat {
=======
};

struct nfsd_stat {
	__be32			status;
};

struct nfsd_attrstat {
	__be32			status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct svc_fh		fh;
	struct kstat		stat;
};

struct nfsd_diropres  {
<<<<<<< HEAD
=======
	__be32			status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct svc_fh		fh;
	struct kstat		stat;
};

struct nfsd_readlinkres {
<<<<<<< HEAD
	int			len;
};

struct nfsd_readres {
	struct svc_fh		fh;
	unsigned long		count;
	struct kstat		stat;
};

struct nfsd_readdirres {
	int			count;

	struct readdir_cd	common;
	__be32 *		buffer;
	int			buflen;
	__be32 *		offset;
};

struct nfsd_statfsres {
=======
	__be32			status;
	int			len;
	struct page		*page;
};

struct nfsd_readres {
	__be32			status;
	struct svc_fh		fh;
	unsigned long		count;
	struct kstat		stat;
	struct page		**pages;
};

struct nfsd_readdirres {
	/* Components of the reply */
	__be32			status;

	int			count;

	/* Used to encode the reply's entry list */
	struct xdr_stream	xdr;
	struct xdr_buf		dirlist;
	struct readdir_cd	common;
	unsigned int		cookie_offset;
};

struct nfsd_statfsres {
	__be32			status;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct kstatfs		stats;
};

/*
 * Storage requirements for XDR arguments and results.
 */
union nfsd_xdrstore {
	struct nfsd_sattrargs	sattr;
	struct nfsd_diropargs	dirop;
	struct nfsd_readargs	read;
	struct nfsd_writeargs	write;
	struct nfsd_createargs	create;
	struct nfsd_renameargs	rename;
	struct nfsd_linkargs	link;
	struct nfsd_symlinkargs	symlink;
	struct nfsd_readdirargs	readdir;
};

#define NFS2_SVC_XDRSIZE	sizeof(union nfsd_xdrstore)


<<<<<<< HEAD
int nfssvc_decode_void(struct svc_rqst *, __be32 *, void *);
int nfssvc_decode_fhandle(struct svc_rqst *, __be32 *, struct nfsd_fhandle *);
int nfssvc_decode_sattrargs(struct svc_rqst *, __be32 *,
				struct nfsd_sattrargs *);
int nfssvc_decode_diropargs(struct svc_rqst *, __be32 *,
				struct nfsd_diropargs *);
int nfssvc_decode_readargs(struct svc_rqst *, __be32 *,
				struct nfsd_readargs *);
int nfssvc_decode_writeargs(struct svc_rqst *, __be32 *,
				struct nfsd_writeargs *);
int nfssvc_decode_createargs(struct svc_rqst *, __be32 *,
				struct nfsd_createargs *);
int nfssvc_decode_renameargs(struct svc_rqst *, __be32 *,
				struct nfsd_renameargs *);
int nfssvc_decode_readlinkargs(struct svc_rqst *, __be32 *,
				struct nfsd_readlinkargs *);
int nfssvc_decode_linkargs(struct svc_rqst *, __be32 *,
				struct nfsd_linkargs *);
int nfssvc_decode_symlinkargs(struct svc_rqst *, __be32 *,
				struct nfsd_symlinkargs *);
int nfssvc_decode_readdirargs(struct svc_rqst *, __be32 *,
				struct nfsd_readdirargs *);
int nfssvc_encode_void(struct svc_rqst *, __be32 *, void *);
int nfssvc_encode_attrstat(struct svc_rqst *, __be32 *, struct nfsd_attrstat *);
int nfssvc_encode_diropres(struct svc_rqst *, __be32 *, struct nfsd_diropres *);
int nfssvc_encode_readlinkres(struct svc_rqst *, __be32 *, struct nfsd_readlinkres *);
int nfssvc_encode_readres(struct svc_rqst *, __be32 *, struct nfsd_readres *);
int nfssvc_encode_statfsres(struct svc_rqst *, __be32 *, struct nfsd_statfsres *);
int nfssvc_encode_readdirres(struct svc_rqst *, __be32 *, struct nfsd_readdirres *);

int nfssvc_encode_entry(void *, const char *name,
			int namlen, loff_t offset, u64 ino, unsigned int);

int nfssvc_release_fhandle(struct svc_rqst *, __be32 *, struct nfsd_fhandle *);

/* Helper functions for NFSv2 ACL code */
__be32 *nfs2svc_encode_fattr(struct svc_rqst *rqstp, __be32 *p, struct svc_fh *fhp);
__be32 *nfs2svc_decode_fh(__be32 *p, struct svc_fh *fhp);
=======
bool nfssvc_decode_fhandleargs(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_decode_sattrargs(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_decode_diropargs(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_decode_readargs(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_decode_writeargs(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_decode_createargs(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_decode_renameargs(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_decode_linkargs(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_decode_symlinkargs(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_decode_readdirargs(struct svc_rqst *rqstp, struct xdr_stream *xdr);

bool nfssvc_encode_statres(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_encode_attrstatres(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_encode_diropres(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_encode_readlinkres(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_encode_readres(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_encode_statfsres(struct svc_rqst *rqstp, struct xdr_stream *xdr);
bool nfssvc_encode_readdirres(struct svc_rqst *rqstp, struct xdr_stream *xdr);

void nfssvc_encode_nfscookie(struct nfsd_readdirres *resp, u32 offset);
int nfssvc_encode_entry(void *data, const char *name, int namlen,
			loff_t offset, u64 ino, unsigned int d_type);

void nfssvc_release_attrstat(struct svc_rqst *rqstp);
void nfssvc_release_diropres(struct svc_rqst *rqstp);
void nfssvc_release_readres(struct svc_rqst *rqstp);

/* Helper functions for NFSv2 ACL code */
bool svcxdr_decode_fhandle(struct xdr_stream *xdr, struct svc_fh *fhp);
bool svcxdr_encode_stat(struct xdr_stream *xdr, __be32 status);
bool svcxdr_encode_fattr(struct svc_rqst *rqstp, struct xdr_stream *xdr,
			 const struct svc_fh *fhp, const struct kstat *stat);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* LINUX_NFSD_H */
