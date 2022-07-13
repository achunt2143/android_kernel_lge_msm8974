<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2001 Sistina Software (UK) Limited.
 * Copyright (C) 2004-2008 Red Hat, Inc. All rights reserved.
 *
 * This file is released under the LGPL.
 */

#ifndef _LINUX_DEVICE_MAPPER_H
#define _LINUX_DEVICE_MAPPER_H

#include <linux/bio.h>
#include <linux/blkdev.h>
<<<<<<< HEAD
=======
#include <linux/dm-ioctl.h>
#include <linux/math64.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/ratelimit.h>

struct dm_dev;
struct dm_target;
struct dm_table;
<<<<<<< HEAD
struct mapped_device;
struct bio_vec;

typedef enum { STATUSTYPE_INFO, STATUSTYPE_TABLE } status_type_t;

union map_info {
	void *ptr;
	unsigned long long ll;
	unsigned target_request_nr;
=======
struct dm_report_zones_args;
struct mapped_device;
struct bio_vec;
enum dax_access_mode;

/*
 * Type of table, mapped_device's mempool and request_queue
 */
enum dm_queue_mode {
	DM_TYPE_NONE		 = 0,
	DM_TYPE_BIO_BASED	 = 1,
	DM_TYPE_REQUEST_BASED	 = 2,
	DM_TYPE_DAX_BIO_BASED	 = 3,
};

typedef enum { STATUSTYPE_INFO, STATUSTYPE_TABLE, STATUSTYPE_IMA } status_type_t;

union map_info {
	void *ptr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * In the constructor the target parameter will already have the
 * table, type, begin and len fields filled in.
 */
typedef int (*dm_ctr_fn) (struct dm_target *target,
			  unsigned int argc, char **argv);

/*
 * The destructor doesn't need to free the dm_target, just
 * anything hidden ti->private.
 */
typedef void (*dm_dtr_fn) (struct dm_target *ti);

/*
 * The map function must return:
 * < 0: error
 * = 0: The target will handle the io by resubmitting it later
 * = 1: simple remap complete
 * = 2: The target wants to push back the io
 */
<<<<<<< HEAD
typedef int (*dm_map_fn) (struct dm_target *ti, struct bio *bio,
			  union map_info *map_context);
typedef int (*dm_map_request_fn) (struct dm_target *ti, struct request *clone,
				  union map_info *map_context);
=======
typedef int (*dm_map_fn) (struct dm_target *ti, struct bio *bio);
typedef int (*dm_clone_and_map_request_fn) (struct dm_target *ti,
					    struct request *rq,
					    union map_info *map_context,
					    struct request **clone);
typedef void (*dm_release_clone_request_fn) (struct request *clone,
					     union map_info *map_context);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Returns:
 * < 0 : error (currently ignored)
 * 0   : ended successfully
 * 1   : for some reason the io has still not completed (eg,
 *       multipath target might want to requeue a failed io).
 * 2   : The target wants to push back the io
 */
typedef int (*dm_endio_fn) (struct dm_target *ti,
<<<<<<< HEAD
			    struct bio *bio, int error,
			    union map_info *map_context);
typedef int (*dm_request_endio_fn) (struct dm_target *ti,
				    struct request *clone, int error,
				    union map_info *map_context);

typedef void (*dm_flush_fn) (struct dm_target *ti);
typedef void (*dm_presuspend_fn) (struct dm_target *ti);
=======
			    struct bio *bio, blk_status_t *error);
typedef int (*dm_request_endio_fn) (struct dm_target *ti,
				    struct request *clone, blk_status_t error,
				    union map_info *map_context);

typedef void (*dm_presuspend_fn) (struct dm_target *ti);
typedef void (*dm_presuspend_undo_fn) (struct dm_target *ti);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
typedef void (*dm_postsuspend_fn) (struct dm_target *ti);
typedef int (*dm_preresume_fn) (struct dm_target *ti);
typedef void (*dm_resume_fn) (struct dm_target *ti);

typedef void (*dm_status_fn) (struct dm_target *ti, status_type_t status_type,
<<<<<<< HEAD
			      char *result, unsigned int maxlen);

typedef int (*dm_message_fn) (struct dm_target *ti, unsigned argc, char **argv);

typedef int (*dm_ioctl_fn) (struct dm_target *ti, unsigned int cmd,
			    unsigned long arg);

typedef int (*dm_merge_fn) (struct dm_target *ti, struct bvec_merge_data *bvm,
			    struct bio_vec *biovec, int max_size);

=======
			      unsigned int status_flags, char *result, unsigned int maxlen);

typedef int (*dm_message_fn) (struct dm_target *ti, unsigned int argc, char **argv,
			      char *result, unsigned int maxlen);

typedef int (*dm_prepare_ioctl_fn) (struct dm_target *ti, struct block_device **bdev);

#ifdef CONFIG_BLK_DEV_ZONED
typedef int (*dm_report_zones_fn) (struct dm_target *ti,
				   struct dm_report_zones_args *args,
				   unsigned int nr_zones);
#else
/*
 * Define dm_report_zones_fn so that targets can assign to NULL if
 * CONFIG_BLK_DEV_ZONED disabled. Otherwise each target needs to do
 * awkward #ifdefs in their target_type, etc.
 */
typedef int (*dm_report_zones_fn) (struct dm_target *dummy);
#endif

/*
 * These iteration functions are typically used to check (and combine)
 * properties of underlying devices.
 * E.g. Does at least one underlying device support flush?
 *      Does any underlying device not support WRITE_SAME?
 *
 * The callout function is called once for each contiguous section of
 * an underlying device.  State can be maintained in *data.
 * Return non-zero to stop iterating through any further devices.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
typedef int (*iterate_devices_callout_fn) (struct dm_target *ti,
					   struct dm_dev *dev,
					   sector_t start, sector_t len,
					   void *data);

<<<<<<< HEAD
=======
/*
 * This function must iterate through each section of device used by the
 * target until it encounters a non-zero return code, which it then returns.
 * Returns zero if no callout returned non-zero.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
typedef int (*dm_iterate_devices_fn) (struct dm_target *ti,
				      iterate_devices_callout_fn fn,
				      void *data);

typedef void (*dm_io_hints_fn) (struct dm_target *ti,
				struct queue_limits *limits);

/*
 * Returns:
 *    0: The target can handle the next I/O immediately.
 *    1: The target can't handle the next I/O immediately.
 */
typedef int (*dm_busy_fn) (struct dm_target *ti);

<<<<<<< HEAD
void dm_error(const char *message);

/*
 * Combine device limits.
 */
int dm_set_device_limits(struct dm_target *ti, struct dm_dev *dev,
			 sector_t start, sector_t len, void *data);

struct dm_dev {
	struct block_device *bdev;
	fmode_t mode;
=======
/*
 * Returns:
 *  < 0 : error
 * >= 0 : the number of bytes accessible at the address
 */
typedef long (*dm_dax_direct_access_fn) (struct dm_target *ti, pgoff_t pgoff,
		long nr_pages, enum dax_access_mode node, void **kaddr,
		pfn_t *pfn);
typedef int (*dm_dax_zero_page_range_fn)(struct dm_target *ti, pgoff_t pgoff,
		size_t nr_pages);

/*
 * Returns:
 * != 0 : number of bytes transferred
 * 0    : recovery write failed
 */
typedef size_t (*dm_dax_recovery_write_fn)(struct dm_target *ti, pgoff_t pgoff,
		void *addr, size_t bytes, struct iov_iter *i);

void dm_error(const char *message);

struct dm_dev {
	struct block_device *bdev;
	struct file *bdev_file;
	struct dax_device *dax_dev;
	blk_mode_t mode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char name[16];
};

/*
 * Constructors should call these functions to ensure destination devices
 * are opened/closed correctly.
 */
<<<<<<< HEAD
int dm_get_device(struct dm_target *ti, const char *path, fmode_t mode,
						 struct dm_dev **result);
=======
int dm_get_device(struct dm_target *ti, const char *path, blk_mode_t mode,
		  struct dm_dev **result);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void dm_put_device(struct dm_target *ti, struct dm_dev *d);

/*
 * Information about a target type
 */

struct target_type {
	uint64_t features;
	const char *name;
	struct module *module;
<<<<<<< HEAD
	unsigned version[3];
	dm_ctr_fn ctr;
	dm_dtr_fn dtr;
	dm_map_fn map;
	dm_map_request_fn map_rq;
	dm_endio_fn end_io;
	dm_request_endio_fn rq_end_io;
	dm_flush_fn flush;
	dm_presuspend_fn presuspend;
=======
	unsigned int version[3];
	dm_ctr_fn ctr;
	dm_dtr_fn dtr;
	dm_map_fn map;
	dm_clone_and_map_request_fn clone_and_map_rq;
	dm_release_clone_request_fn release_clone_rq;
	dm_endio_fn end_io;
	dm_request_endio_fn rq_end_io;
	dm_presuspend_fn presuspend;
	dm_presuspend_undo_fn presuspend_undo;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dm_postsuspend_fn postsuspend;
	dm_preresume_fn preresume;
	dm_resume_fn resume;
	dm_status_fn status;
	dm_message_fn message;
<<<<<<< HEAD
	dm_ioctl_fn ioctl;
	dm_merge_fn merge;
	dm_busy_fn busy;
	dm_iterate_devices_fn iterate_devices;
	dm_io_hints_fn io_hints;
=======
	dm_prepare_ioctl_fn prepare_ioctl;
	dm_report_zones_fn report_zones;
	dm_busy_fn busy;
	dm_iterate_devices_fn iterate_devices;
	dm_io_hints_fn io_hints;
	dm_dax_direct_access_fn direct_access;
	dm_dax_zero_page_range_fn dax_zero_page_range;
	dm_dax_recovery_write_fn dax_recovery_write;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* For internal device-mapper use. */
	struct list_head list;
};

/*
 * Target features
 */

/*
 * Any table that contains an instance of this target must have only one.
 */
#define DM_TARGET_SINGLETON		0x00000001
#define dm_target_needs_singleton(type)	((type)->features & DM_TARGET_SINGLETON)

/*
 * Indicates that a target does not support read-only devices.
 */
#define DM_TARGET_ALWAYS_WRITEABLE	0x00000002
#define dm_target_always_writeable(type) \
		((type)->features & DM_TARGET_ALWAYS_WRITEABLE)

/*
 * Any device that contains a table with an instance of this target may never
 * have tables containing any different target type.
 */
#define DM_TARGET_IMMUTABLE		0x00000004
#define dm_target_is_immutable(type)	((type)->features & DM_TARGET_IMMUTABLE)

<<<<<<< HEAD
=======
/*
 * Indicates that a target may replace any target; even immutable targets.
 * .map, .map_rq, .clone_and_map_rq and .release_clone_rq are all defined.
 */
#define DM_TARGET_WILDCARD		0x00000008
#define dm_target_is_wildcard(type)	((type)->features & DM_TARGET_WILDCARD)

/*
 * A target implements own bio data integrity.
 */
#define DM_TARGET_INTEGRITY		0x00000010
#define dm_target_has_integrity(type)	((type)->features & DM_TARGET_INTEGRITY)

/*
 * A target passes integrity data to the lower device.
 */
#define DM_TARGET_PASSES_INTEGRITY	0x00000020
#define dm_target_passes_integrity(type) ((type)->features & DM_TARGET_PASSES_INTEGRITY)

/*
 * Indicates support for zoned block devices:
 * - DM_TARGET_ZONED_HM: the target also supports host-managed zoned
 *   block devices but does not support combining different zoned models.
 * - DM_TARGET_MIXED_ZONED_MODEL: the target supports combining multiple
 *   devices with different zoned models.
 */
#ifdef CONFIG_BLK_DEV_ZONED
#define DM_TARGET_ZONED_HM		0x00000040
#define dm_target_supports_zoned_hm(type) ((type)->features & DM_TARGET_ZONED_HM)
#else
#define DM_TARGET_ZONED_HM		0x00000000
#define dm_target_supports_zoned_hm(type) (false)
#endif

/*
 * A target handles REQ_NOWAIT
 */
#define DM_TARGET_NOWAIT		0x00000080
#define dm_target_supports_nowait(type) ((type)->features & DM_TARGET_NOWAIT)

/*
 * A target supports passing through inline crypto support.
 */
#define DM_TARGET_PASSES_CRYPTO		0x00000100
#define dm_target_passes_crypto(type) ((type)->features & DM_TARGET_PASSES_CRYPTO)

#ifdef CONFIG_BLK_DEV_ZONED
#define DM_TARGET_MIXED_ZONED_MODEL	0x00000200
#define dm_target_supports_mixed_zoned_model(type) \
	((type)->features & DM_TARGET_MIXED_ZONED_MODEL)
#else
#define DM_TARGET_MIXED_ZONED_MODEL	0x00000000
#define dm_target_supports_mixed_zoned_model(type) (false)
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct dm_target {
	struct dm_table *table;
	struct target_type *type;

	/* target limits */
	sector_t begin;
	sector_t len;

<<<<<<< HEAD
	/* Always a power of 2 */
	sector_t split_io;

	/*
	 * A number of zero-length barrier requests that will be submitted
	 * to the target for the purpose of flushing cache.
	 *
	 * The request number will be placed in union map_info->target_request_nr.
	 * It is a responsibility of the target driver to remap these requests
	 * to the real underlying devices.
	 */
	unsigned num_flush_requests;

	/*
	 * The number of discard requests that will be submitted to the
	 * target.  map_info->request_nr is used just like num_flush_requests.
	 */
	unsigned num_discard_requests;
=======
	/* If non-zero, maximum size of I/O submitted to a target. */
	uint32_t max_io_len;

	/*
	 * A number of zero-length barrier bios that will be submitted
	 * to the target for the purpose of flushing cache.
	 *
	 * The bio number can be accessed with dm_bio_get_target_bio_nr.
	 * It is a responsibility of the target driver to remap these bios
	 * to the real underlying devices.
	 */
	unsigned int num_flush_bios;

	/*
	 * The number of discard bios that will be submitted to the target.
	 * The bio number can be accessed with dm_bio_get_target_bio_nr.
	 */
	unsigned int num_discard_bios;

	/*
	 * The number of secure erase bios that will be submitted to the target.
	 * The bio number can be accessed with dm_bio_get_target_bio_nr.
	 */
	unsigned int num_secure_erase_bios;

	/*
	 * The number of WRITE ZEROES bios that will be submitted to the target.
	 * The bio number can be accessed with dm_bio_get_target_bio_nr.
	 */
	unsigned int num_write_zeroes_bios;

	/*
	 * The minimum number of extra bytes allocated in each io for the
	 * target to use.
	 */
	unsigned int per_io_data_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* target specific data */
	void *private;

	/* Used to provide an error string from the ctr */
	char *error;

	/*
<<<<<<< HEAD
	 * Set if this target needs to receive discards regardless of
	 * whether or not its underlying devices have support.
	 */
	unsigned discards_supported:1;

	/*
	 * Set if this target does not return zeroes on discarded blocks.
	 */
	unsigned discard_zeroes_data_unsupported:1;
};

/* Each target can link one of these into the table */
struct dm_target_callbacks {
	struct list_head list;
	int (*congested_fn) (struct dm_target_callbacks *, int);
};
=======
	 * Set if this target needs to receive flushes regardless of
	 * whether or not its underlying devices have support.
	 */
	bool flush_supported:1;

	/*
	 * Set if this target needs to receive discards regardless of
	 * whether or not its underlying devices have support.
	 */
	bool discards_supported:1;

	/*
	 * Set if this target requires that discards be split on
	 * 'max_discard_sectors' boundaries.
	 */
	bool max_discard_granularity:1;

	/*
	 * Set if this target requires that secure_erases be split on
	 * 'max_secure_erase_sectors' boundaries.
	 */
	bool max_secure_erase_granularity:1;

	/*
	 * Set if this target requires that write_zeroes be split on
	 * 'max_write_zeroes_sectors' boundaries.
	 */
	bool max_write_zeroes_granularity:1;

	/*
	 * Set if we need to limit the number of in-flight bios when swapping.
	 */
	bool limit_swap_bios:1;

	/*
	 * Set if this target implements a zoned device and needs emulation of
	 * zone append operations using regular writes.
	 */
	bool emulate_zone_append:1;

	/*
	 * Set if the target will submit IO using dm_submit_bio_remap()
	 * after returning DM_MAPIO_SUBMITTED from its map function.
	 */
	bool accounts_remapped_io:1;

	/*
	 * Set if the target will submit the DM bio without first calling
	 * bio_set_dev(). NOTE: ideally a target should _not_ need this.
	 */
	bool needs_bio_set_dev:1;
};

void *dm_per_bio_data(struct bio *bio, size_t data_size);
struct bio *dm_bio_from_per_bio_data(void *data, size_t data_size);
unsigned int dm_bio_get_target_bio_nr(const struct bio *bio);

u64 dm_start_time_ns_from_clone(struct bio *bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int dm_register_target(struct target_type *t);
void dm_unregister_target(struct target_type *t);

/*
 * Target argument parsing.
 */
struct dm_arg_set {
<<<<<<< HEAD
	unsigned argc;
=======
	unsigned int argc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char **argv;
};

/*
 * The minimum and maximum value of a numeric argument, together with
 * the error message to use if the number is found to be outside that range.
 */
struct dm_arg {
<<<<<<< HEAD
	unsigned min;
	unsigned max;
=======
	unsigned int min;
	unsigned int max;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *error;
};

/*
 * Validate the next argument, either returning it as *value or, if invalid,
 * returning -EINVAL and setting *error.
 */
<<<<<<< HEAD
int dm_read_arg(struct dm_arg *arg, struct dm_arg_set *arg_set,
		unsigned *value, char **error);
=======
int dm_read_arg(const struct dm_arg *arg, struct dm_arg_set *arg_set,
		unsigned int *value, char **error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Process the next argument as the start of a group containing between
 * arg->min and arg->max further arguments. Either return the size as
 * *num_args or, if invalid, return -EINVAL and set *error.
 */
<<<<<<< HEAD
int dm_read_arg_group(struct dm_arg *arg, struct dm_arg_set *arg_set,
		      unsigned *num_args, char **error);
=======
int dm_read_arg_group(const struct dm_arg *arg, struct dm_arg_set *arg_set,
		      unsigned int *num_args, char **error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Return the current argument and shift to the next.
 */
const char *dm_shift_arg(struct dm_arg_set *as);

/*
 * Move through num_args arguments.
 */
<<<<<<< HEAD
void dm_consume_args(struct dm_arg_set *as, unsigned num_args);

/*-----------------------------------------------------------------
 * Functions for creating and manipulating mapped devices.
 * Drop the reference with dm_put when you finish with the object.
 *---------------------------------------------------------------*/
=======
void dm_consume_args(struct dm_arg_set *as, unsigned int num_args);

/*
 *----------------------------------------------------------------
 * Functions for creating and manipulating mapped devices.
 * Drop the reference with dm_put when you finish with the object.
 *----------------------------------------------------------------
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * DM_ANY_MINOR chooses the next available minor number.
 */
#define DM_ANY_MINOR (-1)
int dm_create(int minor, struct mapped_device **md);

/*
 * Reference counting for md.
 */
struct mapped_device *dm_get_md(dev_t dev);
void dm_get(struct mapped_device *md);
<<<<<<< HEAD
=======
int dm_hold(struct mapped_device *md);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void dm_put(struct mapped_device *md);

/*
 * An arbitrary pointer may be stored alongside a mapped device.
 */
void dm_set_mdptr(struct mapped_device *md, void *ptr);
void *dm_get_mdptr(struct mapped_device *md);

/*
 * A device can still be used while suspended, but I/O is deferred.
 */
<<<<<<< HEAD
int dm_suspend(struct mapped_device *md, unsigned suspend_flags);
=======
int dm_suspend(struct mapped_device *md, unsigned int suspend_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int dm_resume(struct mapped_device *md);

/*
 * Event functions.
 */
uint32_t dm_get_event_nr(struct mapped_device *md);
int dm_wait_event(struct mapped_device *md, int event_nr);
uint32_t dm_next_uevent_seq(struct mapped_device *md);
void dm_uevent_add(struct mapped_device *md, struct list_head *elist);

/*
 * Info functions.
 */
const char *dm_device_name(struct mapped_device *md);
int dm_copy_name_and_uuid(struct mapped_device *md, char *name, char *uuid);
struct gendisk *dm_disk(struct mapped_device *md);
int dm_suspended(struct dm_target *ti);
<<<<<<< HEAD
int dm_noflush_suspending(struct dm_target *ti);
union map_info *dm_get_mapinfo(struct bio *bio);
union map_info *dm_get_rq_mapinfo(struct request *rq);

=======
int dm_post_suspending(struct dm_target *ti);
int dm_noflush_suspending(struct dm_target *ti);
void dm_accept_partial_bio(struct bio *bio, unsigned int n_sectors);
void dm_submit_bio_remap(struct bio *clone, struct bio *tgt_clone);
union map_info *dm_get_rq_mapinfo(struct request *rq);

#ifdef CONFIG_BLK_DEV_ZONED
struct dm_report_zones_args {
	struct dm_target *tgt;
	sector_t next_sector;

	void *orig_data;
	report_zones_cb orig_cb;
	unsigned int zone_idx;

	/* must be filled by ->report_zones before calling dm_report_zones_cb */
	sector_t start;
};
int dm_report_zones(struct block_device *bdev, sector_t start, sector_t sector,
		    struct dm_report_zones_args *args, unsigned int nr_zones);
#endif /* CONFIG_BLK_DEV_ZONED */

/*
 * Device mapper functions to parse and create devices specified by the
 * parameter "dm-mod.create="
 */
int __init dm_early_create(struct dm_ioctl *dmi,
			   struct dm_target_spec **spec_array,
			   char **target_params_array);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Geometry functions.
 */
int dm_get_geometry(struct mapped_device *md, struct hd_geometry *geo);
int dm_set_geometry(struct mapped_device *md, struct hd_geometry *geo);

<<<<<<< HEAD

/*-----------------------------------------------------------------
 * Functions for manipulating device-mapper tables.
 *---------------------------------------------------------------*/
=======
/*
 *---------------------------------------------------------------
 * Functions for manipulating device-mapper tables.
 *---------------------------------------------------------------
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * First create an empty table.
 */
<<<<<<< HEAD
int dm_table_create(struct dm_table **result, fmode_t mode,
		    unsigned num_targets, struct mapped_device *md);
=======
int dm_table_create(struct dm_table **result, blk_mode_t mode,
		    unsigned int num_targets, struct mapped_device *md);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Then call this once for each target.
 */
int dm_table_add_target(struct dm_table *t, const char *type,
			sector_t start, sector_t len, char *params);

/*
<<<<<<< HEAD
 * Target_ctr should call this if it needs to add any callbacks.
 */
void dm_table_add_target_callbacks(struct dm_table *t, struct dm_target_callbacks *cb);
=======
 * Target can use this to set the table's type.
 * Can only ever be called from a target's ctr.
 * Useful for "hybrid" target (supports both bio-based
 * and request-based).
 */
void dm_table_set_type(struct dm_table *t, enum dm_queue_mode type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Finally call this to make the table ready for use.
 */
int dm_table_complete(struct dm_table *t);

/*
<<<<<<< HEAD
 * Table reference counting.
 */
struct dm_table *dm_get_live_table(struct mapped_device *md);
void dm_table_get(struct dm_table *t);
void dm_table_put(struct dm_table *t);
=======
 * Destroy the table when finished.
 */
void dm_table_destroy(struct dm_table *t);

/*
 * Target may require that it is never sent I/O larger than len.
 */
int __must_check dm_set_target_max_io_len(struct dm_target *ti, sector_t len);

/*
 * Table reference counting.
 */
struct dm_table *dm_get_live_table(struct mapped_device *md, int *srcu_idx);
void dm_put_live_table(struct mapped_device *md, int srcu_idx);
void dm_sync_table(struct mapped_device *md);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Queries
 */
sector_t dm_table_get_size(struct dm_table *t);
<<<<<<< HEAD
unsigned int dm_table_get_num_targets(struct dm_table *t);
fmode_t dm_table_get_mode(struct dm_table *t);
struct mapped_device *dm_table_get_md(struct dm_table *t);
=======
blk_mode_t dm_table_get_mode(struct dm_table *t);
struct mapped_device *dm_table_get_md(struct dm_table *t);
const char *dm_table_device_name(struct dm_table *t);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Trigger an event.
 */
void dm_table_event(struct dm_table *t);

/*
<<<<<<< HEAD
=======
 * Run the queue for request-based targets.
 */
void dm_table_run_md_queue_async(struct dm_table *t);

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * The device must be suspended before calling this method.
 * Returns the previous table, which the caller must destroy.
 */
struct dm_table *dm_swap_table(struct mapped_device *md,
			       struct dm_table *t);

/*
<<<<<<< HEAD
 * A wrapper around vmalloc.
 */
void *dm_vcalloc(unsigned long nmemb, unsigned long elem_size);

/*-----------------------------------------------------------------
 * Macros.
 *---------------------------------------------------------------*/
#define DM_NAME "device-mapper"

#ifdef CONFIG_PRINTK
extern struct ratelimit_state dm_ratelimit_state;

#define dm_ratelimit()	__ratelimit(&dm_ratelimit_state)
#else
#define dm_ratelimit()	0
#endif

#define DMCRIT(f, arg...) \
	printk(KERN_CRIT DM_NAME ": " DM_MSG_PREFIX ": " f "\n", ## arg)

#define DMERR(f, arg...) \
	printk(KERN_ERR DM_NAME ": " DM_MSG_PREFIX ": " f "\n", ## arg)
#define DMERR_LIMIT(f, arg...) \
	do { \
		if (dm_ratelimit())	\
			printk(KERN_ERR DM_NAME ": " DM_MSG_PREFIX ": " \
			       f "\n", ## arg); \
	} while (0)

#define DMWARN(f, arg...) \
	printk(KERN_WARNING DM_NAME ": " DM_MSG_PREFIX ": " f "\n", ## arg)
#define DMWARN_LIMIT(f, arg...) \
	do { \
		if (dm_ratelimit())	\
			printk(KERN_WARNING DM_NAME ": " DM_MSG_PREFIX ": " \
			       f "\n", ## arg); \
	} while (0)

#define DMINFO(f, arg...) \
	printk(KERN_INFO DM_NAME ": " DM_MSG_PREFIX ": " f "\n", ## arg)
#define DMINFO_LIMIT(f, arg...) \
	do { \
		if (dm_ratelimit())	\
			printk(KERN_INFO DM_NAME ": " DM_MSG_PREFIX ": " f \
			       "\n", ## arg); \
	} while (0)

#ifdef CONFIG_DM_DEBUG
#  define DMDEBUG(f, arg...) \
	printk(KERN_DEBUG DM_NAME ": " DM_MSG_PREFIX " DEBUG: " f "\n", ## arg)
#  define DMDEBUG_LIMIT(f, arg...) \
	do { \
		if (dm_ratelimit())	\
			printk(KERN_DEBUG DM_NAME ": " DM_MSG_PREFIX ": " f \
			       "\n", ## arg); \
	} while (0)
#else
#  define DMDEBUG(f, arg...) do {} while (0)
#  define DMDEBUG_LIMIT(f, arg...) do {} while (0)
#endif

#define DMEMIT(x...) sz += ((sz >= maxlen) ? \
			  0 : scnprintf(result + sz, maxlen - sz, x))

#define SECTOR_SHIFT 9
=======
 * Table blk_crypto_profile functions
 */
void dm_destroy_crypto_profile(struct blk_crypto_profile *profile);

/*
 *---------------------------------------------------------------
 * Macros.
 *---------------------------------------------------------------
 */
#define DM_NAME "device-mapper"

#define DM_FMT(fmt) DM_NAME ": " DM_MSG_PREFIX ": " fmt "\n"

#define DMCRIT(fmt, ...) pr_crit(DM_FMT(fmt), ##__VA_ARGS__)

#define DMERR(fmt, ...) pr_err(DM_FMT(fmt), ##__VA_ARGS__)
#define DMERR_LIMIT(fmt, ...) pr_err_ratelimited(DM_FMT(fmt), ##__VA_ARGS__)
#define DMWARN(fmt, ...) pr_warn(DM_FMT(fmt), ##__VA_ARGS__)
#define DMWARN_LIMIT(fmt, ...) pr_warn_ratelimited(DM_FMT(fmt), ##__VA_ARGS__)
#define DMINFO(fmt, ...) pr_info(DM_FMT(fmt), ##__VA_ARGS__)
#define DMINFO_LIMIT(fmt, ...) pr_info_ratelimited(DM_FMT(fmt), ##__VA_ARGS__)

#define DMDEBUG(fmt, ...) pr_debug(DM_FMT(fmt), ##__VA_ARGS__)
#define DMDEBUG_LIMIT(fmt, ...) pr_debug_ratelimited(DM_FMT(fmt), ##__VA_ARGS__)

#define DMEMIT(x...) (sz += ((sz >= maxlen) ? 0 : scnprintf(result + sz, maxlen - sz, x)))

#define DMEMIT_TARGET_NAME_VERSION(y) \
		DMEMIT("target_name=%s,target_version=%u.%u.%u", \
		       (y)->name, (y)->version[0], (y)->version[1], (y)->version[2])

/**
 * module_dm() - Helper macro for DM targets that don't do anything
 * special in their module_init and module_exit.
 * Each module may only use this macro once, and calling it replaces
 * module_init() and module_exit().
 *
 * @name: DM target's name
 */
#define module_dm(name) \
static int __init dm_##name##_init(void) \
{ \
	return dm_register_target(&(name##_target)); \
} \
module_init(dm_##name##_init) \
static void __exit dm_##name##_exit(void) \
{ \
	dm_unregister_target(&(name##_target)); \
} \
module_exit(dm_##name##_exit)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Definitions of return values from target end_io function.
 */
<<<<<<< HEAD
#define DM_ENDIO_INCOMPLETE	1
#define DM_ENDIO_REQUEUE	2
=======
#define DM_ENDIO_DONE		0
#define DM_ENDIO_INCOMPLETE	1
#define DM_ENDIO_REQUEUE	2
#define DM_ENDIO_DELAY_REQUEUE	3
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Definitions of return values from target map function.
 */
#define DM_MAPIO_SUBMITTED	0
#define DM_MAPIO_REMAPPED	1
#define DM_MAPIO_REQUEUE	DM_ENDIO_REQUEUE
<<<<<<< HEAD
=======
#define DM_MAPIO_DELAY_REQUEUE	DM_ENDIO_DELAY_REQUEUE
#define DM_MAPIO_KILL		4

#define dm_sector_div64(x, y)( \
{ \
	u64 _res; \
	(x) = div64_u64_rem(x, y, &_res); \
	_res; \
} \
)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Ceiling(n / sz)
 */
#define dm_div_up(n, sz) (((n) + (sz) - 1) / (sz))

#define dm_sector_div_up(n, sz) ( \
{ \
	sector_t _r = ((n) + (sz) - 1); \
	sector_div(_r, (sz)); \
	_r; \
} \
)

/*
 * ceiling(n / size) * size
 */
#define dm_round_up(n, sz) (dm_div_up((n), (sz)) * (sz))

<<<<<<< HEAD
#define dm_array_too_big(fixed, obj, num) \
	((num) > (UINT_MAX - (fixed)) / (obj))

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Sector offset taken relative to the start of the target instead of
 * relative to the start of the device.
 */
#define dm_target_offset(ti, sector) ((sector) - (ti)->begin)

<<<<<<< HEAD
static inline sector_t to_sector(unsigned long n)
=======
static inline sector_t to_sector(unsigned long long n)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return (n >> SECTOR_SHIFT);
}

static inline unsigned long to_bytes(sector_t n)
{
	return (n << SECTOR_SHIFT);
}

<<<<<<< HEAD
/*-----------------------------------------------------------------
 * Helper for block layer and dm core operations
 *---------------------------------------------------------------*/
void dm_dispatch_request(struct request *rq);
void dm_requeue_unmapped_request(struct request *rq);
void dm_kill_unmapped_request(struct request *rq, int error);
int dm_underlying_device_busy(struct request_queue *q);
void dm_end_request(struct request *clone, int error);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* _LINUX_DEVICE_MAPPER_H */
