<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AppArmor security module
 *
 * This file contains AppArmor basic path manipulation function definitions.
 *
 * Copyright (C) 1998-2008 Novell/SUSE
 * Copyright 2009-2010 Canonical Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __AA_PATH_H
#define __AA_PATH_H

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
enum path_flags {
	PATH_IS_DIR = 0x1,		/* path is a directory */
	PATH_CONNECT_PATH = 0x4,	/* connect disconnected paths to / */
	PATH_CHROOT_REL = 0x8,		/* do path lookup relative to chroot */
	PATH_CHROOT_NSCONNECT = 0x10,	/* connect paths that are at ns root */

<<<<<<< HEAD
	PATH_DELEGATE_DELETED = 0x08000, /* delegate deleted files */
	PATH_MEDIATE_DELETED = 0x10000,	/* mediate deleted paths */
};

int aa_path_name(struct path *path, int flags, char **buffer,
		 const char **name, const char **info);
=======
	PATH_DELEGATE_DELETED = 0x10000, /* delegate deleted files */
	PATH_MEDIATE_DELETED = 0x20000,	 /* mediate deleted paths */
};

int aa_path_name(const struct path *path, int flags, char *buffer,
		 const char **name, const char **info,
		 const char *disconnected);

#define IN_ATOMIC true
char *aa_get_buffer(bool in_atomic);
void aa_put_buffer(char *buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __AA_PATH_H */
