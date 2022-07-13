<<<<<<< HEAD
/*
 * Copyright (C) 2003 Christophe Saout <christophe@saout.de>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2003 Jana Saout <jana@saout.de>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This file is released under the GPL.
 */

#include <linux/device-mapper.h>

#include <linux/module.h>
#include <linux/init.h>
#include <linux/bio.h>

#define DM_MSG_PREFIX "zero"

/*
 * Construct a dummy mapping that only returns zeros
 */
static int zero_ctr(struct dm_target *ti, unsigned int argc, char **argv)
{
	if (argc != 0) {
		ti->error = "No arguments required";
		return -EINVAL;
	}

	/*
	 * Silently drop discards, avoiding -EOPNOTSUPP.
	 */
<<<<<<< HEAD
	ti->num_discard_requests = 1;
=======
	ti->num_discard_bios = 1;
	ti->discards_supported = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/*
 * Return zeros only on reads
 */
<<<<<<< HEAD
static int zero_map(struct dm_target *ti, struct bio *bio,
		      union map_info *map_context)
{
	switch(bio_rw(bio)) {
	case READ:
		zero_fill_bio(bio);
		break;
	case READA:
		/* readahead of null bytes only wastes buffer cache */
		return -EIO;
	case WRITE:
		/* writes get silently dropped */
		break;
	}

	bio_endio(bio, 0);
=======
static int zero_map(struct dm_target *ti, struct bio *bio)
{
	switch (bio_op(bio)) {
	case REQ_OP_READ:
		if (bio->bi_opf & REQ_RAHEAD) {
			/* readahead of null bytes only wastes buffer cache */
			return DM_MAPIO_KILL;
		}
		zero_fill_bio(bio);
		break;
	case REQ_OP_WRITE:
	case REQ_OP_DISCARD:
		/* writes get silently dropped */
		break;
	default:
		return DM_MAPIO_KILL;
	}

	bio_endio(bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* accepted bio, don't make new request */
	return DM_MAPIO_SUBMITTED;
}

<<<<<<< HEAD
static struct target_type zero_target = {
	.name   = "zero",
	.version = {1, 0, 0},
	.module = THIS_MODULE,
	.ctr    = zero_ctr,
	.map    = zero_map,
};

static int __init dm_zero_init(void)
{
	int r = dm_register_target(&zero_target);

	if (r < 0)
		DMERR("register failed %d", r);

	return r;
}

static void __exit dm_zero_exit(void)
{
	dm_unregister_target(&zero_target);
}

module_init(dm_zero_init)
module_exit(dm_zero_exit)

MODULE_AUTHOR("Christophe Saout <christophe@saout.de>");
=======
static void zero_io_hints(struct dm_target *ti, struct queue_limits *limits)
{
	limits->max_discard_sectors = UINT_MAX;
	limits->max_hw_discard_sectors = UINT_MAX;
	limits->discard_granularity = 512;
}

static struct target_type zero_target = {
	.name   = "zero",
	.version = {1, 2, 0},
	.features = DM_TARGET_NOWAIT,
	.module = THIS_MODULE,
	.ctr    = zero_ctr,
	.map    = zero_map,
	.io_hints = zero_io_hints,
};
module_dm(zero);

MODULE_AUTHOR("Jana Saout <jana@saout.de>");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_DESCRIPTION(DM_NAME " dummy target returning zeros");
MODULE_LICENSE("GPL");
