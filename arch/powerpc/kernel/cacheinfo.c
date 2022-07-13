<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Processor cache information made available to userspace via sysfs;
 * intended to be compatible with x86 intel_cacheinfo implementation.
 *
 * Copyright 2008 IBM Corporation
 * Author: Nathan Lynch
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 */

#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <linux/init.h>
=======
 */

#define pr_fmt(fmt) "cacheinfo: " fmt

#include <linux/cpu.h>
#include <linux/cpumask.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/list.h>
#include <linux/notifier.h>
#include <linux/of.h>
#include <linux/percpu.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <asm/prom.h>
=======
#include <asm/cputhreads.h>
#include <asm/smp.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "cacheinfo.h"

/* per-cpu object for tracking:
 * - a "cache" kobject for the top-level directory
 * - a list of "index" objects representing the cpu's local cache hierarchy
 */
struct cache_dir {
	struct kobject *kobj; /* bare (not embedded) kobject for cache
			       * directory */
	struct cache_index_dir *index; /* list of index objects */
};

/* "index" object: each cpu's cache directory has an index
 * subdirectory corresponding to a cache object associated with the
 * cpu.  This object's lifetime is managed via the embedded kobject.
 */
struct cache_index_dir {
	struct kobject kobj;
	struct cache_index_dir *next; /* next index in parent directory */
	struct cache *cache;
};

/* Template for determining which OF properties to query for a given
 * cache type */
struct cache_type_info {
	const char *name;
	const char *size_prop;

	/* Allow for both [di]-cache-line-size and
	 * [di]-cache-block-size properties.  According to the PowerPC
	 * Processor binding, -line-size should be provided if it
	 * differs from the cache block size (that which is operated
	 * on by cache instructions), so we look for -line-size first.
	 * See cache_get_line_size(). */

	const char *line_size_props[2];
	const char *nr_sets_prop;
};

/* These are used to index the cache_type_info array. */
#define CACHE_TYPE_UNIFIED     0 /* cache-size, cache-block-size, etc. */
#define CACHE_TYPE_UNIFIED_D   1 /* d-cache-size, d-cache-block-size, etc */
#define CACHE_TYPE_INSTRUCTION 2
#define CACHE_TYPE_DATA        3

static const struct cache_type_info cache_type_info[] = {
	{
		/* Embedded systems that use cache-size, cache-block-size,
		 * etc. for the Unified (typically L2) cache. */
		.name            = "Unified",
		.size_prop       = "cache-size",
		.line_size_props = { "cache-line-size",
				     "cache-block-size", },
		.nr_sets_prop    = "cache-sets",
	},
	{
		/* PowerPC Processor binding says the [di]-cache-*
		 * must be equal on unified caches, so just use
		 * d-cache properties. */
		.name            = "Unified",
		.size_prop       = "d-cache-size",
		.line_size_props = { "d-cache-line-size",
				     "d-cache-block-size", },
		.nr_sets_prop    = "d-cache-sets",
	},
	{
		.name            = "Instruction",
		.size_prop       = "i-cache-size",
		.line_size_props = { "i-cache-line-size",
				     "i-cache-block-size", },
		.nr_sets_prop    = "i-cache-sets",
	},
	{
		.name            = "Data",
		.size_prop       = "d-cache-size",
		.line_size_props = { "d-cache-line-size",
				     "d-cache-block-size", },
		.nr_sets_prop    = "d-cache-sets",
	},
};

/* Cache object: each instance of this corresponds to a distinct cache
 * in the system.  There are separate objects for Harvard caches: one
 * each for instruction and data, and each refers to the same OF node.
 * The refcount of the OF node is elevated for the lifetime of the
 * cache object.  A cache object is released when its shared_cpu_map
 * is cleared (see cache_cpu_clear).
 *
 * A cache object is on two lists: an unsorted global list
 * (cache_list) of cache objects; and a singly-linked list
 * representing the local cache hierarchy, which is ordered by level
 * (e.g. L1d -> L1i -> L2 -> L3).
 */
struct cache {
	struct device_node *ofnode;    /* OF node for this cache, may be cpu */
	struct cpumask shared_cpu_map; /* online CPUs using this cache */
	int type;                      /* split cache disambiguation */
	int level;                     /* level not explicit in device tree */
<<<<<<< HEAD
=======
	int group_id;                  /* id of the group of threads that share this cache */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct list_head list;         /* global list of cache objects */
	struct cache *next_local;      /* next cache of >= level */
};

static DEFINE_PER_CPU(struct cache_dir *, cache_dir_pcpu);

/* traversal/modification of this list occurs only at cpu hotplug time;
 * access is serialized by cpu hotplug locking
 */
static LIST_HEAD(cache_list);

static struct cache_index_dir *kobj_to_cache_index_dir(struct kobject *k)
{
	return container_of(k, struct cache_index_dir, kobj);
}

static const char *cache_type_string(const struct cache *cache)
{
	return cache_type_info[cache->type].name;
}

<<<<<<< HEAD
static void __cpuinit cache_init(struct cache *cache, int type, int level, struct device_node *ofnode)
=======
static void cache_init(struct cache *cache, int type, int level,
		       struct device_node *ofnode, int group_id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	cache->type = type;
	cache->level = level;
	cache->ofnode = of_node_get(ofnode);
<<<<<<< HEAD
=======
	cache->group_id = group_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	INIT_LIST_HEAD(&cache->list);
	list_add(&cache->list, &cache_list);
}

<<<<<<< HEAD
static struct cache *__cpuinit new_cache(int type, int level, struct device_node *ofnode)
=======
static struct cache *new_cache(int type, int level,
			       struct device_node *ofnode, int group_id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cache *cache;

	cache = kzalloc(sizeof(*cache), GFP_KERNEL);
	if (cache)
<<<<<<< HEAD
		cache_init(cache, type, level, ofnode);
=======
		cache_init(cache, type, level, ofnode, group_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return cache;
}

static void release_cache_debugcheck(struct cache *cache)
{
	struct cache *iter;

	list_for_each_entry(iter, &cache_list, list)
		WARN_ONCE(iter->next_local == cache,
<<<<<<< HEAD
			  "cache for %s(%s) refers to cache for %s(%s)\n",
			  iter->ofnode->full_name,
			  cache_type_string(iter),
			  cache->ofnode->full_name,
=======
			  "cache for %pOFP(%s) refers to cache for %pOFP(%s)\n",
			  iter->ofnode,
			  cache_type_string(iter),
			  cache->ofnode,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  cache_type_string(cache));
}

static void release_cache(struct cache *cache)
{
	if (!cache)
		return;

<<<<<<< HEAD
	pr_debug("freeing L%d %s cache for %s\n", cache->level,
		 cache_type_string(cache), cache->ofnode->full_name);
=======
	pr_debug("freeing L%d %s cache for %pOFP\n", cache->level,
		 cache_type_string(cache), cache->ofnode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	release_cache_debugcheck(cache);
	list_del(&cache->list);
	of_node_put(cache->ofnode);
	kfree(cache);
}

static void cache_cpu_set(struct cache *cache, int cpu)
{
	struct cache *next = cache;

	while (next) {
		WARN_ONCE(cpumask_test_cpu(cpu, &next->shared_cpu_map),
<<<<<<< HEAD
			  "CPU %i already accounted in %s(%s)\n",
			  cpu, next->ofnode->full_name,
=======
			  "CPU %i already accounted in %pOFP(%s)\n",
			  cpu, next->ofnode,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  cache_type_string(next));
		cpumask_set_cpu(cpu, &next->shared_cpu_map);
		next = next->next_local;
	}
}

static int cache_size(const struct cache *cache, unsigned int *ret)
{
	const char *propname;
<<<<<<< HEAD
	const u32 *cache_size;
=======
	const __be32 *cache_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	propname = cache_type_info[cache->type].size_prop;

	cache_size = of_get_property(cache->ofnode, propname, NULL);
	if (!cache_size)
		return -ENODEV;

<<<<<<< HEAD
	*ret = *cache_size;
=======
	*ret = of_read_number(cache_size, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int cache_size_kb(const struct cache *cache, unsigned int *ret)
{
	unsigned int size;

	if (cache_size(cache, &size))
		return -ENODEV;

	*ret = size / 1024;
	return 0;
}

/* not cache_line_size() because that's a macro in include/linux/cache.h */
static int cache_get_line_size(const struct cache *cache, unsigned int *ret)
{
<<<<<<< HEAD
	const u32 *line_size;
=======
	const __be32 *line_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i, lim;

	lim = ARRAY_SIZE(cache_type_info[cache->type].line_size_props);

	for (i = 0; i < lim; i++) {
		const char *propname;

		propname = cache_type_info[cache->type].line_size_props[i];
		line_size = of_get_property(cache->ofnode, propname, NULL);
		if (line_size)
			break;
	}

	if (!line_size)
		return -ENODEV;

<<<<<<< HEAD
	*ret = *line_size;
=======
	*ret = of_read_number(line_size, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int cache_nr_sets(const struct cache *cache, unsigned int *ret)
{
	const char *propname;
<<<<<<< HEAD
	const u32 *nr_sets;
=======
	const __be32 *nr_sets;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	propname = cache_type_info[cache->type].nr_sets_prop;

	nr_sets = of_get_property(cache->ofnode, propname, NULL);
	if (!nr_sets)
		return -ENODEV;

<<<<<<< HEAD
	*ret = *nr_sets;
=======
	*ret = of_read_number(nr_sets, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int cache_associativity(const struct cache *cache, unsigned int *ret)
{
	unsigned int line_size;
	unsigned int nr_sets;
	unsigned int size;

	if (cache_nr_sets(cache, &nr_sets))
		goto err;

	/* If the cache is fully associative, there is no need to
	 * check the other properties.
	 */
	if (nr_sets == 1) {
		*ret = 0;
		return 0;
	}

	if (cache_get_line_size(cache, &line_size))
		goto err;
	if (cache_size(cache, &size))
		goto err;

	if (!(nr_sets > 0 && size > 0 && line_size > 0))
		goto err;

	*ret = (size / nr_sets) / line_size;
	return 0;
err:
	return -ENODEV;
}

/* helper for dealing with split caches */
static struct cache *cache_find_first_sibling(struct cache *cache)
{
	struct cache *iter;

	if (cache->type == CACHE_TYPE_UNIFIED ||
	    cache->type == CACHE_TYPE_UNIFIED_D)
		return cache;

	list_for_each_entry(iter, &cache_list, list)
<<<<<<< HEAD
		if (iter->ofnode == cache->ofnode && iter->next_local == cache)
=======
		if (iter->ofnode == cache->ofnode &&
		    iter->group_id == cache->group_id &&
		    iter->next_local == cache)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return iter;

	return cache;
}

<<<<<<< HEAD
/* return the first cache on a local list matching node */
static struct cache *cache_lookup_by_node(const struct device_node *node)
=======
/* return the first cache on a local list matching node and thread-group id */
static struct cache *cache_lookup_by_node_group(const struct device_node *node,
						int group_id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cache *cache = NULL;
	struct cache *iter;

	list_for_each_entry(iter, &cache_list, list) {
<<<<<<< HEAD
		if (iter->ofnode != node)
=======
		if (iter->ofnode != node ||
		    iter->group_id != group_id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;
		cache = cache_find_first_sibling(iter);
		break;
	}

	return cache;
}

static bool cache_node_is_unified(const struct device_node *np)
{
	return of_get_property(np, "cache-unified", NULL);
}

/*
 * Unified caches can have two different sets of tags.  Most embedded
 * use cache-size, etc. for the unified cache size, but open firmware systems
 * use d-cache-size, etc.   Check on initialization for which type we have, and
 * return the appropriate structure type.  Assume it's embedded if it isn't
 * open firmware.  If it's yet a 3rd type, then there will be missing entries
 * in /sys/devices/system/cpu/cpu0/cache/index2/, and this code will need
 * to be extended further.
 */
static int cache_is_unified_d(const struct device_node *np)
{
	return of_get_property(np,
		cache_type_info[CACHE_TYPE_UNIFIED_D].size_prop, NULL) ?
		CACHE_TYPE_UNIFIED_D : CACHE_TYPE_UNIFIED;
}

<<<<<<< HEAD
static struct cache *__cpuinit cache_do_one_devnode_unified(struct device_node *node, int level)
{
	pr_debug("creating L%d ucache for %s\n", level, node->full_name);

	return new_cache(cache_is_unified_d(node), level, node);
}

static struct cache *__cpuinit cache_do_one_devnode_split(struct device_node *node, int level)
{
	struct cache *dcache, *icache;

	pr_debug("creating L%d dcache and icache for %s\n", level,
		 node->full_name);

	dcache = new_cache(CACHE_TYPE_DATA, level, node);
	icache = new_cache(CACHE_TYPE_INSTRUCTION, level, node);
=======
static struct cache *cache_do_one_devnode_unified(struct device_node *node, int group_id,
						  int level)
{
	pr_debug("creating L%d ucache for %pOFP\n", level, node);

	return new_cache(cache_is_unified_d(node), level, node, group_id);
}

static struct cache *cache_do_one_devnode_split(struct device_node *node, int group_id,
						int level)
{
	struct cache *dcache, *icache;

	pr_debug("creating L%d dcache and icache for %pOFP\n", level,
		 node);

	dcache = new_cache(CACHE_TYPE_DATA, level, node, group_id);
	icache = new_cache(CACHE_TYPE_INSTRUCTION, level, node, group_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!dcache || !icache)
		goto err;

	dcache->next_local = icache;

	return dcache;
err:
	release_cache(dcache);
	release_cache(icache);
	return NULL;
}

<<<<<<< HEAD
static struct cache *__cpuinit cache_do_one_devnode(struct device_node *node, int level)
=======
static struct cache *cache_do_one_devnode(struct device_node *node, int group_id, int level)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cache *cache;

	if (cache_node_is_unified(node))
<<<<<<< HEAD
		cache = cache_do_one_devnode_unified(node, level);
	else
		cache = cache_do_one_devnode_split(node, level);
=======
		cache = cache_do_one_devnode_unified(node, group_id, level);
	else
		cache = cache_do_one_devnode_split(node, group_id, level);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return cache;
}

<<<<<<< HEAD
static struct cache *__cpuinit cache_lookup_or_instantiate(struct device_node *node, int level)
{
	struct cache *cache;

	cache = cache_lookup_by_node(node);
=======
static struct cache *cache_lookup_or_instantiate(struct device_node *node,
						 int group_id,
						 int level)
{
	struct cache *cache;

	cache = cache_lookup_by_node_group(node, group_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	WARN_ONCE(cache && cache->level != level,
		  "cache level mismatch on lookup (got %d, expected %d)\n",
		  cache->level, level);

	if (!cache)
<<<<<<< HEAD
		cache = cache_do_one_devnode(node, level);
=======
		cache = cache_do_one_devnode(node, group_id, level);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return cache;
}

<<<<<<< HEAD
static void __cpuinit link_cache_lists(struct cache *smaller, struct cache *bigger)
=======
static void link_cache_lists(struct cache *smaller, struct cache *bigger)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	while (smaller->next_local) {
		if (smaller->next_local == bigger)
			return; /* already linked */
		smaller = smaller->next_local;
	}

	smaller->next_local = bigger;
<<<<<<< HEAD
}

static void __cpuinit do_subsidiary_caches_debugcheck(struct cache *cache)
{
	WARN_ON_ONCE(cache->level != 1);
	WARN_ON_ONCE(strcmp(cache->ofnode->type, "cpu"));
}

static void __cpuinit do_subsidiary_caches(struct cache *cache)
=======

	/*
	 * The cache->next_local list sorts by level ascending:
	 * L1d -> L1i -> L2 -> L3 ...
	 */
	WARN_ONCE((smaller->level == 1 && bigger->level > 2) ||
		  (smaller->level > 1 && bigger->level != smaller->level + 1),
		  "linking L%i cache %pOFP to L%i cache %pOFP; skipped a level?\n",
		  smaller->level, smaller->ofnode, bigger->level, bigger->ofnode);
}

static void do_subsidiary_caches_debugcheck(struct cache *cache)
{
	WARN_ONCE(cache->level != 1,
		  "instantiating cache chain from L%d %s cache for "
		  "%pOFP instead of an L1\n", cache->level,
		  cache_type_string(cache), cache->ofnode);
	WARN_ONCE(!of_node_is_type(cache->ofnode, "cpu"),
		  "instantiating cache chain from node %pOFP of type '%s' "
		  "instead of a cpu node\n", cache->ofnode,
		  of_node_get_device_type(cache->ofnode));
}

/*
 * If sub-groups of threads in a core containing @cpu_id share the
 * L@level-cache (information obtained via "ibm,thread-groups"
 * device-tree property), then we identify the group by the first
 * thread-sibling in the group. We define this to be the group-id.
 *
 * In the absence of any thread-group information for L@level-cache,
 * this function returns -1.
 */
static int get_group_id(unsigned int cpu_id, int level)
{
	if (has_big_cores && level == 1)
		return cpumask_first(per_cpu(thread_group_l1_cache_map,
					     cpu_id));
	else if (thread_group_shares_l2 && level == 2)
		return cpumask_first(per_cpu(thread_group_l2_cache_map,
					     cpu_id));
	else if (thread_group_shares_l3 && level == 3)
		return cpumask_first(per_cpu(thread_group_l3_cache_map,
					     cpu_id));
	return -1;
}

static void do_subsidiary_caches(struct cache *cache, unsigned int cpu_id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct device_node *subcache_node;
	int level = cache->level;

	do_subsidiary_caches_debugcheck(cache);

	while ((subcache_node = of_find_next_cache_node(cache->ofnode))) {
		struct cache *subcache;
<<<<<<< HEAD

		level++;
		subcache = cache_lookup_or_instantiate(subcache_node, level);
=======
		int group_id;

		level++;
		group_id = get_group_id(cpu_id, level);
		subcache = cache_lookup_or_instantiate(subcache_node, group_id, level);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		of_node_put(subcache_node);
		if (!subcache)
			break;

		link_cache_lists(cache, subcache);
		cache = subcache;
	}
}

<<<<<<< HEAD
static struct cache *__cpuinit cache_chain_instantiate(unsigned int cpu_id)
{
	struct device_node *cpu_node;
	struct cache *cpu_cache = NULL;
=======
static struct cache *cache_chain_instantiate(unsigned int cpu_id)
{
	struct device_node *cpu_node;
	struct cache *cpu_cache = NULL;
	int group_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pr_debug("creating cache object(s) for CPU %i\n", cpu_id);

	cpu_node = of_get_cpu_node(cpu_id, NULL);
	WARN_ONCE(!cpu_node, "no OF node found for CPU %i\n", cpu_id);
	if (!cpu_node)
		goto out;

<<<<<<< HEAD
	cpu_cache = cache_lookup_or_instantiate(cpu_node, 1);
	if (!cpu_cache)
		goto out;

	do_subsidiary_caches(cpu_cache);
=======
	group_id = get_group_id(cpu_id, 1);

	cpu_cache = cache_lookup_or_instantiate(cpu_node, group_id, 1);
	if (!cpu_cache)
		goto out;

	do_subsidiary_caches(cpu_cache, cpu_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cache_cpu_set(cpu_cache, cpu_id);
out:
	of_node_put(cpu_node);

	return cpu_cache;
}

<<<<<<< HEAD
static struct cache_dir *__cpuinit cacheinfo_create_cache_dir(unsigned int cpu_id)
=======
static struct cache_dir *cacheinfo_create_cache_dir(unsigned int cpu_id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cache_dir *cache_dir;
	struct device *dev;
	struct kobject *kobj = NULL;

	dev = get_cpu_device(cpu_id);
	WARN_ONCE(!dev, "no dev for CPU %i\n", cpu_id);
	if (!dev)
		goto err;

	kobj = kobject_create_and_add("cache", &dev->kobj);
	if (!kobj)
		goto err;

	cache_dir = kzalloc(sizeof(*cache_dir), GFP_KERNEL);
	if (!cache_dir)
		goto err;

	cache_dir->kobj = kobj;

	WARN_ON_ONCE(per_cpu(cache_dir_pcpu, cpu_id) != NULL);

	per_cpu(cache_dir_pcpu, cpu_id) = cache_dir;

	return cache_dir;
err:
	kobject_put(kobj);
	return NULL;
}

static void cache_index_release(struct kobject *kobj)
{
	struct cache_index_dir *index;

	index = kobj_to_cache_index_dir(kobj);

	pr_debug("freeing index directory for L%d %s cache\n",
		 index->cache->level, cache_type_string(index->cache));

	kfree(index);
}

static ssize_t cache_index_show(struct kobject *k, struct attribute *attr, char *buf)
{
	struct kobj_attribute *kobj_attr;

	kobj_attr = container_of(attr, struct kobj_attribute, attr);

	return kobj_attr->show(k, kobj_attr, buf);
}

static struct cache *index_kobj_to_cache(struct kobject *k)
{
	struct cache_index_dir *index;

	index = kobj_to_cache_index_dir(k);

	return index->cache;
}

static ssize_t size_show(struct kobject *k, struct kobj_attribute *attr, char *buf)
{
	unsigned int size_kb;
	struct cache *cache;

	cache = index_kobj_to_cache(k);

	if (cache_size_kb(cache, &size_kb))
		return -ENODEV;

	return sprintf(buf, "%uK\n", size_kb);
}

static struct kobj_attribute cache_size_attr =
	__ATTR(size, 0444, size_show, NULL);


static ssize_t line_size_show(struct kobject *k, struct kobj_attribute *attr, char *buf)
{
	unsigned int line_size;
	struct cache *cache;

	cache = index_kobj_to_cache(k);

	if (cache_get_line_size(cache, &line_size))
		return -ENODEV;

	return sprintf(buf, "%u\n", line_size);
}

static struct kobj_attribute cache_line_size_attr =
	__ATTR(coherency_line_size, 0444, line_size_show, NULL);

static ssize_t nr_sets_show(struct kobject *k, struct kobj_attribute *attr, char *buf)
{
	unsigned int nr_sets;
	struct cache *cache;

	cache = index_kobj_to_cache(k);

	if (cache_nr_sets(cache, &nr_sets))
		return -ENODEV;

	return sprintf(buf, "%u\n", nr_sets);
}

static struct kobj_attribute cache_nr_sets_attr =
	__ATTR(number_of_sets, 0444, nr_sets_show, NULL);

static ssize_t associativity_show(struct kobject *k, struct kobj_attribute *attr, char *buf)
{
	unsigned int associativity;
	struct cache *cache;

	cache = index_kobj_to_cache(k);

	if (cache_associativity(cache, &associativity))
		return -ENODEV;

	return sprintf(buf, "%u\n", associativity);
}

static struct kobj_attribute cache_assoc_attr =
	__ATTR(ways_of_associativity, 0444, associativity_show, NULL);

static ssize_t type_show(struct kobject *k, struct kobj_attribute *attr, char *buf)
{
	struct cache *cache;

	cache = index_kobj_to_cache(k);

	return sprintf(buf, "%s\n", cache_type_string(cache));
}

static struct kobj_attribute cache_type_attr =
	__ATTR(type, 0444, type_show, NULL);

static ssize_t level_show(struct kobject *k, struct kobj_attribute *attr, char *buf)
{
	struct cache_index_dir *index;
	struct cache *cache;

	index = kobj_to_cache_index_dir(k);
	cache = index->cache;

	return sprintf(buf, "%d\n", cache->level);
}

static struct kobj_attribute cache_level_attr =
	__ATTR(level, 0444, level_show, NULL);

<<<<<<< HEAD
static ssize_t shared_cpu_map_show(struct kobject *k, struct kobj_attribute *attr, char *buf)
{
	struct cache_index_dir *index;
	struct cache *cache;
	int len;
	int n = 0;

	index = kobj_to_cache_index_dir(k);
	cache = index->cache;
	len = PAGE_SIZE - 2;

	if (len > 1) {
		n = cpumask_scnprintf(buf, len, &cache->shared_cpu_map);
		buf[n++] = '\n';
		buf[n] = '\0';
	}
	return n;
=======
static ssize_t
show_shared_cpumap(struct kobject *k, struct kobj_attribute *attr, char *buf, bool list)
{
	struct cache_index_dir *index;
	struct cache *cache;
	const struct cpumask *mask;

	index = kobj_to_cache_index_dir(k);
	cache = index->cache;

	mask = &cache->shared_cpu_map;

	return cpumap_print_to_pagebuf(list, buf, mask);
}

static ssize_t shared_cpu_map_show(struct kobject *k, struct kobj_attribute *attr, char *buf)
{
	return show_shared_cpumap(k, attr, buf, false);
}

static ssize_t shared_cpu_list_show(struct kobject *k, struct kobj_attribute *attr, char *buf)
{
	return show_shared_cpumap(k, attr, buf, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct kobj_attribute cache_shared_cpu_map_attr =
	__ATTR(shared_cpu_map, 0444, shared_cpu_map_show, NULL);

<<<<<<< HEAD
/* Attributes which should always be created -- the kobject/sysfs core
 * does this automatically via kobj_type->default_attrs.  This is the
=======
static struct kobj_attribute cache_shared_cpu_list_attr =
	__ATTR(shared_cpu_list, 0444, shared_cpu_list_show, NULL);

/* Attributes which should always be created -- the kobject/sysfs core
 * does this automatically via kobj_type->default_groups.  This is the
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * minimum data required to uniquely identify a cache.
 */
static struct attribute *cache_index_default_attrs[] = {
	&cache_type_attr.attr,
	&cache_level_attr.attr,
	&cache_shared_cpu_map_attr.attr,
<<<<<<< HEAD
	NULL,
};
=======
	&cache_shared_cpu_list_attr.attr,
	NULL,
};
ATTRIBUTE_GROUPS(cache_index_default);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Attributes which should be created if the cache device node has the
 * right properties -- see cacheinfo_create_index_opt_attrs
 */
static struct kobj_attribute *cache_index_opt_attrs[] = {
	&cache_size_attr,
	&cache_line_size_attr,
	&cache_nr_sets_attr,
	&cache_assoc_attr,
};

static const struct sysfs_ops cache_index_ops = {
	.show = cache_index_show,
};

static struct kobj_type cache_index_type = {
	.release = cache_index_release,
	.sysfs_ops = &cache_index_ops,
<<<<<<< HEAD
	.default_attrs = cache_index_default_attrs,
};

static void __cpuinit cacheinfo_create_index_opt_attrs(struct cache_index_dir *dir)
{
	const char *cache_name;
=======
	.default_groups = cache_index_default_groups,
};

static void cacheinfo_create_index_opt_attrs(struct cache_index_dir *dir)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	const char *cache_type;
	struct cache *cache;
	char *buf;
	int i;

	buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!buf)
		return;

	cache = dir->cache;
<<<<<<< HEAD
	cache_name = cache->ofnode->full_name;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cache_type = cache_type_string(cache);

	/* We don't want to create an attribute that can't provide a
	 * meaningful value.  Check the return value of each optional
	 * attribute's ->show method before registering the
	 * attribute.
	 */
	for (i = 0; i < ARRAY_SIZE(cache_index_opt_attrs); i++) {
		struct kobj_attribute *attr;
		ssize_t rc;

		attr = cache_index_opt_attrs[i];

		rc = attr->show(&dir->kobj, attr, buf);
		if (rc <= 0) {
			pr_debug("not creating %s attribute for "
<<<<<<< HEAD
				 "%s(%s) (rc = %zd)\n",
				 attr->attr.name, cache_name,
=======
				 "%pOFP(%s) (rc = %zd)\n",
				 attr->attr.name, cache->ofnode,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				 cache_type, rc);
			continue;
		}
		if (sysfs_create_file(&dir->kobj, &attr->attr))
<<<<<<< HEAD
			pr_debug("could not create %s attribute for %s(%s)\n",
				 attr->attr.name, cache_name, cache_type);
=======
			pr_debug("could not create %s attribute for %pOFP(%s)\n",
				 attr->attr.name, cache->ofnode, cache_type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	kfree(buf);
}

<<<<<<< HEAD
static void __cpuinit cacheinfo_create_index_dir(struct cache *cache, int index, struct cache_dir *cache_dir)
=======
static void cacheinfo_create_index_dir(struct cache *cache, int index,
				       struct cache_dir *cache_dir)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cache_index_dir *index_dir;
	int rc;

	index_dir = kzalloc(sizeof(*index_dir), GFP_KERNEL);
	if (!index_dir)
<<<<<<< HEAD
		goto err;
=======
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	index_dir->cache = cache;

	rc = kobject_init_and_add(&index_dir->kobj, &cache_index_type,
				  cache_dir->kobj, "index%d", index);
<<<<<<< HEAD
	if (rc)
		goto err;
=======
	if (rc) {
		kobject_put(&index_dir->kobj);
		return;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	index_dir->next = cache_dir->index;
	cache_dir->index = index_dir;

	cacheinfo_create_index_opt_attrs(index_dir);
<<<<<<< HEAD

	return;
err:
	kfree(index_dir);
}

static void __cpuinit cacheinfo_sysfs_populate(unsigned int cpu_id, struct cache *cache_list)
=======
}

static void cacheinfo_sysfs_populate(unsigned int cpu_id,
				     struct cache *cache_list)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cache_dir *cache_dir;
	struct cache *cache;
	int index = 0;

	cache_dir = cacheinfo_create_cache_dir(cpu_id);
	if (!cache_dir)
		return;

	cache = cache_list;
	while (cache) {
		cacheinfo_create_index_dir(cache, index, cache_dir);
		index++;
		cache = cache->next_local;
	}
}

<<<<<<< HEAD
void __cpuinit cacheinfo_cpu_online(unsigned int cpu_id)
=======
void cacheinfo_cpu_online(unsigned int cpu_id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cache *cache;

	cache = cache_chain_instantiate(cpu_id);
	if (!cache)
		return;

	cacheinfo_sysfs_populate(cpu_id, cache);
}

<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG_CPU /* functions needed for cpu offline */
=======
/* functions needed to remove cache entry for cpu offline or suspend/resume */

#if (defined(CONFIG_PPC_PSERIES) && defined(CONFIG_SUSPEND)) || \
    defined(CONFIG_HOTPLUG_CPU)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct cache *cache_lookup_by_cpu(unsigned int cpu_id)
{
	struct device_node *cpu_node;
	struct cache *cache;
<<<<<<< HEAD
=======
	int group_id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cpu_node = of_get_cpu_node(cpu_id, NULL);
	WARN_ONCE(!cpu_node, "no OF node found for CPU %i\n", cpu_id);
	if (!cpu_node)
		return NULL;

<<<<<<< HEAD
	cache = cache_lookup_by_node(cpu_node);
=======
	group_id = get_group_id(cpu_id, 1);
	cache = cache_lookup_by_node_group(cpu_node, group_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	of_node_put(cpu_node);

	return cache;
}

static void remove_index_dirs(struct cache_dir *cache_dir)
{
	struct cache_index_dir *index;

	index = cache_dir->index;

	while (index) {
		struct cache_index_dir *next;

		next = index->next;
		kobject_put(&index->kobj);
		index = next;
	}
}

static void remove_cache_dir(struct cache_dir *cache_dir)
{
	remove_index_dirs(cache_dir);

	/* Remove cache dir from sysfs */
	kobject_del(cache_dir->kobj);

	kobject_put(cache_dir->kobj);

	kfree(cache_dir);
}

static void cache_cpu_clear(struct cache *cache, int cpu)
{
	while (cache) {
		struct cache *next = cache->next_local;

		WARN_ONCE(!cpumask_test_cpu(cpu, &cache->shared_cpu_map),
<<<<<<< HEAD
			  "CPU %i not accounted in %s(%s)\n",
			  cpu, cache->ofnode->full_name,
=======
			  "CPU %i not accounted in %pOFP(%s)\n",
			  cpu, cache->ofnode,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  cache_type_string(cache));

		cpumask_clear_cpu(cpu, &cache->shared_cpu_map);

		/* Release the cache object if all the cpus using it
		 * are offline */
		if (cpumask_empty(&cache->shared_cpu_map))
			release_cache(cache);

		cache = next;
	}
}

void cacheinfo_cpu_offline(unsigned int cpu_id)
{
	struct cache_dir *cache_dir;
	struct cache *cache;

	/* Prevent userspace from seeing inconsistent state - remove
	 * the sysfs hierarchy first */
	cache_dir = per_cpu(cache_dir_pcpu, cpu_id);

	/* careful, sysfs population may have failed */
	if (cache_dir)
		remove_cache_dir(cache_dir);

	per_cpu(cache_dir_pcpu, cpu_id) = NULL;

	/* clear the CPU's bit in its cache chain, possibly freeing
	 * cache objects */
	cache = cache_lookup_by_cpu(cpu_id);
	if (cache)
		cache_cpu_clear(cache, cpu_id);
}
<<<<<<< HEAD
#endif /* CONFIG_HOTPLUG_CPU */
=======

void cacheinfo_teardown(void)
{
	unsigned int cpu;

	lockdep_assert_cpus_held();

	for_each_online_cpu(cpu)
		cacheinfo_cpu_offline(cpu);
}

void cacheinfo_rebuild(void)
{
	unsigned int cpu;

	lockdep_assert_cpus_held();

	for_each_online_cpu(cpu)
		cacheinfo_cpu_online(cpu);
}

#endif /* (CONFIG_PPC_PSERIES && CONFIG_SUSPEND) || CONFIG_HOTPLUG_CPU */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
