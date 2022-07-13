<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2004 Red Hat, Inc. All rights reserved.
 *
 * This file is released under the GPL.
 *
 * Multipath.
 */

#ifndef	DM_MPATH_H
#define	DM_MPATH_H

struct dm_dev;

struct dm_path {
	struct dm_dev *dev;	/* Read-only */
	void *pscontext;	/* For path-selector use */
};

/* Callback for hwh_pg_init_fn to use when complete */
<<<<<<< HEAD
void dm_pg_init_complete(struct dm_path *path, unsigned err_flags);
=======
void dm_pg_init_complete(struct dm_path *path, unsigned int err_flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
