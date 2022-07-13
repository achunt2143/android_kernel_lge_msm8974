<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _FS_CEPH_TYPES_H
#define _FS_CEPH_TYPES_H

/* needed before including ceph_fs.h */
#include <linux/in.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/string.h>

<<<<<<< HEAD
#include "ceph_fs.h"
#include "ceph_frag.h"
#include "ceph_hash.h"
=======
#include <linux/ceph/ceph_fs.h>
#include <linux/ceph/ceph_frag.h>
#include <linux/ceph/ceph_hash.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Identify inodes by both their ino AND snapshot id (a u64).
 */
struct ceph_vino {
	u64 ino;
	u64 snap;
};


/* context for the caps reservation mechanism */
struct ceph_cap_reservation {
	int count;
<<<<<<< HEAD
=======
	int used;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};


#endif
