<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_DIRENT_H
#define _LINUX_DIRENT_H

struct linux_dirent64 {
	u64		d_ino;
	s64		d_off;
	unsigned short	d_reclen;
	unsigned char	d_type;
<<<<<<< HEAD
	char		d_name[0];
=======
	char		d_name[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#endif
