<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * kernel/configs.c
 * Echo the kernel .config file used to build the kernel
 *
 * Copyright (C) 2002 Khalid Aziz <khalid_aziz@hp.com>
 * Copyright (C) 2002 Randy Dunlap <rdunlap@xenotime.net>
 * Copyright (C) 2002 Al Stone <ahs3@fc.hp.com>
 * Copyright (C) 2002 Hewlett-Packard Company
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <asm/uaccess.h>

/**************************************************/
/* the actual current config file                 */

/*
 * Define kernel_config_data and kernel_config_data_size, which contains the
 * wrapped and compressed configuration file.  The file is first compressed
 * with gzip and then bounded by two eight byte magic numbers to allow
 * extraction from a binary kernel image:
 *
 *   IKCFG_ST
 *   <image>
 *   IKCFG_ED
 */
#define MAGIC_START	"IKCFG_ST"
#define MAGIC_END	"IKCFG_ED"
#include "config_data.h"


#define MAGIC_SIZE (sizeof(MAGIC_START) - 1)
#define kernel_config_data_size \
	(sizeof(kernel_config_data) - 1 - MAGIC_SIZE * 2)

#ifdef CONFIG_IKCONFIG_PROC

=======
#include <linux/uaccess.h>

/*
 * "IKCFG_ST" and "IKCFG_ED" are used to extract the config data from
 * a binary kernel image or a module. See scripts/extract-ikconfig.
 */
asm (
"	.pushsection .rodata, \"a\"		\n"
"	.ascii \"IKCFG_ST\"			\n"
"	.global kernel_config_data		\n"
"kernel_config_data:				\n"
"	.incbin \"kernel/config_data.gz\"	\n"
"	.global kernel_config_data_end		\n"
"kernel_config_data_end:			\n"
"	.ascii \"IKCFG_ED\"			\n"
"	.popsection				\n"
);

#ifdef CONFIG_IKCONFIG_PROC

extern char kernel_config_data;
extern char kernel_config_data_end;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t
ikconfig_read_current(struct file *file, char __user *buf,
		      size_t len, loff_t * offset)
{
	return simple_read_from_buffer(buf, len, offset,
<<<<<<< HEAD
				       kernel_config_data + MAGIC_SIZE,
				       kernel_config_data_size);
}

static const struct file_operations ikconfig_file_ops = {
	.owner = THIS_MODULE,
	.read = ikconfig_read_current,
	.llseek = default_llseek,
=======
				       &kernel_config_data,
				       &kernel_config_data_end -
				       &kernel_config_data);
}

static const struct proc_ops config_gz_proc_ops = {
	.proc_read	= ikconfig_read_current,
	.proc_lseek	= default_llseek,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init ikconfig_init(void)
{
	struct proc_dir_entry *entry;

	/* create the current config file */
	entry = proc_create("config.gz", S_IFREG | S_IRUGO, NULL,
<<<<<<< HEAD
			    &ikconfig_file_ops);
	if (!entry)
		return -ENOMEM;

	entry->size = kernel_config_data_size;
=======
			    &config_gz_proc_ops);
	if (!entry)
		return -ENOMEM;

	proc_set_size(entry, &kernel_config_data_end - &kernel_config_data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static void __exit ikconfig_cleanup(void)
{
	remove_proc_entry("config.gz", NULL);
}

module_init(ikconfig_init);
module_exit(ikconfig_cleanup);

#endif /* CONFIG_IKCONFIG_PROC */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Randy Dunlap");
MODULE_DESCRIPTION("Echo the kernel .config file used to build the kernel");
