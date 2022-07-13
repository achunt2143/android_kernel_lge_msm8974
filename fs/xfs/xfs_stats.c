<<<<<<< HEAD
/*
 * Copyright (c) 2000-2003,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "xfs.h"
#include <linux/proc_fs.h>

DEFINE_PER_CPU(struct xfsstats, xfsstats);

static int counter_val(int idx)
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2003,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#include "xfs.h"

struct xstats xfsstats;

static int counter_val(struct xfsstats __percpu *stats, int idx)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int val = 0, cpu;

	for_each_possible_cpu(cpu)
<<<<<<< HEAD
		val += *(((__u32 *)&per_cpu(xfsstats, cpu) + idx));
	return val;
}

static int xfs_stat_proc_show(struct seq_file *m, void *v)
{
	int		i, j;
	__uint64_t	xs_xstrat_bytes = 0;
	__uint64_t	xs_write_bytes = 0;
	__uint64_t	xs_read_bytes = 0;
=======
		val += *(((__u32 *)per_cpu_ptr(stats, cpu) + idx));
	return val;
}

int xfs_stats_format(struct xfsstats __percpu *stats, char *buf)
{
	int		i, j;
	int		len = 0;
	uint64_t	xs_xstrat_bytes = 0;
	uint64_t	xs_write_bytes = 0;
	uint64_t	xs_read_bytes = 0;
	uint64_t	defer_relog = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	static const struct xstats_entry {
		char	*desc;
		int	endpoint;
	} xstats[] = {
<<<<<<< HEAD
		{ "extent_alloc",	XFSSTAT_END_EXTENT_ALLOC	},
		{ "abt",		XFSSTAT_END_ALLOC_BTREE		},
		{ "blk_map",		XFSSTAT_END_BLOCK_MAPPING	},
		{ "bmbt",		XFSSTAT_END_BLOCK_MAP_BTREE	},
		{ "dir",		XFSSTAT_END_DIRECTORY_OPS	},
		{ "trans",		XFSSTAT_END_TRANSACTIONS	},
		{ "ig",			XFSSTAT_END_INODE_OPS		},
		{ "log",		XFSSTAT_END_LOG_OPS		},
		{ "push_ail",		XFSSTAT_END_TAIL_PUSHING	},
		{ "xstrat",		XFSSTAT_END_WRITE_CONVERT	},
		{ "rw",			XFSSTAT_END_READ_WRITE_OPS	},
		{ "attr",		XFSSTAT_END_ATTRIBUTE_OPS	},
		{ "icluster",		XFSSTAT_END_INODE_CLUSTER	},
		{ "vnodes",		XFSSTAT_END_VNODE_OPS		},
		{ "buf",		XFSSTAT_END_BUF			},
		{ "abtb2",		XFSSTAT_END_ABTB_V2		},
		{ "abtc2",		XFSSTAT_END_ABTC_V2		},
		{ "bmbt2",		XFSSTAT_END_BMBT_V2		},
		{ "ibt2",		XFSSTAT_END_IBT_V2		},
		/* we print both series of quota information together */
		{ "qm",			XFSSTAT_END_QM			},
	};

	/* Loop over all stats groups */
	for (i = j = 0; i < ARRAY_SIZE(xstats); i++) {
		seq_printf(m, "%s", xstats[i].desc);
		/* inner loop does each group */
		for (; j < xstats[i].endpoint; j++)
			seq_printf(m, " %u", counter_val(j));
		seq_putc(m, '\n');
	}
	/* extra precision counters */
	for_each_possible_cpu(i) {
		xs_xstrat_bytes += per_cpu(xfsstats, i).xs_xstrat_bytes;
		xs_write_bytes += per_cpu(xfsstats, i).xs_write_bytes;
		xs_read_bytes += per_cpu(xfsstats, i).xs_read_bytes;
	}

	seq_printf(m, "xpc %Lu %Lu %Lu\n",
			xs_xstrat_bytes, xs_write_bytes, xs_read_bytes);
	seq_printf(m, "debug %u\n",
=======
		{ "extent_alloc",	xfsstats_offset(xs_abt_lookup)	},
		{ "abt",		xfsstats_offset(xs_blk_mapr)	},
		{ "blk_map",		xfsstats_offset(xs_bmbt_lookup)	},
		{ "bmbt",		xfsstats_offset(xs_dir_lookup)	},
		{ "dir",		xfsstats_offset(xs_trans_sync)	},
		{ "trans",		xfsstats_offset(xs_ig_attempts)	},
		{ "ig",			xfsstats_offset(xs_log_writes)	},
		{ "log",		xfsstats_offset(xs_try_logspace)},
		{ "push_ail",		xfsstats_offset(xs_xstrat_quick)},
		{ "xstrat",		xfsstats_offset(xs_write_calls)	},
		{ "rw",			xfsstats_offset(xs_attr_get)	},
		{ "attr",		xfsstats_offset(xs_iflush_count)},
		{ "icluster",		xfsstats_offset(vn_active)	},
		{ "vnodes",		xfsstats_offset(xb_get)		},
		{ "buf",		xfsstats_offset(xs_abtb_2)	},
		{ "abtb2",		xfsstats_offset(xs_abtc_2)	},
		{ "abtc2",		xfsstats_offset(xs_bmbt_2)	},
		{ "bmbt2",		xfsstats_offset(xs_ibt_2)	},
		{ "ibt2",		xfsstats_offset(xs_fibt_2)	},
		{ "fibt2",		xfsstats_offset(xs_rmap_2)	},
		{ "rmapbt",		xfsstats_offset(xs_refcbt_2)	},
		{ "refcntbt",		xfsstats_offset(xs_rmap_mem_2)	},
		{ "rmapbt_mem",		xfsstats_offset(xs_rcbag_2)	},
		{ "rcbagbt",		xfsstats_offset(xs_qm_dqreclaims)},
		/* we print both series of quota information together */
		{ "qm",			xfsstats_offset(xs_xstrat_bytes)},
	};

	/* Loop over all stats groups */

	for (i = j = 0; i < ARRAY_SIZE(xstats); i++) {
		len += scnprintf(buf + len, PATH_MAX - len, "%s",
				xstats[i].desc);
		/* inner loop does each group */
		for (; j < xstats[i].endpoint; j++)
			len += scnprintf(buf + len, PATH_MAX - len, " %u",
					counter_val(stats, j));
		len += scnprintf(buf + len, PATH_MAX - len, "\n");
	}
	/* extra precision counters */
	for_each_possible_cpu(i) {
		xs_xstrat_bytes += per_cpu_ptr(stats, i)->s.xs_xstrat_bytes;
		xs_write_bytes += per_cpu_ptr(stats, i)->s.xs_write_bytes;
		xs_read_bytes += per_cpu_ptr(stats, i)->s.xs_read_bytes;
		defer_relog += per_cpu_ptr(stats, i)->s.defer_relog;
	}

	len += scnprintf(buf + len, PATH_MAX-len, "xpc %llu %llu %llu\n",
			xs_xstrat_bytes, xs_write_bytes, xs_read_bytes);
	len += scnprintf(buf + len, PATH_MAX-len, "defer_relog %llu\n",
			defer_relog);
	len += scnprintf(buf + len, PATH_MAX-len, "debug %u\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if defined(DEBUG)
		1);
#else
		0);
#endif
<<<<<<< HEAD
	return 0;
}

static int xfs_stat_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, xfs_stat_proc_show, NULL);
}

static const struct file_operations xfs_stat_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= xfs_stat_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

/* legacy quota interfaces */
#ifdef CONFIG_XFS_QUOTA
=======

	return len;
}

void xfs_stats_clearall(struct xfsstats __percpu *stats)
{
	int		c;
	uint32_t	vn_active;

	xfs_notice(NULL, "Clearing xfsstats");
	for_each_possible_cpu(c) {
		preempt_disable();
		/* save vn_active, it's a universal truth! */
		vn_active = per_cpu_ptr(stats, c)->s.vn_active;
		memset(per_cpu_ptr(stats, c), 0, sizeof(*stats));
		per_cpu_ptr(stats, c)->s.vn_active = vn_active;
		preempt_enable();
	}
}

#ifdef CONFIG_PROC_FS
/* legacy quota interfaces */
#ifdef CONFIG_XFS_QUOTA

#define XFSSTAT_START_XQMSTAT xfsstats_offset(xs_qm_dqreclaims)
#define XFSSTAT_END_XQMSTAT xfsstats_offset(xs_qm_dquot)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int xqm_proc_show(struct seq_file *m, void *v)
{
	/* maximum; incore; ratio free to inuse; freelist */
	seq_printf(m, "%d\t%d\t%d\t%u\n",
<<<<<<< HEAD
			0,
			counter_val(XFSSTAT_END_XQMSTAT),
			0,
			counter_val(XFSSTAT_END_XQMSTAT + 1));
	return 0;
}

static int xqm_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, xqm_proc_show, NULL);
}

static const struct file_operations xqm_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= xqm_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

=======
		   0, counter_val(xfsstats.xs_stats, XFSSTAT_END_XQMSTAT),
		   0, counter_val(xfsstats.xs_stats, XFSSTAT_END_XQMSTAT + 1));
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* legacy quota stats interface no 2 */
static int xqmstat_proc_show(struct seq_file *m, void *v)
{
	int j;

<<<<<<< HEAD
	seq_printf(m, "qm");
	for (j = XFSSTAT_END_IBT_V2; j < XFSSTAT_END_XQMSTAT; j++)
		seq_printf(m, " %u", counter_val(j));
	seq_putc(m, '\n');
	return 0;
}

static int xqmstat_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, xqmstat_proc_show, NULL);
}

static const struct file_operations xqmstat_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= xqmstat_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};
=======
	seq_puts(m, "qm");
	for (j = XFSSTAT_START_XQMSTAT; j < XFSSTAT_END_XQMSTAT; j++)
		seq_printf(m, " %u", counter_val(xfsstats.xs_stats, j));
	seq_putc(m, '\n');
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_XFS_QUOTA */

int
xfs_init_procfs(void)
{
	if (!proc_mkdir("fs/xfs", NULL))
<<<<<<< HEAD
		goto out;

	if (!proc_create("fs/xfs/stat", 0, NULL,
			 &xfs_stat_proc_fops))
		goto out_remove_xfs_dir;
#ifdef CONFIG_XFS_QUOTA
	if (!proc_create("fs/xfs/xqmstat", 0, NULL,
			 &xqmstat_proc_fops))
		goto out_remove_stat_file;
	if (!proc_create("fs/xfs/xqm", 0, NULL,
			 &xqm_proc_fops))
		goto out_remove_xqmstat_file;
#endif
	return 0;

#ifdef CONFIG_XFS_QUOTA
 out_remove_xqmstat_file:
	remove_proc_entry("fs/xfs/xqmstat", NULL);
 out_remove_stat_file:
	remove_proc_entry("fs/xfs/stat", NULL);
#endif
 out_remove_xfs_dir:
	remove_proc_entry("fs/xfs", NULL);
 out:
=======
		return -ENOMEM;

	if (!proc_symlink("fs/xfs/stat", NULL,
			  "/sys/fs/xfs/stats/stats"))
		goto out;

#ifdef CONFIG_XFS_QUOTA
	if (!proc_create_single("fs/xfs/xqmstat", 0, NULL, xqmstat_proc_show))
		goto out;
	if (!proc_create_single("fs/xfs/xqm", 0, NULL, xqm_proc_show))
		goto out;
#endif
	return 0;

out:
	remove_proc_subtree("fs/xfs", NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -ENOMEM;
}

void
xfs_cleanup_procfs(void)
{
<<<<<<< HEAD
#ifdef CONFIG_XFS_QUOTA
	remove_proc_entry("fs/xfs/xqm", NULL);
	remove_proc_entry("fs/xfs/xqmstat", NULL);
#endif
	remove_proc_entry("fs/xfs/stat", NULL);
	remove_proc_entry("fs/xfs", NULL);
}
=======
	remove_proc_subtree("fs/xfs", NULL);
}
#endif /* CONFIG_PROC_FS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
