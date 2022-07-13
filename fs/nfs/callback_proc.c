<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/fs/nfs/callback_proc.c
 *
 * Copyright (C) 2004 Trond Myklebust
 *
 * NFSv4 callback procedures
 */
<<<<<<< HEAD
=======

#include <linux/errno.h>
#include <linux/math.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/nfs4.h>
#include <linux/nfs_fs.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>
<<<<<<< HEAD
=======
#include <linux/types.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "nfs4_fs.h"
#include "callback.h"
#include "delegation.h"
#include "internal.h"
#include "pnfs.h"
<<<<<<< HEAD

#ifdef NFS_DEBUG
#define NFSDBG_FACILITY NFSDBG_CALLBACK
#endif

__be32 nfs4_callback_getattr(struct cb_getattrargs *args,
			     struct cb_getattrres *res,
			     struct cb_process_state *cps)
{
	struct nfs_delegation *delegation;
	struct nfs_inode *nfsi;
=======
#include "nfs4session.h"
#include "nfs4trace.h"

#define NFSDBG_FACILITY NFSDBG_CALLBACK

__be32 nfs4_callback_getattr(void *argp, void *resp,
			     struct cb_process_state *cps)
{
	struct cb_getattrargs *args = argp;
	struct cb_getattrres *res = resp;
	struct nfs_delegation *delegation;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct inode *inode;

	res->status = htonl(NFS4ERR_OP_NOT_IN_SESSION);
	if (!cps->clp) /* Always set for v4.0. Set in cb_sequence for v4.1 */
		goto out;

	res->bitmap[0] = res->bitmap[1] = 0;
	res->status = htonl(NFS4ERR_BADHANDLE);

	dprintk_rcu("NFS: GETATTR callback request from %s\n",
		rpc_peeraddr2str(cps->clp->cl_rpcclient, RPC_DISPLAY_ADDR));

	inode = nfs_delegation_find_inode(cps->clp, &args->fh);
<<<<<<< HEAD
	if (inode == NULL)
		goto out;
	nfsi = NFS_I(inode);
	rcu_read_lock();
	delegation = rcu_dereference(nfsi->delegation);
=======
	if (IS_ERR(inode)) {
		if (inode == ERR_PTR(-EAGAIN))
			res->status = htonl(NFS4ERR_DELAY);
		trace_nfs4_cb_getattr(cps->clp, &args->fh, NULL,
				-ntohl(res->status));
		goto out;
	}
	rcu_read_lock();
	delegation = nfs4_get_valid_delegation(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (delegation == NULL || (delegation->type & FMODE_WRITE) == 0)
		goto out_iput;
	res->size = i_size_read(inode);
	res->change_attr = delegation->change_attr;
<<<<<<< HEAD
	if (nfsi->npages != 0)
		res->change_attr++;
	res->ctime = inode->i_ctime;
	res->mtime = inode->i_mtime;
=======
	if (nfs_have_writebacks(inode))
		res->change_attr++;
	res->ctime = inode_get_ctime(inode);
	res->mtime = inode_get_mtime(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	res->bitmap[0] = (FATTR4_WORD0_CHANGE|FATTR4_WORD0_SIZE) &
		args->bitmap[0];
	res->bitmap[1] = (FATTR4_WORD1_TIME_METADATA|FATTR4_WORD1_TIME_MODIFY) &
		args->bitmap[1];
	res->status = 0;
out_iput:
	rcu_read_unlock();
<<<<<<< HEAD
	iput(inode);
=======
	trace_nfs4_cb_getattr(cps->clp, &args->fh, inode, -ntohl(res->status));
	nfs_iput_and_deactive(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	dprintk("%s: exit with status = %d\n", __func__, ntohl(res->status));
	return res->status;
}

<<<<<<< HEAD
__be32 nfs4_callback_recall(struct cb_recallargs *args, void *dummy,
			    struct cb_process_state *cps)
{
=======
__be32 nfs4_callback_recall(void *argp, void *resp,
			    struct cb_process_state *cps)
{
	struct cb_recallargs *args = argp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct inode *inode;
	__be32 res;
	
	res = htonl(NFS4ERR_OP_NOT_IN_SESSION);
	if (!cps->clp) /* Always set for v4.0. Set in cb_sequence for v4.1 */
		goto out;

	dprintk_rcu("NFS: RECALL callback request from %s\n",
		rpc_peeraddr2str(cps->clp->cl_rpcclient, RPC_DISPLAY_ADDR));

	res = htonl(NFS4ERR_BADHANDLE);
	inode = nfs_delegation_find_inode(cps->clp, &args->fh);
<<<<<<< HEAD
	if (inode == NULL)
		goto out;
=======
	if (IS_ERR(inode)) {
		if (inode == ERR_PTR(-EAGAIN))
			res = htonl(NFS4ERR_DELAY);
		trace_nfs4_cb_recall(cps->clp, &args->fh, NULL,
				&args->stateid, -ntohl(res));
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Set up a helper thread to actually return the delegation */
	switch (nfs_async_inode_return_delegation(inode, &args->stateid)) {
	case 0:
		res = 0;
		break;
	case -ENOENT:
		res = htonl(NFS4ERR_BAD_STATEID);
		break;
	default:
		res = htonl(NFS4ERR_RESOURCE);
	}
<<<<<<< HEAD
	iput(inode);
=======
	trace_nfs4_cb_recall(cps->clp, &args->fh, inode,
			&args->stateid, -ntohl(res));
	nfs_iput_and_deactive(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	dprintk("%s: exit with status = %d\n", __func__, ntohl(res));
	return res;
}

#if defined(CONFIG_NFS_V4_1)

/*
<<<<<<< HEAD
 * Lookup a layout by filehandle.
 *
 * Note: gets a refcount on the layout hdr and on its respective inode.
 * Caller must put the layout hdr and the inode.
 *
 * TODO: keep track of all layouts (and delegations) in a hash table
 * hashed by filehandle.
 */
static struct pnfs_layout_hdr * get_layout_by_fh_locked(struct nfs_client *clp, struct nfs_fh *fh)
{
	struct nfs_server *server;
	struct inode *ino;
	struct pnfs_layout_hdr *lo;

	list_for_each_entry_rcu(server, &clp->cl_superblocks, client_link) {
		list_for_each_entry(lo, &server->layouts, plh_layouts) {
			if (nfs_compare_fh(fh, &NFS_I(lo->plh_inode)->fh))
				continue;
			ino = igrab(lo->plh_inode);
			if (!ino)
				continue;
			get_layout_hdr(lo);
			return lo;
		}
	}

	return NULL;
}

static struct pnfs_layout_hdr * get_layout_by_fh(struct nfs_client *clp, struct nfs_fh *fh)
{
	struct pnfs_layout_hdr *lo;

	spin_lock(&clp->cl_lock);
	rcu_read_lock();
	lo = get_layout_by_fh_locked(clp, fh);
	rcu_read_unlock();
	spin_unlock(&clp->cl_lock);

	return lo;
}

static u32 initiate_file_draining(struct nfs_client *clp,
				  struct cb_layoutrecallargs *args)
=======
 * Lookup a layout inode by stateid
 *
 * Note: returns a refcount on the inode and superblock
 */
static struct inode *nfs_layout_find_inode_by_stateid(struct nfs_client *clp,
		const nfs4_stateid *stateid)
	__must_hold(RCU)
{
	struct nfs_server *server;
	struct inode *inode;
	struct pnfs_layout_hdr *lo;

	rcu_read_lock();
	list_for_each_entry_rcu(server, &clp->cl_superblocks, client_link) {
		list_for_each_entry_rcu(lo, &server->layouts, plh_layouts) {
			if (!pnfs_layout_is_valid(lo))
				continue;
			if (!nfs4_stateid_match_other(stateid, &lo->plh_stateid))
				continue;
			if (nfs_sb_active(server->super))
				inode = igrab(lo->plh_inode);
			else
				inode = ERR_PTR(-EAGAIN);
			rcu_read_unlock();
			if (inode)
				return inode;
			nfs_sb_deactive(server->super);
			return ERR_PTR(-EAGAIN);
		}
	}
	rcu_read_unlock();
	return ERR_PTR(-ENOENT);
}

/*
 * Lookup a layout inode by filehandle.
 *
 * Note: returns a refcount on the inode and superblock
 *
 */
static struct inode *nfs_layout_find_inode_by_fh(struct nfs_client *clp,
		const struct nfs_fh *fh)
{
	struct nfs_server *server;
	struct nfs_inode *nfsi;
	struct inode *inode;
	struct pnfs_layout_hdr *lo;

	rcu_read_lock();
	list_for_each_entry_rcu(server, &clp->cl_superblocks, client_link) {
		list_for_each_entry_rcu(lo, &server->layouts, plh_layouts) {
			nfsi = NFS_I(lo->plh_inode);
			if (nfs_compare_fh(fh, &nfsi->fh))
				continue;
			if (nfsi->layout != lo)
				continue;
			if (nfs_sb_active(server->super))
				inode = igrab(lo->plh_inode);
			else
				inode = ERR_PTR(-EAGAIN);
			rcu_read_unlock();
			if (inode)
				return inode;
			nfs_sb_deactive(server->super);
			return ERR_PTR(-EAGAIN);
		}
	}
	rcu_read_unlock();
	return ERR_PTR(-ENOENT);
}

static struct inode *nfs_layout_find_inode(struct nfs_client *clp,
		const struct nfs_fh *fh,
		const nfs4_stateid *stateid)
{
	struct inode *inode;

	inode = nfs_layout_find_inode_by_stateid(clp, stateid);
	if (inode == ERR_PTR(-ENOENT))
		inode = nfs_layout_find_inode_by_fh(clp, fh);
	return inode;
}

/*
 * Enforce RFC5661 section 12.5.5.2.1. (Layout Recall and Return Sequencing)
 */
static u32 pnfs_check_callback_stateid(struct pnfs_layout_hdr *lo,
					const nfs4_stateid *new,
					struct cb_process_state *cps)
{
	u32 oldseq, newseq;

	/* Is the stateid not initialised? */
	if (!pnfs_layout_is_valid(lo))
		return NFS4ERR_NOMATCHING_LAYOUT;

	/* Mismatched stateid? */
	if (!nfs4_stateid_match_other(&lo->plh_stateid, new))
		return NFS4ERR_BAD_STATEID;

	newseq = be32_to_cpu(new->seqid);
	/* Are we already in a layout recall situation? */
	if (test_bit(NFS_LAYOUT_RETURN, &lo->plh_flags))
		return NFS4ERR_DELAY;

	/*
	 * Check that the stateid matches what we think it should be.
	 * Note that if the server sent us a list of referring calls,
	 * and we know that those have completed, then we trust the
	 * stateid argument is correct.
	 */
	oldseq = be32_to_cpu(lo->plh_stateid.seqid);
	if (newseq > oldseq + 1 && !cps->referring_calls)
		return NFS4ERR_DELAY;

	/* Crazy server! */
	if (newseq <= oldseq)
		return NFS4ERR_OLD_STATEID;

	return NFS_OK;
}

static u32 initiate_file_draining(struct nfs_client *clp,
				  struct cb_layoutrecallargs *args,
				  struct cb_process_state *cps)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *ino;
	struct pnfs_layout_hdr *lo;
	u32 rv = NFS4ERR_NOMATCHING_LAYOUT;
	LIST_HEAD(free_me_list);

<<<<<<< HEAD
	lo = get_layout_by_fh(clp, &args->cbl_fh);
	if (!lo)
		return NFS4ERR_NOMATCHING_LAYOUT;

	ino = lo->plh_inode;
	spin_lock(&ino->i_lock);
	if (test_bit(NFS_LAYOUT_BULK_RECALL, &lo->plh_flags) ||
	    mark_matching_lsegs_invalid(lo, &free_me_list,
					&args->cbl_range))
		rv = NFS4ERR_DELAY;
	else
		rv = NFS4ERR_NOMATCHING_LAYOUT;
	pnfs_set_layout_stateid(lo, &args->cbl_stateid, true);
	spin_unlock(&ino->i_lock);
	pnfs_free_lseg_list(&free_me_list);
	put_layout_hdr(lo);
	iput(ino);
=======
	ino = nfs_layout_find_inode(clp, &args->cbl_fh, &args->cbl_stateid);
	if (IS_ERR(ino)) {
		if (ino == ERR_PTR(-EAGAIN))
			rv = NFS4ERR_DELAY;
		goto out_noput;
	}

	pnfs_layoutcommit_inode(ino, false);


	spin_lock(&ino->i_lock);
	lo = NFS_I(ino)->layout;
	if (!lo) {
		spin_unlock(&ino->i_lock);
		goto out;
	}
	pnfs_get_layout_hdr(lo);
	rv = pnfs_check_callback_stateid(lo, &args->cbl_stateid, cps);
	if (rv != NFS_OK)
		goto unlock;

	/*
	 * Enforce RFC5661 Section 12.5.5.2.1.5 (Bulk Recall and Return)
	 */
	if (test_bit(NFS_LAYOUT_BULK_RECALL, &lo->plh_flags)) {
		rv = NFS4ERR_DELAY;
		goto unlock;
	}

	pnfs_set_layout_stateid(lo, &args->cbl_stateid, NULL, true);
	switch (pnfs_mark_matching_lsegs_return(lo, &free_me_list,
				&args->cbl_range,
				be32_to_cpu(args->cbl_stateid.seqid))) {
	case 0:
	case -EBUSY:
		/* There are layout segments that need to be returned */
		rv = NFS4_OK;
		break;
	case -ENOENT:
		set_bit(NFS_LAYOUT_DRAIN, &lo->plh_flags);
		/* Embrace your forgetfulness! */
		rv = NFS4ERR_NOMATCHING_LAYOUT;

		if (NFS_SERVER(ino)->pnfs_curr_ld->return_range) {
			NFS_SERVER(ino)->pnfs_curr_ld->return_range(lo,
				&args->cbl_range);
		}
	}
unlock:
	spin_unlock(&ino->i_lock);
	pnfs_free_lseg_list(&free_me_list);
	/* Free all lsegs that are attached to commit buckets */
	nfs_commit_inode(ino, 0);
	pnfs_put_layout_hdr(lo);
out:
	nfs_iput_and_deactive(ino);
out_noput:
	trace_nfs4_cb_layoutrecall_file(clp, &args->cbl_fh, ino,
			&args->cbl_stateid, -rv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rv;
}

static u32 initiate_bulk_draining(struct nfs_client *clp,
				  struct cb_layoutrecallargs *args)
{
<<<<<<< HEAD
	struct nfs_server *server;
	struct pnfs_layout_hdr *lo;
	struct inode *ino;
	u32 rv = NFS4ERR_NOMATCHING_LAYOUT;
	struct pnfs_layout_hdr *tmp;
	LIST_HEAD(recall_list);
	LIST_HEAD(free_me_list);
	struct pnfs_layout_range range = {
		.iomode = IOMODE_ANY,
		.offset = 0,
		.length = NFS4_MAX_UINT64,
	};

	spin_lock(&clp->cl_lock);
	rcu_read_lock();
	list_for_each_entry_rcu(server, &clp->cl_superblocks, client_link) {
		if ((args->cbl_recall_type == RETURN_FSID) &&
		    memcmp(&server->fsid, &args->cbl_fsid,
			   sizeof(struct nfs_fsid)))
			continue;

		list_for_each_entry(lo, &server->layouts, plh_layouts) {
			if (!igrab(lo->plh_inode))
				continue;
			get_layout_hdr(lo);
			BUG_ON(!list_empty(&lo->plh_bulk_recall));
			list_add(&lo->plh_bulk_recall, &recall_list);
		}
	}
	rcu_read_unlock();
	spin_unlock(&clp->cl_lock);

	list_for_each_entry_safe(lo, tmp,
				 &recall_list, plh_bulk_recall) {
		ino = lo->plh_inode;
		spin_lock(&ino->i_lock);
		set_bit(NFS_LAYOUT_BULK_RECALL, &lo->plh_flags);
		if (mark_matching_lsegs_invalid(lo, &free_me_list, &range))
			rv = NFS4ERR_DELAY;
		list_del_init(&lo->plh_bulk_recall);
		spin_unlock(&ino->i_lock);
		pnfs_free_lseg_list(&free_me_list);
		put_layout_hdr(lo);
		iput(ino);
	}
	return rv;
}

static u32 do_callback_layoutrecall(struct nfs_client *clp,
				    struct cb_layoutrecallargs *args)
{
	u32 res;

	dprintk("%s enter, type=%i\n", __func__, args->cbl_recall_type);
	if (args->cbl_recall_type == RETURN_FILE)
		res = initiate_file_draining(clp, args);
	else
		res = initiate_bulk_draining(clp, args);
	dprintk("%s returning %i\n", __func__, res);
	return res;

}

__be32 nfs4_callback_layoutrecall(struct cb_layoutrecallargs *args,
				  void *dummy, struct cb_process_state *cps)
{
	u32 res;

	dprintk("%s: -->\n", __func__);

	if (cps->clp)
		res = do_callback_layoutrecall(cps->clp, args);
	else
		res = NFS4ERR_OP_NOT_IN_SESSION;

	dprintk("%s: exit with status = %d\n", __func__, res);
	return cpu_to_be32(res);
}

static void pnfs_recall_all_layouts(struct nfs_client *clp)
=======
	int stat;

	if (args->cbl_recall_type == RETURN_FSID)
		stat = pnfs_destroy_layouts_byfsid(clp, &args->cbl_fsid, true);
	else
		stat = pnfs_destroy_layouts_byclid(clp, true);
	if (stat != 0)
		return NFS4ERR_DELAY;
	return NFS4ERR_NOMATCHING_LAYOUT;
}

static u32 do_callback_layoutrecall(struct nfs_client *clp,
				    struct cb_layoutrecallargs *args,
				    struct cb_process_state *cps)
{
	if (args->cbl_recall_type == RETURN_FILE)
		return initiate_file_draining(clp, args, cps);
	return initiate_bulk_draining(clp, args);
}

__be32 nfs4_callback_layoutrecall(void *argp, void *resp,
				  struct cb_process_state *cps)
{
	struct cb_layoutrecallargs *args = argp;
	u32 res = NFS4ERR_OP_NOT_IN_SESSION;

	if (cps->clp)
		res = do_callback_layoutrecall(cps->clp, args, cps);
	return cpu_to_be32(res);
}

static void pnfs_recall_all_layouts(struct nfs_client *clp,
				    struct cb_process_state *cps)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cb_layoutrecallargs args;

	/* Pretend we got a CB_LAYOUTRECALL(ALL) */
	memset(&args, 0, sizeof(args));
	args.cbl_recall_type = RETURN_ALL;
	/* FIXME we ignore errors, what should we do? */
<<<<<<< HEAD
	do_callback_layoutrecall(clp, &args);
}

__be32 nfs4_callback_devicenotify(struct cb_devicenotifyargs *args,
				  void *dummy, struct cb_process_state *cps)
{
	int i;
	__be32 res = 0;
	struct nfs_client *clp = cps->clp;
	struct nfs_server *server = NULL;

	dprintk("%s: -->\n", __func__);

	if (!clp) {
=======
	do_callback_layoutrecall(clp, &args, cps);
}

__be32 nfs4_callback_devicenotify(void *argp, void *resp,
				  struct cb_process_state *cps)
{
	struct cb_devicenotifyargs *args = argp;
	const struct pnfs_layoutdriver_type *ld = NULL;
	uint32_t i;
	__be32 res = 0;

	if (!cps->clp) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		res = cpu_to_be32(NFS4ERR_OP_NOT_IN_SESSION);
		goto out;
	}

	for (i = 0; i < args->ndevs; i++) {
		struct cb_devicenotifyitem *dev = &args->devs[i];

<<<<<<< HEAD
		if (!server ||
		    server->pnfs_curr_ld->id != dev->cbd_layout_type) {
			rcu_read_lock();
			list_for_each_entry_rcu(server, &clp->cl_superblocks, client_link)
				if (server->pnfs_curr_ld &&
				    server->pnfs_curr_ld->id == dev->cbd_layout_type) {
					rcu_read_unlock();
					goto found;
				}
			rcu_read_unlock();
			dprintk("%s: layout type %u not found\n",
				__func__, dev->cbd_layout_type);
			continue;
		}

	found:
		if (dev->cbd_notify_type == NOTIFY_DEVICEID4_CHANGE)
			dprintk("%s: NOTIFY_DEVICEID4_CHANGE not supported, "
				"deleting instead\n", __func__);
		nfs4_delete_deviceid(server->pnfs_curr_ld, clp, &dev->cbd_dev_id);
	}

out:
	kfree(args->devs);
	dprintk("%s: exit with status = %u\n",
		__func__, be32_to_cpu(res));
=======
		if (!ld || ld->id != dev->cbd_layout_type) {
			pnfs_put_layoutdriver(ld);
			ld = pnfs_find_layoutdriver(dev->cbd_layout_type);
			if (!ld)
				continue;
		}
		nfs4_delete_deviceid(ld, cps->clp, &dev->cbd_dev_id);
	}
	pnfs_put_layoutdriver(ld);
out:
	kfree(args->devs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return res;
}

/*
 * Validate the sequenceID sent by the server.
 * Return success if the sequenceID is one more than what we last saw on
 * this slot, accounting for wraparound.  Increments the slot's sequence.
 *
 * We don't yet implement a duplicate request cache, instead we set the
 * back channel ca_maxresponsesize_cached to zero. This is OK for now
 * since we only currently implement idempotent callbacks anyway.
 *
 * We have a single slot backchannel at this time, so we don't bother
 * checking the used_slots bit array on the table.  The lower layer guarantees
 * a single outstanding callback request at a time.
 */
static __be32
<<<<<<< HEAD
validate_seqid(struct nfs4_slot_table *tbl, struct cb_sequenceargs * args)
{
	struct nfs4_slot *slot;

	dprintk("%s enter. slotid %d seqid %d\n",
		__func__, args->csa_slotid, args->csa_sequenceid);

	if (args->csa_slotid >= NFS41_BC_MAX_CALLBACKS)
		return htonl(NFS4ERR_BADSLOT);

	slot = tbl->slots + args->csa_slotid;
	dprintk("%s slot table seqid: %d\n", __func__, slot->seq_nr);

	/* Normal */
	if (likely(args->csa_sequenceid == slot->seq_nr + 1)) {
		slot->seq_nr++;
		goto out_ok;
	}

	/* Replay */
	if (args->csa_sequenceid == slot->seq_nr) {
		dprintk("%s seqid %d is a replay\n",
			__func__, args->csa_sequenceid);
		/* Signal process_op to set this error on next op */
		if (args->csa_cachethis == 0)
			return htonl(NFS4ERR_RETRY_UNCACHED_REP);

		/* The ca_maxresponsesize_cached is 0 with no DRC */
		else if (args->csa_cachethis == 1)
			return htonl(NFS4ERR_REP_TOO_BIG_TO_CACHE);
	}

	/* Wraparound */
	if (args->csa_sequenceid == 1 && (slot->seq_nr + 1) == 0) {
		slot->seq_nr = 1;
		goto out_ok;
	}

	/* Misordered request */
	return htonl(NFS4ERR_SEQ_MISORDERED);
out_ok:
	tbl->highest_used_slotid = args->csa_slotid;
	return htonl(NFS4_OK);
=======
validate_seqid(const struct nfs4_slot_table *tbl, const struct nfs4_slot *slot,
		const struct cb_sequenceargs * args)
{
	__be32 ret;

	ret = cpu_to_be32(NFS4ERR_BADSLOT);
	if (args->csa_slotid > tbl->server_highest_slotid)
		goto out_err;

	/* Replay */
	if (args->csa_sequenceid == slot->seq_nr) {
		ret = cpu_to_be32(NFS4ERR_DELAY);
		if (nfs4_test_locked_slot(tbl, slot->slot_nr))
			goto out_err;

		/* Signal process_op to set this error on next op */
		ret = cpu_to_be32(NFS4ERR_RETRY_UNCACHED_REP);
		if (args->csa_cachethis == 0)
			goto out_err;

		/* Liar! We never allowed you to set csa_cachethis != 0 */
		ret = cpu_to_be32(NFS4ERR_SEQ_FALSE_RETRY);
		goto out_err;
	}

	/* Note: wraparound relies on seq_nr being of type u32 */
	/* Misordered request */
	ret = cpu_to_be32(NFS4ERR_SEQ_MISORDERED);
	if (args->csa_sequenceid != slot->seq_nr + 1)
		goto out_err;

	return cpu_to_be32(NFS4_OK);

out_err:
	trace_nfs4_cb_seqid_err(args, ret);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * For each referring call triple, check the session's slot table for
 * a match.  If the slot is in use and the sequence numbers match, the
 * client is still waiting for a response to the original request.
 */
<<<<<<< HEAD
static bool referring_call_exists(struct nfs_client *clp,
				  uint32_t nrclists,
				  struct referring_call_list *rclists)
{
	bool status = 0;
=======
static int referring_call_exists(struct nfs_client *clp,
				  uint32_t nrclists,
				  struct referring_call_list *rclists,
				  spinlock_t *lock)
	__releases(lock)
	__acquires(lock)
{
	int status = 0;
	int found = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i, j;
	struct nfs4_session *session;
	struct nfs4_slot_table *tbl;
	struct referring_call_list *rclist;
	struct referring_call *ref;

	/*
	 * XXX When client trunking is implemented, this becomes
	 * a session lookup from within the loop
	 */
	session = clp->cl_session;
	tbl = &session->fc_slot_table;

	for (i = 0; i < nrclists; i++) {
		rclist = &rclists[i];
		if (memcmp(session->sess_id.data,
			   rclist->rcl_sessionid.data,
			   NFS4_MAX_SESSIONID_LEN) != 0)
			continue;

		for (j = 0; j < rclist->rcl_nrefcalls; j++) {
			ref = &rclist->rcl_refcalls[j];
<<<<<<< HEAD

			dprintk("%s: sessionid %x:%x:%x:%x sequenceid %u "
				"slotid %u\n", __func__,
				((u32 *)&rclist->rcl_sessionid.data)[0],
				((u32 *)&rclist->rcl_sessionid.data)[1],
				((u32 *)&rclist->rcl_sessionid.data)[2],
				((u32 *)&rclist->rcl_sessionid.data)[3],
				ref->rc_sequenceid, ref->rc_slotid);

			spin_lock(&tbl->slot_tbl_lock);
			status = (test_bit(ref->rc_slotid, tbl->used_slots) &&
				  tbl->slots[ref->rc_slotid].seq_nr ==
					ref->rc_sequenceid);
			spin_unlock(&tbl->slot_tbl_lock);
			if (status)
				goto out;
=======
			spin_unlock(lock);
			status = nfs4_slot_wait_on_seqid(tbl, ref->rc_slotid,
					ref->rc_sequenceid, HZ >> 1) < 0;
			spin_lock(lock);
			if (status)
				goto out;
			found++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

out:
<<<<<<< HEAD
	return status;
}

__be32 nfs4_callback_sequence(struct cb_sequenceargs *args,
			      struct cb_sequenceres *res,
			      struct cb_process_state *cps)
{
	struct nfs4_slot_table *tbl;
	struct nfs_client *clp;
	int i;
	__be32 status = htonl(NFS4ERR_BADSESSION);

	clp = nfs4_find_client_sessionid(cps->net, args->csa_addr, &args->csa_sessionid);
	if (clp == NULL)
		goto out;

	tbl = &clp->cl_session->bc_slot_table;

	spin_lock(&tbl->slot_tbl_lock);
	/* state manager is resetting the session */
	if (test_bit(NFS4_SESSION_DRAINING, &clp->cl_session->session_state)) {
		spin_unlock(&tbl->slot_tbl_lock);
=======
	return status < 0 ? status : found;
}

__be32 nfs4_callback_sequence(void *argp, void *resp,
			      struct cb_process_state *cps)
{
	struct cb_sequenceargs *args = argp;
	struct cb_sequenceres *res = resp;
	struct nfs4_slot_table *tbl;
	struct nfs4_slot *slot;
	struct nfs_client *clp;
	int ret;
	int i;
	__be32 status = htonl(NFS4ERR_BADSESSION);

	clp = nfs4_find_client_sessionid(cps->net, args->csa_addr,
					 &args->csa_sessionid, cps->minorversion);
	if (clp == NULL)
		goto out;

	if (!(clp->cl_session->flags & SESSION4_BACK_CHAN))
		goto out;

	tbl = &clp->cl_session->bc_slot_table;

	/* Set up res before grabbing the spinlock */
	memcpy(&res->csr_sessionid, &args->csa_sessionid,
	       sizeof(res->csr_sessionid));
	res->csr_sequenceid = args->csa_sequenceid;
	res->csr_slotid = args->csa_slotid;

	spin_lock(&tbl->slot_tbl_lock);
	/* state manager is resetting the session */
	if (test_bit(NFS4_SLOT_TBL_DRAINING, &tbl->slot_tbl_state)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		status = htonl(NFS4ERR_DELAY);
		/* Return NFS4ERR_BADSESSION if we're draining the session
		 * in order to reset it.
		 */
		if (test_bit(NFS4CLNT_SESSION_RESET, &clp->cl_state))
			status = htonl(NFS4ERR_BADSESSION);
<<<<<<< HEAD
		goto out;
	}

	status = validate_seqid(&clp->cl_session->bc_slot_table, args);
	spin_unlock(&tbl->slot_tbl_lock);
	if (status)
		goto out;

	cps->slotid = args->csa_slotid;
=======
		goto out_unlock;
	}

	status = htonl(NFS4ERR_BADSLOT);
	slot = nfs4_lookup_slot(tbl, args->csa_slotid);
	if (IS_ERR(slot))
		goto out_unlock;

	res->csr_highestslotid = tbl->server_highest_slotid;
	res->csr_target_highestslotid = tbl->target_highest_slotid;

	status = validate_seqid(tbl, slot, args);
	if (status)
		goto out_unlock;
	if (!nfs4_try_to_lock_slot(tbl, slot)) {
		status = htonl(NFS4ERR_DELAY);
		goto out_unlock;
	}
	cps->slot = slot;

	/* The ca_maxresponsesize_cached is 0 with no DRC */
	if (args->csa_cachethis != 0) {
		status = htonl(NFS4ERR_REP_TOO_BIG_TO_CACHE);
		goto out_unlock;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Check for pending referring calls.  If a match is found, a
	 * related callback was received before the response to the original
	 * call.
	 */
<<<<<<< HEAD
	if (referring_call_exists(clp, args->csa_nrclists, args->csa_rclists)) {
		status = htonl(NFS4ERR_DELAY);
		goto out;
	}

	memcpy(&res->csr_sessionid, &args->csa_sessionid,
	       sizeof(res->csr_sessionid));
	res->csr_sequenceid = args->csa_sequenceid;
	res->csr_slotid = args->csa_slotid;
	res->csr_highestslotid = NFS41_BC_MAX_CALLBACKS - 1;
	res->csr_target_highestslotid = NFS41_BC_MAX_CALLBACKS - 1;
=======
	ret = referring_call_exists(clp, args->csa_nrclists, args->csa_rclists,
				    &tbl->slot_tbl_lock);
	if (ret < 0) {
		status = htonl(NFS4ERR_DELAY);
		goto out_unlock;
	}
	cps->referring_calls = ret;

	/*
	 * RFC5661 20.9.3
	 * If CB_SEQUENCE returns an error, then the state of the slot
	 * (sequence ID, cached reply) MUST NOT change.
	 */
	slot->seq_nr = args->csa_sequenceid;
out_unlock:
	spin_unlock(&tbl->slot_tbl_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	cps->clp = clp; /* put in nfs4_callback_compound */
	for (i = 0; i < args->csa_nrclists; i++)
		kfree(args->csa_rclists[i].rcl_refcalls);
	kfree(args->csa_rclists);

	if (status == htonl(NFS4ERR_RETRY_UNCACHED_REP)) {
		cps->drc_status = status;
		status = 0;
	} else
		res->csr_status = status;

<<<<<<< HEAD
	dprintk("%s: exit with status = %d res->csr_status %d\n", __func__,
		ntohl(status), ntohl(res->csr_status));
=======
	trace_nfs4_cb_sequence(args, res, status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return status;
}

static bool
<<<<<<< HEAD
validate_bitmap_values(unsigned long mask)
=======
validate_bitmap_values(unsigned int mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return (mask & ~RCA4_TYPE_MASK_ALL) == 0;
}

<<<<<<< HEAD
__be32 nfs4_callback_recallany(struct cb_recallanyargs *args, void *dummy,
			       struct cb_process_state *cps)
{
	__be32 status;
	fmode_t flags = 0;
=======
__be32 nfs4_callback_recallany(void *argp, void *resp,
			       struct cb_process_state *cps)
{
	struct cb_recallanyargs *args = argp;
	__be32 status;
	fmode_t flags = 0;
	bool schedule_manager = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	status = cpu_to_be32(NFS4ERR_OP_NOT_IN_SESSION);
	if (!cps->clp) /* set in cb_sequence */
		goto out;

	dprintk_rcu("NFS: RECALL_ANY callback request from %s\n",
		rpc_peeraddr2str(cps->clp->cl_rpcclient, RPC_DISPLAY_ADDR));

	status = cpu_to_be32(NFS4ERR_INVAL);
	if (!validate_bitmap_values(args->craa_type_mask))
		goto out;

	status = cpu_to_be32(NFS4_OK);
<<<<<<< HEAD
	if (test_bit(RCA4_TYPE_MASK_RDATA_DLG, (const unsigned long *)
		     &args->craa_type_mask))
		flags = FMODE_READ;
	if (test_bit(RCA4_TYPE_MASK_WDATA_DLG, (const unsigned long *)
		     &args->craa_type_mask))
		flags |= FMODE_WRITE;
	if (test_bit(RCA4_TYPE_MASK_FILE_LAYOUT, (const unsigned long *)
		     &args->craa_type_mask))
		pnfs_recall_all_layouts(cps->clp);
	if (flags)
		nfs_expire_all_delegation_types(cps->clp, flags);
=======
	if (args->craa_type_mask & BIT(RCA4_TYPE_MASK_RDATA_DLG))
		flags = FMODE_READ;
	if (args->craa_type_mask & BIT(RCA4_TYPE_MASK_WDATA_DLG))
		flags |= FMODE_WRITE;
	if (flags)
		nfs_expire_unused_delegation_types(cps->clp, flags);

	if (args->craa_type_mask & BIT(RCA4_TYPE_MASK_FILE_LAYOUT))
		pnfs_recall_all_layouts(cps->clp, cps);

	if (args->craa_type_mask & BIT(PNFS_FF_RCA4_TYPE_MASK_READ)) {
		set_bit(NFS4CLNT_RECALL_ANY_LAYOUT_READ, &cps->clp->cl_state);
		schedule_manager = true;
	}
	if (args->craa_type_mask & BIT(PNFS_FF_RCA4_TYPE_MASK_RW)) {
		set_bit(NFS4CLNT_RECALL_ANY_LAYOUT_RW, &cps->clp->cl_state);
		schedule_manager = true;
	}
	if (schedule_manager)
		nfs4_schedule_state_manager(cps->clp);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	dprintk("%s: exit with status = %d\n", __func__, ntohl(status));
	return status;
}

/* Reduce the fore channel's max_slots to the target value */
<<<<<<< HEAD
__be32 nfs4_callback_recallslot(struct cb_recallslotargs *args, void *dummy,
				struct cb_process_state *cps)
{
=======
__be32 nfs4_callback_recallslot(void *argp, void *resp,
				struct cb_process_state *cps)
{
	struct cb_recallslotargs *args = argp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nfs4_slot_table *fc_tbl;
	__be32 status;

	status = htonl(NFS4ERR_OP_NOT_IN_SESSION);
	if (!cps->clp) /* set in cb_sequence */
		goto out;

<<<<<<< HEAD
	dprintk_rcu("NFS: CB_RECALL_SLOT request from %s target max slots %d\n",
		rpc_peeraddr2str(cps->clp->cl_rpcclient, RPC_DISPLAY_ADDR),
		args->crsa_target_max_slots);

	fc_tbl = &cps->clp->cl_session->fc_slot_table;

	status = htonl(NFS4ERR_BAD_HIGH_SLOT);
	if (args->crsa_target_max_slots > fc_tbl->max_slots ||
	    args->crsa_target_max_slots < 1)
		goto out;

	status = htonl(NFS4_OK);
	if (args->crsa_target_max_slots == fc_tbl->max_slots)
		goto out;

	fc_tbl->target_max_slots = args->crsa_target_max_slots;
	nfs41_handle_recall_slot(cps->clp);
=======
	dprintk_rcu("NFS: CB_RECALL_SLOT request from %s target highest slotid %u\n",
		rpc_peeraddr2str(cps->clp->cl_rpcclient, RPC_DISPLAY_ADDR),
		args->crsa_target_highest_slotid);

	fc_tbl = &cps->clp->cl_session->fc_slot_table;

	status = htonl(NFS4_OK);

	nfs41_set_target_slotid(fc_tbl, args->crsa_target_highest_slotid);
	nfs41_notify_server(cps->clp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	dprintk("%s: exit with status = %d\n", __func__, ntohl(status));
	return status;
}
<<<<<<< HEAD
#endif /* CONFIG_NFS_V4_1 */
=======

__be32 nfs4_callback_notify_lock(void *argp, void *resp,
				 struct cb_process_state *cps)
{
	struct cb_notify_lock_args *args = argp;

	if (!cps->clp) /* set in cb_sequence */
		return htonl(NFS4ERR_OP_NOT_IN_SESSION);

	dprintk_rcu("NFS: CB_NOTIFY_LOCK request from %s\n",
		rpc_peeraddr2str(cps->clp->cl_rpcclient, RPC_DISPLAY_ADDR));

	/* Don't wake anybody if the string looked bogus */
	if (args->cbnl_valid)
		__wake_up(&cps->clp->cl_lock_waitq, TASK_NORMAL, 0, args);

	return htonl(NFS4_OK);
}
#endif /* CONFIG_NFS_V4_1 */
#ifdef CONFIG_NFS_V4_2
static void nfs4_copy_cb_args(struct nfs4_copy_state *cp_state,
				struct cb_offloadargs *args)
{
	cp_state->count = args->wr_count;
	cp_state->error = args->error;
	if (!args->error) {
		cp_state->verf.committed = args->wr_writeverf.committed;
		memcpy(&cp_state->verf.verifier.data[0],
			&args->wr_writeverf.verifier.data[0],
			NFS4_VERIFIER_SIZE);
	}
}

__be32 nfs4_callback_offload(void *data, void *dummy,
			     struct cb_process_state *cps)
{
	struct cb_offloadargs *args = data;
	struct nfs_server *server;
	struct nfs4_copy_state *copy, *tmp_copy;
	bool found = false;

	copy = kzalloc(sizeof(struct nfs4_copy_state), GFP_KERNEL);
	if (!copy)
		return htonl(NFS4ERR_SERVERFAULT);

	spin_lock(&cps->clp->cl_lock);
	rcu_read_lock();
	list_for_each_entry_rcu(server, &cps->clp->cl_superblocks,
				client_link) {
		list_for_each_entry(tmp_copy, &server->ss_copies, copies) {
			if (memcmp(args->coa_stateid.other,
					tmp_copy->stateid.other,
					sizeof(args->coa_stateid.other)))
				continue;
			nfs4_copy_cb_args(tmp_copy, args);
			complete(&tmp_copy->completion);
			found = true;
			goto out;
		}
	}
out:
	rcu_read_unlock();
	if (!found) {
		memcpy(&copy->stateid, &args->coa_stateid, NFS4_STATEID_SIZE);
		nfs4_copy_cb_args(copy, args);
		list_add_tail(&copy->copies, &cps->clp->pending_cb_stateids);
	} else
		kfree(copy);
	spin_unlock(&cps->clp->cl_lock);

	trace_nfs4_cb_offload(&args->coa_fh, &args->coa_stateid,
			args->wr_count, args->error,
			args->wr_writeverf.committed);
	return 0;
}
#endif /* CONFIG_NFS_V4_2 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
