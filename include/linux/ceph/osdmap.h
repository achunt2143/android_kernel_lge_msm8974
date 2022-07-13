<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _FS_CEPH_OSDMAP_H
#define _FS_CEPH_OSDMAP_H

#include <linux/rbtree.h>
<<<<<<< HEAD
#include "types.h"
#include "ceph_fs.h"
=======
#include <linux/ceph/types.h>
#include <linux/ceph/decode.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/crush/crush.h>

/*
 * The osd map describes the current membership of the osd cluster and
 * specifies the mapping of objects to placement groups and placement
 * groups to (sets of) osds.  That is, it completely specifies the
 * (desired) distribution of all data objects in the system at some
 * point in time.
 *
 * Each map version is identified by an epoch, which increases monotonically.
 *
 * The map can be updated either via an incremental map (diff) describing
 * the change between two successive epochs, or as a fully encoded map.
 */
<<<<<<< HEAD
struct ceph_pg_pool_info {
	struct rb_node node;
	int id;
	struct ceph_pg_pool v;
	int pg_num_mask, pgp_num_mask, lpg_num_mask, lpgp_num_mask;
	char *name;
=======
struct ceph_pg {
	uint64_t pool;
	uint32_t seed;
};

#define CEPH_SPG_NOSHARD	-1

struct ceph_spg {
	struct ceph_pg pgid;
	s8 shard;
};

int ceph_pg_compare(const struct ceph_pg *lhs, const struct ceph_pg *rhs);
int ceph_spg_compare(const struct ceph_spg *lhs, const struct ceph_spg *rhs);

#define CEPH_POOL_FLAG_HASHPSPOOL	(1ULL << 0) /* hash pg seed and pool id
						       together */
#define CEPH_POOL_FLAG_FULL		(1ULL << 1) /* pool is full */
#define CEPH_POOL_FLAG_FULL_QUOTA	(1ULL << 10) /* pool ran out of quota,
							will set FULL too */
#define CEPH_POOL_FLAG_NEARFULL		(1ULL << 11) /* pool is nearfull */

struct ceph_pg_pool_info {
	struct rb_node node;
	s64 id;
	u8 type; /* CEPH_POOL_TYPE_* */
	u8 size;
	u8 min_size;
	u8 crush_ruleset;
	u8 object_hash;
	u32 last_force_request_resend;
	u32 pg_num, pgp_num;
	int pg_num_mask, pgp_num_mask;
	s64 read_tier;
	s64 write_tier; /* wins for read+write ops */
	u64 flags; /* CEPH_POOL_FLAG_* */
	char *name;

	bool was_full;  /* for handle_one_map() */
};

static inline bool ceph_can_shift_osds(struct ceph_pg_pool_info *pool)
{
	switch (pool->type) {
	case CEPH_POOL_TYPE_REP:
		return true;
	case CEPH_POOL_TYPE_EC:
		return false;
	default:
		BUG();
	}
}

struct ceph_object_locator {
	s64 pool;
	struct ceph_string *pool_ns;
};

static inline void ceph_oloc_init(struct ceph_object_locator *oloc)
{
	oloc->pool = -1;
	oloc->pool_ns = NULL;
}

static inline bool ceph_oloc_empty(const struct ceph_object_locator *oloc)
{
	return oloc->pool == -1;
}

void ceph_oloc_copy(struct ceph_object_locator *dest,
		    const struct ceph_object_locator *src);
void ceph_oloc_destroy(struct ceph_object_locator *oloc);

/*
 * 51-char inline_name is long enough for all cephfs and all but one
 * rbd requests: <imgname> in "<imgname>.rbd"/"rbd_id.<imgname>" can be
 * arbitrarily long (~PAGE_SIZE).  It's done once during rbd map; all
 * other rbd requests fit into inline_name.
 *
 * Makes ceph_object_id 64 bytes on 64-bit.
 */
#define CEPH_OID_INLINE_LEN 52

/*
 * Both inline and external buffers have space for a NUL-terminator,
 * which is carried around.  It's not required though - RADOS object
 * names don't have to be NUL-terminated and may contain NULs.
 */
struct ceph_object_id {
	char *name;
	char inline_name[CEPH_OID_INLINE_LEN];
	int name_len;
};

#define __CEPH_OID_INITIALIZER(oid) { .name = (oid).inline_name }

#define CEPH_DEFINE_OID_ONSTACK(oid)				\
	struct ceph_object_id oid = __CEPH_OID_INITIALIZER(oid)

static inline void ceph_oid_init(struct ceph_object_id *oid)
{
	*oid = (struct ceph_object_id) __CEPH_OID_INITIALIZER(*oid);
}

static inline bool ceph_oid_empty(const struct ceph_object_id *oid)
{
	return oid->name == oid->inline_name && !oid->name_len;
}

void ceph_oid_copy(struct ceph_object_id *dest,
		   const struct ceph_object_id *src);
__printf(2, 3)
void ceph_oid_printf(struct ceph_object_id *oid, const char *fmt, ...);
__printf(3, 4)
int ceph_oid_aprintf(struct ceph_object_id *oid, gfp_t gfp,
		     const char *fmt, ...);
void ceph_oid_destroy(struct ceph_object_id *oid);

struct workspace_manager {
	struct list_head idle_ws;
	spinlock_t ws_lock;
	/* Number of free workspaces */
	int free_ws;
	/* Total number of allocated workspaces */
	atomic_t total_ws;
	/* Waiters for a free workspace */
	wait_queue_head_t ws_wait;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct ceph_pg_mapping {
	struct rb_node node;
	struct ceph_pg pgid;
<<<<<<< HEAD
	int len;
	int osds[];
=======

	union {
		struct {
			int len;
			int osds[];
		} pg_temp, pg_upmap;
		struct {
			int osd;
		} primary_temp;
		struct {
			int len;
			int from_to[][2];
		} pg_upmap_items;
	};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct ceph_osdmap {
	struct ceph_fsid fsid;
	u32 epoch;
<<<<<<< HEAD
	u32 mkfs_epoch;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ceph_timespec created, modified;

	u32 flags;         /* CEPH_OSDMAP_* */

	u32 max_osd;       /* size of osd_state, _offload, _addr arrays */
<<<<<<< HEAD
	u8 *osd_state;     /* CEPH_OSD_* */
=======
	u32 *osd_state;    /* CEPH_OSD_* */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 *osd_weight;   /* 0 = failed, 0x10000 = 100% normal */
	struct ceph_entity_addr *osd_addr;

	struct rb_root pg_temp;
<<<<<<< HEAD
=======
	struct rb_root primary_temp;

	/* remap (post-CRUSH, pre-up) */
	struct rb_root pg_upmap;	/* PG := raw set */
	struct rb_root pg_upmap_items;	/* from -> to within raw set */

	u32 *osd_primary_affinity;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rb_root pg_pools;
	u32 pool_max;

	/* the CRUSH map specifies the mapping of placement groups to
	 * the list of osds that store+replicate them. */
	struct crush_map *crush;
<<<<<<< HEAD
};

/*
 * file layout helpers
 */
#define ceph_file_layout_su(l) ((__s32)le32_to_cpu((l).fl_stripe_unit))
#define ceph_file_layout_stripe_count(l) \
	((__s32)le32_to_cpu((l).fl_stripe_count))
#define ceph_file_layout_object_size(l) ((__s32)le32_to_cpu((l).fl_object_size))
#define ceph_file_layout_cas_hash(l) ((__s32)le32_to_cpu((l).fl_cas_hash))
#define ceph_file_layout_object_su(l) \
	((__s32)le32_to_cpu((l).fl_object_stripe_unit))
#define ceph_file_layout_pg_preferred(l) \
	((__s32)le32_to_cpu((l).fl_pg_preferred))
#define ceph_file_layout_pg_pool(l) \
	((__s32)le32_to_cpu((l).fl_pg_pool))

static inline unsigned ceph_file_layout_stripe_width(struct ceph_file_layout *l)
{
	return le32_to_cpu(l->fl_stripe_unit) *
		le32_to_cpu(l->fl_stripe_count);
}

/* "period" == bytes before i start on a new set of objects */
static inline unsigned ceph_file_layout_period(struct ceph_file_layout *l)
{
	return le32_to_cpu(l->fl_object_size) *
		le32_to_cpu(l->fl_stripe_count);
}


static inline int ceph_osd_is_up(struct ceph_osdmap *map, int osd)
{
	return (osd < map->max_osd) && (map->osd_state[osd] & CEPH_OSD_UP);
}

static inline bool ceph_osdmap_flag(struct ceph_osdmap *map, int flag)
{
	return map && (map->flags & flag);
}

extern char *ceph_osdmap_state_str(char *str, int len, int state);
=======

	struct workspace_manager crush_wsm;
};

static inline bool ceph_osd_exists(struct ceph_osdmap *map, int osd)
{
	return osd >= 0 && osd < map->max_osd &&
	       (map->osd_state[osd] & CEPH_OSD_EXISTS);
}

static inline bool ceph_osd_is_up(struct ceph_osdmap *map, int osd)
{
	return ceph_osd_exists(map, osd) &&
	       (map->osd_state[osd] & CEPH_OSD_UP);
}

static inline bool ceph_osd_is_down(struct ceph_osdmap *map, int osd)
{
	return !ceph_osd_is_up(map, osd);
}

char *ceph_osdmap_state_str(char *str, int len, u32 state);
extern u32 ceph_get_primary_affinity(struct ceph_osdmap *map, int osd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline struct ceph_entity_addr *ceph_osd_addr(struct ceph_osdmap *map,
						     int osd)
{
	if (osd >= map->max_osd)
		return NULL;
	return &map->osd_addr[osd];
}

<<<<<<< HEAD
extern struct ceph_osdmap *osdmap_decode(void **p, void *end);
extern struct ceph_osdmap *osdmap_apply_incremental(void **p, void *end,
					    struct ceph_osdmap *map,
					    struct ceph_messenger *msgr);
extern void ceph_osdmap_destroy(struct ceph_osdmap *map);

/* calculate mapping of a file extent to an object */
extern int ceph_calc_file_object_mapping(struct ceph_file_layout *layout,
					 u64 off, u64 *plen,
					 u64 *bno, u64 *oxoff, u64 *oxlen);

/* calculate mapping of object to a placement group */
extern int ceph_calc_object_layout(struct ceph_object_layout *ol,
				   const char *oid,
				   struct ceph_file_layout *fl,
				   struct ceph_osdmap *osdmap);
extern int ceph_calc_pg_acting(struct ceph_osdmap *osdmap, struct ceph_pg pgid,
			       int *acting);
extern int ceph_calc_pg_primary(struct ceph_osdmap *osdmap,
				struct ceph_pg pgid);

extern int ceph_pg_poolid_by_name(struct ceph_osdmap *map, const char *name);
=======
#define CEPH_PGID_ENCODING_LEN		(1 + 8 + 4 + 4)

static inline int ceph_decode_pgid(void **p, void *end, struct ceph_pg *pgid)
{
	__u8 version;

	if (!ceph_has_room(p, end, CEPH_PGID_ENCODING_LEN)) {
		pr_warn("incomplete pg encoding\n");
		return -EINVAL;
	}
	version = ceph_decode_8(p);
	if (version > 1) {
		pr_warn("do not understand pg encoding %d > 1\n",
			(int)version);
		return -EINVAL;
	}

	pgid->pool = ceph_decode_64(p);
	pgid->seed = ceph_decode_32(p);
	*p += 4;	/* skip deprecated preferred value */

	return 0;
}

struct ceph_osdmap *ceph_osdmap_alloc(void);
struct ceph_osdmap *ceph_osdmap_decode(void **p, void *end, bool msgr2);
struct ceph_osdmap *osdmap_apply_incremental(void **p, void *end, bool msgr2,
					     struct ceph_osdmap *map);
extern void ceph_osdmap_destroy(struct ceph_osdmap *map);

struct ceph_osds {
	int osds[CEPH_PG_MAX_SIZE];
	int size;
	int primary; /* id, NOT index */
};

static inline void ceph_osds_init(struct ceph_osds *set)
{
	set->size = 0;
	set->primary = -1;
}

void ceph_osds_copy(struct ceph_osds *dest, const struct ceph_osds *src);

bool ceph_pg_is_split(const struct ceph_pg *pgid, u32 old_pg_num,
		      u32 new_pg_num);
bool ceph_is_new_interval(const struct ceph_osds *old_acting,
			  const struct ceph_osds *new_acting,
			  const struct ceph_osds *old_up,
			  const struct ceph_osds *new_up,
			  int old_size,
			  int new_size,
			  int old_min_size,
			  int new_min_size,
			  u32 old_pg_num,
			  u32 new_pg_num,
			  bool old_sort_bitwise,
			  bool new_sort_bitwise,
			  bool old_recovery_deletes,
			  bool new_recovery_deletes,
			  const struct ceph_pg *pgid);
bool ceph_osds_changed(const struct ceph_osds *old_acting,
		       const struct ceph_osds *new_acting,
		       bool any_change);

void __ceph_object_locator_to_pg(struct ceph_pg_pool_info *pi,
				 const struct ceph_object_id *oid,
				 const struct ceph_object_locator *oloc,
				 struct ceph_pg *raw_pgid);
int ceph_object_locator_to_pg(struct ceph_osdmap *osdmap,
			      const struct ceph_object_id *oid,
			      const struct ceph_object_locator *oloc,
			      struct ceph_pg *raw_pgid);

void ceph_pg_to_up_acting_osds(struct ceph_osdmap *osdmap,
			       struct ceph_pg_pool_info *pi,
			       const struct ceph_pg *raw_pgid,
			       struct ceph_osds *up,
			       struct ceph_osds *acting);
bool ceph_pg_to_primary_shard(struct ceph_osdmap *osdmap,
			      struct ceph_pg_pool_info *pi,
			      const struct ceph_pg *raw_pgid,
			      struct ceph_spg *spgid);
int ceph_pg_to_acting_primary(struct ceph_osdmap *osdmap,
			      const struct ceph_pg *raw_pgid);

struct crush_loc {
	char *cl_type_name;
	char *cl_name;
};

struct crush_loc_node {
	struct rb_node cl_node;
	struct crush_loc cl_loc;  /* pointers into cl_data */
	char cl_data[];
};

int ceph_parse_crush_location(char *crush_location, struct rb_root *locs);
int ceph_compare_crush_locs(struct rb_root *locs1, struct rb_root *locs2);
void ceph_clear_crush_locs(struct rb_root *locs);

int ceph_get_crush_locality(struct ceph_osdmap *osdmap, int id,
			    struct rb_root *locs);

extern struct ceph_pg_pool_info *ceph_pg_pool_by_id(struct ceph_osdmap *map,
						    u64 id);
extern const char *ceph_pg_pool_name_by_id(struct ceph_osdmap *map, u64 id);
extern int ceph_pg_poolid_by_name(struct ceph_osdmap *map, const char *name);
u64 ceph_pg_pool_flags(struct ceph_osdmap *map, u64 id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
