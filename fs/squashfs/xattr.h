<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Squashfs - a compressed read only filesystem for Linux
 *
 * Copyright (c) 2010
 * Phillip Lougher <phillip@squashfs.org.uk>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * xattr.h
 */

#ifdef CONFIG_SQUASHFS_XATTR
extern __le64 *squashfs_read_xattr_id_table(struct super_block *, u64,
<<<<<<< HEAD
		u64 *, int *);
=======
		u64 *, unsigned int *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int squashfs_xattr_lookup(struct super_block *, unsigned int, int *,
		unsigned int *, unsigned long long *);
#else
static inline __le64 *squashfs_read_xattr_id_table(struct super_block *sb,
<<<<<<< HEAD
		u64 start, u64 *xattr_table_start, int *xattr_ids)
{
	ERROR("Xattrs in filesystem, these will be ignored\n");
	*xattr_table_start = start;
=======
		u64 start, u64 *xattr_table_start, unsigned int *xattr_ids)
{
	struct squashfs_xattr_id_table *id_table;

	id_table = squashfs_read_table(sb, start, sizeof(*id_table));
	if (IS_ERR(id_table))
		return (__le64 *) id_table;

	*xattr_table_start = le64_to_cpu(id_table->xattr_table_start);
	kfree(id_table);

	ERROR("Xattrs in filesystem, these will be ignored\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ERR_PTR(-ENOTSUPP);
}

static inline int squashfs_xattr_lookup(struct super_block *sb,
		unsigned int index, int *count, unsigned int *size,
		unsigned long long *xattr)
{
	return 0;
}
#define squashfs_listxattr NULL
<<<<<<< HEAD
#define generic_getxattr NULL
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define squashfs_xattr_handlers NULL
#endif
