<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef BTRFS_EXPORT_H
#define BTRFS_EXPORT_H

#include <linux/exportfs.h>
<<<<<<< HEAD
=======
#include <linux/types.h>

struct dentry;
struct super_block;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern const struct export_operations btrfs_export_ops;

struct btrfs_fid {
	u64 objectid;
	u64 root_objectid;
	u32 gen;

	u64 parent_objectid;
	u32 parent_gen;

	u64 parent_root_objectid;
} __attribute__ ((packed));

<<<<<<< HEAD
=======
struct dentry *btrfs_get_dentry(struct super_block *sb, u64 objectid,
				u64 root_objectid, u64 generation);
struct dentry *btrfs_get_parent(struct dentry *child);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
