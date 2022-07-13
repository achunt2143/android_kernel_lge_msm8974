<<<<<<< HEAD
/*
 * (C) Copyright David Gibson <dwg@au1.ibm.com>, IBM Corporation.  2005.
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 *                                                                   USA
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * (C) Copyright David Gibson <dwg@au1.ibm.com>, IBM Corporation.  2005.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include "dtc.h"

#include <dirent.h>
#include <sys/stat.h>

static struct node *read_fstree(const char *dirname)
{
	DIR *d;
	struct dirent *de;
	struct stat st;
	struct node *tree;

	d = opendir(dirname);
	if (!d)
		die("Couldn't opendir() \"%s\": %s\n", dirname, strerror(errno));

<<<<<<< HEAD
	tree = build_node(NULL, NULL);

	while ((de = readdir(d)) != NULL) {
		char *tmpnam;
=======
	tree = build_node(NULL, NULL, NULL);

	while ((de = readdir(d)) != NULL) {
		char *tmpname;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (streq(de->d_name, ".")
		    || streq(de->d_name, ".."))
			continue;

<<<<<<< HEAD
		tmpnam = join_path(dirname, de->d_name);

		if (lstat(tmpnam, &st) < 0)
			die("stat(%s): %s\n", tmpnam, strerror(errno));
=======
		tmpname = join_path(dirname, de->d_name);

		if (stat(tmpname, &st) < 0)
			die("stat(%s): %s\n", tmpname, strerror(errno));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (S_ISREG(st.st_mode)) {
			struct property *prop;
			FILE *pfile;

<<<<<<< HEAD
			pfile = fopen(tmpnam, "r");
			if (! pfile) {
				fprintf(stderr,
					"WARNING: Cannot open %s: %s\n",
					tmpnam, strerror(errno));
			} else {
				prop = build_property(xstrdup(de->d_name),
						      data_copy_file(pfile,
								     st.st_size));
=======
			pfile = fopen(tmpname, "rb");
			if (! pfile) {
				fprintf(stderr,
					"WARNING: Cannot open %s: %s\n",
					tmpname, strerror(errno));
			} else {
				prop = build_property(xstrdup(de->d_name),
						      data_copy_file(pfile,
								     st.st_size),
						      NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				add_property(tree, prop);
				fclose(pfile);
			}
		} else if (S_ISDIR(st.st_mode)) {
			struct node *newchild;

<<<<<<< HEAD
			newchild = read_fstree(tmpnam);
=======
			newchild = read_fstree(tmpname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			newchild = name_node(newchild, xstrdup(de->d_name));
			add_child(tree, newchild);
		}

<<<<<<< HEAD
		free(tmpnam);
=======
		free(tmpname);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	closedir(d);
	return tree;
}

<<<<<<< HEAD
struct boot_info *dt_from_fs(const char *dirname)
=======
struct dt_info *dt_from_fs(const char *dirname)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct node *tree;

	tree = read_fstree(dirname);
	tree = name_node(tree, "");

<<<<<<< HEAD
	return build_boot_info(NULL, tree, guess_boot_cpuid(tree));
}

=======
	return build_dt_info(DTSF_V1, NULL, tree, guess_boot_cpuid(tree));
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
