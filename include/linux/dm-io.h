<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2003 Sistina Software
 * Copyright (C) 2004 - 2008 Red Hat, Inc. All rights reserved.
 *
 * Device-Mapper low-level I/O.
 *
 * This file is released under the GPL.
 */

#ifndef _LINUX_DM_IO_H
#define _LINUX_DM_IO_H

#ifdef __KERNEL__

#include <linux/types.h>
<<<<<<< HEAD
=======
#include <linux/blk_types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct dm_io_region {
	struct block_device *bdev;
	sector_t sector;
	sector_t count;		/* If this is zero the region is ignored. */
};

struct page_list {
	struct page_list *next;
	struct page *page;
};

<<<<<<< HEAD
typedef void (*io_notify_fn)(unsigned long error, void *context);

enum dm_io_mem_type {
	DM_IO_PAGE_LIST,/* Page list */
	DM_IO_BVEC,	/* Bio vector */
=======
typedef void (*io_notify_fn)(unsigned int long error, void *context);

enum dm_io_mem_type {
	DM_IO_PAGE_LIST,/* Page list */
	DM_IO_BIO,	/* Bio vector */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	DM_IO_VMA,	/* Virtual memory area */
	DM_IO_KMEM,	/* Kernel memory */
};

struct dm_io_memory {
	enum dm_io_mem_type type;

<<<<<<< HEAD
	unsigned offset;

	union {
		struct page_list *pl;
		struct bio_vec *bvec;
=======
	unsigned int offset;

	union {
		struct page_list *pl;
		struct bio *bio;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		void *vma;
		void *addr;
	} ptr;
};

struct dm_io_notify {
	io_notify_fn fn;	/* Callback for asynchronous requests */
	void *context;		/* Passed to callback */
};

/*
 * IO request structure
 */
struct dm_io_client;
struct dm_io_request {
<<<<<<< HEAD
	int bi_rw;			/* READ|WRITE - not READA */
=======
	blk_opf_t	    bi_opf;	/* Request type and flags */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct dm_io_memory mem;	/* Memory to use for io */
	struct dm_io_notify notify;	/* Synchronous if notify.fn is NULL */
	struct dm_io_client *client;	/* Client memory handler */
};

/*
 * For async io calls, users can alternatively use the dm_io() function below
 * and dm_io_client_create() to create private mempools for the client.
 *
 * Create/destroy may block.
 */
struct dm_io_client *dm_io_client_create(void);
void dm_io_client_destroy(struct dm_io_client *client);

/*
 * IO interface using private per-client pools.
 * Each bit in the optional 'sync_error_bits' bitset indicates whether an
 * error occurred doing io to the corresponding region.
 */
<<<<<<< HEAD
int dm_io(struct dm_io_request *io_req, unsigned num_regions,
	  struct dm_io_region *region, unsigned long *sync_error_bits);
=======
int dm_io(struct dm_io_request *io_req, unsigned int num_regions,
	  struct dm_io_region *region, unsigned int long *sync_error_bits,
	  unsigned short ioprio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* __KERNEL__ */
#endif	/* _LINUX_DM_IO_H */
