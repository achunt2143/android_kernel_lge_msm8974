<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* NFS filesystem cache interface definitions
 *
 * Copyright (C) 2008 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _NFS_FSCACHE_H
#define _NFS_FSCACHE_H

<<<<<<< HEAD
=======
#include <linux/swap.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/nfs_fs.h>
#include <linux/nfs_mount.h>
#include <linux/nfs4_mount.h>
#include <linux/fscache.h>
<<<<<<< HEAD
=======
#include <linux/iversion.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_NFS_FSCACHE

/*
<<<<<<< HEAD
 * set of NFS FS-Cache objects that form a superblock key
 */
struct nfs_fscache_key {
	struct rb_node		node;
	struct nfs_client	*nfs_client;	/* the server */

	/* the elements of the unique key - as used by nfs_compare_super() and
	 * nfs_compare_mount_options() to distinguish superblocks */
	struct {
		struct {
			unsigned long	s_flags;	/* various flags
							 * (& NFS_MS_MASK) */
		} super;

		struct {
			struct nfs_fsid fsid;
			int		flags;
			unsigned int	rsize;		/* read size */
			unsigned int	wsize;		/* write size */
			unsigned int	acregmin;	/* attr cache timeouts */
			unsigned int	acregmax;
			unsigned int	acdirmin;
			unsigned int	acdirmax;
		} nfs_server;

		struct {
			rpc_authflavor_t au_flavor;
		} rpc_auth;

		/* uniquifier - can be used if nfs_server.flags includes
		 * NFS_MOUNT_UNSHARED  */
		u8 uniq_len;
		char uniquifier[0];
	} key;
};

/*
 * fscache-index.c
 */
extern struct fscache_netfs nfs_fscache_netfs;
extern const struct fscache_cookie_def nfs_fscache_server_index_def;
extern const struct fscache_cookie_def nfs_fscache_super_index_def;
extern const struct fscache_cookie_def nfs_fscache_inode_object_def;

extern int nfs_fscache_register(void);
extern void nfs_fscache_unregister(void);
=======
 * Definition of the auxiliary data attached to NFS inode storage objects
 * within the cache.
 *
 * The contents of this struct are recorded in the on-disk local cache in the
 * auxiliary data attached to the data storage object backing an inode.  This
 * permits coherency to be managed when a new inode binds to an already extant
 * cache object.
 */
struct nfs_fscache_inode_auxdata {
	s64	mtime_sec;
	s64	mtime_nsec;
	s64	ctime_sec;
	s64	ctime_nsec;
	u64	change_attr;
};

struct nfs_netfs_io_data {
	/*
	 * NFS may split a netfs_io_subrequest into multiple RPCs, each
	 * with their own read completion.  In netfs, we can only call
	 * netfs_subreq_terminated() once for each subrequest.  Use the
	 * refcount here to double as a marker of the last RPC completion,
	 * and only call netfs via netfs_subreq_terminated() once.
	 */
	refcount_t			refcount;
	struct netfs_io_subrequest	*sreq;

	/*
	 * Final disposition of the netfs_io_subrequest, sent in
	 * netfs_subreq_terminated()
	 */
	atomic64_t	transferred;
	int		error;
};

static inline void nfs_netfs_get(struct nfs_netfs_io_data *netfs)
{
	refcount_inc(&netfs->refcount);
}

static inline void nfs_netfs_put(struct nfs_netfs_io_data *netfs)
{
	ssize_t final_len;

	/* Only the last RPC completion should call netfs_subreq_terminated() */
	if (!refcount_dec_and_test(&netfs->refcount))
		return;

	/*
	 * The NFS pageio interface may read a complete page, even when netfs
	 * only asked for a partial page.  Specifically, this may be seen when
	 * one thread is truncating a file while another one is reading the last
	 * page of the file.
	 * Correct the final length here to be no larger than the netfs subrequest
	 * length, and thus avoid netfs's "Subreq overread" warning message.
	 */
	final_len = min_t(s64, netfs->sreq->len, atomic64_read(&netfs->transferred));
	netfs_subreq_terminated(netfs->sreq, netfs->error ?: final_len, false);
	kfree(netfs);
}
static inline void nfs_netfs_inode_init(struct nfs_inode *nfsi)
{
	netfs_inode_init(&nfsi->netfs, &nfs_netfs_ops, false);
}
extern void nfs_netfs_initiate_read(struct nfs_pgio_header *hdr);
extern void nfs_netfs_read_completion(struct nfs_pgio_header *hdr);
extern int nfs_netfs_folio_unlock(struct folio *folio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * fscache.c
 */
<<<<<<< HEAD
extern void nfs_fscache_get_client_cookie(struct nfs_client *);
extern void nfs_fscache_release_client_cookie(struct nfs_client *);

extern void nfs_fscache_get_super_cookie(struct super_block *,
					 const char *,
					 struct nfs_clone_mount *);
extern void nfs_fscache_release_super_cookie(struct super_block *);

extern void nfs_fscache_init_inode_cookie(struct inode *);
extern void nfs_fscache_release_inode_cookie(struct inode *);
extern void nfs_fscache_zap_inode_cookie(struct inode *);
extern void nfs_fscache_set_inode_cookie(struct inode *, struct file *);
extern void nfs_fscache_reset_inode_cookie(struct inode *);

extern void __nfs_fscache_invalidate_page(struct page *, struct inode *);
extern int nfs_fscache_release_page(struct page *, gfp_t);

extern int __nfs_readpage_from_fscache(struct nfs_open_context *,
				       struct inode *, struct page *);
extern int __nfs_readpages_from_fscache(struct nfs_open_context *,
					struct inode *, struct address_space *,
					struct list_head *, unsigned *);
extern void __nfs_readpage_to_fscache(struct inode *, struct page *, int);

/*
 * wait for a page to complete writing to the cache
 */
static inline void nfs_fscache_wait_on_page_write(struct nfs_inode *nfsi,
						  struct page *page)
{
	if (PageFsCache(page))
		fscache_wait_on_page_write(nfsi->fscache, page);
}

/*
 * release the caching state associated with a page if undergoing complete page
 * invalidation
 */
static inline void nfs_fscache_invalidate_page(struct page *page,
					       struct inode *inode)
{
	if (PageFsCache(page))
		__nfs_fscache_invalidate_page(page, inode);
}

/*
 * Retrieve a page from an inode data storage object.
 */
static inline int nfs_readpage_from_fscache(struct nfs_open_context *ctx,
					    struct inode *inode,
					    struct page *page)
{
	if (NFS_I(inode)->fscache)
		return __nfs_readpage_from_fscache(ctx, inode, page);
	return -ENOBUFS;
}

/*
 * Retrieve a set of pages from an inode data storage object.
 */
static inline int nfs_readpages_from_fscache(struct nfs_open_context *ctx,
					     struct inode *inode,
					     struct address_space *mapping,
					     struct list_head *pages,
					     unsigned *nr_pages)
{
	if (NFS_I(inode)->fscache)
		return __nfs_readpages_from_fscache(ctx, inode, mapping, pages,
						    nr_pages);
	return -ENOBUFS;
}

/*
 * Store a page newly fetched from the server in an inode data storage object
 * in the cache.
 */
static inline void nfs_readpage_to_fscache(struct inode *inode,
					   struct page *page,
					   int sync)
{
	if (PageFsCache(page))
		__nfs_readpage_to_fscache(inode, page, sync);
=======
extern int nfs_fscache_get_super_cookie(struct super_block *, const char *, int);
extern void nfs_fscache_release_super_cookie(struct super_block *);

extern void nfs_fscache_init_inode(struct inode *);
extern void nfs_fscache_clear_inode(struct inode *);
extern void nfs_fscache_open_file(struct inode *, struct file *);
extern void nfs_fscache_release_file(struct inode *, struct file *);
extern int nfs_netfs_readahead(struct readahead_control *ractl);
extern int nfs_netfs_read_folio(struct file *file, struct folio *folio);

static inline bool nfs_fscache_release_folio(struct folio *folio, gfp_t gfp)
{
	if (folio_test_fscache(folio)) {
		if (current_is_kswapd() || !(gfp & __GFP_FS))
			return false;
		folio_wait_fscache(folio);
	}
	fscache_note_page_release(netfs_i_cookie(netfs_inode(folio->mapping->host)));
	return true;
}

static inline void nfs_fscache_update_auxdata(struct nfs_fscache_inode_auxdata *auxdata,
					      struct inode *inode)
{
	memset(auxdata, 0, sizeof(*auxdata));
	auxdata->mtime_sec  = inode_get_mtime(inode).tv_sec;
	auxdata->mtime_nsec = inode_get_mtime(inode).tv_nsec;
	auxdata->ctime_sec  = inode_get_ctime(inode).tv_sec;
	auxdata->ctime_nsec = inode_get_ctime(inode).tv_nsec;

	if (NFS_SERVER(inode)->nfs_client->rpc_ops->version == 4)
		auxdata->change_attr = inode_peek_iversion_raw(inode);
}

/*
 * Invalidate the contents of fscache for this inode.  This will not sleep.
 */
static inline void nfs_fscache_invalidate(struct inode *inode, int flags)
{
	struct nfs_fscache_inode_auxdata auxdata;
	struct fscache_cookie *cookie =  netfs_i_cookie(&NFS_I(inode)->netfs);

	nfs_fscache_update_auxdata(&auxdata, inode);
	fscache_invalidate(cookie, &auxdata, i_size_read(inode), flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * indicate the client caching state as readable text
 */
static inline const char *nfs_server_fscache_state(struct nfs_server *server)
{
<<<<<<< HEAD
	if (server->fscache && (server->options & NFS_OPTION_FSCACHE))
=======
	if (server->fscache)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return "yes";
	return "no ";
}

<<<<<<< HEAD

#else /* CONFIG_NFS_FSCACHE */
static inline int nfs_fscache_register(void) { return 0; }
static inline void nfs_fscache_unregister(void) {}

static inline void nfs_fscache_get_client_cookie(struct nfs_client *clp) {}
static inline void nfs_fscache_release_client_cookie(struct nfs_client *clp) {}

static inline void nfs_fscache_get_super_cookie(
	struct super_block *sb,
	const char *uniq,
	struct nfs_clone_mount *mntdata)
{
}
static inline void nfs_fscache_release_super_cookie(struct super_block *sb) {}

static inline void nfs_fscache_init_inode_cookie(struct inode *inode) {}
static inline void nfs_fscache_release_inode_cookie(struct inode *inode) {}
static inline void nfs_fscache_zap_inode_cookie(struct inode *inode) {}
static inline void nfs_fscache_set_inode_cookie(struct inode *inode,
						struct file *filp) {}
static inline void nfs_fscache_reset_inode_cookie(struct inode *inode) {}

static inline int nfs_fscache_release_page(struct page *page, gfp_t gfp)
{
	return 1; /* True: may release page */
}
static inline void nfs_fscache_invalidate_page(struct page *page,
					       struct inode *inode) {}
static inline void nfs_fscache_wait_on_page_write(struct nfs_inode *nfsi,
						  struct page *page) {}

static inline int nfs_readpage_from_fscache(struct nfs_open_context *ctx,
					    struct inode *inode,
					    struct page *page)
{
	return -ENOBUFS;
}
static inline int nfs_readpages_from_fscache(struct nfs_open_context *ctx,
					     struct inode *inode,
					     struct address_space *mapping,
					     struct list_head *pages,
					     unsigned *nr_pages)
{
	return -ENOBUFS;
}
static inline void nfs_readpage_to_fscache(struct inode *inode,
					   struct page *page, int sync) {}
=======
static inline void nfs_netfs_set_pgio_header(struct nfs_pgio_header *hdr,
					     struct nfs_pageio_descriptor *desc)
{
	hdr->netfs = desc->pg_netfs;
}
static inline void nfs_netfs_set_pageio_descriptor(struct nfs_pageio_descriptor *desc,
						   struct nfs_pgio_header *hdr)
{
	desc->pg_netfs = hdr->netfs;
}
static inline void nfs_netfs_reset_pageio_descriptor(struct nfs_pageio_descriptor *desc)
{
	desc->pg_netfs = NULL;
}
#else /* CONFIG_NFS_FSCACHE */
static inline void nfs_netfs_inode_init(struct nfs_inode *nfsi) {}
static inline void nfs_netfs_initiate_read(struct nfs_pgio_header *hdr) {}
static inline void nfs_netfs_read_completion(struct nfs_pgio_header *hdr) {}
static inline int nfs_netfs_folio_unlock(struct folio *folio)
{
	return 1;
}
static inline void nfs_fscache_release_super_cookie(struct super_block *sb) {}

static inline void nfs_fscache_init_inode(struct inode *inode) {}
static inline void nfs_fscache_clear_inode(struct inode *inode) {}
static inline void nfs_fscache_open_file(struct inode *inode,
					 struct file *filp) {}
static inline void nfs_fscache_release_file(struct inode *inode, struct file *file) {}
static inline int nfs_netfs_readahead(struct readahead_control *ractl)
{
	return -ENOBUFS;
}
static inline int nfs_netfs_read_folio(struct file *file, struct folio *folio)
{
	return -ENOBUFS;
}

static inline bool nfs_fscache_release_folio(struct folio *folio, gfp_t gfp)
{
	return true; /* may release folio */
}
static inline void nfs_fscache_invalidate(struct inode *inode, int flags) {}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline const char *nfs_server_fscache_state(struct nfs_server *server)
{
	return "no ";
}
<<<<<<< HEAD

=======
static inline void nfs_netfs_set_pgio_header(struct nfs_pgio_header *hdr,
					     struct nfs_pageio_descriptor *desc) {}
static inline void nfs_netfs_set_pageio_descriptor(struct nfs_pageio_descriptor *desc,
						   struct nfs_pgio_header *hdr) {}
static inline void nfs_netfs_reset_pageio_descriptor(struct nfs_pageio_descriptor *desc) {}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_NFS_FSCACHE */
#endif /* _NFS_FSCACHE_H */
