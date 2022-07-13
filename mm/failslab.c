<<<<<<< HEAD
#include <linux/fault-inject.h>
#include <linux/slab.h>

static struct {
	struct fault_attr attr;
	u32 ignore_gfp_wait;
	int cache_filter;
} failslab = {
	.attr = FAULT_ATTR_INITIALIZER,
	.ignore_gfp_wait = 1,
	.cache_filter = 0,
};

bool should_failslab(size_t size, gfp_t gfpflags, unsigned long cache_flags)
{
	if (gfpflags & __GFP_NOFAIL)
		return false;

        if (failslab.ignore_gfp_wait && (gfpflags & __GFP_WAIT))
		return false;

	if (failslab.cache_filter && !(cache_flags & SLAB_FAILSLAB))
		return false;

	return should_fail(&failslab.attr, size);
=======
// SPDX-License-Identifier: GPL-2.0
#include <linux/fault-inject.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include "slab.h"

static struct {
	struct fault_attr attr;
	bool ignore_gfp_reclaim;
	bool cache_filter;
} failslab = {
	.attr = FAULT_ATTR_INITIALIZER,
	.ignore_gfp_reclaim = true,
	.cache_filter = false,
};

bool __should_failslab(struct kmem_cache *s, gfp_t gfpflags)
{
	int flags = 0;

	/* No fault-injection for bootstrap cache */
	if (unlikely(s == kmem_cache))
		return false;

	if (gfpflags & __GFP_NOFAIL)
		return false;

	if (failslab.ignore_gfp_reclaim &&
			(gfpflags & __GFP_DIRECT_RECLAIM))
		return false;

	if (failslab.cache_filter && !(s->flags & SLAB_FAILSLAB))
		return false;

	/*
	 * In some cases, it expects to specify __GFP_NOWARN
	 * to avoid printing any information(not just a warning),
	 * thus avoiding deadlocks. See commit 6b9dbedbe349 for
	 * details.
	 */
	if (gfpflags & __GFP_NOWARN)
		flags |= FAULT_NOWARN;

	return should_fail_ex(&failslab.attr, s->object_size, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init setup_failslab(char *str)
{
	return setup_fault_attr(&failslab.attr, str);
}
__setup("failslab=", setup_failslab);

#ifdef CONFIG_FAULT_INJECTION_DEBUG_FS
static int __init failslab_debugfs_init(void)
{
	struct dentry *dir;
<<<<<<< HEAD
	umode_t mode = S_IFREG | S_IRUSR | S_IWUSR;
=======
	umode_t mode = S_IFREG | 0600;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dir = fault_create_debugfs_attr("failslab", NULL, &failslab.attr);
	if (IS_ERR(dir))
		return PTR_ERR(dir);

<<<<<<< HEAD
	if (!debugfs_create_bool("ignore-gfp-wait", mode, dir,
				&failslab.ignore_gfp_wait))
		goto fail;
	if (!debugfs_create_bool("cache-filter", mode, dir,
				&failslab.cache_filter))
		goto fail;

	return 0;
fail:
	debugfs_remove_recursive(dir);

	return -ENOMEM;
=======
	debugfs_create_bool("ignore-gfp-wait", mode, dir,
			    &failslab.ignore_gfp_reclaim);
	debugfs_create_bool("cache-filter", mode, dir,
			    &failslab.cache_filter);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

late_initcall(failslab_debugfs_init);

#endif /* CONFIG_FAULT_INJECTION_DEBUG_FS */
