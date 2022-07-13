<<<<<<< HEAD
/* Network filesystem caching backend to use cache files on a premounted
 * filesystem
 *
 * Copyright (C) 2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version
 * 2 of the Licence, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/* Network filesystem caching backend to use cache files on a premounted
 * filesystem
 *
 * Copyright (C) 2021 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/completion.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/namei.h>
#include <linux/mount.h>
#include <linux/statfs.h>
#include <linux/sysctl.h>
#include <linux/miscdevice.h>
<<<<<<< HEAD
=======
#include <linux/netfs.h>
#include <trace/events/netfs.h>
#define CREATE_TRACE_POINTS
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "internal.h"

unsigned cachefiles_debug;
module_param_named(debug, cachefiles_debug, uint, S_IWUSR | S_IRUGO);
MODULE_PARM_DESC(cachefiles_debug, "CacheFiles debugging mask");

MODULE_DESCRIPTION("Mounted-filesystem based cache");
MODULE_AUTHOR("Red Hat, Inc.");
MODULE_LICENSE("GPL");

struct kmem_cache *cachefiles_object_jar;

static struct miscdevice cachefiles_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "cachefiles",
	.fops	= &cachefiles_daemon_fops,
};

<<<<<<< HEAD
static void cachefiles_object_init_once(void *_object)
{
	struct cachefiles_object *object = _object;

	memset(object, 0, sizeof(*object));
	spin_lock_init(&object->work_lock);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * initialise the fs caching module
 */
static int __init cachefiles_init(void)
{
	int ret;

<<<<<<< HEAD
=======
	ret = cachefiles_register_error_injection();
	if (ret < 0)
		goto error_einj;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = misc_register(&cachefiles_dev);
	if (ret < 0)
		goto error_dev;

	/* create an object jar */
	ret = -ENOMEM;
	cachefiles_object_jar =
		kmem_cache_create("cachefiles_object_jar",
				  sizeof(struct cachefiles_object),
<<<<<<< HEAD
				  0,
				  SLAB_HWCACHE_ALIGN,
				  cachefiles_object_init_once);
	if (!cachefiles_object_jar) {
		printk(KERN_NOTICE
		       "CacheFiles: Failed to allocate an object jar\n");
		goto error_object_jar;
	}

	ret = cachefiles_proc_init();
	if (ret < 0)
		goto error_proc;

	printk(KERN_INFO "CacheFiles: Loaded\n");
	return 0;

error_proc:
	kmem_cache_destroy(cachefiles_object_jar);
error_object_jar:
	misc_deregister(&cachefiles_dev);
error_dev:
	kerror("failed to register: %d", ret);
=======
				  0, SLAB_HWCACHE_ALIGN, NULL);
	if (!cachefiles_object_jar) {
		pr_notice("Failed to allocate an object jar\n");
		goto error_object_jar;
	}

	pr_info("Loaded\n");
	return 0;

error_object_jar:
	misc_deregister(&cachefiles_dev);
error_dev:
	cachefiles_unregister_error_injection();
error_einj:
	pr_err("failed to register: %d\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

fs_initcall(cachefiles_init);

/*
 * clean up on module removal
 */
static void __exit cachefiles_exit(void)
{
<<<<<<< HEAD
	printk(KERN_INFO "CacheFiles: Unloading\n");

	cachefiles_proc_cleanup();
	kmem_cache_destroy(cachefiles_object_jar);
	misc_deregister(&cachefiles_dev);
=======
	pr_info("Unloading\n");

	kmem_cache_destroy(cachefiles_object_jar);
	misc_deregister(&cachefiles_dev);
	cachefiles_unregister_error_injection();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_exit(cachefiles_exit);
