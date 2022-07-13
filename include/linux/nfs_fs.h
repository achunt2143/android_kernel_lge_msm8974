<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/include/linux/nfs_fs.h
 *
 *  Copyright (C) 1992  Rick Sladkey
 *
 *  OS-specific nfs filesystem definitions and declarations
 */
<<<<<<< HEAD

#ifndef _LINUX_NFS_FS_H
#define _LINUX_NFS_FS_H

#include <linux/magic.h>

/* Default timeout values */
#define NFS_DEF_UDP_TIMEO	(11)
#define NFS_DEF_UDP_RETRANS	(3)
#define NFS_DEF_TCP_TIMEO	(600)
#define NFS_DEF_TCP_RETRANS	(2)

#define NFS_MAX_UDP_TIMEOUT	(60*HZ)
#define NFS_MAX_TCP_TIMEOUT	(600*HZ)

#define NFS_DEF_ACREGMIN	(3)
#define NFS_DEF_ACREGMAX	(60)
#define NFS_DEF_ACDIRMIN	(30)
#define NFS_DEF_ACDIRMAX	(60)

/*
 * When flushing a cluster of dirty pages, there can be different
 * strategies:
 */
#define FLUSH_SYNC		1	/* file being synced, or contention */
#define FLUSH_STABLE		4	/* commit to stable storage */
#define FLUSH_LOWPRI		8	/* low priority background flush */
#define FLUSH_HIGHPRI		16	/* high priority memory reclaim flush */
#define FLUSH_COND_STABLE	32	/* conditional stable write - only stable
					 * if everything fits in one RPC */

#ifdef __KERNEL__
=======
#ifndef _LINUX_NFS_FS_H
#define _LINUX_NFS_FS_H

#include <uapi/linux/nfs_fs.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Enable dprintk() debugging support for nfs client.
 */
#ifdef CONFIG_NFS_DEBUG
# define NFS_DEBUG
#endif

#include <linux/in.h>
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/rbtree.h>
<<<<<<< HEAD
=======
#include <linux/refcount.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/rwsem.h>
#include <linux/wait.h>

#include <linux/sunrpc/debug.h>
#include <linux/sunrpc/auth.h>
#include <linux/sunrpc/clnt.h>

<<<<<<< HEAD
=======
#ifdef CONFIG_NFS_FSCACHE
#include <linux/netfs.h>
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/nfs.h>
#include <linux/nfs2.h>
#include <linux/nfs3.h>
#include <linux/nfs4.h>
#include <linux/nfs_xdr.h>
#include <linux/nfs_fs_sb.h>

#include <linux/mempool.h>

/*
<<<<<<< HEAD
 * These are the default flags for swap requests
 */
#define NFS_RPC_SWAPFLAGS		(RPC_TASK_SWAPPER|RPC_TASK_ROOTCREDS)
=======
 * These are the default for number of transports to different server IPs
 */
#define NFS_MAX_TRANSPORTS 16

/*
 * Size of the NFS directory verifier
 */
#define NFS_DIR_VERIFIER_SIZE		2
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * NFSv3/v4 Access mode cache entry
 */
struct nfs_access_entry {
	struct rb_node		rb_node;
	struct list_head	lru;
<<<<<<< HEAD
	unsigned long		jiffies;
	struct rpc_cred *	cred;
	int			mask;
};

struct nfs_lock_context {
	atomic_t count;
	struct list_head list;
	struct nfs_open_context *open_context;
	fl_owner_t lockowner;
	pid_t pid;
=======
	kuid_t			fsuid;
	kgid_t			fsgid;
	struct group_info	*group_info;
	u64			timestamp;
	__u32			mask;
	struct rcu_head		rcu_head;
};

struct nfs_lock_context {
	refcount_t count;
	struct list_head list;
	struct nfs_open_context *open_context;
	fl_owner_t lockowner;
	atomic_t io_count;
	struct rcu_head	rcu_head;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct nfs4_state;
struct nfs_open_context {
	struct nfs_lock_context lock_context;
<<<<<<< HEAD
	struct dentry *dentry;
	struct rpc_cred *cred;
=======
	fl_owner_t flock_owner;
	struct dentry *dentry;
	const struct cred *cred;
	struct rpc_cred __rcu *ll_cred;	/* low-level cred - use to check for expiry */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nfs4_state *state;
	fmode_t mode;

	unsigned long flags;
<<<<<<< HEAD
#define NFS_CONTEXT_ERROR_WRITE		(0)
	int error;

	struct list_head list;
};

struct nfs_open_dir_context {
	struct rpc_cred *cred;
	unsigned long attr_gencount;
	__u64 dir_cookie;
	__u64 dup_cookie;
	signed char duped;
=======
#define NFS_CONTEXT_BAD			(2)
#define NFS_CONTEXT_UNLOCK	(3)
#define NFS_CONTEXT_FILE_OPEN		(4)
	int error;

	struct list_head list;
	struct nfs4_threshold	*mdsthreshold;
	struct rcu_head	rcu_head;
};

struct nfs_open_dir_context {
	struct list_head list;
	atomic_t cache_hits;
	atomic_t cache_misses;
	unsigned long attr_gencount;
	__be32	verf[NFS_DIR_VERIFIER_SIZE];
	__u64 dir_cookie;
	__u64 last_cookie;
	pgoff_t page_index;
	unsigned int dtsize;
	bool force_clear;
	bool eof;
	struct rcu_head rcu_head;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * NFSv4 delegation
 */
struct nfs_delegation;

struct posix_acl;

<<<<<<< HEAD
=======
struct nfs4_xattr_cache;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * nfs fs inode data in memory
 */
struct nfs_inode {
	/*
	 * The 64bit 'inode number'
	 */
	__u64 fileid;

	/*
	 * NFS file handle
	 */
	struct nfs_fh		fh;

	/*
	 * Various flags
	 */
	unsigned long		flags;			/* atomic bit ops */
	unsigned long		cache_validity;		/* bit mask */

	/*
	 * read_cache_jiffies is when we started read-caching this inode.
	 * attrtimeo is for how long the cached information is assumed
	 * to be valid. A successful attribute revalidation doubles
	 * attrtimeo (up to acregmax/acdirmax), a failure resets it to
	 * acregmin/acdirmin.
	 *
	 * We need to revalidate the cached attrs for this inode if
	 *
	 *	jiffies - read_cache_jiffies >= attrtimeo
	 *
	 * Please note the comparison is greater than or equal
	 * so that zero timeout values can be specified.
	 */
	unsigned long		read_cache_jiffies;
	unsigned long		attrtimeo;
	unsigned long		attrtimeo_timestamp;

	unsigned long		attr_gencount;
<<<<<<< HEAD
	/* "Generation counter" for the attribute cache. This is
	 * bumped whenever we update the metadata on the
	 * server.
	 */
	unsigned long		cache_change_attribute;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	struct rb_root		access_cache;
	struct list_head	access_cache_entry_lru;
	struct list_head	access_cache_inode_lru;
<<<<<<< HEAD
#ifdef CONFIG_NFS_V3_ACL
	struct posix_acl	*acl_access;
	struct posix_acl	*acl_default;
#endif

	/*
	 * This is the cookie verifier used for NFSv3 readdir
	 * operations
	 */
	__be32			cookieverf[2];

	unsigned long		npages;
	unsigned long		ncommit;
	struct list_head	commit_list;
=======

	union {
		/* Directory */
		struct {
			/* "Generation counter" for the attribute cache.
			 * This is bumped whenever we update the metadata
			 * on the server.
			 */
			unsigned long	cache_change_attribute;
			/*
			 * This is the cookie verifier used for NFSv3 readdir
			 * operations
			 */
			__be32		cookieverf[NFS_DIR_VERIFIER_SIZE];
			/* Readers: in-flight sillydelete RPC calls */
			/* Writers: rmdir */
			struct rw_semaphore	rmdir_sem;
		};
		/* Regular file */
		struct {
			atomic_long_t	nrequests;
			atomic_long_t	redirtied_pages;
			struct nfs_mds_commit_info commit_info;
			struct mutex	commit_mutex;
		};
	};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Open contexts for shared mmap writes */
	struct list_head	open_files;

<<<<<<< HEAD
	/* Number of in-flight sillydelete RPC calls */
	atomic_t		silly_count;
	/* List of deferred sillydelete requests */
	struct hlist_head	silly_list;
	wait_queue_head_t	waitqueue;

#ifdef CONFIG_NFS_V4
=======
	/* Keep track of out-of-order replies.
	 * The ooo array contains start/end pairs of
	 * numbers from the changeid sequence when
	 * the inode's iversion has been updated.
	 * It also contains end/start pair (i.e. reverse order)
	 * of sections of the changeid sequence that have
	 * been seen in replies from the server.
	 * Normally these should match and when both
	 * A:B and B:A are found in ooo, they are both removed.
	 * And if a reply with A:B causes an iversion update
	 * of A:B, then neither are added.
	 * When a reply has pre_change that doesn't match
	 * iversion, then the changeid pair and any consequent
	 * change in iversion ARE added.  Later replies
	 * might fill in the gaps, or possibly a gap is caused
	 * by a change from another client.
	 * When a file or directory is opened, if the ooo table
	 * is not empty, then we assume the gaps were due to
	 * another client and we invalidate the cached data.
	 *
	 * We can only track a limited number of concurrent gaps.
	 * Currently that limit is 16.
	 * We allocate the table on demand.  If there is insufficient
	 * memory, then we probably cannot cache the file anyway
	 * so there is no loss.
	 */
	struct {
		int cnt;
		struct {
			u64 start, end;
		} gap[16];
	} *ooo;

#if IS_ENABLED(CONFIG_NFS_V4)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nfs4_cached_acl	*nfs4_acl;
        /* NFSv4 state */
	struct list_head	open_states;
	struct nfs_delegation __rcu *delegation;
<<<<<<< HEAD
	fmode_t			 delegation_state;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rw_semaphore	rwsem;

	/* pNFS layout information */
	struct pnfs_layout_hdr *layout;
<<<<<<< HEAD
	atomic_t		commits_outstanding;
#endif /* CONFIG_NFS_V4*/
#ifdef CONFIG_NFS_FSCACHE
	struct fscache_cookie	*fscache;
#endif
	struct inode		vfs_inode;
};

/*
 * Cache validity bit flags
 */
#define NFS_INO_INVALID_ATTR	0x0001		/* cached attrs are invalid */
#define NFS_INO_INVALID_DATA	0x0002		/* cached data is invalid */
#define NFS_INO_INVALID_ATIME	0x0004		/* cached atime is invalid */
#define NFS_INO_INVALID_ACCESS	0x0008		/* cached access cred invalid */
#define NFS_INO_INVALID_ACL	0x0010		/* cached acls are invalid */
#define NFS_INO_REVAL_PAGECACHE	0x0020		/* must revalidate pagecache */
#define NFS_INO_REVAL_FORCED	0x0040		/* force revalidation ignoring a delegation */
=======
#endif /* CONFIG_NFS_V4*/
	/* how many bytes have been written/read and how many bytes queued up */
	__u64 write_io;
	__u64 read_io;
#ifdef CONFIG_NFS_V4_2
	struct nfs4_xattr_cache *xattr_cache;
#endif
	union {
		struct inode		vfs_inode;
#ifdef CONFIG_NFS_FSCACHE
		struct netfs_inode	netfs; /* netfs context and VFS inode */
#endif
	};
};

struct nfs4_copy_state {
	struct list_head	copies;
	struct list_head	src_copies;
	nfs4_stateid		stateid;
	struct completion	completion;
	uint64_t		count;
	struct nfs_writeverf	verf;
	int			error;
	int			flags;
	struct nfs4_state	*parent_src_state;
	struct nfs4_state	*parent_dst_state;
};

/*
 * Access bit flags
 */
#define NFS_ACCESS_READ        0x0001
#define NFS_ACCESS_LOOKUP      0x0002
#define NFS_ACCESS_MODIFY      0x0004
#define NFS_ACCESS_EXTEND      0x0008
#define NFS_ACCESS_DELETE      0x0010
#define NFS_ACCESS_EXECUTE     0x0020
#define NFS_ACCESS_XAREAD      0x0040
#define NFS_ACCESS_XAWRITE     0x0080
#define NFS_ACCESS_XALIST      0x0100

/*
 * Cache validity bit flags
 */
#define NFS_INO_INVALID_DATA	BIT(1)		/* cached data is invalid */
#define NFS_INO_INVALID_ATIME	BIT(2)		/* cached atime is invalid */
#define NFS_INO_INVALID_ACCESS	BIT(3)		/* cached access cred invalid */
#define NFS_INO_INVALID_ACL	BIT(4)		/* cached acls are invalid */
#define NFS_INO_REVAL_FORCED	BIT(6)		/* force revalidation ignoring a delegation */
#define NFS_INO_INVALID_LABEL	BIT(7)		/* cached label is invalid */
#define NFS_INO_INVALID_CHANGE	BIT(8)		/* cached change is invalid */
#define NFS_INO_INVALID_CTIME	BIT(9)		/* cached ctime is invalid */
#define NFS_INO_INVALID_MTIME	BIT(10)		/* cached mtime is invalid */
#define NFS_INO_INVALID_SIZE	BIT(11)		/* cached size is invalid */
#define NFS_INO_INVALID_OTHER	BIT(12)		/* other attrs are invalid */
#define NFS_INO_DATA_INVAL_DEFER	\
				BIT(13)		/* Deferred cache invalidation */
#define NFS_INO_INVALID_BLOCKS	BIT(14)         /* cached blocks are invalid */
#define NFS_INO_INVALID_XATTR	BIT(15)		/* xattrs are invalid */
#define NFS_INO_INVALID_NLINK	BIT(16)		/* cached nlinks is invalid */
#define NFS_INO_INVALID_MODE	BIT(17)		/* cached mode is invalid */

#define NFS_INO_INVALID_ATTR	(NFS_INO_INVALID_CHANGE \
		| NFS_INO_INVALID_CTIME \
		| NFS_INO_INVALID_MTIME \
		| NFS_INO_INVALID_SIZE \
		| NFS_INO_INVALID_NLINK \
		| NFS_INO_INVALID_MODE \
		| NFS_INO_INVALID_OTHER)	/* inode metadata is invalid */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Bit offsets in flags field
 */
<<<<<<< HEAD
#define NFS_INO_ADVISE_RDPLUS	(0)		/* advise readdirplus */
#define NFS_INO_STALE		(1)		/* possible stale inode */
#define NFS_INO_ACL_LRU_SET	(2)		/* Inode is on the LRU list */
#define NFS_INO_FLUSHING	(4)		/* inode is flushing out data */
#define NFS_INO_FSCACHE		(5)		/* inode can be cached by FS-Cache */
#define NFS_INO_FSCACHE_LOCK	(6)		/* FS-Cache cookie management lock */
#define NFS_INO_COMMIT		(7)		/* inode is committing unstable writes */
#define NFS_INO_PNFS_COMMIT	(8)		/* use pnfs code for commit */
#define NFS_INO_LAYOUTCOMMIT	(9)		/* layoutcommit required */
#define NFS_INO_LAYOUTCOMMITTING (10)		/* layoutcommit inflight */
=======
#define NFS_INO_STALE		(1)		/* possible stale inode */
#define NFS_INO_ACL_LRU_SET	(2)		/* Inode is on the LRU list */
#define NFS_INO_INVALIDATING	(3)		/* inode is being invalidated */
#define NFS_INO_PRESERVE_UNLINKED (4)		/* preserve file if removed while open */
#define NFS_INO_LAYOUTCOMMIT	(9)		/* layoutcommit required */
#define NFS_INO_LAYOUTCOMMITTING (10)		/* layoutcommit inflight */
#define NFS_INO_LAYOUTSTATS	(11)		/* layoutstats inflight */
#define NFS_INO_ODIRECT		(12)		/* I/O setting is O_DIRECT */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline struct nfs_inode *NFS_I(const struct inode *inode)
{
	return container_of(inode, struct nfs_inode, vfs_inode);
}

static inline struct nfs_server *NFS_SB(const struct super_block *s)
{
	return (struct nfs_server *)(s->s_fs_info);
}

static inline struct nfs_fh *NFS_FH(const struct inode *inode)
{
	return &NFS_I(inode)->fh;
}

static inline struct nfs_server *NFS_SERVER(const struct inode *inode)
{
	return NFS_SB(inode->i_sb);
}

static inline struct rpc_clnt *NFS_CLIENT(const struct inode *inode)
{
	return NFS_SERVER(inode)->client;
}

static inline const struct nfs_rpc_ops *NFS_PROTO(const struct inode *inode)
{
	return NFS_SERVER(inode)->nfs_client->rpc_ops;
}

static inline unsigned NFS_MINATTRTIMEO(const struct inode *inode)
{
	struct nfs_server *nfss = NFS_SERVER(inode);
	return S_ISDIR(inode->i_mode) ? nfss->acdirmin : nfss->acregmin;
}

static inline unsigned NFS_MAXATTRTIMEO(const struct inode *inode)
{
	struct nfs_server *nfss = NFS_SERVER(inode);
	return S_ISDIR(inode->i_mode) ? nfss->acdirmax : nfss->acregmax;
}

static inline int NFS_STALE(const struct inode *inode)
{
	return test_bit(NFS_INO_STALE, &NFS_I(inode)->flags);
}

<<<<<<< HEAD
static inline int NFS_FSCACHE(const struct inode *inode)
{
	return test_bit(NFS_INO_FSCACHE, &NFS_I(inode)->flags);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline __u64 NFS_FILEID(const struct inode *inode)
{
	return NFS_I(inode)->fileid;
}

static inline void set_nfs_fileid(struct inode *inode, __u64 fileid)
{
	NFS_I(inode)->fileid = fileid;
}

static inline void nfs_mark_for_revalidate(struct inode *inode)
{
	struct nfs_inode *nfsi = NFS_I(inode);

	spin_lock(&inode->i_lock);
<<<<<<< HEAD
	nfsi->cache_validity |= NFS_INO_INVALID_ATTR|NFS_INO_INVALID_ACCESS;
	if (S_ISDIR(inode->i_mode))
		nfsi->cache_validity |= NFS_INO_REVAL_PAGECACHE|NFS_INO_INVALID_DATA;
	spin_unlock(&inode->i_lock);
}

static inline int nfs_server_capable(struct inode *inode, int cap)
=======
	nfsi->cache_validity |= NFS_INO_INVALID_ACCESS | NFS_INO_INVALID_ACL |
				NFS_INO_INVALID_CHANGE | NFS_INO_INVALID_CTIME |
				NFS_INO_INVALID_SIZE;
	if (S_ISDIR(inode->i_mode))
		nfsi->cache_validity |= NFS_INO_INVALID_DATA;
	spin_unlock(&inode->i_lock);
}

static inline int nfs_server_capable(const struct inode *inode, int cap)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return NFS_SERVER(inode)->caps & cap;
}

<<<<<<< HEAD
static inline int NFS_USE_READDIRPLUS(struct inode *inode)
{
	return test_bit(NFS_INO_ADVISE_RDPLUS, &NFS_I(inode)->flags);
}

static inline void nfs_set_verifier(struct dentry * dentry, unsigned long verf)
{
	dentry->d_time = verf;
}

/**
 * nfs_save_change_attribute - Returns the inode attribute change cookie
 * @dir - pointer to parent directory inode
 * The "change attribute" is updated every time we finish an operation
 * that will result in a metadata change on the server.
=======
/**
 * nfs_save_change_attribute - Returns the inode attribute change cookie
 * @dir - pointer to parent directory inode
 * The "cache change attribute" is updated when we need to revalidate
 * our dentry cache after a directory was seen to change on the server.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static inline unsigned long nfs_save_change_attribute(struct inode *dir)
{
	return NFS_I(dir)->cache_change_attribute;
}

<<<<<<< HEAD
/**
 * nfs_verify_change_attribute - Detects NFS remote directory changes
 * @dir - pointer to parent directory inode
 * @chattr - previously saved change attribute
 * Return "false" if the verifiers doesn't match the change attribute.
 * This would usually indicate that the directory contents have changed on
 * the server, and that any dentries need revalidating.
 */
static inline int nfs_verify_change_attribute(struct inode *dir, unsigned long chattr)
{
	return chattr == NFS_I(dir)->cache_change_attribute;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/fs/nfs/inode.c
 */
extern int nfs_sync_mapping(struct address_space *mapping);
extern void nfs_zap_mapping(struct inode *inode, struct address_space *mapping);
extern void nfs_zap_caches(struct inode *);
<<<<<<< HEAD
extern void nfs_invalidate_atime(struct inode *);
extern struct inode *nfs_fhget(struct super_block *, struct nfs_fh *,
				struct nfs_fattr *);
extern int nfs_refresh_inode(struct inode *, struct nfs_fattr *);
extern int nfs_post_op_update_inode(struct inode *inode, struct nfs_fattr *fattr);
extern int nfs_post_op_update_inode_force_wcc(struct inode *inode, struct nfs_fattr *fattr);
extern int nfs_getattr(struct vfsmount *, struct dentry *, struct kstat *);
extern int nfs_permission(struct inode *, int);
extern int nfs_open(struct inode *, struct file *);
extern int nfs_release(struct inode *, struct file *);
extern int nfs_attribute_timeout(struct inode *inode);
extern int nfs_revalidate_inode(struct nfs_server *server, struct inode *inode);
extern int __nfs_revalidate_inode(struct nfs_server *, struct inode *);
extern int nfs_revalidate_mapping(struct inode *inode, struct address_space *mapping);
extern int nfs_setattr(struct dentry *, struct iattr *);
extern void nfs_setattr_update_inode(struct inode *inode, struct iattr *attr);
extern struct nfs_open_context *get_nfs_open_context(struct nfs_open_context *ctx);
extern void put_nfs_open_context(struct nfs_open_context *ctx);
extern struct nfs_open_context *nfs_find_open_context(struct inode *inode, struct rpc_cred *cred, fmode_t mode);
extern struct nfs_open_context *alloc_nfs_open_context(struct dentry *dentry, fmode_t f_mode);
extern void nfs_file_set_open_context(struct file *filp, struct nfs_open_context *ctx);
=======
extern void nfs_set_inode_stale(struct inode *inode);
extern void nfs_invalidate_atime(struct inode *);
extern struct inode *nfs_fhget(struct super_block *, struct nfs_fh *,
				struct nfs_fattr *);
struct inode *nfs_ilookup(struct super_block *sb, struct nfs_fattr *, struct nfs_fh *);
extern int nfs_refresh_inode(struct inode *, struct nfs_fattr *);
extern int nfs_post_op_update_inode(struct inode *inode, struct nfs_fattr *fattr);
extern int nfs_post_op_update_inode_force_wcc(struct inode *inode, struct nfs_fattr *fattr);
extern int nfs_post_op_update_inode_force_wcc_locked(struct inode *inode, struct nfs_fattr *fattr);
extern int nfs_getattr(struct mnt_idmap *, const struct path *,
		       struct kstat *, u32, unsigned int);
extern void nfs_access_add_cache(struct inode *, struct nfs_access_entry *, const struct cred *);
extern void nfs_access_set_mask(struct nfs_access_entry *, u32);
extern int nfs_permission(struct mnt_idmap *, struct inode *, int);
extern int nfs_open(struct inode *, struct file *);
extern int nfs_attribute_cache_expired(struct inode *inode);
extern int nfs_revalidate_inode(struct inode *inode, unsigned long flags);
extern int __nfs_revalidate_inode(struct nfs_server *, struct inode *);
extern int nfs_clear_invalid_mapping(struct address_space *mapping);
extern bool nfs_mapping_need_revalidate_inode(struct inode *inode);
extern int nfs_revalidate_mapping(struct inode *inode, struct address_space *mapping);
extern int nfs_revalidate_mapping_rcu(struct inode *inode);
extern int nfs_setattr(struct mnt_idmap *, struct dentry *, struct iattr *);
extern void nfs_setattr_update_inode(struct inode *inode, struct iattr *attr, struct nfs_fattr *);
extern void nfs_setsecurity(struct inode *inode, struct nfs_fattr *fattr);
extern struct nfs_open_context *get_nfs_open_context(struct nfs_open_context *ctx);
extern void put_nfs_open_context(struct nfs_open_context *ctx);
extern struct nfs_open_context *nfs_find_open_context(struct inode *inode, const struct cred *cred, fmode_t mode);
extern struct nfs_open_context *alloc_nfs_open_context(struct dentry *dentry, fmode_t f_mode, struct file *filp);
extern void nfs_inode_attach_open_context(struct nfs_open_context *ctx);
extern void nfs_file_set_open_context(struct file *filp, struct nfs_open_context *ctx);
extern void nfs_file_clear_open_context(struct file *flip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern struct nfs_lock_context *nfs_get_lock_context(struct nfs_open_context *ctx);
extern void nfs_put_lock_context(struct nfs_lock_context *l_ctx);
extern u64 nfs_compat_user_ino64(u64 fileid);
extern void nfs_fattr_init(struct nfs_fattr *fattr);
<<<<<<< HEAD
extern unsigned long nfs_inc_attr_generation_counter(void);

extern struct nfs_fattr *nfs_alloc_fattr(void);

static inline void nfs_free_fattr(const struct nfs_fattr *fattr)
{
=======
extern void nfs_fattr_set_barrier(struct nfs_fattr *fattr);
extern unsigned long nfs_inc_attr_generation_counter(void);

extern struct nfs_fattr *nfs_alloc_fattr(void);
extern struct nfs_fattr *nfs_alloc_fattr_with_label(struct nfs_server *server);

static inline void nfs4_label_free(struct nfs4_label *label)
{
#ifdef CONFIG_NFS_V4_SECURITY_LABEL
	if (label) {
		kfree(label->label);
		kfree(label);
	}
#endif
}

static inline void nfs_free_fattr(const struct nfs_fattr *fattr)
{
	if (fattr)
		nfs4_label_free(fattr->label);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(fattr);
}

extern struct nfs_fh *nfs_alloc_fhandle(void);

static inline void nfs_free_fhandle(const struct nfs_fh *fh)
{
	kfree(fh);
}

#ifdef NFS_DEBUG
extern u32 _nfs_display_fhandle_hash(const struct nfs_fh *fh);
static inline u32 nfs_display_fhandle_hash(const struct nfs_fh *fh)
{
	return _nfs_display_fhandle_hash(fh);
}
extern void _nfs_display_fhandle(const struct nfs_fh *fh, const char *caption);
#define nfs_display_fhandle(fh, caption)			\
	do {							\
		if (unlikely(nfs_debug & NFSDBG_FACILITY))	\
			_nfs_display_fhandle(fh, caption);	\
	} while (0)
#else
static inline u32 nfs_display_fhandle_hash(const struct nfs_fh *fh)
{
	return 0;
}
static inline void nfs_display_fhandle(const struct nfs_fh *fh,
				       const char *caption)
{
}
#endif

/*
 * linux/fs/nfs/nfsroot.c
 */
extern int  nfs_root_data(char **root_device, char **root_data); /*__init*/
/* linux/net/ipv4/ipconfig.c: trims ip addr off front of name, too. */
extern __be32 root_nfs_parse_addr(char *name); /*__init*/

/*
 * linux/fs/nfs/file.c
 */
<<<<<<< HEAD
extern const struct inode_operations nfs_file_inode_operations;
#ifdef CONFIG_NFS_V3
extern const struct inode_operations nfs3_file_inode_operations;
#endif /* CONFIG_NFS_V3 */
extern const struct file_operations nfs_file_operations;
#ifdef CONFIG_NFS_V4
=======
extern const struct file_operations nfs_file_operations;
#if IS_ENABLED(CONFIG_NFS_V4)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern const struct file_operations nfs4_file_operations;
#endif /* CONFIG_NFS_V4 */
extern const struct address_space_operations nfs_file_aops;
extern const struct address_space_operations nfs_dir_aops;

static inline struct nfs_open_context *nfs_file_open_context(struct file *filp)
{
	return filp->private_data;
}

<<<<<<< HEAD
static inline struct rpc_cred *nfs_file_cred(struct file *file)
=======
static inline const struct cred *nfs_file_cred(struct file *file)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (file != NULL) {
		struct nfs_open_context *ctx =
			nfs_file_open_context(file);
		if (ctx)
			return ctx->cred;
	}
	return NULL;
}

/*
<<<<<<< HEAD
 * linux/fs/nfs/xattr.c
 */
#ifdef CONFIG_NFS_V3_ACL
extern ssize_t nfs3_listxattr(struct dentry *, char *, size_t);
extern ssize_t nfs3_getxattr(struct dentry *, const char *, void *, size_t);
extern int nfs3_setxattr(struct dentry *, const char *,
			const void *, size_t, int);
extern int nfs3_removexattr (struct dentry *, const char *name);
#else
# define nfs3_listxattr NULL
# define nfs3_getxattr NULL
# define nfs3_setxattr NULL
# define nfs3_removexattr NULL
#endif

/*
 * linux/fs/nfs/direct.c
 */
extern ssize_t nfs_direct_IO(int, struct kiocb *, const struct iovec *, loff_t,
			unsigned long);
extern ssize_t nfs_file_direct_read(struct kiocb *iocb,
			const struct iovec *iov, unsigned long nr_segs,
			loff_t pos);
extern ssize_t nfs_file_direct_write(struct kiocb *iocb,
			const struct iovec *iov, unsigned long nr_segs,
			loff_t pos);
=======
 * linux/fs/nfs/direct.c
 */
int nfs_swap_rw(struct kiocb *iocb, struct iov_iter *iter);
ssize_t nfs_file_direct_read(struct kiocb *iocb,
			     struct iov_iter *iter, bool swap);
ssize_t nfs_file_direct_write(struct kiocb *iocb,
			      struct iov_iter *iter, bool swap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * linux/fs/nfs/dir.c
 */
<<<<<<< HEAD
extern const struct inode_operations nfs_dir_inode_operations;
#ifdef CONFIG_NFS_V3
extern const struct inode_operations nfs3_dir_inode_operations;
#endif /* CONFIG_NFS_V3 */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern const struct file_operations nfs_dir_operations;
extern const struct dentry_operations nfs_dentry_operations;

extern void nfs_force_lookup_revalidate(struct inode *dir);
<<<<<<< HEAD
extern int nfs_instantiate(struct dentry *dentry, struct nfs_fh *fh, struct nfs_fattr *fattr);
extern int nfs_may_open(struct inode *inode, struct rpc_cred *cred, int openflags);
extern void nfs_access_zap_cache(struct inode *inode);
=======
extern void nfs_set_verifier(struct dentry * dentry, unsigned long verf);
#if IS_ENABLED(CONFIG_NFS_V4)
extern void nfs_clear_verifier_delegated(struct inode *inode);
#endif /* IS_ENABLED(CONFIG_NFS_V4) */
extern struct dentry *nfs_add_or_obtain(struct dentry *dentry,
			struct nfs_fh *fh, struct nfs_fattr *fattr);
extern int nfs_instantiate(struct dentry *dentry, struct nfs_fh *fh,
			struct nfs_fattr *fattr);
extern int nfs_may_open(struct inode *inode, const struct cred *cred, int openflags);
extern void nfs_access_zap_cache(struct inode *inode);
extern int nfs_access_get_cached(struct inode *inode, const struct cred *cred,
				 u32 *mask, bool may_block);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * linux/fs/nfs/symlink.c
 */
extern const struct inode_operations nfs_symlink_inode_operations;

/*
 * linux/fs/nfs/sysctl.c
 */
#ifdef CONFIG_SYSCTL
extern int nfs_register_sysctl(void);
extern void nfs_unregister_sysctl(void);
#else
#define nfs_register_sysctl() 0
#define nfs_unregister_sysctl() do { } while(0)
#endif

/*
 * linux/fs/nfs/namespace.c
 */
extern const struct inode_operations nfs_mountpoint_inode_operations;
extern const struct inode_operations nfs_referral_inode_operations;
extern int nfs_mountpoint_expiry_timeout;
extern void nfs_release_automount_timer(void);

/*
 * linux/fs/nfs/unlink.c
 */
extern void nfs_complete_unlink(struct dentry *dentry, struct inode *);
<<<<<<< HEAD
extern void nfs_block_sillyrename(struct dentry *dentry);
extern void nfs_unblock_sillyrename(struct dentry *dentry);
extern int  nfs_sillyrename(struct inode *dir, struct dentry *dentry);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * linux/fs/nfs/write.c
 */
extern int  nfs_congestion_kb;
<<<<<<< HEAD
extern int  nfs_writepage(struct page *page, struct writeback_control *wbc);
extern int  nfs_writepages(struct address_space *, struct writeback_control *);
extern int  nfs_flush_incompatible(struct file *file, struct page *page);
extern int  nfs_updatepage(struct file *, struct page *, unsigned int, unsigned int);
extern void nfs_writeback_done(struct rpc_task *, struct nfs_write_data *);
=======
extern int  nfs_writepages(struct address_space *, struct writeback_control *);
extern int  nfs_flush_incompatible(struct file *file, struct folio *folio);
extern int  nfs_update_folio(struct file *file, struct folio *folio,
			     unsigned int offset, unsigned int count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Try to write back everything synchronously (but check the
 * return value!)
 */
<<<<<<< HEAD
extern int nfs_wb_all(struct inode *inode);
extern int nfs_wb_page(struct inode *inode, struct page* page);
extern int nfs_wb_page_cancel(struct inode *inode, struct page* page);
#if defined(CONFIG_NFS_V3) || defined(CONFIG_NFS_V4)
extern int  nfs_commit_inode(struct inode *, int);
extern struct nfs_write_data *nfs_commitdata_alloc(void);
extern void nfs_commit_free(struct nfs_write_data *wdata);
#else
static inline int
nfs_commit_inode(struct inode *inode, int how)
{
	return 0;
}
#endif

static inline int
nfs_have_writebacks(struct inode *inode)
{
	return NFS_I(inode)->npages != 0;
}

/*
 * Allocate nfs_write_data structures
 */
extern struct nfs_write_data *nfs_writedata_alloc(unsigned int npages);
extern void nfs_writedata_free(struct nfs_write_data *);

/*
 * linux/fs/nfs/read.c
 */
extern int  nfs_readpage(struct file *, struct page *);
extern int  nfs_readpages(struct file *, struct address_space *,
		struct list_head *, unsigned);
extern int  nfs_readpage_result(struct rpc_task *, struct nfs_read_data *);
extern int  nfs_readpage_async(struct nfs_open_context *, struct inode *,
			       struct page *);

/*
 * Allocate nfs_read_data structures
 */
extern struct nfs_read_data *nfs_readdata_alloc(unsigned int npages);
extern void nfs_readdata_free(struct nfs_read_data *);

/*
 * linux/fs/nfs3proc.c
 */
#ifdef CONFIG_NFS_V3_ACL
extern struct posix_acl *nfs3_proc_getacl(struct inode *inode, int type);
extern int nfs3_proc_setacl(struct inode *inode, int type,
			    struct posix_acl *acl);
extern int nfs3_proc_set_default_acl(struct inode *dir, struct inode *inode,
		umode_t mode);
extern void nfs3_forget_cached_acls(struct inode *inode);
#else
static inline int nfs3_proc_set_default_acl(struct inode *dir,
					    struct inode *inode,
					    umode_t mode)
{
	return 0;
}

static inline void nfs3_forget_cached_acls(struct inode *inode)
{
}
#endif /* CONFIG_NFS_V3_ACL */
=======
extern int nfs_sync_inode(struct inode *inode);
extern int nfs_wb_all(struct inode *inode);
extern int nfs_wb_folio(struct inode *inode, struct folio *folio);
int nfs_wb_folio_cancel(struct inode *inode, struct folio *folio);
extern int  nfs_commit_inode(struct inode *, int);
extern struct nfs_commit_data *nfs_commitdata_alloc(void);
extern void nfs_commit_free(struct nfs_commit_data *data);
void nfs_commit_begin(struct nfs_mds_commit_info *cinfo);
bool nfs_commit_end(struct nfs_mds_commit_info *cinfo);

static inline bool nfs_have_writebacks(const struct inode *inode)
{
	if (S_ISREG(inode->i_mode))
		return atomic_long_read(&NFS_I(inode)->nrequests) != 0;
	return false;
}

/*
 * linux/fs/nfs/read.c
 */
int  nfs_read_folio(struct file *, struct folio *);
void nfs_readahead(struct readahead_control *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * inline functions
 */

static inline loff_t nfs_size_to_loff_t(__u64 size)
{
<<<<<<< HEAD
	if (size > (__u64) OFFSET_MAX - 1)
		return OFFSET_MAX - 1;
	return (loff_t) size;
=======
	return min_t(u64, size, OFFSET_MAX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline ino_t
nfs_fileid_to_ino_t(u64 fileid)
{
	ino_t ino = (ino_t) fileid;
	if (sizeof(ino_t) < sizeof(u64))
		ino ^= fileid >> (sizeof(u64)-sizeof(ino_t)) * 8;
	return ino;
}

<<<<<<< HEAD
#define NFS_JUKEBOX_RETRY_TIME (5 * HZ)

#endif /* __KERNEL__ */

/*
 * NFS debug flags
 */
#define NFSDBG_VFS		0x0001
#define NFSDBG_DIRCACHE		0x0002
#define NFSDBG_LOOKUPCACHE	0x0004
#define NFSDBG_PAGECACHE	0x0008
#define NFSDBG_PROC		0x0010
#define NFSDBG_XDR		0x0020
#define NFSDBG_FILE		0x0040
#define NFSDBG_ROOT		0x0080
#define NFSDBG_CALLBACK		0x0100
#define NFSDBG_CLIENT		0x0200
#define NFSDBG_MOUNT		0x0400
#define NFSDBG_FSCACHE		0x0800
#define NFSDBG_PNFS		0x1000
#define NFSDBG_PNFS_LD		0x2000
#define NFSDBG_ALL		0xFFFF

#ifdef __KERNEL__
=======
static inline void nfs_ooo_clear(struct nfs_inode *nfsi)
{
	nfsi->cache_validity &= ~NFS_INO_DATA_INVAL_DEFER;
	kfree(nfsi->ooo);
	nfsi->ooo = NULL;
}

static inline bool nfs_ooo_test(struct nfs_inode *nfsi)
{
	return (nfsi->cache_validity & NFS_INO_DATA_INVAL_DEFER) ||
		(nfsi->ooo && nfsi->ooo->cnt > 0);

}

#define NFS_JUKEBOX_RETRY_TIME (5 * HZ)

/* We need to block new opens while a file is being unlinked.
 * If it is opened *before* we decide to unlink, we will silly-rename
 * instead. If it is opened *after*, then we need to create or will fail.
 * If we allow the two to race, we could end up with a file that is open
 * but deleted on the server resulting in ESTALE.
 * So use ->d_fsdata to record when the unlink is happening
 * and block dentry revalidation while it is set.
 */
#define NFS_FSDATA_BLOCKED ((void*)1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

# undef ifdebug
# ifdef NFS_DEBUG
#  define ifdebug(fac)		if (unlikely(nfs_debug & NFSDBG_##fac))
#  define NFS_IFDEBUG(x)	x
# else
#  define ifdebug(fac)		if (0)
#  define NFS_IFDEBUG(x)
# endif
<<<<<<< HEAD
#endif /* __KERNEL */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
