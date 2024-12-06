<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * f2fs shrinker support
 *   the basic infra was copied from fs/ubifs/shrinker.c
 *
 * Copyright (c) 2015 Motorola Mobility
 * Copyright (c) 2015 Jaegeuk Kim <jaegeuk@kernel.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/fs.h>
#include <linux/f2fs_fs.h>

#include "f2fs.h"
<<<<<<< HEAD
=======
#include "node.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static LIST_HEAD(f2fs_list);
static DEFINE_SPINLOCK(f2fs_list_lock);
static unsigned int shrinker_run_no;

static unsigned long __count_nat_entries(struct f2fs_sb_info *sbi)
{
<<<<<<< HEAD
	return NM_I(sbi)->nat_cnt - NM_I(sbi)->dirty_nat_cnt;
=======
	return NM_I(sbi)->nat_cnt[RECLAIMABLE_NAT];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static unsigned long __count_free_nids(struct f2fs_sb_info *sbi)
{
<<<<<<< HEAD
	if (NM_I(sbi)->fcnt > NAT_ENTRY_PER_BLOCK)
		return NM_I(sbi)->fcnt - NAT_ENTRY_PER_BLOCK;
	return 0;
}

static unsigned long __count_extent_cache(struct f2fs_sb_info *sbi)
{
	return atomic_read(&sbi->total_zombie_tree) +
				atomic_read(&sbi->total_ext_node);
}

int f2fs_shrink_count(struct shrinker *shrink,
=======
	long count = NM_I(sbi)->nid_cnt[FREE_NID] - MAX_FREE_NIDS;

	return count > 0 ? count : 0;
}

static unsigned long __count_extent_cache(struct f2fs_sb_info *sbi,
					enum extent_type type)
{
	struct extent_tree_info *eti = &sbi->extent_tree[type];

	return atomic_read(&eti->total_zombie_tree) +
				atomic_read(&eti->total_ext_node);
}

unsigned long f2fs_shrink_count(struct shrinker *shrink,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				struct shrink_control *sc)
{
	struct f2fs_sb_info *sbi;
	struct list_head *p;
	unsigned long count = 0;

	spin_lock(&f2fs_list_lock);
	p = f2fs_list.next;
	while (p != &f2fs_list) {
		sbi = list_entry(p, struct f2fs_sb_info, s_list);

		/* stop f2fs_put_super */
		if (!mutex_trylock(&sbi->umount_mutex)) {
			p = p->next;
			continue;
		}
		spin_unlock(&f2fs_list_lock);

<<<<<<< HEAD
		/* count extent cache entries */
		count += __count_extent_cache(sbi);

		/* shrink clean nat cache entries */
=======
		/* count read extent cache entries */
		count += __count_extent_cache(sbi, EX_READ);

		/* count block age extent cache entries */
		count += __count_extent_cache(sbi, EX_BLOCK_AGE);

		/* count clean nat cache entries */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		count += __count_nat_entries(sbi);

		/* count free nids cache entries */
		count += __count_free_nids(sbi);

		spin_lock(&f2fs_list_lock);
		p = p->next;
		mutex_unlock(&sbi->umount_mutex);
	}
	spin_unlock(&f2fs_list_lock);
	return count;
}

<<<<<<< HEAD
int f2fs_shrink_scan(struct shrinker *shrink,
=======
unsigned long f2fs_shrink_scan(struct shrinker *shrink,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				struct shrink_control *sc)
{
	unsigned long nr = sc->nr_to_scan;
	struct f2fs_sb_info *sbi;
	struct list_head *p;
	unsigned int run_no;
	unsigned long freed = 0;

	spin_lock(&f2fs_list_lock);
	do {
		run_no = ++shrinker_run_no;
	} while (run_no == 0);
	p = f2fs_list.next;
	while (p != &f2fs_list) {
		sbi = list_entry(p, struct f2fs_sb_info, s_list);

		if (sbi->shrinker_run_no == run_no)
			break;

		/* stop f2fs_put_super */
		if (!mutex_trylock(&sbi->umount_mutex)) {
			p = p->next;
			continue;
		}
		spin_unlock(&f2fs_list_lock);

		sbi->shrinker_run_no = run_no;

		/* shrink extent cache entries */
<<<<<<< HEAD
		freed += f2fs_shrink_extent_tree(sbi, nr >> 1);

		/* shrink clean nat cache entries */
		if (freed < nr)
			freed += try_to_free_nats(sbi, nr - freed);

		/* shrink free nids cache entries */
		if (freed < nr)
			freed += try_to_free_nids(sbi, nr - freed);
=======
		freed += f2fs_shrink_age_extent_tree(sbi, nr >> 2);

		/* shrink read extent cache entries */
		freed += f2fs_shrink_read_extent_tree(sbi, nr >> 2);

		/* shrink clean nat cache entries */
		if (freed < nr)
			freed += f2fs_try_to_free_nats(sbi, nr - freed);

		/* shrink free nids cache entries */
		if (freed < nr)
			freed += f2fs_try_to_free_nids(sbi, nr - freed);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		spin_lock(&f2fs_list_lock);
		p = p->next;
		list_move_tail(&sbi->s_list, &f2fs_list);
		mutex_unlock(&sbi->umount_mutex);
		if (freed >= nr)
			break;
	}
	spin_unlock(&f2fs_list_lock);
<<<<<<< HEAD
	return f2fs_shrink_count(NULL, NULL);
=======
	return freed;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void f2fs_join_shrinker(struct f2fs_sb_info *sbi)
{
	spin_lock(&f2fs_list_lock);
	list_add_tail(&sbi->s_list, &f2fs_list);
	spin_unlock(&f2fs_list_lock);
}

void f2fs_leave_shrinker(struct f2fs_sb_info *sbi)
{
<<<<<<< HEAD
	f2fs_shrink_extent_tree(sbi, __count_extent_cache(sbi));

	spin_lock(&f2fs_list_lock);
	list_del(&sbi->s_list);
=======
	f2fs_shrink_read_extent_tree(sbi, __count_extent_cache(sbi, EX_READ));
	f2fs_shrink_age_extent_tree(sbi,
				__count_extent_cache(sbi, EX_BLOCK_AGE));

	spin_lock(&f2fs_list_lock);
	list_del_init(&sbi->s_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&f2fs_list_lock);
}
