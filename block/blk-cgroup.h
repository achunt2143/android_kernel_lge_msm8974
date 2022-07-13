<<<<<<< HEAD
#ifndef _BLK_CGROUP_H
#define _BLK_CGROUP_H
/*
 * Common Block IO controller cgroup interface
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _BLK_CGROUP_PRIVATE_H
#define _BLK_CGROUP_PRIVATE_H
/*
 * block cgroup private header
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Based on ideas and code from CFQ, CFS and BFQ:
 * Copyright (C) 2003 Jens Axboe <axboe@kernel.dk>
 *
 * Copyright (C) 2008 Fabio Checconi <fabio@gandalf.sssup.it>
 *		      Paolo Valente <paolo.valente@unimore.it>
 *
 * Copyright (C) 2009 Vivek Goyal <vgoyal@redhat.com>
 * 	              Nauman Rafique <nauman@google.com>
 */

<<<<<<< HEAD
#include <linux/cgroup.h>
#include <linux/u64_stats_sync.h>

enum blkio_policy_id {
	BLKIO_POLICY_PROP = 0,		/* Proportional Bandwidth division */
	BLKIO_POLICY_THROTL,		/* Throttling */
};

/* Max limits for throttle policy */
#define THROTL_IOPS_MAX		UINT_MAX

#if defined(CONFIG_BLK_CGROUP) || defined(CONFIG_BLK_CGROUP_MODULE)

#ifndef CONFIG_BLK_CGROUP
/* When blk-cgroup is a module, its subsys_id isn't a compile-time constant */
extern struct cgroup_subsys blkio_subsys;
#define blkio_subsys_id blkio_subsys.subsys_id
#endif

enum stat_type {
	/* Total time spent (in ns) between request dispatch to the driver and
	 * request completion for IOs doen by this cgroup. This may not be
	 * accurate when NCQ is turned on. */
	BLKIO_STAT_SERVICE_TIME = 0,
	/* Total time spent waiting in scheduler queue in ns */
	BLKIO_STAT_WAIT_TIME,
	/* Number of IOs queued up */
	BLKIO_STAT_QUEUED,
	/* All the single valued stats go below this */
	BLKIO_STAT_TIME,
#ifdef CONFIG_DEBUG_BLK_CGROUP
	/* Time not charged to this cgroup */
	BLKIO_STAT_UNACCOUNTED_TIME,
	BLKIO_STAT_AVG_QUEUE_SIZE,
	BLKIO_STAT_IDLE_TIME,
	BLKIO_STAT_EMPTY_TIME,
	BLKIO_STAT_GROUP_WAIT_TIME,
	BLKIO_STAT_DEQUEUE
#endif
};

/* Per cpu stats */
enum stat_type_cpu {
	BLKIO_STAT_CPU_SECTORS,
	/* Total bytes transferred */
	BLKIO_STAT_CPU_SERVICE_BYTES,
	/* Total IOs serviced, post merge */
	BLKIO_STAT_CPU_SERVICED,
	/* Number of IOs merged */
	BLKIO_STAT_CPU_MERGED,
	BLKIO_STAT_CPU_NR
};

enum stat_sub_type {
	BLKIO_STAT_READ = 0,
	BLKIO_STAT_WRITE,
	BLKIO_STAT_SYNC,
	BLKIO_STAT_ASYNC,
	BLKIO_STAT_TOTAL
};

/* blkg state flags */
enum blkg_state_flags {
	BLKG_waiting = 0,
	BLKG_idling,
	BLKG_empty,
};

/* cgroup files owned by proportional weight policy */
enum blkcg_file_name_prop {
	BLKIO_PROP_weight = 1,
	BLKIO_PROP_weight_device,
	BLKIO_PROP_io_service_bytes,
	BLKIO_PROP_io_serviced,
	BLKIO_PROP_time,
	BLKIO_PROP_sectors,
	BLKIO_PROP_unaccounted_time,
	BLKIO_PROP_io_service_time,
	BLKIO_PROP_io_wait_time,
	BLKIO_PROP_io_merged,
	BLKIO_PROP_io_queued,
	BLKIO_PROP_avg_queue_size,
	BLKIO_PROP_group_wait_time,
	BLKIO_PROP_idle_time,
	BLKIO_PROP_empty_time,
	BLKIO_PROP_dequeue,
};

/* cgroup files owned by throttle policy */
enum blkcg_file_name_throtl {
	BLKIO_THROTL_read_bps_device,
	BLKIO_THROTL_write_bps_device,
	BLKIO_THROTL_read_iops_device,
	BLKIO_THROTL_write_iops_device,
	BLKIO_THROTL_io_service_bytes,
	BLKIO_THROTL_io_serviced,
};

struct blkio_cgroup {
	struct cgroup_subsys_state css;
	unsigned int weight;
	spinlock_t lock;
	struct hlist_head blkg_list;
	struct list_head policy_list; /* list of blkio_policy_node */
};

struct blkio_group_stats {
	/* total disk time and nr sectors dispatched by this group */
	uint64_t time;
	uint64_t stat_arr[BLKIO_STAT_QUEUED + 1][BLKIO_STAT_TOTAL];
#ifdef CONFIG_DEBUG_BLK_CGROUP
	/* Time not charged to this cgroup */
	uint64_t unaccounted_time;

	/* Sum of number of IOs queued across all samples */
	uint64_t avg_queue_size_sum;
	/* Count of samples taken for average */
	uint64_t avg_queue_size_samples;
	/* How many times this group has been removed from service tree */
	unsigned long dequeue;

	/* Total time spent waiting for it to be assigned a timeslice. */
	uint64_t group_wait_time;
	uint64_t start_group_wait_time;

	/* Time spent idling for this blkio_group */
	uint64_t idle_time;
	uint64_t start_idle_time;
	/*
	 * Total time when we have requests queued and do not contain the
	 * current active queue.
	 */
	uint64_t empty_time;
	uint64_t start_empty_time;
	uint16_t flags;
#endif
};

/* Per cpu blkio group stats */
struct blkio_group_stats_cpu {
	uint64_t sectors;
	uint64_t stat_arr_cpu[BLKIO_STAT_CPU_NR][BLKIO_STAT_TOTAL];
	struct u64_stats_sync syncp;
};

struct blkio_group {
	/* An rcu protected unique identifier for the group */
	void *key;
	struct hlist_node blkcg_node;
	unsigned short blkcg_id;
	/* Store cgroup path */
	char path[128];
	/* The device MKDEV(major, minor), this group has been created for */
	dev_t dev;
	/* policy which owns this blk group */
	enum blkio_policy_id plid;

	/* Need to serialize the stats in the case of reset/update */
	spinlock_t stats_lock;
	struct blkio_group_stats stats;
	/* Per cpu stats pointer */
	struct blkio_group_stats_cpu __percpu *stats_cpu;
};

struct blkio_policy_node {
	struct list_head node;
	dev_t dev;
	/* This node belongs to max bw policy or porportional weight policy */
	enum blkio_policy_id plid;
	/* cgroup file to which this rule belongs to */
	int fileid;

	union {
		unsigned int weight;
		/*
		 * Rate read/write in terms of bytes per second
		 * Whether this rate represents read or write is determined
		 * by file type "fileid".
		 */
		u64 bps;
		unsigned int iops;
	} val;
};

extern unsigned int blkcg_get_weight(struct blkio_cgroup *blkcg,
				     dev_t dev);
extern uint64_t blkcg_get_read_bps(struct blkio_cgroup *blkcg,
				     dev_t dev);
extern uint64_t blkcg_get_write_bps(struct blkio_cgroup *blkcg,
				     dev_t dev);
extern unsigned int blkcg_get_read_iops(struct blkio_cgroup *blkcg,
				     dev_t dev);
extern unsigned int blkcg_get_write_iops(struct blkio_cgroup *blkcg,
				     dev_t dev);

typedef void (blkio_unlink_group_fn) (void *key, struct blkio_group *blkg);

typedef void (blkio_update_group_weight_fn) (void *key,
			struct blkio_group *blkg, unsigned int weight);
typedef void (blkio_update_group_read_bps_fn) (void * key,
			struct blkio_group *blkg, u64 read_bps);
typedef void (blkio_update_group_write_bps_fn) (void *key,
			struct blkio_group *blkg, u64 write_bps);
typedef void (blkio_update_group_read_iops_fn) (void *key,
			struct blkio_group *blkg, unsigned int read_iops);
typedef void (blkio_update_group_write_iops_fn) (void *key,
			struct blkio_group *blkg, unsigned int write_iops);

struct blkio_policy_ops {
	blkio_unlink_group_fn *blkio_unlink_group_fn;
	blkio_update_group_weight_fn *blkio_update_group_weight_fn;
	blkio_update_group_read_bps_fn *blkio_update_group_read_bps_fn;
	blkio_update_group_write_bps_fn *blkio_update_group_write_bps_fn;
	blkio_update_group_read_iops_fn *blkio_update_group_read_iops_fn;
	blkio_update_group_write_iops_fn *blkio_update_group_write_iops_fn;
};

struct blkio_policy_type {
	struct list_head list;
	struct blkio_policy_ops ops;
	enum blkio_policy_id plid;
};

/* Blkio controller policy registration */
extern void blkio_policy_register(struct blkio_policy_type *);
extern void blkio_policy_unregister(struct blkio_policy_type *);

static inline char *blkg_path(struct blkio_group *blkg)
{
	return blkg->path;
}

#else

struct blkio_group {
};

struct blkio_policy_type {
};

static inline void blkio_policy_register(struct blkio_policy_type *blkiop) { }
static inline void blkio_policy_unregister(struct blkio_policy_type *blkiop) { }

static inline char *blkg_path(struct blkio_group *blkg) { return NULL; }

#endif

#define BLKIO_WEIGHT_MIN	10
#define BLKIO_WEIGHT_MAX	1000
#define BLKIO_WEIGHT_DEFAULT	500

#ifdef CONFIG_DEBUG_BLK_CGROUP
void blkiocg_update_avg_queue_size_stats(struct blkio_group *blkg);
void blkiocg_update_dequeue_stats(struct blkio_group *blkg,
				unsigned long dequeue);
void blkiocg_update_set_idle_time_stats(struct blkio_group *blkg);
void blkiocg_update_idle_time_stats(struct blkio_group *blkg);
void blkiocg_set_start_empty_time(struct blkio_group *blkg);

#define BLKG_FLAG_FNS(name)						\
static inline void blkio_mark_blkg_##name(				\
		struct blkio_group_stats *stats)			\
{									\
	stats->flags |= (1 << BLKG_##name);				\
}									\
static inline void blkio_clear_blkg_##name(				\
		struct blkio_group_stats *stats)			\
{									\
	stats->flags &= ~(1 << BLKG_##name);				\
}									\
static inline int blkio_blkg_##name(struct blkio_group_stats *stats)	\
{									\
	return (stats->flags & (1 << BLKG_##name)) != 0;		\
}									\

BLKG_FLAG_FNS(waiting)
BLKG_FLAG_FNS(idling)
BLKG_FLAG_FNS(empty)
#undef BLKG_FLAG_FNS
#else
static inline void blkiocg_update_avg_queue_size_stats(
						struct blkio_group *blkg) {}
static inline void blkiocg_update_dequeue_stats(struct blkio_group *blkg,
						unsigned long dequeue) {}
static inline void blkiocg_update_set_idle_time_stats(struct blkio_group *blkg)
{}
static inline void blkiocg_update_idle_time_stats(struct blkio_group *blkg) {}
static inline void blkiocg_set_start_empty_time(struct blkio_group *blkg) {}
#endif

#if defined(CONFIG_BLK_CGROUP) || defined(CONFIG_BLK_CGROUP_MODULE)
extern struct blkio_cgroup blkio_root_cgroup;
extern struct blkio_cgroup *cgroup_to_blkio_cgroup(struct cgroup *cgroup);
extern struct blkio_cgroup *task_blkio_cgroup(struct task_struct *tsk);
extern void blkiocg_add_blkio_group(struct blkio_cgroup *blkcg,
	struct blkio_group *blkg, void *key, dev_t dev,
	enum blkio_policy_id plid);
extern int blkio_alloc_blkg_stats(struct blkio_group *blkg);
extern int blkiocg_del_blkio_group(struct blkio_group *blkg);
extern struct blkio_group *blkiocg_lookup_group(struct blkio_cgroup *blkcg,
						void *key);
void blkiocg_update_timeslice_used(struct blkio_group *blkg,
					unsigned long time,
					unsigned long unaccounted_time);
void blkiocg_update_dispatch_stats(struct blkio_group *blkg, uint64_t bytes,
						bool direction, bool sync);
void blkiocg_update_completion_stats(struct blkio_group *blkg,
	uint64_t start_time, uint64_t io_start_time, bool direction, bool sync);
void blkiocg_update_io_merged_stats(struct blkio_group *blkg, bool direction,
					bool sync);
void blkiocg_update_io_add_stats(struct blkio_group *blkg,
		struct blkio_group *curr_blkg, bool direction, bool sync);
void blkiocg_update_io_remove_stats(struct blkio_group *blkg,
					bool direction, bool sync);
#else
struct cgroup;
static inline struct blkio_cgroup *
cgroup_to_blkio_cgroup(struct cgroup *cgroup) { return NULL; }
static inline struct blkio_cgroup *
task_blkio_cgroup(struct task_struct *tsk) { return NULL; }

static inline void blkiocg_add_blkio_group(struct blkio_cgroup *blkcg,
		struct blkio_group *blkg, void *key, dev_t dev,
		enum blkio_policy_id plid) {}

static inline int blkio_alloc_blkg_stats(struct blkio_group *blkg) { return 0; }

static inline int
blkiocg_del_blkio_group(struct blkio_group *blkg) { return 0; }

static inline struct blkio_group *
blkiocg_lookup_group(struct blkio_cgroup *blkcg, void *key) { return NULL; }
static inline void blkiocg_update_timeslice_used(struct blkio_group *blkg,
						unsigned long time,
						unsigned long unaccounted_time)
{}
static inline void blkiocg_update_dispatch_stats(struct blkio_group *blkg,
				uint64_t bytes, bool direction, bool sync) {}
static inline void blkiocg_update_completion_stats(struct blkio_group *blkg,
		uint64_t start_time, uint64_t io_start_time, bool direction,
		bool sync) {}
static inline void blkiocg_update_io_merged_stats(struct blkio_group *blkg,
						bool direction, bool sync) {}
static inline void blkiocg_update_io_add_stats(struct blkio_group *blkg,
		struct blkio_group *curr_blkg, bool direction, bool sync) {}
static inline void blkiocg_update_io_remove_stats(struct blkio_group *blkg,
						bool direction, bool sync) {}
#endif
#endif /* _BLK_CGROUP_H */
=======
#include <linux/blk-cgroup.h>
#include <linux/cgroup.h>
#include <linux/kthread.h>
#include <linux/blk-mq.h>
#include <linux/llist.h>
#include "blk.h"

struct blkcg_gq;
struct blkg_policy_data;


/* percpu_counter batch for blkg_[rw]stats, per-cpu drift doesn't matter */
#define BLKG_STAT_CPU_BATCH	(INT_MAX / 2)

#ifdef CONFIG_BLK_CGROUP

enum blkg_iostat_type {
	BLKG_IOSTAT_READ,
	BLKG_IOSTAT_WRITE,
	BLKG_IOSTAT_DISCARD,

	BLKG_IOSTAT_NR,
};

struct blkg_iostat {
	u64				bytes[BLKG_IOSTAT_NR];
	u64				ios[BLKG_IOSTAT_NR];
};

struct blkg_iostat_set {
	struct u64_stats_sync		sync;
	struct blkcg_gq		       *blkg;
	struct llist_node		lnode;
	int				lqueued;	/* queued in llist */
	struct blkg_iostat		cur;
	struct blkg_iostat		last;
};

/* association between a blk cgroup and a request queue */
struct blkcg_gq {
	/* Pointer to the associated request_queue */
	struct request_queue		*q;
	struct list_head		q_node;
	struct hlist_node		blkcg_node;
	struct blkcg			*blkcg;

	/* all non-root blkcg_gq's are guaranteed to have access to parent */
	struct blkcg_gq			*parent;

	/* reference count */
	struct percpu_ref		refcnt;

	/* is this blkg online? protected by both blkcg and q locks */
	bool				online;

	struct blkg_iostat_set __percpu	*iostat_cpu;
	struct blkg_iostat_set		iostat;

	struct blkg_policy_data		*pd[BLKCG_MAX_POLS];
#ifdef CONFIG_BLK_CGROUP_PUNT_BIO
	spinlock_t			async_bio_lock;
	struct bio_list			async_bios;
#endif
	union {
		struct work_struct	async_bio_work;
		struct work_struct	free_work;
	};

	atomic_t			use_delay;
	atomic64_t			delay_nsec;
	atomic64_t			delay_start;
	u64				last_delay;
	int				last_use;

	struct rcu_head			rcu_head;
};

struct blkcg {
	struct cgroup_subsys_state	css;
	spinlock_t			lock;
	refcount_t			online_pin;

	struct radix_tree_root		blkg_tree;
	struct blkcg_gq	__rcu		*blkg_hint;
	struct hlist_head		blkg_list;

	struct blkcg_policy_data	*cpd[BLKCG_MAX_POLS];

	struct list_head		all_blkcgs_node;

	/*
	 * List of updated percpu blkg_iostat_set's since the last flush.
	 */
	struct llist_head __percpu	*lhead;

#ifdef CONFIG_BLK_CGROUP_FC_APPID
	char                            fc_app_id[FC_APPID_LEN];
#endif
#ifdef CONFIG_CGROUP_WRITEBACK
	struct list_head		cgwb_list;
#endif
};

static inline struct blkcg *css_to_blkcg(struct cgroup_subsys_state *css)
{
	return css ? container_of(css, struct blkcg, css) : NULL;
}

/*
 * A blkcg_gq (blkg) is association between a block cgroup (blkcg) and a
 * request_queue (q).  This is used by blkcg policies which need to track
 * information per blkcg - q pair.
 *
 * There can be multiple active blkcg policies and each blkg:policy pair is
 * represented by a blkg_policy_data which is allocated and freed by each
 * policy's pd_alloc/free_fn() methods.  A policy can allocate private data
 * area by allocating larger data structure which embeds blkg_policy_data
 * at the beginning.
 */
struct blkg_policy_data {
	/* the blkg and policy id this per-policy data belongs to */
	struct blkcg_gq			*blkg;
	int				plid;
	bool				online;
};

/*
 * Policies that need to keep per-blkcg data which is independent from any
 * request_queue associated to it should implement cpd_alloc/free_fn()
 * methods.  A policy can allocate private data area by allocating larger
 * data structure which embeds blkcg_policy_data at the beginning.
 * cpd_init() is invoked to let each policy handle per-blkcg data.
 */
struct blkcg_policy_data {
	/* the blkcg and policy id this per-policy data belongs to */
	struct blkcg			*blkcg;
	int				plid;
};

typedef struct blkcg_policy_data *(blkcg_pol_alloc_cpd_fn)(gfp_t gfp);
typedef void (blkcg_pol_init_cpd_fn)(struct blkcg_policy_data *cpd);
typedef void (blkcg_pol_free_cpd_fn)(struct blkcg_policy_data *cpd);
typedef void (blkcg_pol_bind_cpd_fn)(struct blkcg_policy_data *cpd);
typedef struct blkg_policy_data *(blkcg_pol_alloc_pd_fn)(struct gendisk *disk,
		struct blkcg *blkcg, gfp_t gfp);
typedef void (blkcg_pol_init_pd_fn)(struct blkg_policy_data *pd);
typedef void (blkcg_pol_online_pd_fn)(struct blkg_policy_data *pd);
typedef void (blkcg_pol_offline_pd_fn)(struct blkg_policy_data *pd);
typedef void (blkcg_pol_free_pd_fn)(struct blkg_policy_data *pd);
typedef void (blkcg_pol_reset_pd_stats_fn)(struct blkg_policy_data *pd);
typedef void (blkcg_pol_stat_pd_fn)(struct blkg_policy_data *pd,
				struct seq_file *s);

struct blkcg_policy {
	int				plid;
	/* cgroup files for the policy */
	struct cftype			*dfl_cftypes;
	struct cftype			*legacy_cftypes;

	/* operations */
	blkcg_pol_alloc_cpd_fn		*cpd_alloc_fn;
	blkcg_pol_free_cpd_fn		*cpd_free_fn;

	blkcg_pol_alloc_pd_fn		*pd_alloc_fn;
	blkcg_pol_init_pd_fn		*pd_init_fn;
	blkcg_pol_online_pd_fn		*pd_online_fn;
	blkcg_pol_offline_pd_fn		*pd_offline_fn;
	blkcg_pol_free_pd_fn		*pd_free_fn;
	blkcg_pol_reset_pd_stats_fn	*pd_reset_stats_fn;
	blkcg_pol_stat_pd_fn		*pd_stat_fn;
};

extern struct blkcg blkcg_root;
extern bool blkcg_debug_stats;

void blkg_init_queue(struct request_queue *q);
int blkcg_init_disk(struct gendisk *disk);
void blkcg_exit_disk(struct gendisk *disk);

/* Blkio controller policy registration */
int blkcg_policy_register(struct blkcg_policy *pol);
void blkcg_policy_unregister(struct blkcg_policy *pol);
int blkcg_activate_policy(struct gendisk *disk, const struct blkcg_policy *pol);
void blkcg_deactivate_policy(struct gendisk *disk,
			     const struct blkcg_policy *pol);

const char *blkg_dev_name(struct blkcg_gq *blkg);
void blkcg_print_blkgs(struct seq_file *sf, struct blkcg *blkcg,
		       u64 (*prfill)(struct seq_file *,
				     struct blkg_policy_data *, int),
		       const struct blkcg_policy *pol, int data,
		       bool show_total);
u64 __blkg_prfill_u64(struct seq_file *sf, struct blkg_policy_data *pd, u64 v);

struct blkg_conf_ctx {
	char				*input;
	char				*body;
	struct block_device		*bdev;
	struct blkcg_gq			*blkg;
};

void blkg_conf_init(struct blkg_conf_ctx *ctx, char *input);
int blkg_conf_open_bdev(struct blkg_conf_ctx *ctx);
int blkg_conf_prep(struct blkcg *blkcg, const struct blkcg_policy *pol,
		   struct blkg_conf_ctx *ctx);
void blkg_conf_exit(struct blkg_conf_ctx *ctx);

/**
 * bio_issue_as_root_blkg - see if this bio needs to be issued as root blkg
 * @return: true if this bio needs to be submitted with the root blkg context.
 *
 * In order to avoid priority inversions we sometimes need to issue a bio as if
 * it were attached to the root blkg, and then backcharge to the actual owning
 * blkg.  The idea is we do bio_blkcg_css() to look up the actual context for
 * the bio and attach the appropriate blkg to the bio.  Then we call this helper
 * and if it is true run with the root blkg for that queue and then do any
 * backcharging to the originating cgroup once the io is complete.
 */
static inline bool bio_issue_as_root_blkg(struct bio *bio)
{
	return (bio->bi_opf & (REQ_META | REQ_SWAP)) != 0;
}

/**
 * blkg_lookup - lookup blkg for the specified blkcg - q pair
 * @blkcg: blkcg of interest
 * @q: request_queue of interest
 *
 * Lookup blkg for the @blkcg - @q pair.

 * Must be called in a RCU critical section.
 */
static inline struct blkcg_gq *blkg_lookup(struct blkcg *blkcg,
					   struct request_queue *q)
{
	struct blkcg_gq *blkg;

	if (blkcg == &blkcg_root)
		return q->root_blkg;

	blkg = rcu_dereference_check(blkcg->blkg_hint,
			lockdep_is_held(&q->queue_lock));
	if (blkg && blkg->q == q)
		return blkg;

	blkg = radix_tree_lookup(&blkcg->blkg_tree, q->id);
	if (blkg && blkg->q != q)
		blkg = NULL;
	return blkg;
}

/**
 * blkg_to_pdata - get policy private data
 * @blkg: blkg of interest
 * @pol: policy of interest
 *
 * Return pointer to private data associated with the @blkg-@pol pair.
 */
static inline struct blkg_policy_data *blkg_to_pd(struct blkcg_gq *blkg,
						  struct blkcg_policy *pol)
{
	return blkg ? blkg->pd[pol->plid] : NULL;
}

static inline struct blkcg_policy_data *blkcg_to_cpd(struct blkcg *blkcg,
						     struct blkcg_policy *pol)
{
	return blkcg ? blkcg->cpd[pol->plid] : NULL;
}

/**
 * pdata_to_blkg - get blkg associated with policy private data
 * @pd: policy private data of interest
 *
 * @pd is policy private data.  Determine the blkg it's associated with.
 */
static inline struct blkcg_gq *pd_to_blkg(struct blkg_policy_data *pd)
{
	return pd ? pd->blkg : NULL;
}

static inline struct blkcg *cpd_to_blkcg(struct blkcg_policy_data *cpd)
{
	return cpd ? cpd->blkcg : NULL;
}

/**
 * blkg_path - format cgroup path of blkg
 * @blkg: blkg of interest
 * @buf: target buffer
 * @buflen: target buffer length
 *
 * Format the path of the cgroup of @blkg into @buf.
 */
static inline int blkg_path(struct blkcg_gq *blkg, char *buf, int buflen)
{
	return cgroup_path(blkg->blkcg->css.cgroup, buf, buflen);
}

/**
 * blkg_get - get a blkg reference
 * @blkg: blkg to get
 *
 * The caller should be holding an existing reference.
 */
static inline void blkg_get(struct blkcg_gq *blkg)
{
	percpu_ref_get(&blkg->refcnt);
}

/**
 * blkg_tryget - try and get a blkg reference
 * @blkg: blkg to get
 *
 * This is for use when doing an RCU lookup of the blkg.  We may be in the midst
 * of freeing this blkg, so we can only use it if the refcnt is not zero.
 */
static inline bool blkg_tryget(struct blkcg_gq *blkg)
{
	return blkg && percpu_ref_tryget(&blkg->refcnt);
}

/**
 * blkg_put - put a blkg reference
 * @blkg: blkg to put
 */
static inline void blkg_put(struct blkcg_gq *blkg)
{
	percpu_ref_put(&blkg->refcnt);
}

/**
 * blkg_for_each_descendant_pre - pre-order walk of a blkg's descendants
 * @d_blkg: loop cursor pointing to the current descendant
 * @pos_css: used for iteration
 * @p_blkg: target blkg to walk descendants of
 *
 * Walk @c_blkg through the descendants of @p_blkg.  Must be used with RCU
 * read locked.  If called under either blkcg or queue lock, the iteration
 * is guaranteed to include all and only online blkgs.  The caller may
 * update @pos_css by calling css_rightmost_descendant() to skip subtree.
 * @p_blkg is included in the iteration and the first node to be visited.
 */
#define blkg_for_each_descendant_pre(d_blkg, pos_css, p_blkg)		\
	css_for_each_descendant_pre((pos_css), &(p_blkg)->blkcg->css)	\
		if (((d_blkg) = blkg_lookup(css_to_blkcg(pos_css),	\
					    (p_blkg)->q)))

/**
 * blkg_for_each_descendant_post - post-order walk of a blkg's descendants
 * @d_blkg: loop cursor pointing to the current descendant
 * @pos_css: used for iteration
 * @p_blkg: target blkg to walk descendants of
 *
 * Similar to blkg_for_each_descendant_pre() but performs post-order
 * traversal instead.  Synchronization rules are the same.  @p_blkg is
 * included in the iteration and the last node to be visited.
 */
#define blkg_for_each_descendant_post(d_blkg, pos_css, p_blkg)		\
	css_for_each_descendant_post((pos_css), &(p_blkg)->blkcg->css)	\
		if (((d_blkg) = blkg_lookup(css_to_blkcg(pos_css),	\
					    (p_blkg)->q)))

static inline void blkcg_bio_issue_init(struct bio *bio)
{
	bio_issue_init(&bio->bi_issue, bio_sectors(bio));
}

static inline void blkcg_use_delay(struct blkcg_gq *blkg)
{
	if (WARN_ON_ONCE(atomic_read(&blkg->use_delay) < 0))
		return;
	if (atomic_add_return(1, &blkg->use_delay) == 1)
		atomic_inc(&blkg->blkcg->css.cgroup->congestion_count);
}

static inline int blkcg_unuse_delay(struct blkcg_gq *blkg)
{
	int old = atomic_read(&blkg->use_delay);

	if (WARN_ON_ONCE(old < 0))
		return 0;
	if (old == 0)
		return 0;

	/*
	 * We do this song and dance because we can race with somebody else
	 * adding or removing delay.  If we just did an atomic_dec we'd end up
	 * negative and we'd already be in trouble.  We need to subtract 1 and
	 * then check to see if we were the last delay so we can drop the
	 * congestion count on the cgroup.
	 */
	while (old && !atomic_try_cmpxchg(&blkg->use_delay, &old, old - 1))
		;

	if (old == 0)
		return 0;
	if (old == 1)
		atomic_dec(&blkg->blkcg->css.cgroup->congestion_count);
	return 1;
}

/**
 * blkcg_set_delay - Enable allocator delay mechanism with the specified delay amount
 * @blkg: target blkg
 * @delay: delay duration in nsecs
 *
 * When enabled with this function, the delay is not decayed and must be
 * explicitly cleared with blkcg_clear_delay(). Must not be mixed with
 * blkcg_[un]use_delay() and blkcg_add_delay() usages.
 */
static inline void blkcg_set_delay(struct blkcg_gq *blkg, u64 delay)
{
	int old = atomic_read(&blkg->use_delay);

	/* We only want 1 person setting the congestion count for this blkg. */
	if (!old && atomic_try_cmpxchg(&blkg->use_delay, &old, -1))
		atomic_inc(&blkg->blkcg->css.cgroup->congestion_count);

	atomic64_set(&blkg->delay_nsec, delay);
}

/**
 * blkcg_clear_delay - Disable allocator delay mechanism
 * @blkg: target blkg
 *
 * Disable use_delay mechanism. See blkcg_set_delay().
 */
static inline void blkcg_clear_delay(struct blkcg_gq *blkg)
{
	int old = atomic_read(&blkg->use_delay);

	/* We only want 1 person clearing the congestion count for this blkg. */
	if (old && atomic_try_cmpxchg(&blkg->use_delay, &old, 0))
		atomic_dec(&blkg->blkcg->css.cgroup->congestion_count);
}

/**
 * blk_cgroup_mergeable - Determine whether to allow or disallow merges
 * @rq: request to merge into
 * @bio: bio to merge
 *
 * @bio and @rq should belong to the same cgroup and their issue_as_root should
 * match. The latter is necessary as we don't want to throttle e.g. a metadata
 * update because it happens to be next to a regular IO.
 */
static inline bool blk_cgroup_mergeable(struct request *rq, struct bio *bio)
{
	return rq->bio->bi_blkg == bio->bi_blkg &&
		bio_issue_as_root_blkg(rq->bio) == bio_issue_as_root_blkg(bio);
}

void blk_cgroup_bio_start(struct bio *bio);
void blkcg_add_delay(struct blkcg_gq *blkg, u64 now, u64 delta);
#else	/* CONFIG_BLK_CGROUP */

struct blkg_policy_data {
};

struct blkcg_policy_data {
};

struct blkcg_policy {
};

struct blkcg {
};

static inline struct blkcg_gq *blkg_lookup(struct blkcg *blkcg, void *key) { return NULL; }
static inline void blkg_init_queue(struct request_queue *q) { }
static inline int blkcg_init_disk(struct gendisk *disk) { return 0; }
static inline void blkcg_exit_disk(struct gendisk *disk) { }
static inline int blkcg_policy_register(struct blkcg_policy *pol) { return 0; }
static inline void blkcg_policy_unregister(struct blkcg_policy *pol) { }
static inline int blkcg_activate_policy(struct gendisk *disk,
					const struct blkcg_policy *pol) { return 0; }
static inline void blkcg_deactivate_policy(struct gendisk *disk,
					   const struct blkcg_policy *pol) { }

static inline struct blkg_policy_data *blkg_to_pd(struct blkcg_gq *blkg,
						  struct blkcg_policy *pol) { return NULL; }
static inline struct blkcg_gq *pd_to_blkg(struct blkg_policy_data *pd) { return NULL; }
static inline char *blkg_path(struct blkcg_gq *blkg) { return NULL; }
static inline void blkg_get(struct blkcg_gq *blkg) { }
static inline void blkg_put(struct blkcg_gq *blkg) { }
static inline void blkcg_bio_issue_init(struct bio *bio) { }
static inline void blk_cgroup_bio_start(struct bio *bio) { }
static inline bool blk_cgroup_mergeable(struct request *rq, struct bio *bio) { return true; }

#define blk_queue_for_each_rl(rl, q)	\
	for ((rl) = &(q)->root_rl; (rl); (rl) = NULL)

#endif	/* CONFIG_BLK_CGROUP */

#endif /* _BLK_CGROUP_PRIVATE_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
