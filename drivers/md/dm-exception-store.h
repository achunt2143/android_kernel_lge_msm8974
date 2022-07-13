<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2001-2002 Sistina Software (UK) Limited.
 * Copyright (C) 2008 Red Hat, Inc. All rights reserved.
 *
 * Device-mapper snapshot exception store.
 *
 * This file is released under the GPL.
 */

#ifndef _LINUX_DM_EXCEPTION_STORE
#define _LINUX_DM_EXCEPTION_STORE

#include <linux/blkdev.h>
<<<<<<< HEAD
=======
#include <linux/list_bl.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/device-mapper.h>

/*
 * The snapshot code deals with largish chunks of the disk at a
 * time. Typically 32k - 512k.
 */
typedef sector_t chunk_t;

/*
 * An exception is used where an old chunk of data has been
 * replaced by a new one.
 * If chunk_t is 64 bits in size, the top 8 bits of new_chunk hold the number
 * of chunks that follow contiguously.  Remaining bits hold the number of the
 * chunk within the device.
 */
struct dm_exception {
<<<<<<< HEAD
	struct list_head hash_list;
=======
	struct hlist_bl_node hash_list;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	chunk_t old_chunk;
	chunk_t new_chunk;
};

/*
 * Abstraction to handle the meta/layout of exception stores (the
 * COW device).
 */
struct dm_exception_store;
struct dm_exception_store_type {
	const char *name;
	struct module *module;

<<<<<<< HEAD
	int (*ctr) (struct dm_exception_store *store,
		    unsigned argc, char **argv);
=======
	int (*ctr)(struct dm_exception_store *store, char *options);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Destroys this object when you've finished with it.
	 */
<<<<<<< HEAD
	void (*dtr) (struct dm_exception_store *store);
=======
	void (*dtr)(struct dm_exception_store *store);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The target shouldn't read the COW device until this is
	 * called.  As exceptions are read from the COW, they are
	 * reported back via the callback.
	 */
<<<<<<< HEAD
	int (*read_metadata) (struct dm_exception_store *store,
			      int (*callback)(void *callback_context,
					      chunk_t old, chunk_t new),
			      void *callback_context);
=======
	int (*read_metadata)(struct dm_exception_store *store,
			     int (*callback)(void *callback_context,
					     chunk_t old, chunk_t new),
			     void *callback_context);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Find somewhere to store the next exception.
	 */
<<<<<<< HEAD
	int (*prepare_exception) (struct dm_exception_store *store,
				  struct dm_exception *e);
=======
	int (*prepare_exception)(struct dm_exception_store *store,
				 struct dm_exception *e);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Update the metadata with this exception.
	 */
<<<<<<< HEAD
	void (*commit_exception) (struct dm_exception_store *store,
				  struct dm_exception *e,
				  void (*callback) (void *, int success),
				  void *callback_context);
=======
	void (*commit_exception)(struct dm_exception_store *store,
				 struct dm_exception *e, int valid,
				 void (*callback)(void *, int success),
				 void *callback_context);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Returns 0 if the exception store is empty.
	 *
	 * If there are exceptions still to be merged, sets
	 * *last_old_chunk and *last_new_chunk to the most recent
	 * still-to-be-merged chunk and returns the number of
	 * consecutive previous ones.
	 */
<<<<<<< HEAD
	int (*prepare_merge) (struct dm_exception_store *store,
			      chunk_t *last_old_chunk, chunk_t *last_new_chunk);
=======
	int (*prepare_merge)(struct dm_exception_store *store,
			     chunk_t *last_old_chunk, chunk_t *last_new_chunk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Clear the last n exceptions.
	 * nr_merged must be <= the value returned by prepare_merge.
	 */
<<<<<<< HEAD
	int (*commit_merge) (struct dm_exception_store *store, int nr_merged);
=======
	int (*commit_merge)(struct dm_exception_store *store, int nr_merged);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The snapshot is invalid, note this in the metadata.
	 */
<<<<<<< HEAD
	void (*drop_snapshot) (struct dm_exception_store *store);

	unsigned (*status) (struct dm_exception_store *store,
			    status_type_t status, char *result,
			    unsigned maxlen);
=======
	void (*drop_snapshot)(struct dm_exception_store *store);

	unsigned int (*status)(struct dm_exception_store *store,
			       status_type_t status, char *result,
			       unsigned int maxlen);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Return how full the snapshot is.
	 */
<<<<<<< HEAD
	void (*usage) (struct dm_exception_store *store,
		       sector_t *total_sectors, sector_t *sectors_allocated,
		       sector_t *metadata_sectors);
=======
	void (*usage)(struct dm_exception_store *store,
		      sector_t *total_sectors, sector_t *sectors_allocated,
		      sector_t *metadata_sectors);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* For internal device-mapper use only. */
	struct list_head list;
};

struct dm_snapshot;

struct dm_exception_store {
	struct dm_exception_store_type *type;
	struct dm_snapshot *snap;

	/* Size of data blocks saved - must be a power of 2 */
<<<<<<< HEAD
	unsigned chunk_size;
	unsigned chunk_mask;
	unsigned chunk_shift;

	void *context;
=======
	unsigned int chunk_size;
	unsigned int chunk_mask;
	unsigned int chunk_shift;

	void *context;

	bool userspace_supports_overflow;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Obtain the origin or cow device used by a given snapshot.
 */
struct dm_dev *dm_snap_origin(struct dm_snapshot *snap);
struct dm_dev *dm_snap_cow(struct dm_snapshot *snap);

/*
 * Funtions to manipulate consecutive chunks
 */
<<<<<<< HEAD
#  if defined(CONFIG_LBDAF) || (BITS_PER_LONG == 64)
#    define DM_CHUNK_CONSECUTIVE_BITS 8
#    define DM_CHUNK_NUMBER_BITS 56
=======
#define DM_CHUNK_CONSECUTIVE_BITS 8
#define DM_CHUNK_NUMBER_BITS 56
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline chunk_t dm_chunk_number(chunk_t chunk)
{
	return chunk & (chunk_t)((1ULL << DM_CHUNK_NUMBER_BITS) - 1ULL);
}

<<<<<<< HEAD
static inline unsigned dm_consecutive_chunk_count(struct dm_exception *e)
=======
static inline unsigned int dm_consecutive_chunk_count(struct dm_exception *e)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return e->new_chunk >> DM_CHUNK_NUMBER_BITS;
}

static inline void dm_consecutive_chunk_count_inc(struct dm_exception *e)
{
	e->new_chunk += (1ULL << DM_CHUNK_NUMBER_BITS);

	BUG_ON(!dm_consecutive_chunk_count(e));
}

static inline void dm_consecutive_chunk_count_dec(struct dm_exception *e)
{
	BUG_ON(!dm_consecutive_chunk_count(e));

	e->new_chunk -= (1ULL << DM_CHUNK_NUMBER_BITS);
}

<<<<<<< HEAD
#  else
#    define DM_CHUNK_CONSECUTIVE_BITS 0

static inline chunk_t dm_chunk_number(chunk_t chunk)
{
	return chunk;
}

static inline unsigned dm_consecutive_chunk_count(struct dm_exception *e)
{
	return 0;
}

static inline void dm_consecutive_chunk_count_inc(struct dm_exception *e)
{
}

static inline void dm_consecutive_chunk_count_dec(struct dm_exception *e)
{
}

#  endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Return the number of sectors in the device.
 */
static inline sector_t get_dev_size(struct block_device *bdev)
{
<<<<<<< HEAD
	return i_size_read(bdev->bd_inode) >> SECTOR_SHIFT;
=======
	return bdev_nr_sectors(bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline chunk_t sector_to_chunk(struct dm_exception_store *store,
				      sector_t sector)
{
	return sector >> store->chunk_shift;
}

int dm_exception_store_type_register(struct dm_exception_store_type *type);
int dm_exception_store_type_unregister(struct dm_exception_store_type *type);

int dm_exception_store_set_chunk_size(struct dm_exception_store *store,
<<<<<<< HEAD
				      unsigned chunk_size,
=======
				      unsigned int chunk_size,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				      char **error);

int dm_exception_store_create(struct dm_target *ti, int argc, char **argv,
			      struct dm_snapshot *snap,
<<<<<<< HEAD
			      unsigned *args_used,
=======
			      unsigned int *args_used,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			      struct dm_exception_store **store);
void dm_exception_store_destroy(struct dm_exception_store *store);

int dm_exception_store_init(void);
void dm_exception_store_exit(void);

/*
 * Two exception store implementations.
 */
int dm_persistent_snapshot_init(void);
void dm_persistent_snapshot_exit(void);

int dm_transient_snapshot_init(void);
void dm_transient_snapshot_exit(void);

#endif /* _LINUX_DM_EXCEPTION_STORE */
