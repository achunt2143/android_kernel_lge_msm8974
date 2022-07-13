<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* NFS filesystem cache interface
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

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/nfs_fs.h>
#include <linux/nfs_fs_sb.h>
#include <linux/in6.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/iversion.h>
#include <linux/xarray.h>
#include <linux/fscache.h>
#include <linux/netfs.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "internal.h"
#include "iostat.h"
#include "fscache.h"
<<<<<<< HEAD

#define NFSDBG_FACILITY		NFSDBG_FSCACHE

static struct rb_root nfs_fscache_keys = RB_ROOT;
static DEFINE_SPINLOCK(nfs_fscache_keys_lock);
=======
#include "nfstrace.h"

#define NFS_MAX_KEY_LEN 1000

static bool nfs_append_int(char *key, int *_len, unsigned long long x)
{
	if (*_len > NFS_MAX_KEY_LEN)
		return false;
	if (x == 0)
		key[(*_len)++] = ',';
	else
		*_len += sprintf(key + *_len, ",%llx", x);
	return true;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Get the per-client index cookie for an NFS client if the appropriate mount
 * flag was set
 * - We always try and get an index cookie for the client, but get filehandle
 *   cookies on a per-superblock basis, depending on the mount flags
 */
<<<<<<< HEAD
void nfs_fscache_get_client_cookie(struct nfs_client *clp)
{
	/* create a cache index for looking up filehandles */
	clp->fscache = fscache_acquire_cookie(nfs_fscache_netfs.primary_index,
					      &nfs_fscache_server_index_def,
					      clp);
	dfprintk(FSCACHE, "NFS: get client cookie (0x%p/0x%p)\n",
		 clp, clp->fscache);
}

/*
 * Dispose of a per-client cookie
 */
void nfs_fscache_release_client_cookie(struct nfs_client *clp)
{
	dfprintk(FSCACHE, "NFS: releasing client cookie (0x%p/0x%p)\n",
		 clp, clp->fscache);

	fscache_relinquish_cookie(clp->fscache, 0);
	clp->fscache = NULL;
}

/*
 * Get the cache cookie for an NFS superblock.  We have to handle
 * uniquification here because the cache doesn't do it for us.
=======
static bool nfs_fscache_get_client_key(struct nfs_client *clp,
				       char *key, int *_len)
{
	const struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *) &clp->cl_addr;
	const struct sockaddr_in *sin = (struct sockaddr_in *) &clp->cl_addr;

	*_len += snprintf(key + *_len, NFS_MAX_KEY_LEN - *_len,
			  ",%u.%u,%x",
			  clp->rpc_ops->version,
			  clp->cl_minorversion,
			  clp->cl_addr.ss_family);

	switch (clp->cl_addr.ss_family) {
	case AF_INET:
		if (!nfs_append_int(key, _len, sin->sin_port) ||
		    !nfs_append_int(key, _len, sin->sin_addr.s_addr))
			return false;
		return true;

	case AF_INET6:
		if (!nfs_append_int(key, _len, sin6->sin6_port) ||
		    !nfs_append_int(key, _len, sin6->sin6_addr.s6_addr32[0]) ||
		    !nfs_append_int(key, _len, sin6->sin6_addr.s6_addr32[1]) ||
		    !nfs_append_int(key, _len, sin6->sin6_addr.s6_addr32[2]) ||
		    !nfs_append_int(key, _len, sin6->sin6_addr.s6_addr32[3]))
			return false;
		return true;

	default:
		printk(KERN_WARNING "NFS: Unknown network family '%d'\n",
		       clp->cl_addr.ss_family);
		return false;
	}
}

/*
 * Get the cache cookie for an NFS superblock.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * The default uniquifier is just an empty string, but it may be overridden
 * either by the 'fsc=xxx' option to mount, or by inheriting it from the parent
 * superblock across an automount point of some nature.
 */
<<<<<<< HEAD
void nfs_fscache_get_super_cookie(struct super_block *sb, const char *uniq,
				  struct nfs_clone_mount *mntdata)
{
	struct nfs_fscache_key *key, *xkey;
	struct nfs_server *nfss = NFS_SB(sb);
	struct rb_node **p, *parent;
	int diff, ulen;

	if (uniq) {
		ulen = strlen(uniq);
	} else if (mntdata) {
		struct nfs_server *mnt_s = NFS_SB(mntdata->sb);
		if (mnt_s->fscache_key) {
			uniq = mnt_s->fscache_key->key.uniquifier;
			ulen = mnt_s->fscache_key->key.uniq_len;
		}
	}

	if (!uniq) {
		uniq = "";
		ulen = 1;
	}

	key = kzalloc(sizeof(*key) + ulen, GFP_KERNEL);
	if (!key)
		return;

	key->nfs_client = nfss->nfs_client;
	key->key.super.s_flags = sb->s_flags & NFS_MS_MASK;
	key->key.nfs_server.flags = nfss->flags;
	key->key.nfs_server.rsize = nfss->rsize;
	key->key.nfs_server.wsize = nfss->wsize;
	key->key.nfs_server.acregmin = nfss->acregmin;
	key->key.nfs_server.acregmax = nfss->acregmax;
	key->key.nfs_server.acdirmin = nfss->acdirmin;
	key->key.nfs_server.acdirmax = nfss->acdirmax;
	key->key.nfs_server.fsid = nfss->fsid;
	key->key.rpc_auth.au_flavor = nfss->client->cl_auth->au_flavor;

	key->key.uniq_len = ulen;
	memcpy(key->key.uniquifier, uniq, ulen);

	spin_lock(&nfs_fscache_keys_lock);
	p = &nfs_fscache_keys.rb_node;
	parent = NULL;
	while (*p) {
		parent = *p;
		xkey = rb_entry(parent, struct nfs_fscache_key, node);

		if (key->nfs_client < xkey->nfs_client)
			goto go_left;
		if (key->nfs_client > xkey->nfs_client)
			goto go_right;

		diff = memcmp(&key->key, &xkey->key, sizeof(key->key));
		if (diff < 0)
			goto go_left;
		if (diff > 0)
			goto go_right;

		if (key->key.uniq_len == 0)
			goto non_unique;
		diff = memcmp(key->key.uniquifier,
			      xkey->key.uniquifier,
			      key->key.uniq_len);
		if (diff < 0)
			goto go_left;
		if (diff > 0)
			goto go_right;
		goto non_unique;

	go_left:
		p = &(*p)->rb_left;
		continue;
	go_right:
		p = &(*p)->rb_right;
	}

	rb_link_node(&key->node, parent, p);
	rb_insert_color(&key->node, &nfs_fscache_keys);
	spin_unlock(&nfs_fscache_keys_lock);
	nfss->fscache_key = key;

	/* create a cache index for looking up filehandles */
	nfss->fscache = fscache_acquire_cookie(nfss->nfs_client->fscache,
					       &nfs_fscache_super_index_def,
					       nfss);
	dfprintk(FSCACHE, "NFS: get superblock cookie (0x%p/0x%p)\n",
		 nfss, nfss->fscache);
	return;

non_unique:
	spin_unlock(&nfs_fscache_keys_lock);
	kfree(key);
	nfss->fscache_key = NULL;
	nfss->fscache = NULL;
	printk(KERN_WARNING "NFS:"
	       " Cache request denied due to non-unique superblock keys\n");
=======
int nfs_fscache_get_super_cookie(struct super_block *sb, const char *uniq, int ulen)
{
	struct fscache_volume *vcookie;
	struct nfs_server *nfss = NFS_SB(sb);
	unsigned int len = 3;
	char *key;

	if (uniq) {
		nfss->fscache_uniq = kmemdup_nul(uniq, ulen, GFP_KERNEL);
		if (!nfss->fscache_uniq)
			return -ENOMEM;
	}

	key = kmalloc(NFS_MAX_KEY_LEN + 24, GFP_KERNEL);
	if (!key)
		return -ENOMEM;

	memcpy(key, "nfs", 3);
	if (!nfs_fscache_get_client_key(nfss->nfs_client, key, &len) ||
	    !nfs_append_int(key, &len, nfss->fsid.major) ||
	    !nfs_append_int(key, &len, nfss->fsid.minor) ||
	    !nfs_append_int(key, &len, sb->s_flags & NFS_SB_MASK) ||
	    !nfs_append_int(key, &len, nfss->flags) ||
	    !nfs_append_int(key, &len, nfss->rsize) ||
	    !nfs_append_int(key, &len, nfss->wsize) ||
	    !nfs_append_int(key, &len, nfss->acregmin) ||
	    !nfs_append_int(key, &len, nfss->acregmax) ||
	    !nfs_append_int(key, &len, nfss->acdirmin) ||
	    !nfs_append_int(key, &len, nfss->acdirmax) ||
	    !nfs_append_int(key, &len, nfss->client->cl_auth->au_flavor))
		goto out;

	if (ulen > 0) {
		if (ulen > NFS_MAX_KEY_LEN - len)
			goto out;
		key[len++] = ',';
		memcpy(key + len, uniq, ulen);
		len += ulen;
	}
	key[len] = 0;

	/* create a cache index for looking up filehandles */
	vcookie = fscache_acquire_volume(key,
					 NULL, /* preferred_cache */
					 NULL, 0 /* coherency_data */);
	if (IS_ERR(vcookie)) {
		if (vcookie != ERR_PTR(-EBUSY)) {
			kfree(key);
			return PTR_ERR(vcookie);
		}
		pr_err("NFS: Cache volume key already in use (%s)\n", key);
		vcookie = NULL;
	}
	nfss->fscache = vcookie;

out:
	kfree(key);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * release a per-superblock cookie
 */
void nfs_fscache_release_super_cookie(struct super_block *sb)
{
	struct nfs_server *nfss = NFS_SB(sb);

<<<<<<< HEAD
	dfprintk(FSCACHE, "NFS: releasing superblock cookie (0x%p/0x%p)\n",
		 nfss, nfss->fscache);

	fscache_relinquish_cookie(nfss->fscache, 0);
	nfss->fscache = NULL;

	if (nfss->fscache_key) {
		spin_lock(&nfs_fscache_keys_lock);
		rb_erase(&nfss->fscache_key->node, &nfs_fscache_keys);
		spin_unlock(&nfs_fscache_keys_lock);
		kfree(nfss->fscache_key);
		nfss->fscache_key = NULL;
	}
=======
	fscache_relinquish_volume(nfss->fscache, NULL, false);
	nfss->fscache = NULL;
	kfree(nfss->fscache_uniq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Initialise the per-inode cache cookie pointer for an NFS inode.
 */
<<<<<<< HEAD
void nfs_fscache_init_inode_cookie(struct inode *inode)
{
	NFS_I(inode)->fscache = NULL;
	if (S_ISREG(inode->i_mode))
		set_bit(NFS_INO_FSCACHE, &NFS_I(inode)->flags);
}

/*
 * Get the per-inode cache cookie for an NFS inode.
 */
static void nfs_fscache_enable_inode_cookie(struct inode *inode)
{
	struct super_block *sb = inode->i_sb;
	struct nfs_inode *nfsi = NFS_I(inode);

	if (nfsi->fscache || !NFS_FSCACHE(inode))
		return;

	if ((NFS_SB(sb)->options & NFS_OPTION_FSCACHE)) {
		nfsi->fscache = fscache_acquire_cookie(
			NFS_SB(sb)->fscache,
			&nfs_fscache_inode_object_def,
			nfsi);

		dfprintk(FSCACHE, "NFS: get FH cookie (0x%p/0x%p/0x%p)\n",
			 sb, nfsi, nfsi->fscache);
	}
=======
void nfs_fscache_init_inode(struct inode *inode)
{
	struct nfs_fscache_inode_auxdata auxdata;
	struct nfs_server *nfss = NFS_SERVER(inode);
	struct nfs_inode *nfsi = NFS_I(inode);

	netfs_inode(inode)->cache = NULL;
	if (!(nfss->fscache && S_ISREG(inode->i_mode)))
		return;

	nfs_fscache_update_auxdata(&auxdata, inode);

	netfs_inode(inode)->cache = fscache_acquire_cookie(
					       nfss->fscache,
					       0,
					       nfsi->fh.data, /* index_key */
					       nfsi->fh.size,
					       &auxdata,      /* aux_data */
					       sizeof(auxdata),
					       i_size_read(inode));

	if (netfs_inode(inode)->cache)
		mapping_set_release_always(inode->i_mapping);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Release a per-inode cookie.
 */
<<<<<<< HEAD
void nfs_fscache_release_inode_cookie(struct inode *inode)
{
	struct nfs_inode *nfsi = NFS_I(inode);

	dfprintk(FSCACHE, "NFS: clear cookie (0x%p/0x%p)\n",
		 nfsi, nfsi->fscache);

	fscache_relinquish_cookie(nfsi->fscache, 0);
	nfsi->fscache = NULL;
}

/*
 * Retire a per-inode cookie, destroying the data attached to it.
 */
void nfs_fscache_zap_inode_cookie(struct inode *inode)
{
	struct nfs_inode *nfsi = NFS_I(inode);

	dfprintk(FSCACHE, "NFS: zapping cookie (0x%p/0x%p)\n",
		 nfsi, nfsi->fscache);

	fscache_relinquish_cookie(nfsi->fscache, 1);
	nfsi->fscache = NULL;
}

/*
 * Turn off the cache with regard to a per-inode cookie if opened for writing,
 * invalidating all the pages in the page cache relating to the associated
 * inode to clear the per-page caching.
 */
static void nfs_fscache_disable_inode_cookie(struct inode *inode)
{
	clear_bit(NFS_INO_FSCACHE, &NFS_I(inode)->flags);

	if (NFS_I(inode)->fscache) {
		dfprintk(FSCACHE,
			 "NFS: nfsi 0x%p turning cache off\n", NFS_I(inode));

		/* Need to uncache any pages attached to this inode that
		 * fscache knows about before turning off the cache.
		 */
		fscache_uncache_all_inode_pages(NFS_I(inode)->fscache, inode);
		nfs_fscache_zap_inode_cookie(inode);
	}
}

/*
 * wait_on_bit() sleep function for uninterruptible waiting
 */
static int nfs_fscache_wait_bit(void *flags)
{
	schedule();
	return 0;
}

/*
 * Lock against someone else trying to also acquire or relinquish a cookie
 */
static inline void nfs_fscache_inode_lock(struct inode *inode)
{
	struct nfs_inode *nfsi = NFS_I(inode);

	while (test_and_set_bit(NFS_INO_FSCACHE_LOCK, &nfsi->flags))
		wait_on_bit(&nfsi->flags, NFS_INO_FSCACHE_LOCK,
			    nfs_fscache_wait_bit, TASK_UNINTERRUPTIBLE);
}

/*
 * Unlock cookie management lock
 */
static inline void nfs_fscache_inode_unlock(struct inode *inode)
{
	struct nfs_inode *nfsi = NFS_I(inode);

	smp_mb__before_clear_bit();
	clear_bit(NFS_INO_FSCACHE_LOCK, &nfsi->flags);
	smp_mb__after_clear_bit();
	wake_up_bit(&nfsi->flags, NFS_INO_FSCACHE_LOCK);
}

/*
 * Decide if we should enable or disable local caching for this inode.
 * - For now, with NFS, only regular files that are open read-only will be able
 *   to use the cache.
 * - May be invoked multiple times in parallel by parallel nfs_open() functions.
 */
void nfs_fscache_set_inode_cookie(struct inode *inode, struct file *filp)
{
	if (NFS_FSCACHE(inode)) {
		nfs_fscache_inode_lock(inode);
		if ((filp->f_flags & O_ACCMODE) != O_RDONLY)
			nfs_fscache_disable_inode_cookie(inode);
		else
			nfs_fscache_enable_inode_cookie(inode);
		nfs_fscache_inode_unlock(inode);
	}
}

/*
 * Replace a per-inode cookie due to revalidation detecting a file having
 * changed on the server.
 */
void nfs_fscache_reset_inode_cookie(struct inode *inode)
{
	struct nfs_inode *nfsi = NFS_I(inode);
	struct nfs_server *nfss = NFS_SERVER(inode);
	NFS_IFDEBUG(struct fscache_cookie *old = nfsi->fscache);

	nfs_fscache_inode_lock(inode);
	if (nfsi->fscache) {
		/* retire the current fscache cache and get a new one */
		fscache_relinquish_cookie(nfsi->fscache, 1);

		nfsi->fscache = fscache_acquire_cookie(
			nfss->nfs_client->fscache,
			&nfs_fscache_inode_object_def,
			nfsi);

		dfprintk(FSCACHE,
			 "NFS: revalidation new cookie (0x%p/0x%p/0x%p/0x%p)\n",
			 nfss, nfsi, old, nfsi->fscache);
	}
	nfs_fscache_inode_unlock(inode);
}

/*
 * Release the caching state associated with a page, if the page isn't busy
 * interacting with the cache.
 * - Returns true (can release page) or false (page busy).
 */
int nfs_fscache_release_page(struct page *page, gfp_t gfp)
{
	if (PageFsCache(page)) {
		struct nfs_inode *nfsi = NFS_I(page->mapping->host);
		struct fscache_cookie *cookie = nfsi->fscache;

		BUG_ON(!cookie);
		dfprintk(FSCACHE, "NFS: fscache releasepage (0x%p/0x%p/0x%p)\n",
			 cookie, page, nfsi);

		if (!fscache_maybe_release_page(cookie, page, gfp))
			return 0;

		nfs_add_fscache_stats(page->mapping->host,
				      NFSIOS_FSCACHE_PAGES_UNCACHED, 1);
	}
=======
void nfs_fscache_clear_inode(struct inode *inode)
{
	fscache_relinquish_cookie(netfs_i_cookie(netfs_inode(inode)), false);
	netfs_inode(inode)->cache = NULL;
}

/*
 * Enable or disable caching for a file that is being opened as appropriate.
 * The cookie is allocated when the inode is initialised, but is not enabled at
 * that time.  Enablement is deferred to file-open time to avoid stat() and
 * access() thrashing the cache.
 *
 * For now, with NFS, only regular files that are open read-only will be able
 * to use the cache.
 *
 * We enable the cache for an inode if we open it read-only and it isn't
 * currently open for writing.  We disable the cache if the inode is open
 * write-only.
 *
 * The caller uses the file struct to pin i_writecount on the inode before
 * calling us when a file is opened for writing, so we can make use of that.
 *
 * Note that this may be invoked multiple times in parallel by parallel
 * nfs_open() functions.
 */
void nfs_fscache_open_file(struct inode *inode, struct file *filp)
{
	struct nfs_fscache_inode_auxdata auxdata;
	struct fscache_cookie *cookie = netfs_i_cookie(netfs_inode(inode));
	bool open_for_write = inode_is_open_for_write(inode);

	if (!fscache_cookie_valid(cookie))
		return;

	fscache_use_cookie(cookie, open_for_write);
	if (open_for_write) {
		nfs_fscache_update_auxdata(&auxdata, inode);
		fscache_invalidate(cookie, &auxdata, i_size_read(inode),
				   FSCACHE_INVAL_DIO_WRITE);
	}
}
EXPORT_SYMBOL_GPL(nfs_fscache_open_file);

void nfs_fscache_release_file(struct inode *inode, struct file *filp)
{
	struct nfs_fscache_inode_auxdata auxdata;
	struct fscache_cookie *cookie = netfs_i_cookie(netfs_inode(inode));
	loff_t i_size = i_size_read(inode);

	nfs_fscache_update_auxdata(&auxdata, inode);
	fscache_unuse_cookie(cookie, &auxdata, &i_size);
}

int nfs_netfs_read_folio(struct file *file, struct folio *folio)
{
	if (!netfs_inode(folio_inode(folio))->cache)
		return -ENOBUFS;

	return netfs_read_folio(file, folio);
}

int nfs_netfs_readahead(struct readahead_control *ractl)
{
	struct inode *inode = ractl->mapping->host;

	if (!netfs_inode(inode)->cache)
		return -ENOBUFS;

	netfs_readahead(ractl);
	return 0;
}

static atomic_t nfs_netfs_debug_id;
static int nfs_netfs_init_request(struct netfs_io_request *rreq, struct file *file)
{
	rreq->netfs_priv = get_nfs_open_context(nfs_file_open_context(file));
	rreq->debug_id = atomic_inc_return(&nfs_netfs_debug_id);

	return 0;
}

static void nfs_netfs_free_request(struct netfs_io_request *rreq)
{
	put_nfs_open_context(rreq->netfs_priv);
}

static struct nfs_netfs_io_data *nfs_netfs_alloc(struct netfs_io_subrequest *sreq)
{
	struct nfs_netfs_io_data *netfs;

	netfs = kzalloc(sizeof(*netfs), GFP_KERNEL_ACCOUNT);
	if (!netfs)
		return NULL;
	netfs->sreq = sreq;
	refcount_set(&netfs->refcount, 1);
	return netfs;
}

static bool nfs_netfs_clamp_length(struct netfs_io_subrequest *sreq)
{
	size_t	rsize = NFS_SB(sreq->rreq->inode->i_sb)->rsize;

	sreq->len = min(sreq->len, rsize);
	return true;
}

static void nfs_netfs_issue_read(struct netfs_io_subrequest *sreq)
{
	struct nfs_netfs_io_data	*netfs;
	struct nfs_pageio_descriptor	pgio;
	struct inode *inode = sreq->rreq->inode;
	struct nfs_open_context *ctx = sreq->rreq->netfs_priv;
	struct page *page;
	unsigned long idx;
	int err;
	pgoff_t start = (sreq->start + sreq->transferred) >> PAGE_SHIFT;
	pgoff_t last = ((sreq->start + sreq->len -
			 sreq->transferred - 1) >> PAGE_SHIFT);

	nfs_pageio_init_read(&pgio, inode, false,
			     &nfs_async_read_completion_ops);

	netfs = nfs_netfs_alloc(sreq);
	if (!netfs)
		return netfs_subreq_terminated(sreq, -ENOMEM, false);

	pgio.pg_netfs = netfs; /* used in completion */

	xa_for_each_range(&sreq->rreq->mapping->i_pages, idx, page, start, last) {
		/* nfs_read_add_folio() may schedule() due to pNFS layout and other RPCs  */
		err = nfs_read_add_folio(&pgio, ctx, page_folio(page));
		if (err < 0) {
			netfs->error = err;
			goto out;
		}
	}
out:
	nfs_pageio_complete_read(&pgio);
	nfs_netfs_put(netfs);
}

void nfs_netfs_initiate_read(struct nfs_pgio_header *hdr)
{
	struct nfs_netfs_io_data        *netfs = hdr->netfs;

	if (!netfs)
		return;

	nfs_netfs_get(netfs);
}

int nfs_netfs_folio_unlock(struct folio *folio)
{
	struct inode *inode = folio_file_mapping(folio)->host;

	/*
	 * If fscache is enabled, netfs will unlock pages.
	 */
	if (netfs_inode(inode)->cache)
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 1;
}

<<<<<<< HEAD
/*
 * Release the caching state associated with a page if undergoing complete page
 * invalidation.
 */
void __nfs_fscache_invalidate_page(struct page *page, struct inode *inode)
{
	struct nfs_inode *nfsi = NFS_I(inode);
	struct fscache_cookie *cookie = nfsi->fscache;

	BUG_ON(!cookie);

	dfprintk(FSCACHE, "NFS: fscache invalidatepage (0x%p/0x%p/0x%p)\n",
		 cookie, page, nfsi);

	fscache_wait_on_page_write(cookie, page);

	BUG_ON(!PageLocked(page));
	fscache_uncache_page(cookie, page);
	nfs_add_fscache_stats(page->mapping->host,
			      NFSIOS_FSCACHE_PAGES_UNCACHED, 1);
}

/*
 * Handle completion of a page being read from the cache.
 * - Called in process (keventd) context.
 */
static void nfs_readpage_from_fscache_complete(struct page *page,
					       void *context,
					       int error)
{
	dfprintk(FSCACHE,
		 "NFS: readpage_from_fscache_complete (0x%p/0x%p/%d)\n",
		 page, context, error);

	/* if the read completes with an error, we just unlock the page and let
	 * the VM reissue the readpage */
	if (!error) {
		SetPageUptodate(page);
		unlock_page(page);
	} else {
		error = nfs_readpage_async(context, page->mapping->host, page);
		if (error)
			unlock_page(page);
	}
}

/*
 * Retrieve a page from fscache
 */
int __nfs_readpage_from_fscache(struct nfs_open_context *ctx,
				struct inode *inode, struct page *page)
{
	int ret;

	dfprintk(FSCACHE,
		 "NFS: readpage_from_fscache(fsc:%p/p:%p(i:%lx f:%lx)/0x%p)\n",
		 NFS_I(inode)->fscache, page, page->index, page->flags, inode);

	ret = fscache_read_or_alloc_page(NFS_I(inode)->fscache,
					 page,
					 nfs_readpage_from_fscache_complete,
					 ctx,
					 GFP_KERNEL);

	switch (ret) {
	case 0: /* read BIO submitted (page in fscache) */
		dfprintk(FSCACHE,
			 "NFS:    readpage_from_fscache: BIO submitted\n");
		nfs_add_fscache_stats(inode, NFSIOS_FSCACHE_PAGES_READ_OK, 1);
		return ret;

	case -ENOBUFS: /* inode not in cache */
	case -ENODATA: /* page not in cache */
		nfs_add_fscache_stats(inode, NFSIOS_FSCACHE_PAGES_READ_FAIL, 1);
		dfprintk(FSCACHE,
			 "NFS:    readpage_from_fscache %d\n", ret);
		return 1;

	default:
		dfprintk(FSCACHE, "NFS:    readpage_from_fscache %d\n", ret);
		nfs_add_fscache_stats(inode, NFSIOS_FSCACHE_PAGES_READ_FAIL, 1);
	}
	return ret;
}

/*
 * Retrieve a set of pages from fscache
 */
int __nfs_readpages_from_fscache(struct nfs_open_context *ctx,
				 struct inode *inode,
				 struct address_space *mapping,
				 struct list_head *pages,
				 unsigned *nr_pages)
{
	unsigned npages = *nr_pages;
	int ret;

	dfprintk(FSCACHE, "NFS: nfs_getpages_from_fscache (0x%p/%u/0x%p)\n",
		 NFS_I(inode)->fscache, npages, inode);

	ret = fscache_read_or_alloc_pages(NFS_I(inode)->fscache,
					  mapping, pages, nr_pages,
					  nfs_readpage_from_fscache_complete,
					  ctx,
					  mapping_gfp_mask(mapping));
	if (*nr_pages < npages)
		nfs_add_fscache_stats(inode, NFSIOS_FSCACHE_PAGES_READ_OK,
				      npages);
	if (*nr_pages > 0)
		nfs_add_fscache_stats(inode, NFSIOS_FSCACHE_PAGES_READ_FAIL,
				      *nr_pages);

	switch (ret) {
	case 0: /* read submitted to the cache for all pages */
		BUG_ON(!list_empty(pages));
		BUG_ON(*nr_pages != 0);
		dfprintk(FSCACHE,
			 "NFS: nfs_getpages_from_fscache: submitted\n");

		return ret;

	case -ENOBUFS: /* some pages aren't cached and can't be */
	case -ENODATA: /* some pages aren't cached */
		dfprintk(FSCACHE,
			 "NFS: nfs_getpages_from_fscache: no page: %d\n", ret);
		return 1;

	default:
		dfprintk(FSCACHE,
			 "NFS: nfs_getpages_from_fscache: ret  %d\n", ret);
	}

	return ret;
}

/*
 * Store a newly fetched page in fscache
 * - PG_fscache must be set on the page
 */
void __nfs_readpage_to_fscache(struct inode *inode, struct page *page, int sync)
{
	int ret;

	dfprintk(FSCACHE,
		 "NFS: readpage_to_fscache(fsc:%p/p:%p(i:%lx f:%lx)/%d)\n",
		 NFS_I(inode)->fscache, page, page->index, page->flags, sync);

	ret = fscache_write_page(NFS_I(inode)->fscache, page, GFP_KERNEL);
	dfprintk(FSCACHE,
		 "NFS:     readpage_to_fscache: p:%p(i:%lu f:%lx) ret %d\n",
		 page, page->index, page->flags, ret);

	if (ret != 0) {
		fscache_uncache_page(NFS_I(inode)->fscache, page);
		nfs_add_fscache_stats(inode,
				      NFSIOS_FSCACHE_PAGES_WRITTEN_FAIL, 1);
		nfs_add_fscache_stats(inode, NFSIOS_FSCACHE_PAGES_UNCACHED, 1);
	} else {
		nfs_add_fscache_stats(inode,
				      NFSIOS_FSCACHE_PAGES_WRITTEN_OK, 1);
	}
}
=======
void nfs_netfs_read_completion(struct nfs_pgio_header *hdr)
{
	struct nfs_netfs_io_data        *netfs = hdr->netfs;
	struct netfs_io_subrequest      *sreq;

	if (!netfs)
		return;

	sreq = netfs->sreq;
	if (test_bit(NFS_IOHDR_EOF, &hdr->flags))
		__set_bit(NETFS_SREQ_CLEAR_TAIL, &sreq->flags);

	if (hdr->error)
		netfs->error = hdr->error;
	else
		atomic64_add(hdr->res.count, &netfs->transferred);

	nfs_netfs_put(netfs);
	hdr->netfs = NULL;
}

const struct netfs_request_ops nfs_netfs_ops = {
	.init_request		= nfs_netfs_init_request,
	.free_request		= nfs_netfs_free_request,
	.issue_read		= nfs_netfs_issue_read,
	.clamp_length		= nfs_netfs_clamp_length
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
