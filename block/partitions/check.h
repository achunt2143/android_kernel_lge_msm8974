<<<<<<< HEAD
#include <linux/pagemap.h>
#include <linux/blkdev.h>
#include <linux/genhd.h>
=======
/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/pagemap.h>
#include <linux/blkdev.h>
#include "../blk.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * add_gd_partition adds a partitions details to the devices partition
 * description.
 */
struct parsed_partitions {
<<<<<<< HEAD
	struct block_device *bdev;
=======
	struct gendisk *disk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char name[BDEVNAME_SIZE];
	struct {
		sector_t from;
		sector_t size;
		int flags;
		bool has_info;
		struct partition_meta_info info;
<<<<<<< HEAD
	} parts[DISK_MAX_PARTS];
=======
	} *parts;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int next;
	int limit;
	bool access_beyond_eod;
	char *pp_buf;
};

<<<<<<< HEAD
struct parsed_partitions *
check_partition(struct gendisk *, struct block_device *);

static inline void *read_part_sector(struct parsed_partitions *state,
				     sector_t n, Sector *p)
{
	if (n >= get_capacity(state->bdev->bd_disk)) {
		state->access_beyond_eod = true;
		return NULL;
	}
	return read_dev_sector(state->bdev, n, p);
=======
typedef struct {
	struct folio *v;
} Sector;

void *read_part_sector(struct parsed_partitions *state, sector_t n, Sector *p);
static inline void put_dev_sector(Sector p)
{
	folio_put(p.v);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void
put_partition(struct parsed_partitions *p, int n, sector_t from, sector_t size)
{
	if (n < p->limit) {
		char tmp[1 + BDEVNAME_SIZE + 10 + 1];

		p->parts[n].from = from;
		p->parts[n].size = size;
		snprintf(tmp, sizeof(tmp), " %s%d", p->name, n);
		strlcat(p->pp_buf, tmp, PAGE_SIZE);
	}
}

<<<<<<< HEAD
extern int warn_no_part;

=======
/* detection routines go here in alphabetical order: */
int adfspart_check_ADFS(struct parsed_partitions *state);
int adfspart_check_CUMANA(struct parsed_partitions *state);
int adfspart_check_EESOX(struct parsed_partitions *state);
int adfspart_check_ICS(struct parsed_partitions *state);
int adfspart_check_POWERTEC(struct parsed_partitions *state);
int aix_partition(struct parsed_partitions *state);
int amiga_partition(struct parsed_partitions *state);
int atari_partition(struct parsed_partitions *state);
int cmdline_partition(struct parsed_partitions *state);
int efi_partition(struct parsed_partitions *state);
int ibm_partition(struct parsed_partitions *);
int karma_partition(struct parsed_partitions *state);
int ldm_partition(struct parsed_partitions *state);
int mac_partition(struct parsed_partitions *state);
int msdos_partition(struct parsed_partitions *state);
int osf_partition(struct parsed_partitions *state);
int sgi_partition(struct parsed_partitions *state);
int sun_partition(struct parsed_partitions *state);
int sysv68_partition(struct parsed_partitions *state);
int ultrix_partition(struct parsed_partitions *state);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
