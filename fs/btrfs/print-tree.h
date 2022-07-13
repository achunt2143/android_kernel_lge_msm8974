<<<<<<< HEAD
/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 */

#ifndef __PRINT_TREE_
#define __PRINT_TREE_
void btrfs_print_leaf(struct btrfs_root *root, struct extent_buffer *l);
void btrfs_print_tree(struct btrfs_root *root, struct extent_buffer *t);
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2007 Oracle.  All rights reserved.
 */

#ifndef BTRFS_PRINT_TREE_H
#define BTRFS_PRINT_TREE_H

/* Buffer size to contain tree name and possibly additional data (offset) */
#define BTRFS_ROOT_NAME_BUF_LEN				48

struct extent_buffer;
struct btrfs_key;

void btrfs_print_leaf(const struct extent_buffer *l);
void btrfs_print_tree(const struct extent_buffer *c, bool follow);
const char *btrfs_root_name(const struct btrfs_key *key, char *buf);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
