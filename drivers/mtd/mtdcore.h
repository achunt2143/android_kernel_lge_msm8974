<<<<<<< HEAD
/* linux/drivers/mtd/mtdcore.h
 *
 * Header file for driver private mtdcore exports
 *
 */

/* These are exported solely for the purpose of mtd_blkdevs.c. You
   should not use them for _anything_ else */

extern struct mutex mtd_table_mutex;
extern struct mtd_info *__mtd_next_device(int i);

extern int add_mtd_device(struct mtd_info *mtd);
extern int del_mtd_device(struct mtd_info *mtd);
extern int add_mtd_partitions(struct mtd_info *, const struct mtd_partition *,
			      int);
extern int del_mtd_partitions(struct mtd_info *);
extern int parse_mtd_partitions(struct mtd_info *master, const char **types,
				struct mtd_partition **pparts,
				struct mtd_part_parser_data *data);
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * These are exported solely for the purpose of mtd_blkdevs.c and mtdchar.c.
 * You should not use them for _anything_ else.
 */

extern struct mutex mtd_table_mutex;
extern struct backing_dev_info *mtd_bdi;

struct mtd_info *__mtd_next_device(int i);
int __must_check add_mtd_device(struct mtd_info *mtd);
int del_mtd_device(struct mtd_info *mtd);
int add_mtd_partitions(struct mtd_info *, const struct mtd_partition *, int);
int del_mtd_partitions(struct mtd_info *);
void release_mtd_partition(struct mtd_info *mtd);

struct mtd_partitions;

int parse_mtd_partitions(struct mtd_info *master, const char * const *types,
			 struct mtd_part_parser_data *data);

void mtd_part_parser_cleanup(struct mtd_partitions *parts);

int __init init_mtdchar(void);
void __exit cleanup_mtdchar(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define mtd_for_each_device(mtd)			\
	for ((mtd) = __mtd_next_device(0);		\
	     (mtd) != NULL;				\
	     (mtd) = __mtd_next_device(mtd->index + 1))
